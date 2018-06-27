#ifndef PHIANALYZER__PHIKAONTREE_H
#define PHIANALYZER__PHIKAONTREE_H

#include <iostream>

#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TTree.h>
#include <TVector3.h>
#include <TLorentzVector.h>

// user include files

#include "PhiAnalyzer/PhiAnalyzer/interface/utility.h"
#include "PhiAnalyzer/PhiAnalyzer/interface/kaon.h"
#include "PhiAnalyzer/PhiAnalyzer/interface/phi.h"

class PhiKaonTree : public edm::one::EDAnalyzer<edm::one::SharedResources> {
    public:
        explicit PhiKaonTree(const edm::ParameterSet&);
        ~PhiKaonTree();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    private:
        virtual void beginJob() override;
        virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
        virtual void endJob() override;

        edm::EDGetTokenT<reco::TrackCollection> _trkSrc;
        edm::EDGetTokenT<reco::VertexCollection> _vertexCollName;
        edm::EDGetTokenT<edm::ValueMap<reco::DeDxData> > _Dedx_Harmonic2;

        TH1D* h_nEvt;
        TH2D* h_masspt;

        TTree* phiKaonTree;

        utility::tree_particle phiKaonCandidate;
};

#endif
