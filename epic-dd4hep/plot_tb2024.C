#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TStyle.h>
#include <iostream>
#include <vector>

void plot_tb2024(
    const char* filename = "sim_output/tb2024_fixed.edm4hep.root")
{
    TFile* file = TFile::Open(filename);
    if (!file || file->IsZombie()) {
        std::cerr << "Cannot open " << filename << std::endl;
        return;
    }

    auto tree = file->Get<TTree>("events");
    if (!tree || tree->GetEntries() == 0) {
        std::cerr << "No saved events found." << std::endl;
        return;
    }

    // Read only event 0. Draw buffers contain one value per hit.
    tree->SetEstimate(1000000);
    Long64_t n = tree->Draw(
        "LFHCALHits.energy:LFHCALHits.position.z:LFHCALHits.cellID",
        "", "goff", 1, 0);

    if (n <= 0) {
        std::cerr << "No hits found, or branch names did not match."
                  << std::endl;
        return;
    }

    const double* energy = tree->GetV1();
    const double* z      = tree->GetV2();
    const double* cellID = tree->GetV3();

    std::vector<double> calZ, calE, triggerZ, triggerE;
    double calSum = 0, triggerSum = 0;

    for (Long64_t i = 0; i < n; ++i) {
        auto id = static_cast<ULong64_t>(cellID[i]);

        // XML encoding: system:8, moduleIDx:4, moduleIDy:4, ...
        unsigned moduleX = (id >> 8)  & 0xF;
        unsigned moduleY = (id >> 12) & 0xF;
        bool isTrigger = (moduleX == 15 && moduleY == 15);

        double eMeV = 1000.0 * energy[i];

        if (isTrigger) {
            triggerZ.push_back(z[i]);
            triggerE.push_back(eMeV);
            triggerSum += eMeV;
        } else {
            calZ.push_back(z[i]);
            calE.push_back(eMeV);
            calSum += eMeV;
        }
    }

    gStyle->SetOptStat(0);

    auto canvas = new TCanvas("tb2024", "Single-event LFHCal plots",
                               1100, 850);
    canvas->Divide(1, 2);

    // Plot 1: all hits, with triggers distinguished by colour.
    canvas->cd(1);
    gPad->SetGrid();
    gPad->SetLeftMargin(0.12);

    auto cal = new TGraph(calZ.size(), calZ.data(), calE.data());
    cal->SetMarkerStyle(20);
    cal->SetMarkerSize(0.8);
    cal->SetMarkerColor(kBlue + 1);

    auto triggers = new TGraph(
        triggerZ.size(), triggerZ.data(), triggerE.data());
    triggers->SetMarkerStyle(21);
    triggers->SetMarkerSize(1.1);
    triggers->SetMarkerColor(kOrange + 7);

    auto all = new TMultiGraph();
    all->SetTitle(
        "Event 0: 10 GeV muon, beam along -z;"
        "Global z [mm];Hit energy [MeV]");

    if (!calZ.empty())     all->Add(cal, "P");
    if (!triggerZ.empty()) all->Add(triggers, "P");
    all->Draw("A");
    all->SetMinimum(0);

    auto legend = new TLegend(0.68, 0.15, 0.89, 0.34);
    legend->SetBorderSize(0);
    if (!calZ.empty())
        legend->AddEntry(cal, "Calorimeter", "p");
    if (!triggerZ.empty())
        legend->AddEntry(triggers, "Trigger scintillators", "p");
    legend->Draw();

    // Plot 2: calorimeter hits only, expanded view.
    canvas->cd(2);
    gPad->SetGrid();
    gPad->SetLeftMargin(0.12);

    if (!calZ.empty()) {
        auto detail = new TGraph(
            calZ.size(), calZ.data(), calE.data());
        detail->SetTitle(
            "Event 0: calorimeter hits only;"
            "Global z [mm];Hit energy [MeV]");
        detail->SetMarkerStyle(20);
        detail->SetMarkerColor(kBlue + 1);
        detail->SetMinimum(0);
        detail->Draw("AP");
    }

    canvas->SaveAs("sim_output/tb2024_event0.png");
    canvas->SaveAs("sim_output/tb2024_event0.pdf");

    std::cout << "Calorimeter hits: " << calZ.size()
              << ", energy: " << calSum << " MeV\n"
              << "Trigger hits: " << triggerZ.size()
              << ", energy: " << triggerSum << " MeV\n"
              << "Total energy: " << calSum + triggerSum
              << " MeV" << std::endl;
}
