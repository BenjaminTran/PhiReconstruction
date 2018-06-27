#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

void PhiBDT(double pt_low = 0, double pt_high = 100, int sigTrainTest=0, int bkgTrainTest=30000, bool openGui = true)
{
    TTree* signalTree = 0;
    TTree* backgroundTree = 0;
    TTree* singleTree = 0;

    std::ostringstream strs;

    bool useSingleTree = false;
    double signalWeight = 1.0;
    double backgroundWeight = 1.0;

    strs << "PhiBDT_DataBkg_ptIndependentMassCut_pt" << pt_low << "-" << pt_high << ".root";

    TMVA::Tools::Instance();
    TString outfileName(strs.str().c_str());

    TFile* input = TFile::Open("/Users/btran/research/RootFiles/Phi/TMVA_SignalBackground_MC_pPb.root");
    TFile* input_bkg = TFile::Open("/Users/btran/research/RootFiles/Phi/BDT/PhiBDT_DataBkg_1-3_240618.root");
    TFile* outputFile = TFile::Open(outfileName,"RECREATE");

    strs.str("");
    strs << "dataset_pt" << pt_low << "-" << pt_high;

    TMVA::DataLoader *dataloader = new TMVA::DataLoader(strs.str().c_str());

    TMVA::Factory *factory = new TMVA::Factory("Phi_BDT", outputFile, "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
    //TMVA::Factory *factory = new TMVA::Factory("Phi_BDT", outputFile, "!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification" );

    if(useSingleTree)
    {
        input->GetObject("PhiGenMatch/SignalTree",singleTree);
        TCut signalCut_high = "mass < 1.025";
        TCut signalCut_low = "mass > 1.0125";
        TCut backgroundCut_high = "mass > 1.025";
        TCut backgroundCut_low = "mass < 1.0125";
        dataloader->SetInputTrees(singleTree, signalCut_high && signalCut_low, backgroundCut_low || backgroundCut_high);
    }
    else
    {
        input->GetObject("PhiGenMatch/SignalTree",signalTree);
        //input->GetObject("PhiGenMatch/BackgroundTree",backgroundTree);
        //
        input_bkg->GetObject("PhiKaonTree/PhiKaonTree",backgroundTree);
        cout << "Trees acquired" << endl;

        /*
         * Disable unused branches
         */
        //signalTree->SetBranchStatus("*",0);
        signalTree->SetBranchStatus("momentum",0);
        //signalTree->SetBranchStatus("pt_1",1);
        //signalTree->SetBranchStatus("pt_2",1);
        //signalTree->SetBranchStatus("ptError_1",1);
        //signalTree->SetBranchStatus("ptError_2",1);
        //signalTree->SetBranchStatus("dz_1",1);
        //signalTree->SetBranchStatus("dz_2",1);
        //signalTree->SetBranchStatus("dzError_1",1);
        //signalTree->SetBranchStatus("dzError_2",1);
        //signalTree->SetBranchStatus("dxy_1",1);
        //signalTree->SetBranchStatus("dxy_2",1);
        //signalTree->SetBranchStatus("dxyError_1",1);
        //signalTree->SetBranchStatus("dxyError_2",1);
        //signalTree->SetBranchStatus("rapidity_1",1);
        //signalTree->SetBranchStatus("rapidity_2",1);
        //signalTree->SetBranchStatus("nhits_1",1);
        //signalTree->SetBranchStatus("nhits_2",1);
        //signalTree->SetBranchStatus("dedx_1",1);
        //signalTree->SetBranchStatus("dedx_2",1);
        //signalTree->SetBranchStatus("eta_1",1);
        //signalTree->SetBranchStatus("eta_2",1);
        //signalTree->SetBranchStatus("momentum_1",1);
        //signalTree->SetBranchStatus("momentum_2",1);

        //backgroundTree->SetBranchStatus("*",0);
        backgroundTree->SetBranchStatus("momentum",0);
        //backgroundTree->SetBranchStatus("mass",1);
        //backgroundTree->SetBranchStatus("pt_1",1);
        //backgroundTree->SetBranchStatus("pt_2",1);
        //backgroundTree->SetBranchStatus("ptError_1",1);
        //backgroundTree->SetBranchStatus("ptError_2",1);
        //backgroundTree->SetBranchStatus("dz_1",1);
        //backgroundTree->SetBranchStatus("dz_2",1);
        //backgroundTree->SetBranchStatus("dzError_1",1);
        //backgroundTree->SetBranchStatus("dzError_2",1);
        //backgroundTree->SetBranchStatus("dxy_1",1);
        //backgroundTree->SetBranchStatus("dxy_2",1);
        //backgroundTree->SetBranchStatus("dxyError_1",1);
        //backgroundTree->SetBranchStatus("dxyError_2",1);
        //backgroundTree->SetBranchStatus("rapidity_1",1);
        //backgroundTree->SetBranchStatus("rapidity_2",1);
        //backgroundTree->SetBranchStatus("nhits_1",1);
        //backgroundTree->SetBranchStatus("nhits_2",1);
        //backgroundTree->SetBranchStatus("dedx_1",1);
        //backgroundTree->SetBranchStatus("dedx_2",1);
        //backgroundTree->SetBranchStatus("eta_1",1);
        //backgroundTree->SetBranchStatus("eta_2",1);
        //backgroundTree->SetBranchStatus("momentum_1",1);
        //backgroundTree->SetBranchStatus("momentum_2",1);

        cout << "Branches selected" << endl;

        //input_bkg->GetObject("PhiKaonTree/PhiKaonTree",backgroundTree);
        //TCut ptCut = "sqrt(TMath::Power(momentum_1,2) + TMath::Power(momentum_2,2) + 2*(px_1*px_2 + py_1*py_2 + pz_1*pz_2)) > pt_low &&\
                      //sqrt(TMath::Power(momentum_1,2) + TMath::Power(momentum_2,2) + 2*(px_1*px_2 + py_1*py_2 + pz_1*pz_2)) < pt_high";
        //TCut ptCut = "momentum_1 > pt_low";
        TCut bkgCut = "mass < 1.017 || mass > 1.023";
        //dataloader->AddTree(signalTree,"Signal",signalWeight,ptCut);
        //dataloader->AddTree(backgroundTree,"Background",backgroundWeight,ptCut);
        //dataloader->AddTree(signalTree,"Signal",signalWeight);
        dataloader->AddSignalTree(signalTree, signalWeight);
        cout << "Signal Tree Added" << endl;
        dataloader->AddTree(backgroundTree,"Background",backgroundWeight,bkgCut);
        cout << "Background Tree Added" << endl;
        //dataloader->AddBackgroundTree(backgroundTree, backgroundWeight);
    }


    dataloader->AddVariable("fabs(pt_1/ptError_1)", "Relative Pt Error 1", 'F');
    dataloader->AddVariable("fabs(dz_1/dzError_1)", "DCA z1", 'F');
    dataloader->AddVariable("fabs(dxy_1/dxyError_1)", "DCA xy1", 'F');
    dataloader->AddVariable("rapidity_1", 'F');
    dataloader->AddVariable("nhits_1", 'I');
    dataloader->AddVariable("dedx_1", 'F');
    dataloader->AddVariable("eta_1", 'F');
    dataloader->AddVariable("momentum_1", 'F');

    dataloader->AddVariable("fabs(pt_2/ptError_2)", "Relative Pt Error 2", 'F');
    dataloader->AddVariable("fabs(dz_2/dzError_2)", "DCA z2", 'F');
    dataloader->AddVariable("fabs(dxy_2/dxyError_2)", "DCA xy2", 'F');
    dataloader->AddVariable("rapidity_2", 'F');
    dataloader->AddVariable("nhits_2", 'I');
    dataloader->AddVariable("dedx_2", 'F');
    dataloader->AddVariable("eta_2", 'F');
    dataloader->AddVariable("momentum_2", 'F');


    //dataloader->AddSpectator("mass", 'F');

    //dataloader->SetBackgroundWeightExpression("weight");

    //TCut preCut = "mass > 1.015 && mass < 1.025";
    TCut preCut = "";
    strs.str("");
    //strs << "pt > " << pt_low << " && pt < " << pt_high;
    strs << "pt > " << pt_low << " && pt < " << pt_high << " && pt_1 > 0.5" << " && pt_2 > 0.5"; // For additional Pt Cut on daughters

    TCut ptCut = strs.str().c_str();
    strs.str("");
    strs << "nTrain_Signal=" << sigTrainTest << ":nTrain_Background=" << bkgTrainTest << ":nTest_Signal=" << sigTrainTest << ":nTest_Background=" << bkgTrainTest << ":SplitMode=Random:NormMode=NumEvents:V";
    dataloader->PrepareTrainingAndTestTree(ptCut, strs.str().c_str());
    //dataloader->PrepareTrainingAndTestTree(ptCut, "nTrain_Signal=80:nTrain_Background=80:nTest_Signal=120:nTest_Background=120:SplitMode=Random:NormMode=NumEvents:V" );


    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT",
            "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

    //factory->BookMethod( dataloader, TMVA::Types::kCuts, "Cuts",
                       //"!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );

    //factory->BookMethod( dataloader, TMVA::Types::kCuts, "CutsSA",
                        //"!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    outputFile->Close();

    if(openGui)
    {
        TMVA::TMVAGui( outfileName );
    }
}
