#include "PhiAnalyzer/PhiAnalyzer/interface/PhiKaonTree.h"


using namespace std;

PhiKaonTree::PhiKaonTree(const edm::ParameterSet& iConfig)
{
    _trkSrc = consumes<reco::TrackCollection>(iConfig.getUntrackedParameter<edm::InputTag>("trkSrc"));
    _vertexCollName = consumes<reco::VertexCollection>(iConfig.getUntrackedParameter<edm::InputTag>("vertexCollName"));
    _Dedx_Harmonic2 = consumes<edm::ValueMap<reco::DeDxData> >(edm::InputTag("dedxHarmonic2"));
}

PhiKaonTree::~PhiKaonTree()
{
}


void
PhiKaonTree::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    h_nEvt->Fill(1);

    std::vector<PhiMeson> Phis;
    std::vector<kaon> bkgPKp;
    std::vector<kaon> bkgPKm;

    edm::Handle<reco::TrackCollection> trkSrc;
    edm::Handle<reco::VertexCollection> vertices;
    edm::Handle<edm::ValueMap<reco::DeDxData> > DeDx_Harm;
    try{
        utility::GetCollection<reco::TrackCollection>(iEvent,_trkSrc,trkSrc);
        utility::GetCollection<reco::VertexCollection>(iEvent,_vertexCollName,vertices);
        utility::GetCollection<edm::ValueMap<reco::DeDxData> >(iEvent,_Dedx_Harmonic2,DeDx_Harm);
    }
    catch(const std::invalid_argument& e){
        std::cerr << e.what();
        return;
    }

    utility::myVertex vertex = utility::MyVertexBuild(vertices);

    for(reco::TrackCollection::const_iterator trk = trkSrc->begin();
            trk != trkSrc->end();
            ++trk)
    {
        //if(!utility::SelectionCut(trk,vertex,false,1.0,1.0,2.4,0,5))
        if(!trk->quality(reco::TrackBase::highPurity)) continue;
        reco::TrackRef track_ref = reco::TrackRef(trkSrc,trk - trkSrc->begin());
        utility::track_combo track_bundle(trk, track_ref);

        kaon::cutVariables kaonCutVariables_;
        kaonCutVariables_.ptError  = trk->ptError();
        kaonCutVariables_.dz       = trk->dz(vertex.bestvtx);
        kaonCutVariables_.dzError  = sqrt(TMath::Power(trk->dzError(),2) + TMath::Power(vertex.bestvzError,2));
        kaonCutVariables_.dxy      = trk->dxy(vertex.bestvtx);
        kaonCutVariables_.dxyError = sqrt(TMath::Power(trk->d0Error(),2) + vertex.bestvxError*vertex.bestvyError);
        kaonCutVariables_.nhits    = trk->numberOfValidHits();
        kaonCutVariables_.chi2     = trk->chi2();
        kaonCutVariables_.chi2norm = trk->normalizedChi2();
        kaonCutVariables_.vx       = trk->vx();
        kaonCutVariables_.vy       = trk->vy();
        kaonCutVariables_.vz       = trk->vz();
        kaonCutVariables_.ndof     = trk->ndof();

        kaon K(TVector3(trk->px(), trk->py(), trk->pz()), trk->eta(), trk->phi(), kaonCutVariables_, trk->charge(), utility::getDeDx(track_bundle, DeDx_Harm), false);

        if(K.getCharge() == 1)
            bkgPKp.push_back(K);
        else if(K.getCharge() == -1)
            bkgPKm.push_back(K);
    }

    //Build Phis
    //Phis = PhiMeson::EventCombinatorialPhi(bkgPKp,bkgPKm,true,100,50);
    Phis = PhiMeson::EventCombinatorialPhi(bkgPKp,bkgPKm);

    for(PhiMeson phi : Phis)
    {
        if(phi.getMass() < 1.0 || phi.getMass() > 1.04) continue;
        ////if(phi.getMass() < 1.028 && phi.getMass() > 1.012)
        //{
            //h_masspt->Fill(phi.getMass(),phi.getPt());
            //continue;
        //}
        //if(phi.getMass() < 1.025 && phi.getMass() > 1.015) continue;
        //if(phi.getMass() < 1.023 && phi.getMass() > 1.017) continue;
        utility::FillTreeStruct(phiKaonCandidate, &phi);
        phiKaonTree->Fill();
        h_masspt->Fill(phi.getMass(),phi.getPt());
    }
}

