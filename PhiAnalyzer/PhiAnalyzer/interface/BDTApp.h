#ifndef PHIANALYZER__PHIBDTAPP_H
#define PHIANALYZER__PHIBDTAPP_H

#include <iostream>

#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TTree.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TFile.h>
#include "TMVA/Reader.h"

// user include files

#include "PhiAnalyzer/PhiAnalyzer/interface/utility.h"
#include "PhiAnalyzer/PhiAnalyzer/interface/kaon.h"
#include "PhiAnalyzer/PhiAnalyzer/interface/phi.h"



class BDTApp : public edm::one::EDAnalyzer<edm::one::SharedResources> {
    public:
        explicit BDTApp(const edm::ParameterSet&);
        ~BDTApp();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    private:
        virtual void beginJob() override;
        virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
        virtual void endJob() override;

        edm::EDGetTokenT<reco::TrackCollection> _trkSrc;
        edm::EDGetTokenT<reco::VertexCollection> _vertexCollName;
        edm::EDGetTokenT<edm::ValueMap<reco::DeDxData> > _Dedx_Harmonic2;

        TMVA::Reader* reader;
        float local_relpterr_1    , local_relpterr_2;
        float local_dca_z1     , local_dca_z2;
        float local_dca_xy1    , local_dca_xy2;
        float local_rapidity_1 , local_rapidity_2;
        float local_nhits_1    , local_nhits_2;
        float local_dedx_1     , local_dedx_2;
        float local_eta_1      , local_eta_2;
        float local_momentum_1 , local_momentum_2;

        float local_pt_1 , local_pt_2;
        float local_ptError_1, local_ptError_2;
        float local_dzError_1 , local_dzError_2;
        float local_dxyError_1 , local_dxyError_2;
        float local_dz_1, local_dz_2;
        float local_dxy_1, local_dxy_2;

        float local_pt;
        float local_mass;

        TH1D* h_nEvt;
        TH1D* h_BDTresponse1;
        TH1D* h_BDTresponse2;
        TH2D* h_masspt;
        TH2D* h_dedxMom;

        //std::vector<TH2D*> v_hdedxMom;
        std::vector<std::string> v_weightFileName;
        //std::vector<double> pts = {0,0.5,1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6}; // For MC modeled bkg
        std::vector<double> pts = {0,0.5,1,1.5,2,2.5,3,3.5,4}; // For data modeled bkg

        std::vector<double> v_mass = std::vector<double> (pts.size()-1);
        std::vector<double> v_BDTresponse = std::vector<double> (pts.size()-1);
        std::vector<double> v_pt = std::vector<double> (pts.size()-1);

        std::vector<TTree*> v_Trees;

        utility::tree_particle phiKaonCandidate;
};

#endif
