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

void plot_pion_summary(
    const char* filename =
        "sim_output/tb2026_opt1_piminus_10GeV_1000events.edm4hep.root")
{
    // ------------------------------------------------------------
    // 1. Open input file.
    // ------------------------------------------------------------
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

    std::vector<double> eventEnergy;
    std::vector<double> hitEnergy;
    std::vector<double> hitZ;

    eventEnergy.reserve(nEvents);

    double moduleSum[2][3] = {};
    Long64_t moduleActiveEvents[2][3] = {};
    double triggerSum = 0;

    tree->SetEstimate(1000000);

    // ------------------------------------------------------------
    // 2. Read every event and decode module IDs.
    // ------------------------------------------------------------
    for (Long64_t event = 0; event < nEvents; ++event) {
        Long64_t nHits = tree->Draw(
            "LFHCALHits.energy:LFHCALHits.position.z:LFHCALHits.cellID",
            "", "goff", 1, event);

        if (nHits < 0 || nHits >= tree->GetEstimate()) {
            std::cerr << "Failed to read event " << event
                      << ", or hit buffer is too small.\n";
            file->Close();
            return;
        }

        const double* energy = tree->GetV1();
        const double* z      = tree->GetV2();
        const double* ids    = tree->GetV3();

        double eventTotal = 0;
        double moduleEvent[2][3] = {};

        for (Long64_t i = 0; i < nHits; ++i) {
            // Encoding starts with:
            // system:8, moduleIDx:4, moduleIDy:4, ...
            auto id = static_cast<ULong64_t>(ids[i]);

            unsigned mx = (id >> 8)  & 0xF;
            unsigned my = (id >> 12) & 0xF;

            double eMeV = 1000.0 * energy[i];

            // Trigger scintillators have module IDs (15,15).
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

            eventTotal += eMeV;
            moduleEvent[mx][my] += eMeV;

            hitEnergy.push_back(eMeV);
            hitZ.push_back(z[i]);
        }

        // Zero-signal events are included.
        eventEnergy.push_back(eventTotal);

        for (int mx = 0; mx < 2; ++mx) {
            for (int my = 0; my < 3; ++my) {
                moduleSum[mx][my] += moduleEvent[mx][my];

                if (moduleEvent[mx][my] > 0)
                    moduleActiveEvents[mx][my]++;
            }
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

    // ------------------------------------------------------------
    // 3. Choose ranges that include all recorded values.
    // ------------------------------------------------------------
    double maximumEvent = *std::max_element(
        eventEnergy.begin(), eventEnergy.end());

    double maximumHit = *std::max_element(
        hitEnergy.begin(), hitEnergy.end());

    double minimumZ = *std::min_element(hitZ.begin(), hitZ.end());
    double maximumZ = *std::max_element(hitZ.begin(), hitZ.end());

    const double binWidthZ = 10.0; // mm

    double zLow = std::min(
        0.0, std::floor(minimumZ / binWidthZ) * binWidthZ);

    double zHigh = std::max(
        1400.0,
        (std::floor(maximumZ / binWidthZ) + 1.0) * binWidthZ);

    int nZBins = static_cast<int>(
        std::lround((zHigh - zLow) / binWidthZ));

    // ------------------------------------------------------------
    // 4. Total calorimeter energy distribution.
    //    One entry per event.
    // ------------------------------------------------------------
    auto response = new TH1D(
        "pion_total_energy",
        Form("%lld incident 10 GeV #pi^{-} events;"
             "Calorimeter scintillator energy per event [MeV];Events",
             nEvents),
        100, 0, std::max(1.0, 1.10 * maximumEvent));

    response->SetDirectory(nullptr);

    for (double value : eventEnergy)
        response->Fill(value);

    // ------------------------------------------------------------
    // 5. Mean energy per module.
    //    Divide by ALL incident events.
    // ------------------------------------------------------------
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

    // ------------------------------------------------------------
    // 6. Individual cell-hit energy versus global z.
    //    Colour represents the number of hits.
    // ------------------------------------------------------------
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

    // ------------------------------------------------------------
    // 7. Average longitudinal energy profile.
    //    Sum all cell energies in each z bin, then divide by events.
    // ------------------------------------------------------------
    auto longitudinal = new TH1D(
        "pion_mean_longitudinal_energy",
        "Average longitudinal energy profile;"
        "Global z [mm];Mean energy [MeV/event per 10 mm]",
        nZBins, zLow, zHigh);

    longitudinal->SetDirectory(nullptr);
    longitudinal->SetStats(false);

    for (size_t i = 0; i < hitEnergy.size(); ++i) {
        energyZ->Fill(hitZ[i], hitEnergy[i]);

        longitudinal->Fill(
            hitZ[i], hitEnergy[i] / nEvents);
    }

    // ------------------------------------------------------------
    // 8. Draw a four-panel summary.
    // ------------------------------------------------------------
    gStyle->SetOptStat(1110);
    gStyle->SetPalette(kBird);
    gStyle->SetPaintTextFormat(".2f");
    gStyle->SetTitleFontSize(0.04);

    auto canvas = new TCanvas(
        "pion_summary", "10 GeV pion sample summary",
        1800, 1200);

    canvas->Divide(2, 2);

    // Top left: event-energy distribution.
    canvas->cd(1);
    gPad->SetLeftMargin(0.14);
    gPad->SetBottomMargin(0.14);
    gPad->SetGrid();

    response->SetLineColor(kBlue + 1);
    response->SetLineWidth(2);
    response->Draw("HIST");

    // Top right: six-module average-energy map.
    canvas->cd(2);
    gPad->SetLeftMargin(0.12);
    gPad->SetRightMargin(0.23);
    gPad->SetBottomMargin(0.14);

    moduleMap->GetZaxis()->SetTitleOffset(1.5);
    moduleMap->Draw("COLZ TEXT");

    // Bottom left: cell-hit energy versus z.
    canvas->cd(3);
    gPad->SetLeftMargin(0.14);
    gPad->SetRightMargin(0.18);
    gPad->SetBottomMargin(0.14);
    gPad->SetLogz();

    energyZ->GetZaxis()->SetTitleOffset(1.3);
    energyZ->Draw("COLZ");

    // Bottom right: average longitudinal profile.
    canvas->cd(4);
    gPad->SetLeftMargin(0.17);
    gPad->SetBottomMargin(0.14);
    gPad->SetGrid();

    longitudinal->SetLineColor(kBlue + 1);
    longitudinal->SetLineWidth(2);
    longitudinal->SetMinimum(0);
    longitudinal->Draw("HIST");

    canvas->Modified();
    canvas->Update();

    // ------------------------------------------------------------
    // 9. Save plots and histograms.
    // ------------------------------------------------------------
    gSystem->mkdir("sim_output", kTRUE);

    canvas->SaveAs("sim_output/pion2026_1000events_summary.png");
    canvas->SaveAs("sim_output/pion2026_1000events_summary.pdf");

    {
        TFile output(
            "sim_output/pion2026_1000events_histograms.root",
            "RECREATE");

        response->Write();
        moduleMap->Write();
        energyZ->Write();
        longitudinal->Write();
    }

    // ------------------------------------------------------------
    // 10. Print results and consistency checks.
    // ------------------------------------------------------------
    double meanModuleSum = 0;

    std::cout << std::fixed << std::setprecision(3)
              << "\nEvents processed: " << nEvents
              << "\nCalorimeter cell hits: " << hitEnergy.size()
              << "\nMean calorimeter energy: "
              << response->GetMean() << " MeV/event"
              << "\nResponse standard deviation: "
              << response->GetStdDev() << " MeV"
              << "\nMean trigger energy (excluded): "
              << triggerSum / nEvents << " MeV/event\n\n"
              << "Module X  Module Y  Mean [MeV/event]  Events with signal\n";

    for (int my = 0; my < 3; ++my) {
        for (int mx = 0; mx < 2; ++mx) {
            double mean = moduleSum[mx][my] / nEvents;
            meanModuleSum += mean;

            std::cout << "    " << mx
                      << "         " << my
                      << "         " << mean
                      << "            "
                      << moduleActiveEvents[mx][my]
                      << '\n';
        }
    }

    std::cout << "\nConsistency checks -- these should agree:\n"
              << "Mean total energy:          "
              << response->GetMean() << " MeV/event\n"
              << "Sum of six module means:    "
              << meanModuleSum << " MeV/event\n"
              << "Longitudinal-bin sum:       "
              << longitudinal->Integral() << " MeV/event\n";

    file->Close();
}
