#include "PhiAnalyzer/PhiAnalyzer/interface/PhiGenMatch.h"
#include "PhiAnalyzer/PhiAnalyzer/interface/kaon.h"
#include "PhiAnalyzer/PhiAnalyzer/interface/phi.h"

using namespace std;

//Constructors and destructor
PhiGenMatch::PhiGenMatch(const edm::ParameterSet& iConfig)
{
    _genCollection  = consumes<reco::GenParticleCollection>(iConfig.getUntrackedParameter<edm::InputTag>("genCollection"));
    _trkSrc         = consumes<reco::TrackCollection>(iConfig.getUntrackedParameter<edm::InputTag>("trkSrc"));
    _vertexCollName = consumes<reco::VertexCollection>(iConfig.getUntrackedParameter<edm::InputTag>("vertexCollName"));
    dedxConstraint_ = iConfig.getUntrackedParameter<string>("dedxConstraint");
    _Dedx_Harmonic2 = consumes<edm::ValueMap<reco::DeDxData> >(edm::InputTag("dedxHarmonic2"));
}

PhiGenMatch::~PhiGenMatch()
{}

// Member functions

// ------------ method called for each event  ------------
void
PhiGenMatch::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    h_nEvt->Fill(1);

    std::vector<std::vector<kaon> > genDauKaons;
    std::vector<PhiMeson> SignalPhis;
    std::vector<PhiMeson> BackgroundPhis;
    /*
     * For background kaons that did not match. Will be used to construct background phis
     */
    std::vector<kaon> bkgPKp;
    std::vector<kaon> bkgPKm;

    edm::Handle<reco::GenParticleCollection> gencand;
    edm::Handle<reco::TrackCollection> trkSrc;
    edm::Handle<reco::VertexCollection> vertices;
    edm::Handle<edm::ValueMap<reco::DeDxData> > DeDx_Harm;
    try{
        utility::GetCollection<reco::GenParticleCollection>(iEvent,_genCollection,gencand);
        utility::GetCollection<reco::TrackCollection>(iEvent,_trkSrc,trkSrc);
        utility::GetCollection<reco::VertexCollection>(iEvent,_vertexCollName,vertices);
        utility::GetCollection<edm::ValueMap<reco::DeDxData> >(iEvent,_Dedx_Harmonic2,DeDx_Harm);
    }
    catch(const std::invalid_argument& e){
        std::cerr << e.what();
        return;
    }

    utility::myVertex vertex = utility::MyVertexBuild(vertices);


    for(reco::GenParticleCollection::const_iterator gncand = gencand->begin();
            gncand != gencand->end();
            ++gncand)
    {
        int id = gncand->pdgId();
        int mid = 0;
        double rap = gncand->rapidity();
        double eta = gncand->eta();

        if(gncand->numberOfDaughters() > 2) continue;

        if(fabs(id) == 333 && fabs(eta) < 2.4)
        {
            h_phi_yield_norap->Fill(gncand->mass());
            const reco::Candidate *d1 = gncand->daughter(0);
            const reco::Candidate *d2 = gncand->daughter(1);

            if(fabs(d1->pdgId()) != 321 && fabs(d2->pdgId()) != 321) continue;

            std::vector<kaon> tmpKaonDau;
            tmpKaonDau.push_back(kaon(TVector3(d1->px(), d1->py(), d1->pz()), d1->eta(), d1->phi(), d1->charge(), true));
            tmpKaonDau.push_back(kaon(TVector3(d2->px(), d2->py(), d2->pz()), d2->eta(), d2->phi(), d2->charge(), true));

            genDauKaons.push_back(tmpKaonDau);
        }
    }

    int NumberOfGenPhis = genDauKaons.size();

    std::vector<std::vector<kaon>> trackKaonPairs(NumberOfGenPhis);

    //Perform Matching
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

        bool kaonMatched = false;

        for(unsigned i=0; i<genDauKaons.size(); i++)
        {
            std::vector<kaon> genKaonPair = genDauKaons[i];
            for(unsigned j=0; j<genKaonPair.size(); j++)
            {
                try
                {
                    if(K.matched(&(genKaonPair[j])))
                    {
                        trackKaonPairs.at(i).push_back(K);
                        kaonMatched = true;
                        break;
                    }
                }
                catch(const std::invalid_argument& e)
                {
                    std::cerr << e.what();
                }
            }
            if(kaonMatched)
                break;
        }
        if(!kaonMatched)
        {
            if(K.getCharge() == 1)
                bkgPKp.push_back(K);
            else if(K.getCharge() == -1)
                bkgPKm.push_back(K);
        }
    }

    //Build signal Phis
    for(std::vector<kaon> kaonPair : trackKaonPairs)
    {
        //Check if there are two kaons in each container. If not then skip
        if(kaonPair.size() < 2) continue;

        PhiMeson phi = PhiMeson::BuildPhi(kaonPair[0],kaonPair[1],true);

        SignalPhis.push_back(phi);
    }

    //Build background Phis
    BackgroundPhis = PhiMeson::EventCombinatorialPhi(bkgPKp, bkgPKm, true, 100, 1);

    for(PhiMeson phi : SignalPhis)
    {
        utility::FillTreeStruct(signalStruct, &phi);
        Signal->Fill();
        h_masspt_sig->Fill(phi.getMass(),phi.getPt());
    }

    for(PhiMeson phi : BackgroundPhis)
    {
        if(phi.getMass() < 1.0 || phi.getMass() > 1.04) continue;
        utility::FillTreeStruct(backgroundStruct, &phi);
        Background->Fill();
        h_masspt_bkg->Fill(phi.getMass(),phi.getPt());
    }
}

