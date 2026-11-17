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
#include <vector>

void plot_pion_summary_layers(
    const char* filename =
        "sim_output/tb2026_opt1_piminus_10GeV_1000events.edm4hep.root")
{
    // ---------------------------------------------------------
    // Open the simulation output.
    // ---------------------------------------------------------
    auto file = TFile::Open(filename);

    if (!file || file->IsZombie()) {
        std::cerr << "Cannot open " << filename << '\n';
        return;
    }

    auto tree = file->Get<TTree>("events");

    if (!tree || tree->GetEntries() == 0) {
        std::cerr << "No events found.\n";
        file->Close();
        return;
    }

    const Long64_t nEvents = tree->GetEntries();
    const int expectedLayers = 60;

    // Merge tiny floating-point differences in cell-centre z.
    // This tolerance is much smaller than the ~20.5 mm pitch.
    const double zTolerance = 0.1; // mm

    struct Hit {
        Long64_t event;
        double z;
        double energy; // MeV
    };

    std::vector<Hit> hits;
    std::vector<double> eventEnergy;
    std::vector<double> sortedZ;

    eventEnergy.reserve(nEvents);

    double moduleSum[2][3] = {};
    double triggerSum = 0;

    tree->SetEstimate(1000000);

    // ---------------------------------------------------------
    // Read all events.
    // ---------------------------------------------------------
    for (Long64_t event = 0; event < nEvents; ++event) {
        Long64_t nHits = tree->Draw(
            "LFHCALHits.energy:LFHCALHits.position.z:LFHCALHits.cellID",
            "", "goff", 1, event);

        if (nHits < 0 || nHits >= tree->GetEstimate()) {
            std::cerr << "Read error or insufficient hit buffer"
                      << " in event " << event << '\n';
            file->Close();
            return;
        }

        const double* energy = tree->GetV1();
        const double* z = tree->GetV2();
        const double* ids = tree->GetV3();

        double total = 0;

        for (Long64_t i = 0; i < nHits; ++i) {
            auto id = static_cast<ULong64_t>(ids[i]);

            // Readout encoding begins:
            // system:8,moduleIDx:4,moduleIDy:4,...
            unsigned mx = (id >> 8) & 0xF;
            unsigned my = (id >> 12) & 0xF;

            double eMeV = 1000.0 * energy[i];

            // Exclude trigger scintillators.
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

            total += eMeV;
            moduleSum[mx][my] += eMeV;

            hits.push_back({event, z[i], eMeV});
            sortedZ.push_back(z[i]);
        }

        // Include zero-signal events in the sample.
        eventEnergy.push_back(total);

        if ((event + 1) % 100 == 0)
            std::cout << "Processed " << event + 1
                      << "/" << nEvents << " events\n";
    }

    if (hits.empty()) {
        std::cerr << "No calorimeter hits found.\n";
        file->Close();
        return;
    }

    // ---------------------------------------------------------
    // Identify physical layers from cell-centre z positions.
    // Pool the positions across all six modules and all events.
    // ---------------------------------------------------------
    std::sort(sortedZ.begin(), sortedZ.end());

    std::vector<double> layerZ;

    for (double z : sortedZ) {
        if (layerZ.empty() ||
            std::abs(z - layerZ.back()) > zTolerance) {
            layerZ.push_back(z);
        }
    }

    if (static_cast<int>(layerZ.size()) != expectedLayers) {
        std::cerr << "\nExpected 60 physical layer positions, found "
                  << layerZ.size() << ".\n"
                  << "Check whether all layers recorded hits and "
                  << "whether hit positions are cell centres.\n"
                  << "No layer-number plot has been produced.\n";

        std::cerr << "Identified z positions [mm]:\n";
        for (double z : layerZ)
            std::cerr << z << " ";
        std::cerr << '\n';

        file->Close();
        return;
    }

    // Per-event layer sums allow statistical errors on the mean.
    std::vector<std::vector<double>> layerEvent(
        nEvents, std::vector<double>(expectedLayers, 0.0));

    for (const auto& hit : hits) {
        // Find the nearest identified layer.
        auto it = std::lower_bound(
            layerZ.begin(), layerZ.end(), hit.z);

        int layer;

        if (it == layerZ.begin()) {
            layer = 0;
        } else if (it == layerZ.end()) {
            layer = expectedLayers - 1;
        } else {
            int upper = static_cast<int>(it - layerZ.begin());
            int lower = upper - 1;

            layer =
                std::abs(hit.z - layerZ[lower]) <=
                std::abs(hit.z - layerZ[upper])
                ? lower : upper;
        }

        if (std::abs(hit.z - layerZ[layer]) > zTolerance) {
            std::cerr << "Cannot assign hit at z = "
                      << hit.z << " mm to a layer.\n";
            file->Close();
            return;
        }

        layerEvent[hit.event][layer] += hit.energy;
    }

    // ---------------------------------------------------------
    // Plot 1: total calorimeter energy per event.
    // ---------------------------------------------------------
    double maximumEvent = *std::max_element(
        eventEnergy.begin(), eventEnergy.end());

    auto response = new TH1D(
        "pion_total_energy",
        Form("%lld incident 10 GeV #pi^{-} events;"
             "Calorimeter scintillator energy per event [MeV];Events",
             nEvents),
        100, 0, std::max(1.0, 1.10 * maximumEvent));

    response->SetDirectory(nullptr);

    for (double energy : eventEnergy)
        response->Fill(energy);

    // ---------------------------------------------------------
    // Plot 2: average energy in each of the six modules.
    // ---------------------------------------------------------
    auto moduleMap = new TH2D(
        "pion_mean_module_energy",
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

        for (int my = 0; my < 3; ++my) {
            moduleMap->SetBinContent(
                mx + 1, my + 1,
                moduleSum[mx][my] / nEvents);
        }
    }

    for (int my = 0; my < 3; ++my)
        moduleMap->GetYaxis()->SetBinLabel(
            my + 1, Form("%d", my));

    // ---------------------------------------------------------
    // Plot 3: cell-hit energy versus global z, all events.
    // Retain this as a density plot for comparison.
    // ---------------------------------------------------------
    double maximumHit = 0;
    for (const auto& hit : hits)
        maximumHit = std::max(maximumHit, hit.energy);

    double zLow = std::min(
        0.0, std::floor(layerZ.front() / 10.0) * 10.0);

    double zHigh = std::max(
        1400.0,
        (std::floor(layerZ.back() / 10.0) + 1.0) * 10.0);

    int nZBins = static_cast<int>(
        std::lround((zHigh - zLow) / 10.0));

    auto energyZ = new TH2D(
        "pion_hit_energy_vs_z",
        "All events: calorimeter cell hits;"
        "Global z [mm];Cell-hit energy [MeV];Number of hits",
        nZBins, zLow, zHigh,
        100, 0, std::max(1.0, 1.10 * maximumHit));

    energyZ->SetDirectory(nullptr);
    energyZ->SetStats(false);
    energyZ->SetMinimum(1);
    energyZ->SetContour(80);

    for (const auto& hit : hits)
        energyZ->Fill(hit.z, hit.energy);

    // ---------------------------------------------------------
    // Plot 4: mean energy in each PHYSICAL layer.
    // All modules are summed before averaging over events.
    // ---------------------------------------------------------
    auto layerProfile = new TH1D(
        "pion_mean_energy_per_layer",
        "Average longitudinal profile: all six modules;"
        "Physical layer number, increasing along +z;"
        "Mean scintillator energy [MeV/event per layer]",
        expectedLayers, 0.5, expectedLayers + 0.5);

    layerProfile->SetDirectory(nullptr);
    layerProfile->SetStats(false);
    layerProfile->SetMinimum(0);

    for (int layer = 0; layer < expectedLayers; ++layer) {
        double sum = 0;
        double sumSquares = 0;

        for (Long64_t event = 0; event < nEvents; ++event) {
            double value = layerEvent[event][layer];
            sum += value;
            sumSquares += value * value;
        }

        double mean = sum / nEvents;

        // Standard error of the mean across independent events.
        double error = 0;
        if (nEvents > 1) {
            double numerator =
                sumSquares - sum * sum / nEvents;

            error = std::sqrt(
                std::max(0.0, numerator) /
                (static_cast<double>(nEvents) * (nEvents - 1)));
        }

        layerProfile->SetBinContent(layer + 1, mean);
        layerProfile->SetBinError(layer + 1, error);
    }

    // ---------------------------------------------------------
    // Draw the four panels.
    // ---------------------------------------------------------
    gStyle->SetOptStat(1110);
    gStyle->SetPalette(kBird);
    gStyle->SetPaintTextFormat(".2f");
    gStyle->SetTitleFontSize(0.04);

    auto canvas = new TCanvas(
        "pion_summary_layers",
        "10 GeV pion summary with physical layers",
        1800, 1200);

    canvas->Divide(2, 2);

    canvas->cd(1);
    gPad->SetLeftMargin(0.14);
    gPad->SetBottomMargin(0.14);
    gPad->SetGrid();

    response->SetLineColor(kBlue + 1);
    response->SetLineWidth(2);
    response->Draw("HIST");

    canvas->cd(2);
    gPad->SetLeftMargin(0.12);
    gPad->SetRightMargin(0.23);
    gPad->SetBottomMargin(0.14);

    moduleMap->GetZaxis()->SetTitleOffset(1.5);
    moduleMap->Draw("COLZ TEXT");

    canvas->cd(3);
    gPad->SetLeftMargin(0.14);
    gPad->SetRightMargin(0.18);
    gPad->SetBottomMargin(0.14);
    gPad->SetLogz();

    energyZ->GetZaxis()->SetTitleOffset(1.3);
    energyZ->Draw("COLZ");

    canvas->cd(4);
    gPad->SetLeftMargin(0.18);
    gPad->SetBottomMargin(0.15);
    gPad->SetGrid();

    layerProfile->SetLineColor(kBlue + 1);
    layerProfile->SetMarkerColor(kBlue + 1);
    layerProfile->SetMarkerStyle(20);
    layerProfile->SetMarkerSize(0.6);
    layerProfile->Draw("E1");

    canvas->Modified();
    canvas->Update();

    // ---------------------------------------------------------
    // Save plots, histograms, and the layer-position table.
    // ---------------------------------------------------------
    gSystem->mkdir("sim_output", kTRUE);

    canvas->SaveAs(
        "sim_output/pion2026_summary_physical_layers.png");
    canvas->SaveAs(
        "sim_output/pion2026_summary_physical_layers.pdf");

    {
        TFile output(
            "sim_output/pion2026_summary_physical_layers.root",
            "RECREATE");

        response->Write();
        moduleMap->Write();
        energyZ->Write();
        layerProfile->Write();

        TTree positions(
            "layer_positions",
            "Physical layer numbers and cell-centre z positions");

        int physicalLayer;
        double globalZ;

        positions.Branch("layer", &physicalLayer, "layer/I");
        positions.Branch("z_mm", &globalZ, "z_mm/D");

        for (int layer = 0; layer < expectedLayers; ++layer) {
            physicalLayer = layer + 1;
            globalZ = layerZ[layer];
            positions.Fill();
        }

        positions.Write();
    }

    // ---------------------------------------------------------
    // Numerical checks.
    // ---------------------------------------------------------
    double moduleMeanSum = 0;

    std::cout << std::fixed << std::setprecision(3)
              << "\nEvents processed: " << nEvents
              << "\nPhysical layers identified: " << layerZ.size()
              << "\nMean trigger signal, excluded: "
              << triggerSum / nEvents << " MeV/event\n\n";

    std::cout << "Module X  Module Y  Mean energy [MeV/event]\n";

    for (int my = 0; my < 3; ++my) {
        for (int mx = 0; mx < 2; ++mx) {
            double mean = moduleSum[mx][my] / nEvents;
            moduleMeanSum += mean;

            std::cout << "    " << mx << "         " << my
                      << "         " << mean << '\n';
        }
    }

    std::cout << "\nLayer  Global z [mm]  Mean [MeV/event]  Error on mean\n";

    for (int layer = 0; layer < expectedLayers; ++layer) {
        std::cout << std::setw(4) << layer + 1
                  << std::setw(15) << layerZ[layer]
                  << std::setw(18)
                  << layerProfile->GetBinContent(layer + 1)
                  << std::setw(15)
                  << layerProfile->GetBinError(layer + 1)
                  << '\n';
    }

    std::cout << "\nConsistency checks -- these should agree:\n"
              << "Mean total energy:       "
              << response->GetMean() << " MeV/event\n"
              << "Sum of module means:     "
              << moduleMeanSum << " MeV/event\n"
              << "Sum of layer means:      "
              << layerProfile->Integral() << " MeV/event\n";

    file->Close();
}
