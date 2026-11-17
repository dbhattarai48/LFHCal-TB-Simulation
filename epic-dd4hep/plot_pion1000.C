#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TPad.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

void plot_pion1000(
    const char* filename =
        "sim_output/tb2026_opt1_piminus_10GeV_1000events.edm4hep.root")
{
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
    eventEnergy.reserve(nEvents);

    double moduleSum[2][3] = {};
    Long64_t moduleActiveEvents[2][3] = {};
    double triggerSum = 0;

    tree->SetEstimate(1000000);

    // Process each event separately.
    for (Long64_t event = 0; event < nEvents; ++event) {
        Long64_t nHits = tree->Draw(
            "LFHCALHits.energy:LFHCALHits.cellID",
            "", "goff", 1, event);

        if (nHits < 0 || nHits >= tree->GetEstimate()) {
            std::cerr << "Failed to read event " << event
                      << ", or hit buffer is too small.\n";
            file->Close();
            return;
        }

        const double* energy = tree->GetV1();
        const double* ids    = tree->GetV2();

        double moduleEvent[2][3] = {};
        double totalEvent = 0;

        for (Long64_t i = 0; i < nHits; ++i) {
            // Encoding:
            // system:8, moduleIDx:4, moduleIDy:4, ...
            auto id = static_cast<ULong64_t>(ids[i]);

            unsigned mx = (id >> 8)  & 0xF;
            unsigned my = (id >> 12) & 0xF;

            double eMeV = energy[i] * 1000.0;

            // Exclude trigger scintillators.
            if (mx == 15 && my == 15) {
                triggerSum += eMeV;
                continue;
            }

            // Expected calorimeter IDs: X=0,1 and Y=0,1,2.
            if (mx >= 2 || my >= 3) {
                std::cerr << "Unexpected module ID ("
                          << mx << "," << my
                          << ") in event " << event << '\n';
                file->Close();
                return;
            }

            moduleEvent[mx][my] += eMeV;
            totalEvent += eMeV;
        }

        // Include every event, even if its recorded energy is zero.
        eventEnergy.push_back(totalEvent);

        for (int mx = 0; mx < 2; ++mx) {
            for (int my = 0; my < 3; ++my) {
                moduleSum[mx][my] += moduleEvent[mx][my];

                if (moduleEvent[mx][my] > 0)
                    moduleActiveEvents[mx][my]++;
            }
        }
    }

    // Choose a range that includes the entire sample.
    double maximum = *std::max_element(
        eventEnergy.begin(), eventEnergy.end());

    double upper = std::max(1.0, maximum * 1.10);

    auto response = new TH1D(
        "pion_total_response",
        Form("%lld incident 10 GeV #pi^{-} events;"
             "Total calorimeter scintillator energy per event [MeV];"
             "Events",
             nEvents),
        100, 0, upper);

    response->SetDirectory(nullptr);

    for (double value : eventEnergy)
        response->Fill(value);

    auto average = new TH2D(
        "pion_average_module_energy",
        "Average module energy per incident pion;"
        "Module ID X;Module ID Y;Mean energy [MeV/event]",
        2, -0.5, 1.5,
        3, -0.5, 2.5);

    average->SetDirectory(nullptr);
    average->SetStats(false);
    average->SetMinimum(0);
    average->SetContour(80);
    average->SetMarkerSize(1.7);

    for (int mx = 0; mx < 2; ++mx) {
        average->GetXaxis()->SetBinLabel(
            mx + 1, Form("%d", mx));

        for (int my = 0; my < 3; ++my) {
            average->SetBinContent(
                mx + 1, my + 1,
                moduleSum[mx][my] / nEvents);
        }
    }

    for (int my = 0; my < 3; ++my)
        average->GetYaxis()->SetBinLabel(
            my + 1, Form("%d", my));

    gStyle->SetOptStat(1110);
    gStyle->SetPalette(kBird);
    gStyle->SetPaintTextFormat(".2f");

    auto canvas = new TCanvas(
        "pion_sample", "Pion sample response", 1500, 700);

    canvas->Divide(2, 1);

    // LEFT: one histogram entry per incident pion.
    canvas->cd(1);
    gPad->SetLeftMargin(0.13);
    gPad->SetBottomMargin(0.15);
    gPad->SetGrid();

    response->SetLineColor(kBlue + 1);
    response->SetLineWidth(2);
    response->Draw("HIST");

    // RIGHT: average across all incident pions.
    canvas->cd(2);
    gPad->SetLeftMargin(0.12);
    gPad->SetRightMargin(0.23);
    gPad->SetBottomMargin(0.15);

    average->GetZaxis()->SetTitleOffset(1.5);
    average->Draw("COLZ TEXT");

    gSystem->mkdir("sim_output", kTRUE);

    canvas->SaveAs("sim_output/pion2026_all_1000_events.png");
    canvas->SaveAs("sim_output/pion2026_all_1000_events.pdf");

    // Also save histograms for later analysis.
    {
        TFile output("sim_output/pion2026_summary.root", "RECREATE");
        response->Write();
        average->Write();
    }

    std::cout << std::fixed << std::setprecision(3)
              << "\nEvents processed: " << nEvents
              << "\nMean calorimeter signal: "
              << response->GetMean() << " MeV/event"
              << "\nStandard deviation: "
              << response->GetStdDev() << " MeV"
              << "\nMean trigger signal (excluded): "
              << triggerSum / nEvents << " MeV/event\n\n"
              << "Module X  Module Y  Mean [MeV/event]  Events with signal\n";

    for (int my = 0; my < 3; ++my) {
        for (int mx = 0; mx < 2; ++mx) {
            std::cout << "    " << mx
                      << "         " << my
                      << "         "
                      << moduleSum[mx][my] / nEvents
                      << "            "
                      << moduleActiveEvents[mx][my]
                      << '\n';
        }
    }

    file->Close();
}
