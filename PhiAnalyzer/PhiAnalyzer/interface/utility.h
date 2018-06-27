#ifndef UTIL_H
#define UTIL_H
// system include files
#include <iostream>

#include <TMath.h>
#include <TFile.h>

// user include files
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/TrackReco/interface/Track.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/TrackReco/interface/DeDxData.h"

// User classes

class PhiMeson;
namespace utility
{
    extern double const kaonMass;

    struct myVertex{
        double bestvx = -999;
        double bestvy = -999;
        double bestvz = -999;
        double bestvxError = -999;
        double bestvyError = -999;
        double bestvzError = -999;

        const reco::Vertex &vtx;
        math::XYZPoint bestvtx;

        myVertex(double vx, double vy, double vz, double vxErr, double vyErr, double vzErr, const reco::Vertex &vtx_, math::XYZPoint bestvtx_) :
            bestvx(vx), bestvy(vy), bestvz(vz), bestvxError(vxErr), bestvyError(vyErr), bestvzError(vzErr), vtx(vtx_), bestvtx(bestvtx_) {}
    };

    struct tree_particle{
        float mass;
        float momentum;
        float pt;
        float momentum_1;
        float pt_1;
        float ptError_1;
        float energy_1;
        float dedx_1;
        float charge_1;
        float dz_1;
        float dzError_1;
        float dxy_1;
        float dxyError_1;
        float eta_1;
        float rapidity_1;
        float phi_1;
        float vx_1;
        float vy_1;
        float vz_1;
        float px_1;
        float py_1;
        float pz_1;
        float vzFlip_1;
        float chi2_1;
        float chi2norm_1;
        float ndof_1;
        float nhits_1;
        float momentum_2;
        float pt_2;
        float ptError_2;
        float energy_2;
        float dedx_2;
        float charge_2;
        float dz_2;
        float dzError_2;
        float dxy_2;
        float dxyError_2;
        float eta_2;
        float rapidity_2;
        float phi_2;
        float vx_2;
        float vy_2;
        float vz_2;
        float px_2;
        float py_2;
        float pz_2;
        float vzFlip_2;
        float chi2_2;
        float chi2norm_2;
        float ndof_2;
        float nhits_2;
    };

    struct track_combo{
        reco::TrackCollection::const_iterator track;
        reco::TrackRef track_ref;

        track_combo(reco::TrackCollection::const_iterator &track_, reco::TrackRef track_ref_) :
            track(track_), track_ref(track_ref_) {}
    };

    void FillTreeStruct(tree_particle& treeStruct, PhiMeson* phi);

    myVertex MyVertexBuild(edm::Handle<reco::VertexCollection> vertices);

    bool isTrackGood(reco::TrackCollection::const_iterator &track, myVertex myVtx, bool trackPtCut);

    bool SelectionCut(reco::TrackCollection::const_iterator &track, myVertex myVtx, bool trackPtCut, double dzdca, double dxydca, double eta, double ptCutVal, int nhits);

    int Multiplicity(edm::Handle<reco::TrackCollection> tracks,
            edm::Handle<reco::VertexCollection> vertices);

    bool AcceptTrackDeDx(track_combo track_combo_, edm::Handle<edm::ValueMap<reco::DeDxData> > DeDxTrack, std::string constraint);

    template <class T>
    void GetCollection(const edm::Event& iEvent_,edm::EDGetTokenT<T> const& tag, edm::Handle<T>& result)
    {
        iEvent_.getByToken(tag,result);
        if(!result.isValid())
        {
           throw std::invalid_argument("Collection is invalid.");
        }
    }

    double getDeDx(utility::track_combo track_combo_, edm::Handle<edm::ValueMap<reco::DeDxData> > DeDxTrack);

}

#endif
