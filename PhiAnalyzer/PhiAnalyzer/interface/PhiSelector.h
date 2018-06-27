#ifndef PHIANALYZER__PHISELECTOR_H
#define PHIANALYZER__PHISELECTOR_H
// system include files
#include <iostream>
#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>

#include "PhiAnalyzer/PhiAnalyzer/interface/utility.h"
#include "PhiAnalyzer/PhiAnalyzer/interface/kaon.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class PhiSelector : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit PhiSelector(const edm::ParameterSet&);
      ~PhiSelector();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      void DeDxFiller(utility::track_combo track_combo_, edm::Handle<edm::ValueMap<reco::DeDxData> > DeDxTrack, TH2D* dedx_p, std::string constraint);
      void FillKaonContainer(utility::track_combo track_combo_, edm::Handle<edm::ValueMap<reco::DeDxData> > DeDxTrack, std::vector<kaon> &pkp, std::vector<kaon> &pkm, std::string constraint);
      void CombinatorialMass(std::vector<kaon> PKp, std::vector<kaon> PKm, TH1D* h_mass_);


      // ----------member data ---------------------------
       edm::EDGetTokenT<reco::TrackCollection> _trkSrc;
       edm::EDGetTokenT<reco::VertexCollection> _vtxSrc;
       edm::EDGetTokenT<edm::ValueMap<reco::DeDxData> > _Dedx_Harmonic2;

       int multMin_;
       int multMax_;
       int nhits_;
       double ZDCA_;
       double XYDCA_;
       double ptCut_;
       double eta_;
       std::string dedxConstraint_;
       bool trackPtCut_;

       /* If you want to create a TTree
        */
       //float H2dedx;
       //float T2dedx;

       TH1D* h_nEvt;
       TH1D* h_mult;
       TH1D* h_mass_Harm;
       TH2D* h_Dedx_p_Harm;
};

#endif
