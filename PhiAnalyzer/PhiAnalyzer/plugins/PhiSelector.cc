// -*- C++ -*-
//
// Package:    PhiSelector/PhiSelector
// Class:      PhiSelector
//
/**\class PhiSelector PhiSelector.cc PhiSelector/PhiSelector/plugins/PhiSelector.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Benjamin Tran
//         Created:  Tue, 30 Jan 2018 19:09:22 GMT
//
//

#include "PhiAnalyzer/PhiAnalyzer/interface/PhiSelector.h"
#include "PhiAnalyzer/PhiAnalyzer/interface/kaon.h"
#include "PhiAnalyzer/PhiAnalyzer/interface/utility.h"

using namespace std;
using namespace utility;

//
// constructors and destructor
//
PhiSelector::PhiSelector(const edm::ParameterSet& iConfig)
{
    multMin_ = iConfig.getUntrackedParameter<int>("multMin");
    multMax_ = iConfig.getUntrackedParameter<int>("multMax");
    nhits_ = iConfig.getUntrackedParameter<int>("nhits");
    ptCut_ = iConfig.getUntrackedParameter<double>("ptCut");
    ZDCA_ = iConfig.getUntrackedParameter<double>("ZDCA");
    XYDCA_ = iConfig.getUntrackedParameter<double>("XYDCA");
    eta_ = iConfig.getUntrackedParameter<double>("eta");
    trackPtCut_ = iConfig.getUntrackedParameter<bool>("trackPtCut");
    dedxConstraint_ = iConfig.getUntrackedParameter<string>("dedxConstraint");
    _trkSrc = consumes<reco::TrackCollection>(iConfig.getUntrackedParameter<edm::InputTag>("trkSrc"));
    _vtxSrc = consumes<reco::VertexCollection>(iConfig.getUntrackedParameter<edm::InputTag>("vtxSrc"));
    _Dedx_Harmonic2 = consumes<edm::ValueMap<reco::DeDxData> >(edm::InputTag("dedxHarmonic2"));
}


PhiSelector::~PhiSelector()
{
}


//
// member functions
//

void
PhiSelector::DeDxFiller(utility::track_combo track_combo_, edm::Handle<edm::ValueMap<reco::DeDxData> > DeDxTrack, TH2D* dedx_p, std::string constraint )
{
        double dedx     = -999.9;
        double momentum = track_combo_.track->p();
        dedx = utility::getDeDx(track_combo_,DeDxTrack);
        try
        {
            if(utility::AcceptTrackDeDx(track_combo_,DeDxTrack,constraint))
                dedx_p->Fill(momentum,dedx);
        }
        catch( const std::invalid_argument& e)
        {
            std::cerr << e.what();
        }

}

void
PhiSelector::FillKaonContainer(utility::track_combo track_combo_, edm::Handle<edm::ValueMap<reco::DeDxData> > DeDxTrack, std::vector<kaon> &pkp, std::vector<kaon> &pkm, std::string constraint)
{
    try
    {
        if(!utility::AcceptTrackDeDx(track_combo_, DeDxTrack,constraint))
            return;
    }
    catch( const std::invalid_argument& e)
    {
        std::cerr << e.what();
    }

    double energy = sqrt(TMath::Power(utility::kaonMass,2) + TMath::Power(track_combo_.track->p(),2));
    TVector3 momentum(track_combo_.track->px(), track_combo_.track->py(), track_combo_.track->pz());
    kaon pk(momentum, track_combo_.track->eta(), track_combo_.track->phi(),  track_combo_.track->charge(), utility::getDeDx(track_combo_,DeDxTrack));

    //positive kaons
    if(track_combo_.track->charge() == 1)
        pkp.push_back(pk);

    //negative kaons
    if(track_combo_.track->charge() == -1)
        pkm.push_back(pk);
}

void
PhiSelector::CombinatorialMass(std::vector<kaon> PKp, std::vector<kaon> PKm, TH1D* h_mass_)
{
    for(kaon Pkp : PKp)
    {
        for(kaon Pkm : PKm)
        {
            TVector3 dau1p = Pkp.getMomentumVect();
            TVector3 dau2p = Pkm.getMomentumVect();
            TVector3 dauPsum(dau1p + dau2p);
            double mass = sqrt(TMath::Power(Pkp.getEnergy()+Pkm.getEnergy(),2) - dauPsum.Mag2());
            h_mass_->Fill(mass);
        }
    }
}


// ------------ method called for each event  ------------
void
PhiSelector::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   //Vectors to hold kaons to perform combinatorial mass reconstruction. Following PEN naming scheme
   std::vector<kaon> PKp_Harm; //Positive charged
   std::vector<kaon> PKm_Harm; //Negative charged

   h_nEvt->Fill(1);

   edm::Handle<reco::TrackCollection> tracks;
   iEvent.getByToken(_trkSrc,tracks);

   edm::Handle<reco::VertexCollection> vertices;
   iEvent.getByToken(_vtxSrc,vertices);

   edm::Handle<edm::ValueMap<reco::DeDxData> > DeDx_Harm;
   iEvent.getByToken(_Dedx_Harmonic2,DeDx_Harm);
   if(!DeDx_Harm.isValid())
   {
       cout << "Bad DeDx collection" << endl;
       return;
   }

   utility::myVertex vertex = utility::MyVertexBuild(vertices);

   // Multiplicity selection
   int mult = utility::Multiplicity(tracks,vertices);
   h_mult->Fill(mult);

   for(reco::TrackCollection::const_iterator it = tracks->begin();
           it != tracks->end();
           ++it)
   {
       /*Use only tracks that pass the following kinematic cuts:
        * DCA
        * eta
        * nhits
        */
       if(!utility::SelectionCut(it,vertex,trackPtCut_,ZDCA_,XYDCA_,eta_,ptCut_,nhits_)) continue;

       reco::TrackRef track_ref = reco::TrackRef(tracks,it - tracks->begin());
       utility::track_combo track_bundle(it,track_ref);

       //Fill in the dEdx histograms
       DeDxFiller(track_bundle,DeDx_Harm,h_Dedx_p_Harm,dedxConstraint_);

       // Make the vector of kaons to calculate invariant mass at the end
       FillKaonContainer(track_bundle,DeDx_Harm,PKp_Harm,PKm_Harm,dedxConstraint_);
   }
   CombinatorialMass(PKp_Harm,PKm_Harm,h_mass_Harm);
}