void
PhiKaonTree::beginJob()
{
    TH1::SetDefaultSumw2();
    edm::Service<TFileService> fs;

    h_nEvt = fs->make<TH1D>("nEvt","",10,0,10);
    h_masspt = fs->make<TH2D>("masspt","",100,1,1.05,200,0,20);
    phiKaonTree = fs->make<TTree>("PhiKaonTree","PhiKaonTree");

    phiKaonTree->Branch( "mass"       , &phiKaonCandidate.mass       );
    phiKaonTree->Branch( "momentum" , &phiKaonCandidate.momentum);
    phiKaonTree->Branch( "pt"       , &phiKaonCandidate.pt       );
    phiKaonTree->Branch( "momentum_1" , &phiKaonCandidate.momentum_1 );
    phiKaonTree->Branch( "pt_1"       , &phiKaonCandidate.pt_1       );
    phiKaonTree->Branch( "ptError_1"  , &phiKaonCandidate.ptError_1  );
    //phiKaonTree->Branch( "energy_1"   , &phiKaonCandidate.energy_1   );
    phiKaonTree->Branch( "dedx_1"     , &phiKaonCandidate.dedx_1     );
    //phiKaonTree->Branch( "charge_1"   , &phiKaonCandidate.charge_1   );
    phiKaonTree->Branch( "dz_1"       , &phiKaonCandidate.dz_1       );
    phiKaonTree->Branch( "dzError_1"  , &phiKaonCandidate.dzError_1  );
    phiKaonTree->Branch( "dxy_1"      , &phiKaonCandidate.dxy_1      );
    phiKaonTree->Branch( "dxyError_1" , &phiKaonCandidate.dxyError_1 );
    phiKaonTree->Branch( "eta_1"      , &phiKaonCandidate.eta_1      );
    phiKaonTree->Branch( "rapidity_1" , &phiKaonCandidate.rapidity_1 );
    //phiKaonTree->Branch( "phi_1"      , &phiKaonCandidate.phi_1      );
    //phiKaonTree->Branch( "vx_1"       , &phiKaonCandidate.vx_1       );
    //phiKaonTree->Branch( "vy_1"       , &phiKaonCandidate.vy_1       );
    //phiKaonTree->Branch( "vz_1"       , &phiKaonCandidate.vz_1       );
    //phiKaonTree->Branch( "px_1"       , &phiKaonCandidate.px_1       );
    //phiKaonTree->Branch( "py_1"       , &phiKaonCandidate.py_1       );
    //phiKaonTree->Branch( "pz_1"       , &phiKaonCandidate.pz_1       );
    //phiKaonTree->Branch( "vzFlip_1"   , &phiKaonCandidate.vzFlip_1   );
    //phiKaonTree->Branch( "chi2_1"     , &phiKaonCandidate.chi2_1     );
    //phiKaonTree->Branch( "chi2norm_1" , &phiKaonCandidate.chi2norm_1 );
    //phiKaonTree->Branch( "ndof_1"     , &phiKaonCandidate.ndof_1     );
    phiKaonTree->Branch( "nhits_1"    , &phiKaonCandidate.nhits_1    );
    phiKaonTree->Branch( "momentum_2" , &phiKaonCandidate.momentum_2 );
    phiKaonTree->Branch( "pt_2"       , &phiKaonCandidate.pt_2       );
    phiKaonTree->Branch( "ptError_2"  , &phiKaonCandidate.ptError_2  );
    //phiKaonTree->Branch( "energy_2"   , &phiKaonCandidate.energy_2   );
    phiKaonTree->Branch( "dedx_2"     , &phiKaonCandidate.dedx_2     );
    //phiKaonTree->Branch( "charge_2"   , &phiKaonCandidate.charge_2   );
    phiKaonTree->Branch( "dz_2"       , &phiKaonCandidate.dz_2       );
    phiKaonTree->Branch( "dzError_2"  , &phiKaonCandidate.dzError_2  );
    phiKaonTree->Branch( "dxy_2"      , &phiKaonCandidate.dxy_2      );
    phiKaonTree->Branch( "dxyError_2" , &phiKaonCandidate.dxyError_2 );
    phiKaonTree->Branch( "eta_2"      , &phiKaonCandidate.eta_2      );
    phiKaonTree->Branch( "rapidity_2" , &phiKaonCandidate.rapidity_2 );
    //phiKaonTree->Branch( "phi_2"      , &phiKaonCandidate.phi_2      );
    //phiKaonTree->Branch( "vx_2"       , &phiKaonCandidate.vx_2       );
    //phiKaonTree->Branch( "vy_2"       , &phiKaonCandidate.vy_2       );
    //phiKaonTree->Branch( "vz_2"       , &phiKaonCandidate.vz_2       );
    //phiKaonTree->Branch( "px_2"       , &phiKaonCandidate.px_2       );
    //phiKaonTree->Branch( "py_2"       , &phiKaonCandidate.py_2       );
    //phiKaonTree->Branch( "pz_2"       , &phiKaonCandidate.pz_2       );
    //phiKaonTree->Branch( "vzFlip_2"   , &phiKaonCandidate.vzFlip_2   );
    //phiKaonTree->Branch( "chi2_2"     , &phiKaonCandidate.chi2_2     );
    //phiKaonTree->Branch( "chi2norm_2" , &phiKaonCandidate.chi2norm_2 );
    //phiKaonTree->Branch( "ndof_2"     , &phiKaonCandidate.ndof_2     );
    phiKaonTree->Branch( "nhits_2"    , &phiKaonCandidate.nhits_2    );

}

void
PhiKaonTree::endJob()
{}

void
PhiKaonTree::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
    edm::ParameterSetDescription desc;
    desc.addUntracked<edm::InputTag>("trkSrc",edm::InputTag("generalTracks"));
    desc.addUntracked<edm::InputTag>("vertexCollName",edm::InputTag("offlinePrimaryVertices"));
    descriptions.add("PhiKaonTree",desc);
}

DEFINE_FWK_MODULE(PhiKaonTree);
