#ifndef PHIANALYZER__PHIGENMATCH_H
#define PHIANALYZER__PHIGENMATCH_H

#include <TTree.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "PhiAnalyzer/PhiAnalyzer/interface/utility.h"
#include "PhiAnalyzer/PhiAnalyzer/interface/kaon.h"
#include "PhiAnalyzer/PhiAnalyzer/interface/phi.h"

class PhiGenMatch : public edm::EDAnalyzer {
    public:
        explicit PhiGenMatch(const edm::ParameterSet&);
        ~PhiGenMatch();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    private:
        virtual void beginJob() override;
        virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
        virtual void endJob() override;

        edm::EDGetTokenT<reco::GenParticleCollection> _genCollection;
        edm::EDGetTokenT<reco::TrackCollection> _trkSrc;
        edm::EDGetTokenT<reco::VertexCollection> _vertexCollName;
        edm::EDGetTokenT<edm::ValueMap<reco::DeDxData> > _Dedx_Harmonic2;

        TH1D* h_nEvt;
        TH1D* h_phi_yield_rap_1;
        TH1D* h_phi_yield_norap;
        TH1D* h_phid1_mass;
        TH1D* h_phid2_mass;
        TH1D* h_momid;

        TH2D* h_masspt_sig;
        TH2D* h_masspt_bkg;

        edm::Service<TFileService> fs;

        TTree* Signal;
        TTree* Background;

        utility::tree_particle signalStruct;
        utility::tree_particle backgroundStruct;

        std::string dedxConstraint_;

};

#endif
