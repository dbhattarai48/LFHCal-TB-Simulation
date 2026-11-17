#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TPad.h>
#include <TString.h>

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <vector>

void plot_hepmc_summary(
    const char* filename =
        "sim_output_hepmc/tb2026_hepmc_test.edm4hep.root")
{
    // =========================================================
    // 1. Open the simulation output.
    // =========================================================
    auto file = TFile::Open(filename);

    if (!file || file->IsZombie()) {
        std::cerr << "Cannot open " << filename << '\n';
        return;
    }

    auto tree = file->Get<TTree>("events");

    if (!tree || tree->GetEntries() == 0) {
        std::cerr << "No saved events found.\n";
        file->Close();
        return;
    }

    const Long64_t nEvents = tree->GetEntries();

    tree->SetEstimate(1000000);

    // =========================================================
    // 2. Identify the 60 physical scintillator-layer positions.
    //
    // This assumes all six modules have aligned layers.
    // Round positions to 0.001 mm to suppress numerical noise.
    // =========================================================
    auto positionKey = [](double z) -> long long {
        return std::llround(z * 1000.0);
    };

    std::set<long long> layerPositions;

    for (Long64_t event = 0; event < nEvents; ++event) {
        Long64_t n = tree->Draw(
            "LFHCALHits.position.z:LFHCALHits.cellID",
            "", "goff", 1, event);

        if (n < 0 || n >= tree->GetEstimate()) {
            std::cerr << "Layer-position read error in event "
                      << event << '\n';
            file->Close();
            return;
        }

        const double* positions = tree->GetV1();
        const double* ids = tree->GetV2();

        for (Long64_t i = 0; i < n; ++i) {
            auto id = static_cast<ULong64_t>(ids[i]);

            unsigned mx = (id >> 8) & 0xF;
            unsigned my = (id >> 12) & 0xF;

            // Include only the six calorimeter modules.
            if (mx < 2 && my < 3)
                layerPositions.insert(positionKey(positions[i]));
        }
    }

    if (layerPositions.size() != 60) {
        std::cerr
            << "Expected 60 distinct scintillator-layer positions, "
            << "but found " << layerPositions.size() << ".\n"
            << "Check the geometry and hit positions before "
            << "assigning physical layer numbers.\n";

        file->Close();
        return;
    }

    // Increasing z corresponds to increasing depth for this +z beam.
    std::map<long long, int> physicalLayer;

    int nextLayer = 1;

    std::cout << "\nPhysical layer -> global z [mm]\n";

    for (long long position : layerPositions) {
        physicalLayer[position] = nextLayer;

        std::cout << nextLayer << " -> "
                  << position / 1000.0 << '\n';

        ++nextLayer;
    }

    // =========================================================
    // 3. Prepare event and hit storage.
    // =========================================================
    std::vector<double> eventEnergy;
    std::vector<double> hitEnergy;
    std::vector<double> hitZ;

    eventEnergy.reserve(nEvents);

    double moduleSum[2][3] = {};
    Long64_t moduleActiveEvents[2][3] = {};

    double layerSum[60] = {};
    double layerSumSquared[60] = {};

    double triggerSum = 0;

    // =========================================================
    // 4. Read all events and accumulate energy.
    // =========================================================
    for (Long64_t event = 0; event < nEvents; ++event) {
        Long64_t nHits = tree->Draw(
            "LFHCALHits.energy:LFHCALHits.position.z:LFHCALHits.cellID",
            "", "goff", 1, event);

        if (nHits < 0 || nHits >= tree->GetEstimate()) {
            std::cerr << "Read error or insufficient hit buffer "
                      << "for event " << event << '\n';
            file->Close();
            return;
        }

        const double* energy = tree->GetV1();
        const double* z = tree->GetV2();
        const double* ids = tree->GetV3();

        double totalEvent = 0;
        double moduleEvent[2][3] = {};
        double layerEvent[60] = {};

        for (Long64_t i = 0; i < nHits; ++i) {
            // Encoding starts with:
            // system:8, moduleIDx:4, moduleIDy:4, ...
            auto id = static_cast<ULong64_t>(ids[i]);

            unsigned mx = (id >> 8) & 0xF;
            unsigned my = (id >> 12) & 0xF;

            double eMeV = 1000.0 * energy[i];

            // Trigger scintillators use module IDs (15,15).
            if (mx == 15 && my == 15) {
                triggerSum += eMeV;
                continue;
            }

            if (mx >= 2 || my >= 3) {
                std::cerr << "Unexpected module ID ("
                          << mx << "," << my
                          << ") in event " << event << '\n';
                file->Close();
                return;
            }

            auto found = physicalLayer.find(positionKey(z[i]));

            if (found == physicalLayer.end()) {
                std::cerr << "Unmapped layer position: "
                          << z[i] << " mm\n";
                file->Close();
                return;
            }

            int layerIndex = found->second - 1;

            totalEvent += eMeV;
            moduleEvent[mx][my] += eMeV;
            layerEvent[layerIndex] += eMeV;

            hitEnergy.push_back(eMeV);
            hitZ.push_back(z[i]);
        }

        // Include zero-signal events.
        eventEnergy.push_back(totalEvent);

        for (int mx = 0; mx < 2; ++mx) {
            for (int my = 0; my < 3; ++my) {
                moduleSum[mx][my] += moduleEvent[mx][my];

                if (moduleEvent[mx][my] > 0)
                    moduleActiveEvents[mx][my]++;
            }
        }

        // Accumulate PER-EVENT layer totals, not individual hits.
        for (int layer = 0; layer < 60; ++layer) {
            double value = layerEvent[layer];

            layerSum[layer] += value;
            layerSumSquared[layer] += value * value;
        }

        if ((event + 1) % 100 == 0)
            std::cout << "Processed " << event + 1
                      << "/" << nEvents << " events\n";
    }

    if (hitEnergy.empty()) {
        std::cerr << "No calorimeter hits found.\n";
        file->Close();
        return;
    }

    // =========================================================
    // 5. Determine plot ranges without truncating the data.
    // =========================================================
    double maximumEvent = *std::max_element(
        eventEnergy.begin(), eventEnergy.end());

    double maximumHit = *std::max_element(
        hitEnergy.begin(), hitEnergy.end());

    double minimumZ = *std::min_element(
        hitZ.begin(), hitZ.end());

    double maximumZ = *std::max_element(
        hitZ.begin(), hitZ.end());

    const double zBinWidth = 10.0;

    double zLow = std::min(
        0.0, std::floor(minimumZ / zBinWidth) * zBinWidth);

    double zHigh = std::max(
        1400.0,
        (std::floor(maximumZ / zBinWidth) + 1.0) * zBinWidth);

    int nZBins = static_cast<int>(
        std::lround((zHigh - zLow) / zBinWidth));

    // =========================================================
    // 6. Plot 1: total calorimeter energy per event.
    // =========================================================
    auto response = new TH1D(
        "hepmc_total_energy",
        Form("%lld HepMC 10 GeV #pi^{-} events;"
             "Calorimeter scintillator energy per event [MeV];Events",
             nEvents),
        100, 0, std::max(1.0, maximumEvent * 1.10));

    response->SetDirectory(nullptr);

    for (double value : eventEnergy)
        response->Fill(value);

    // =========================================================
    // 7. Plot 2: average energy per module.
    // =========================================================
    auto moduleMap = new TH2D(
        "hepmc_mean_module_energy",
        "Average energy per incident pion;"
        "Module ID X;Module ID Y;Mean energy [MeV/event]",
        2, -0.5, 1.5,
        3, -0.5, 2.5);

    moduleMap->SetDirectory(nullptr);
    moduleMap->SetStats(false);
    moduleMap->SetMinimum(0);
    moduleMap->SetContour(80);
    moduleMap->SetMarkerSize(1.8);

    for (int mx = 0; mx < 2; ++mx) {
        moduleMap->GetXaxis()->SetBinLabel(
            mx + 1, Form("%d", mx));

        for (int my = 0; my < 3; ++my)
            moduleMap->SetBinContent(
                mx + 1, my + 1,
                moduleSum[mx][my] / nEvents);
    }

    for (int my = 0; my < 3; ++my)
        moduleMap->GetYaxis()->SetBinLabel(
            my + 1, Form("%d", my));

    // =========================================================
    // 8. Plot 3: individual cell-hit energy versus global z.
    // =========================================================
    auto energyZ = new TH2D(
        "hepmc_hit_energy_vs_z",
        "All events: calorimeter cell hits;"
        "Global z [mm];Cell-hit energy [MeV];Number of hits",
        nZBins, zLow, zHigh,
        100, 0, std::max(1.0, maximumHit * 1.10));

    energyZ->SetDirectory(nullptr);
    energyZ->SetStats(false);
    energyZ->SetMinimum(1);
    energyZ->SetContour(80);

    for (size_t i = 0; i < hitEnergy.size(); ++i)
        energyZ->Fill(hitZ[i], hitEnergy[i]);

    // =========================================================
    // 9. Plot 4: mean energy versus physical layer number.
    //
    // Error = standard deviation of per-event layer totals / sqrt(N).
    // The calculation includes all events, including zeros.
    // =========================================================
    auto longitudinal = new TH1D(
        "hepmc_mean_layer_energy",
        "Average longitudinal profile: all six modules;"
        "Physical layer number, increasing along +z;"
        "Mean scintillator energy [MeV/event per layer]",
        60, 0.5, 60.5);

    longitudinal->SetDirectory(nullptr);
    longitudinal->SetStats(false);
    longitudinal->GetXaxis()->SetNdivisions(510);

    for (int layer = 0; layer < 60; ++layer) {
        double mean = layerSum[layer] / nEvents;
        double error = 0;

        if (nEvents > 1) {
            double numerator =
                layerSumSquared[layer]
                - layerSum[layer] * layerSum[layer] / nEvents;

            double varianceOfMean =
                std::max(0.0, numerator) /
                (double(nEvents) * (nEvents - 1));

            error = std::sqrt(varianceOfMean);
        }

        longitudinal->SetBinContent(layer + 1, mean);
        longitudinal->SetBinError(layer + 1, error);
    }

    // =========================================================
    // 10. Draw the four-panel summary.
    // =========================================================
    gStyle->SetOptStat(1110);
    gStyle->SetPalette(kBird);
    gStyle->SetPaintTextFormat(".2f");
    gStyle->SetTitleFontSize(0.04);

    auto canvas = new TCanvas(
        "hepmc_summary",
        "HepMC pion simulation summary",
        1800, 1200);

    canvas->Divide(2, 2);

    // Top left: total energy distribution.
    canvas->cd(1);
    gPad->SetLeftMargin(0.14);
    gPad->SetBottomMargin(0.14);
    gPad->SetGrid();

    response->SetLineColor(kBlue + 1);
    response->SetLineWidth(2);
    response->Draw("HIST");

    // Top right: module averages.
    canvas->cd(2);
    gPad->SetLeftMargin(0.12);
    gPad->SetRightMargin(0.23);
    gPad->SetBottomMargin(0.14);

    moduleMap->GetZaxis()->SetTitleOffset(1.5);
    moduleMap->Draw("COLZ TEXT");

    // Bottom left: individual hit energy versus global z.
    canvas->cd(3);
    gPad->SetLeftMargin(0.14);
    gPad->SetRightMargin(0.18);
    gPad->SetBottomMargin(0.14);
    gPad->SetLogz();

    energyZ->GetZaxis()->SetTitleOffset(1.3);
    energyZ->Draw("COLZ");

    // Bottom right: physical-layer profile.
    canvas->cd(4);
    gPad->SetLeftMargin(0.17);
    gPad->SetBottomMargin(0.16);
    gPad->SetGrid();

    longitudinal->SetLineColor(kBlue + 1);
    longitudinal->SetMarkerColor(kBlue + 1);
    longitudinal->SetMarkerStyle(20);
    longitudinal->SetMarkerSize(0.45);
    longitudinal->SetMinimum(0);
    longitudinal->Draw("E1");

    canvas->Modified();
    canvas->Update();

    // =========================================================
    // 11. Save all output in sim_output_hepmc.
    // =========================================================
    gSystem->mkdir("sim_output_hepmc", kTRUE);

    canvas->SaveAs(
        "sim_output_hepmc/pion2026_hepmc_summary.png");

    canvas->SaveAs(
        "sim_output_hepmc/pion2026_hepmc_summary.pdf");

    {
        TFile output(
            "sim_output_hepmc/pion2026_hepmc_histograms.root",
            "RECREATE");

        response->Write();
        moduleMap->Write();
        energyZ->Write();
        longitudinal->Write();
    }

    // =========================================================
    // 12. Print numerical results and consistency checks.
    // =========================================================
    double sumModuleMeans = 0;

    std::cout << std::fixed << std::setprecision(3)
              << "\nEvents processed: " << nEvents
              << "\nPhysical layers identified: "
              << layerPositions.size()
              << "\nCalorimeter hits: " << hitEnergy.size()
              << "\nMean calorimeter signal: "
              << response->GetMean() << " MeV/event"
              << "\nStandard deviation: "
              << response->GetStdDev() << " MeV"
              << "\nMean trigger signal (excluded): "
              << triggerSum / nEvents << " MeV/event\n\n"
              << "Module X  Module Y  Mean [MeV/event]  Events with signal\n";

    for (int my = 0; my < 3; ++my) {
        for (int mx = 0; mx < 2; ++mx) {
            double mean = moduleSum[mx][my] / nEvents;
            sumModuleMeans += mean;

            std::cout << "    " << mx
                      << "         " << my
                      << "         " << mean
                      << "            "
                      << moduleActiveEvents[mx][my]
                      << '\n';
        }
    }

    std::cout << "\nConsistency checks: these should agree.\n"
              << "Mean event energy:       "
              << response->GetMean() << " MeV/event\n"
              << "Sum of module means:     "
              << sumModuleMeans << " MeV/event\n"
              << "Sum of layer means:      "
              << longitudinal->Integral() << " MeV/event\n";

    file->Close();
}
