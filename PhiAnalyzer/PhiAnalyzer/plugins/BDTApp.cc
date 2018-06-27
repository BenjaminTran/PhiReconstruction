#include "PhiAnalyzer/PhiAnalyzer/interface/BDTApp.h"


using namespace std;

BDTApp::BDTApp(const edm::ParameterSet& iConfig)
{
    _trkSrc = consumes<reco::TrackCollection>(iConfig.getUntrackedParameter<edm::InputTag>("trkSrc"));
    _vertexCollName = consumes<reco::VertexCollection>(iConfig.getUntrackedParameter<edm::InputTag>("vertexCollName"));
    _Dedx_Harmonic2 = consumes<edm::ValueMap<reco::DeDxData> >(edm::InputTag("dedxHarmonic2"));
}

BDTApp::~BDTApp()
{
}


void
BDTApp::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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
    Phis = PhiMeson::EventCombinatorialPhi(bkgPKp,bkgPKm);

    for(PhiMeson phi : Phis)
    {
        if(phi.getMass() < 1.0 || phi.getMass() > 1.04) continue;
        if(fabs(phi.getRapidity()) > 1.0) continue;
        utility::FillTreeStruct(phiKaonCandidate, &phi);
        local_mass = phiKaonCandidate.mass;
        local_pt = phiKaonCandidate.pt;
        local_pt_1 = phiKaonCandidate.pt_1;
        local_ptError_1 = phiKaonCandidate.ptError_1;
        local_dz_1 = phiKaonCandidate.dz_1;
        local_dzError_1 = phiKaonCandidate.dzError_1;
        local_dxy_1 = phiKaonCandidate.dxy_1;
        local_dxyError_1 = phiKaonCandidate.dxyError_1;
        local_rapidity_1 = phiKaonCandidate.rapidity_1;
        local_nhits_1 = phiKaonCandidate.nhits_1;
        local_dedx_1 = phiKaonCandidate.dedx_1;
        local_eta_1 = phiKaonCandidate.eta_1;
        local_momentum_1 = phiKaonCandidate.momentum_1;

        local_pt_2 = phiKaonCandidate.pt_2;
        local_ptError_2 = phiKaonCandidate.ptError_2;
        local_dz_2 = phiKaonCandidate.dz_2;
        local_dzError_2 = phiKaonCandidate.dzError_2;
        local_dxy_2 = phiKaonCandidate.dxy_2;
        local_dxyError_2 = phiKaonCandidate.dxyError_2;
        local_rapidity_2 = phiKaonCandidate.rapidity_2;
        local_nhits_2 = phiKaonCandidate.nhits_2;
        local_dedx_2 = phiKaonCandidate.dedx_2;
        local_eta_2 = phiKaonCandidate.eta_2;
        local_momentum_2 = phiKaonCandidate.momentum_2;

        local_relpterr_1 = fabs(local_pt_1/local_ptError_1);
        local_dca_z1 = fabs(local_dz_1/local_dzError_1);
        local_dca_xy1 = fabs(local_dxy_1/local_dxyError_1);

        local_relpterr_2 = fabs(local_pt_2/local_ptError_2);
        local_dca_z2 = fabs(local_dz_2/local_dzError_2);
        local_dca_xy2 = fabs(local_dxy_2/local_dxyError_2);
        for(unsigned i=0; i<pts.size()-1; i++)
        {
            if(local_pt > pts[i] && local_pt < pts[i+1])
            {
                v_mass.at(i) = local_mass;
                v_pt[i] = local_pt;
                v_BDTresponse[i] = reader->EvaluateMVA(Form("BDT_%.1f",pts[i]));
                v_Trees[i]->Fill();
                break;
            }
        }
        kaon* dau1 = phi.getKaonDau(0);
        kaon* dau2 = phi.getKaonDau(1);
        h_dedxMom->Fill(dau1->getDedx(),dau1->getP());
        h_dedxMom->Fill(dau2->getDedx(),dau2->getP());
        h_masspt->Fill(phi.getMass(),phi.getPt());
    }

}

void
BDTApp::beginJob()
{
    TH1::SetDefaultSumw2();
    edm::Service<TFileService> fs;

    h_nEvt = fs->make<TH1D>("nEvt","",10,0,10);
    h_masspt = fs->make<TH2D>("masspt","",100,1,1.05,200,0,20);
    h_dedxMom = fs->make<TH2D>("dedx","Dedx vs Momentum",200,0,5,150,0,10);

    for(unsigned i=0; i<pts.size()-1; i++)
    {
        // setup path to xml files
        std::ostringstream fipstr;
        std::string prefix = "PhiAnalyzer/PhiAnalyzer/data/Phi_BDT_BDT";
        //std::string bkgtype = "MC";
        std::string bkgtype = "Data";
        std::string suffix = ".weights.xml";
        fipstr << prefix << bkgtype << pts[i] << "-" << pts[i+1] << suffix;
        edm::FileInPath tmpfip(fipstr.str().c_str());

        v_weightFileName.push_back(tmpfip.fullPath());

        // Create Trees
        TTree* tmp = fs->make<TTree>(Form("BDTAppTree_%.1f",pts[i]),Form("BDTAppTree_%.1f",pts[i]));
        v_Trees.push_back(tmp);
        v_Trees[i]->Branch("mass", &(v_mass.at(i)));
        v_Trees[i]->Branch("pt", &(v_pt[i]));
        v_Trees[i]->Branch("BDTresponse", &(v_BDTresponse[i]));
    }

    //
    //Setup reader
    reader = new TMVA::Reader( "!Color:!Silent" );

    reader->AddVariable("fabs(pt_1/ptError_1)",&local_relpterr_1);
    reader->AddVariable("fabs(dz_1/dzError_1)",&local_dca_z1);
    reader->AddVariable("fabs(dxy_1/dxyError_1)",&local_dca_xy1);
    reader->AddVariable("rapidity_1",&local_rapidity_1);
    reader->AddVariable("nhits_1",&local_nhits_1);
    reader->AddVariable("dedx_1",&local_dedx_1);
    reader->AddVariable("eta_1",&local_eta_1);
    reader->AddVariable("momentum_1",&local_momentum_1);

    reader->AddVariable("fabs(pt_2/ptError_2)",&local_relpterr_2);
    reader->AddVariable("fabs(dz_2/dzError_2)",&local_dca_z2);
    reader->AddVariable("fabs(dxy_2/dxyError_2)",&local_dca_xy2);
    reader->AddVariable("rapidity_2",&local_rapidity_2);
    reader->AddVariable("nhits_2",&local_nhits_2);
    reader->AddVariable("dedx_2",&local_dedx_2);
    reader->AddVariable("eta_2",&local_eta_2);
    reader->AddVariable("momentum_2",&local_momentum_2);

    for(unsigned i=0; i<pts.size()-1; i++)
    {
        reader->BookMVA(Form("BDT_%.1f",pts[i]),v_weightFileName[i].c_str());
    }
}

void
BDTApp::endJob()
{
}

void
BDTApp::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
    edm::ParameterSetDescription desc;
    desc.addUntracked<edm::InputTag>("trkSrc",edm::InputTag("generalTracks"));
    desc.addUntracked<edm::InputTag>("vertexCollName",edm::InputTag("offlinePrimaryVertices"));
    descriptions.add("BDTApp",desc);
}

DEFINE_FWK_MODULE(BDTApp);
