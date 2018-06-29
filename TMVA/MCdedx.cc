#include "MCdedx.h"

void MCdedx::Init(TTree *tree)
{
    reader.SetTree(tree);
}

void MCdedx::SlaveBegin(TTree *tree)
{
    h_nEvt = new TH1D("nEvt","Events",10,0,10);
    h_dedx = new TH2D("dedx","dedx v momentum",200,0,20,300,0,15);

    GetOutputList()->Add(h_nEvt);
    GetOutputList()->Add(h_dedx);

    TH1::SetDefaultSumw2();
}

bool MCdedx::Process(Long64_t entry)
{
    h_nEvt->Fill(1);
    reader.SetEntry(entry);
    double d_momentum_1 = *momentum_1;
    double d_momentum_2 = *momentum_2;
    double d_dedx_1 = *dedx_1;
    double d_dedx_2 = *dedx_2;

    h_dedx->Fill(d_momentum_1,d_dedx_1);
    h_dedx->Fill(d_momentum_2,d_dedx_2);

    return true;
}

void MCdedx::Terminate()
{
    TCanvas* c1 = new TCanvas("nEvt","nEvt",600,600);
    TCanvas* c2 = new TCanvas("Output","Output",600,600);

    out = new TFile("MCdedx.root","RECREATE");

    c1->cd();
    h_nEvt->Draw();
    h_nEvt->Write();

    c2->cd();

    h_dedx->Draw();
    h_dedx->Write();

    out->Close();
}