void
PhiGenMatch::beginJob()
{
    TH1::SetDefaultSumw2();

    h_nEvt            = fs->make<TH1D>("h_nEvt","Events",10,0,10);
    h_phi_yield_norap = fs->make<TH1D>("h_phi_yield_norap","Gen Phi Yield no rap",50,1,1.05);
    h_masspt_sig = fs->make<TH2D>("masspt_sig","",200,0.95,1.05,200,0,20);
    h_masspt_bkg = fs->make<TH2D>("masspt_bkg","",100,1.0,1.05,200,0,20);

    Signal     = fs->make<TTree>("SignalTree","SignalTree");
    Background = fs->make<TTree>("BackgroundTree","BackgroundTree");

    Signal->Branch( "mass"       , &signalStruct.mass       );
    Signal->Branch( "momentum"       , &signalStruct.momentum       );
    Signal->Branch( "pt"       , &signalStruct.pt       );
    Signal->Branch( "momentum_1" , &signalStruct.momentum_1 );
    Signal->Branch( "pt_1"       , &signalStruct.pt_1       );
    Signal->Branch( "ptError_1"  , &signalStruct.ptError_1  );
    Signal->Branch( "energy_1"   , &signalStruct.energy_1   );
    Signal->Branch( "dedx_1"     , &signalStruct.dedx_1     );
    Signal->Branch( "charge_1"   , &signalStruct.charge_1   );
    Signal->Branch( "dz_1"       , &signalStruct.dz_1       );
    Signal->Branch( "dzError_1"  , &signalStruct.dzError_1  );
    Signal->Branch( "dxy_1"      , &signalStruct.dxy_1      );
    Signal->Branch( "dxyError_1" , &signalStruct.dxyError_1 );
    Signal->Branch( "eta_1"      , &signalStruct.eta_1      );
    Signal->Branch( "rapidity_1" , &signalStruct.rapidity_1 );
    Signal->Branch( "phi_1"      , &signalStruct.phi_1      );
    Signal->Branch( "vx_1"       , &signalStruct.vx_1       );
    Signal->Branch( "vy_1"       , &signalStruct.vy_1       );
    Signal->Branch( "vz_1"       , &signalStruct.vz_1       );
    Signal->Branch( "px_1"       , &signalStruct.px_1       );
    Signal->Branch( "py_1"       , &signalStruct.py_1       );
    Signal->Branch( "pz_1"       , &signalStruct.pz_1       );
    Signal->Branch( "vzFlip_1"   , &signalStruct.vzFlip_1   );
    Signal->Branch( "chi2_1"     , &signalStruct.chi2_1     );
    Signal->Branch( "chi2norm_1" , &signalStruct.chi2norm_1 );
    Signal->Branch( "ndof_1"     , &signalStruct.ndof_1     );
    Signal->Branch( "nhits_1"    , &signalStruct.nhits_1    );
    Signal->Branch( "momentum_2" , &signalStruct.momentum_2 );
    Signal->Branch( "pt_2"       , &signalStruct.pt_2       );
    Signal->Branch( "ptError_2"  , &signalStruct.ptError_2  );
    Signal->Branch( "energy_2"   , &signalStruct.energy_2   );
    Signal->Branch( "dedx_2"     , &signalStruct.dedx_2     );
    Signal->Branch( "charge_2"   , &signalStruct.charge_2   );
    Signal->Branch( "dz_2"       , &signalStruct.dz_2       );
    Signal->Branch( "dzError_2"  , &signalStruct.dzError_2  );
    Signal->Branch( "dxy_2"      , &signalStruct.dxy_2      );
    Signal->Branch( "dxyError_2" , &signalStruct.dxyError_2 );
    Signal->Branch( "eta_2"      , &signalStruct.eta_2      );
    Signal->Branch( "rapidity_2" , &signalStruct.rapidity_2 );
    Signal->Branch( "phi_2"      , &signalStruct.phi_2      );
    Signal->Branch( "vx_2"       , &signalStruct.vx_2       );
    Signal->Branch( "vy_2"       , &signalStruct.vy_2       );
    Signal->Branch( "vz_2"       , &signalStruct.vz_2       );
    Signal->Branch( "px_2"       , &signalStruct.px_2       );
    Signal->Branch( "py_2"       , &signalStruct.py_2       );
    Signal->Branch( "pz_2"       , &signalStruct.pz_2       );
    Signal->Branch( "vzFlip_2"   , &signalStruct.vzFlip_2   );
    Signal->Branch( "chi2_2"     , &signalStruct.chi2_2     );
    Signal->Branch( "chi2norm_2" , &signalStruct.chi2norm_2 );
    Signal->Branch( "ndof_2"     , &signalStruct.ndof_2     );
    Signal->Branch( "nhits_2"    , &signalStruct.nhits_2    );

    Background->Branch( "mass"       , &backgroundStruct.mass       );
    Background->Branch( "momentum"   , &backgroundStruct.momentum   );
    Background->Branch( "pt"         , &backgroundStruct.pt         );
    Background->Branch( "momentum_1" , &backgroundStruct.momentum_1 );
    Background->Branch( "pt_1"       , &backgroundStruct.pt_1       );
    Background->Branch( "ptError_1"  , &backgroundStruct.ptError_1  );
    Background->Branch( "energy_1"   , &backgroundStruct.energy_1   );
    Background->Branch( "dedx_1"     , &backgroundStruct.dedx_1     );
    Background->Branch( "charge_1"   , &backgroundStruct.charge_1   );
    Background->Branch( "dz_1"       , &backgroundStruct.dz_1       );
    Background->Branch( "dzError_1"  , &backgroundStruct.dzError_1  );
    Background->Branch( "dxy_1"      , &backgroundStruct.dxy_1      );
    Background->Branch( "dxyError_1" , &backgroundStruct.dxyError_1 );
    Background->Branch( "eta_1"      , &backgroundStruct.eta_1      );
    Background->Branch( "rapidity_1" , &backgroundStruct.rapidity_1 );
    Background->Branch( "phi_1"      , &backgroundStruct.phi_1      );
    Background->Branch( "vx_1"       , &backgroundStruct.vx_1       );
    Background->Branch( "vy_1"       , &backgroundStruct.vy_1       );
    Background->Branch( "vz_1"       , &backgroundStruct.vz_1       );
    Background->Branch( "px_1"       , &backgroundStruct.px_1       );
    Background->Branch( "py_1"       , &backgroundStruct.py_1       );
    Background->Branch( "pz_1"       , &backgroundStruct.pz_1       );
    Background->Branch( "vzFlip_1"   , &backgroundStruct.vzFlip_1   );
    Background->Branch( "chi2_1"     , &backgroundStruct.chi2_1     );
    Background->Branch( "chi2norm_1" , &backgroundStruct.chi2norm_1 );
    Background->Branch( "ndof_1"     , &backgroundStruct.ndof_1     );
    Background->Branch( "nhits_1"    , &backgroundStruct.nhits_1    );
    Background->Branch( "momentum_2" , &backgroundStruct.momentum_2 );
    Background->Branch( "pt_2"       , &backgroundStruct.pt_2       );
    Background->Branch( "ptError_2"  , &backgroundStruct.ptError_2  );
    Background->Branch( "energy_2"   , &backgroundStruct.energy_2   );
    Background->Branch( "dedx_2"     , &backgroundStruct.dedx_2     );
    Background->Branch( "charge_2"   , &backgroundStruct.charge_2   );
    Background->Branch( "dz_2"       , &backgroundStruct.dz_2       );
    Background->Branch( "dzError_2"  , &backgroundStruct.dzError_2  );
    Background->Branch( "dxy_2"      , &backgroundStruct.dxy_2      );
    Background->Branch( "dxyError_2" , &backgroundStruct.dxyError_2 );
    Background->Branch( "eta_2"      , &backgroundStruct.eta_2      );
    Background->Branch( "rapidity_2" , &backgroundStruct.rapidity_2 );
    Background->Branch( "phi_2"      , &backgroundStruct.phi_2      );
    Background->Branch( "vx_2"       , &backgroundStruct.vx_2       );
    Background->Branch( "vy_2"       , &backgroundStruct.vy_2       );
    Background->Branch( "vz_2"       , &backgroundStruct.vz_2       );
    Background->Branch( "px_2"       , &backgroundStruct.px_2       );
    Background->Branch( "py_2"       , &backgroundStruct.py_2       );
    Background->Branch( "pz_2"       , &backgroundStruct.pz_2       );
    Background->Branch( "vzFlip_2"   , &backgroundStruct.vzFlip_2   );
    Background->Branch( "chi2_2"     , &backgroundStruct.chi2_2     );
    Background->Branch( "chi2norm_2" , &backgroundStruct.chi2norm_2 );
    Background->Branch( "ndof_2"     , &backgroundStruct.ndof_2     );
    Background->Branch( "nhits_2"    , &backgroundStruct.nhits_2    );
}

void
PhiGenMatch::endJob()
{
}

void
PhiGenMatch::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
    edm::ParameterSetDescription desc;
    desc.addUntracked<edm::InputTag>("genCollection",edm::InputTag("genParticles"));
    desc.addUntracked<edm::InputTag>("trkSrc",edm::InputTag("generalTracks"));
    desc.addUntracked<edm::InputTag>("vertexCollName",edm::InputTag("offlinePrimaryVertices"));
    desc.addUntracked<string>("dedxConstraint","loose");
    descriptions.add("PhiGenMatch",desc);
}

DEFINE_FWK_MODULE(PhiGenMatch);
