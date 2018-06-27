#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"

void cutResponse(TTree* t, TH1D* h_mass, double responseCut = 0)
{

    double mass;
    double BDTresponse;
    t->SetBranchAddress("mass", &mass);
    t->SetBranchAddress("BDTresponse", &BDTresponse);

    for(Long64_t ievt=0; ievt<t->GetEntries();ievt++) {
        t->GetEntry(ievt);
        if(BDTresponse > responseCut)
            h_mass->Fill(mass);
    }
}

void BDTscanner()
{
    TFile* f = TFile::Open("/Users/btran/research/RootFiles/Phi/BDT/PhiBDT_2M_150618.root");
    std::vector<TTree*> v_trees;

    std::vector<TH1D*> v_hmass;

    std::vector<double> pts = {0,0.5,1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6};

    //std::vector<double> bdtCuts = {}

    for(unsigned i=0; i<pts.size()-1; i++)
    {
        TTree* t = 0;
        f->GetObject(Form("BDTApp/BDTAppTree_%.1f",pts[i]),t);
        //v_trees.push_back(t);
        TH1D* h_tmpmass = new TH1D(Form("h_mass_%.1f",pts[i]),Form("mass %.1f",pts[i]),80,1,1.04);
        cutResponse(t,h_tmpmass,0);
        v_hmass.push_back(h_tmpmass);
    }

    for(unsigned i=0; i<v_hmass.size();i++)
    {
        TCanvas* c = new TCanvas(Form("h_mass_%.1f",pts[i]),Form("h_mass_%.1f",pts[i]),800,600);
        v_hmass[i]->SetMarkerStyle(20);
        v_hmass[i]->SetMarkerSize(1.1);
        v_hmass[i]->Draw("P");
        c->Print(Form("PhiMass_pt%.1f-%.1f.pdf",pts[i],pts[i+1]));
    }
}

