void BDTProofStart()
{
    TProof::Open("");
    TChain* chain = new TChain("BDTApp/BDTAppTree_0.5");
    chain->AddFile("/Users/btran/research/RootFiles/Phi/BDT/PhiBDT_30M_rapLT1_180618.root");
    chain->SetProof();
    chain->Process("BDTProof.cc+","");
}
