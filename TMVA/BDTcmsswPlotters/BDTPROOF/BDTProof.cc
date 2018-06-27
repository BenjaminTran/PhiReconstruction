#include "BDTProof.h"

void BDTProof::Init(TTree *tree)
{
    reader.SetTree(tree);
    treeName = tree->GetTitle();
}

void BDTProof::SlaveBegin(TTree *tree)
{
    h_nEvt = new TH1D("nEvt","Events",10,0,10);


    for(unsigned i=0; i<BDTval.size(); ++i)
    {
        v_hmass.push_back(new TH1D(Form("mass_%.2f",BDTval[i]),Form("Phi Mass (BDT>%.2f)",BDTval[i]),40,1,1.04));
        //v_hmass.push_back(new TH1D(Form("mass_%d",i),Form("Phi Mass (BDT>%.1f)",BDTval[i]),80,1,1.04));
    }

    GetOutputList()->Add(h_nEvt);

    for(unsigned i=0; i<v_hmass.size(); ++i)
    {
        GetOutputList()->Add(v_hmass[i]);
    }
    TH1::SetDefaultSumw2();
}

bool BDTProof::FillHist(int& counter)
{
    double d_mass = *mass;
    double d_BDT = *BDT;
    if(d_BDT > BDTval[counter])
    {
        for(int i=counter; i>=0; i--)
            v_hmass[counter]->Fill(d_mass);
        return true;
    }
}

bool BDTProof::Process(Long64_t entry)
{
    h_nEvt->Fill(1);
    reader.SetEntry(entry);
    double d_mass = *mass;
    double d_BDT = *BDT;

    for(int i=0; i<BDTval.size(); i++)
    {
        if(d_BDT > BDTval[i])
            v_hmass[i]->Fill(d_mass);
    }

    return true;
}

void BDTProof::Terminate()
{
    TCanvas* c1 = new TCanvas("nEvt","nEvt",600,600);
    TCanvas* c2 = new TCanvas("Output","Output",1200,900);

    out = new TFile(("PhiBDT_30M_rapLT1_pt" + pt_bin_range + ".root").c_str(),"RECREATE");

    TNamed n("pt_range",pt_bin_range.c_str());
    n.Write();

    c1->cd();
    h_nEvt->Draw();
    h_nEvt->Write();

    c2->cd();
    c2->Divide(4,3);

    std::vector<TH1D*> c_mass;

    //TH1D* c_mass[12];

    cout << v_hmass.size() << endl;

    for(unsigned i=0; i<BDTval.size(); i++)
    {
        c_mass.push_back(dynamic_cast<TH1D*>(GetOutputList()->FindObject(Form("mass_%.2f",BDTval[i]))));
        c_mass[i]->Write(Form("mass_%.2f",BDTval[i]));
        c2->cd(i);
        c_mass[i]->SetMarkerStyle(20);
        c_mass[i]->SetMarkerSize(1.1);
        c_mass[i]->Draw("P");
    }
    cout << "NAMED " << treeName << endl;

    out->Close();
}
