void MCdedxStart()
{
    TProof::Open("");
    TChain* chain = new TChain("PhiGenMatch/SignalTree");
    chain->AddFile("/Users/btran/research/RootFiles/Phi/TMVA_SignalBackground_MC_pPb.root");
    chain->SetProof();
    chain->Process("MCdedx.cc+","");
}
