void MCdedxStart()
{
    TProof::Open("");
    TChain* chain = new TChain("PhiGenMatch/SignalTree");
    chain->AddFile("/Users/btran/research/RootFiles/Phi/BDT/PhiGenMatch_improved.root");
    chain->SetProof();
    chain->Process("MCdedx.cc+","");
}
