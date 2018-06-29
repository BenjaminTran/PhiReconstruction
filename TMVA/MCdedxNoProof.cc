void MCdedxNoProof()
{
    TFile* f = TFile::Open("/Users/btran/research/RootFiles/Phi/TMVA_SignalBackground_MC_pPb.root");

    TTreeReader reader("PhiGenMatch/SignalTree",f);

    TTreeReaderValue<double> momentum_1(reader,"momentum_1");
    TTreeReaderValue<double> momentum_2(reader,"momentum_2");
    TTreeReaderValue<double> dedx_1(reader,"dedx_1");
    TTreeReaderValue<double> dedx_2(reader,"dedx_2");

    TH2D* h_dedx =  new TH2D("dedx","dedx v momentum",200,0,20,300,0,15);

    while(reader.Next())
    {
        double d_momentum_1 = *momentum_1;
        double d_momentum_2 = *momentum_2;
        double d_dedx_1 = *dedx_1;
        double d_dedx_2 = *dedx_2;

        h_dedx->Fill(d_momentum_1,d_dedx_1);
        h_dedx->Fill(d_momentum_2,d_dedx_2);
    }

    TFile fout("MCdedx.root","RECREATE");
    h_dedx->Write();
    h_dedx->Draw();

    fout.Close();
}
