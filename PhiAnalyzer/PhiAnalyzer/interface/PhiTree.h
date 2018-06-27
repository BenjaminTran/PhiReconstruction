#ifndef PHIANALYZER__PHITree_H
#define PHIANALYZER__PHITree_H
// system include files
#include <iostream>

#include <TTree.h>
#include <TMath.h>
#include <TH1D.h>

// user include files

#include "PhiAnalyzer/PhiAnalyzer/interface/utility.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class PhiTree : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit PhiTree(const edm::ParameterSet&);
      ~PhiTree();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      struct track_particle{
          std::vector<float> momentum;
          std::vector<float> pt;
          std::vector<float> ptError;
          std::vector<float> energy;
          std::vector<float> dedx;
          std::vector<float> charge;
          std::vector<float> dz;
          std::vector<float> dzError;
          std::vector<float> dxy;
          std::vector<float> dxyError;
          std::vector<float> eta;
          std::vector<float> rapidity;
          std::vector<float> phi;
          std::vector<float> vx;
          std::vector<float> vy;
          std::vector<float> vz;
          std::vector<float> px;
          std::vector<float> py;
          std::vector<float> pz;
          std::vector<float> vzFlip;
          std::vector<float> chi2;
          std::vector<float> chi2norm;
          std::vector<float> ndof;
          std::vector<float> nhits;
      };

      // ----------member data ---------------------------
       edm::EDGetTokenT<reco::TrackCollection> _trkSrc;
       edm::EDGetTokenT<reco::VertexCollection> _vtxSrc;
       edm::EDGetTokenT<edm::ValueMap<reco::DeDxData> > _Dedx_Harmonic2;

       int multMin_;
       int multMax_;

       int mult;
       int multRaw;
       std::string dedxConstraint_;

       TTree* trackTree;

       TH1D* h_nEvt;
       TH1D* h_mult;
       TH1D* h_multRaw;

       track_particle track_particle_;
};

#endif