// ------------ method called once each job just before starting event loop  ------------
void
PhiSelector::beginJob()
{
    TH1::SetDefaultSumw2();
    edm::Service<TFileService> fs;

    h_nEvt = fs->make<TH1D>("nEvt","",10,0,10);
    h_mult = fs->make<TH1D>("mult","",400,0,400);
    h_mass_Harm = fs->make<TH1D>("mass_harm",";GeV",80,1.00,1.04);
    h_Dedx_p_Harm = fs->make<TH2D>("Dedx_harm",";p;dE/dx",200,0,5,250,0,15);
}

// ------------ method called once each job just after ending the event loop  ------------
void
PhiSelector::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PhiSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {

  edm::ParameterSetDescription desc;
  desc.addUntracked<edm::InputTag>("trkSrc",edm::InputTag("generalTracks"));
  desc.addUntracked<edm::InputTag>("vtxSrc",edm::InputTag("offlinePrimaryVertices"));
  desc.addUntracked<int>("multMin",0);
  desc.addUntracked<int>("multMax",999);
  desc.addUntracked<int>("nhits",5);
  desc.addUntracked<bool>("trackPtCut",false);
  desc.addUntracked<double>("ptCut",0);
  desc.addUntracked<double>("ZDCA",1);
  desc.addUntracked<double>("XYDCA",1);
  desc.addUntracked<double>("eta",2.4);
  desc.addUntracked<string>("dedxConstraint","default"); //tight | default | loose
  descriptions.add("PhiSelector",desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PhiSelector);
