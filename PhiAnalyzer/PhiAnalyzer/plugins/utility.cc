/*
 * Making methods that are frequently used across modules
 */

#include "PhiAnalyzer/PhiAnalyzer/interface/utility.h"
#include "PhiAnalyzer/PhiAnalyzer/interface/phi.h"

class kaon; //forward declartion
namespace utility
{
    const double kaonMass = 0.493677;
    myVertex MyVertexBuild(edm::Handle<reco::VertexCollection> vertices)
    {
        const reco::Vertex & vtx = (*vertices)[0];
        math::XYZPoint vtx_point(vtx.x(),vtx.y(),vtx.z());
        myVertex myVertex_(vtx.x(),vtx.y(),vtx.z(),vtx.xError(),vtx.yError(),vtx.zError(),vtx,vtx_point);
        return myVertex_;
    }

    //Multiplicity criteria
    bool isTrackGood(reco::TrackCollection::const_iterator &track, myVertex myVtx, bool ptCut = true)
    {
        double dzvtx = track->dz(myVtx.bestvtx);
        double dxyvtx = track->dxy(myVtx.bestvtx);
        double dzerror = sqrt(track->dzError()*track->dzError()+myVtx.bestvzError*myVtx.bestvzError);
        double dxyerror = sqrt(track->d0Error()*track->d0Error()+myVtx.bestvxError*myVtx.bestvyError);

        if(!track->quality(reco::TrackBase::highPurity)) return false;
        if(fabs(track->ptError())/track->pt()>0.10) return false;
        if(fabs(dzvtx/dzerror) > 3) return false;
        if(fabs(dxyvtx/dxyerror) > 3) return false;
        if(fabs(track->eta()) > 3) return false;
        if(ptCut)
        {
            if(track->pt() <= 0.4) return false;
        }

        return true;
    }

    //Check if track passes the given constraints
    bool SelectionCut(reco::TrackCollection::const_iterator &track, myVertex myVtx, bool ptCut, double dzdca, double dxydca, double eta, double ptCutVal, int nhits)
    {
        double dzvtx = track->dz(myVtx.bestvtx);
        double dxyvtx = track->dxy(myVtx.bestvtx);
        double dzerror = sqrt(track->dzError()*track->dzError()+myVtx.bestvzError*myVtx.bestvzError);
        double dxyerror = sqrt(track->d0Error()*track->d0Error()+myVtx.bestvxError*myVtx.bestvyError);

        if(!track->quality(reco::TrackBase::highPurity)) return false;
        if(track->numberOfValidHits() < nhits) return false;
        if(fabs(track->ptError())/track->pt()>0.10) return false;
        if(fabs(dzvtx/dzerror) > dzdca) return false;
        if(fabs(dxyvtx/dxyerror) > dxydca) return false;
        if(fabs(track->eta()) > eta) return false;
        if(ptCut)
        {
            if(track->pt() <= ptCutVal) return false;
        }

        return true;

    }


    //Return the multiplicity of the event
    int Multiplicity(edm::Handle<reco::TrackCollection> tracks,
            edm::Handle<reco::VertexCollection> vertices)
    {
        int Multiplicity = 0;

        myVertex myVtx = MyVertexBuild(vertices);

        for(reco::TrackCollection::const_iterator it = tracks->begin();
                it != tracks->end();
                ++it)
        {
            if(isTrackGood(it,myVtx,true))
                Multiplicity++;
            else
                continue;
        }

        return Multiplicity;
    }

    /*
     * For constraint use tight, default, or loose. Default will always be
     * defined as the band of choice
     */
    bool AcceptTrackDeDx(track_combo track_combo_, edm::Handle<edm::ValueMap<reco::DeDxData> > DeDxTrack, std::string constraint = "default")
    {
        double functionValueTop = -999;
        double functionValueBot = -999;
        double momentum = track_combo_.track->p();
        double dedx = getDeDx(track_combo_, DeDxTrack);
        //int nhits = track_combo_.track->numberOfValidHits();
        if(constraint == "tight")
        {
            functionValueTop = 0.55*(TMath::Power(1.6/momentum,2) - 2.95*TMath::Power(0.6/momentum,1)) + 3.3;
            functionValueBot = 0.55*(TMath::Power(1.15/momentum,2) - 1.7*TMath::Power(0.6/momentum,1)) + 2.7;

            if(dedx < functionValueTop && dedx > functionValueBot)
                return true;
            else
                return false;
        }
        else if(constraint == "default")
        {
            functionValueTop = 0.55*(TMath::Power(1.62/momentum,2) - 2*TMath::Power(0.6/momentum,1)) + 3.6;
            functionValueBot = 0.55*(TMath::Power(1.15/momentum,2) - 1.7*TMath::Power(0.6/momentum,1)) + 2.5;

            if(dedx < functionValueTop && dedx > functionValueBot)
                return true;
            else
                return false;
        }
        else if(constraint == "loose")
        {

            functionValueTop = 0.55*(TMath::Power(1.62/momentum,2) - 2*TMath::Power(0.6/momentum,1)) + 3.6;
            functionValueBot = 0.5*(TMath::Power(0.5/momentum,4) - 1*TMath::Power(0.50/momentum,2)) + 2.7;

            if(dedx < functionValueTop && dedx > functionValueBot)
                return true;
            else
                return false;
        }
        else
        {
            throw std::invalid_argument(constraint + " is not a valid dedx function constraint!");
        }
    }

    double getDeDx(utility::track_combo track_combo_, edm::Handle<edm::ValueMap<reco::DeDxData> > DeDxTrack)
    {
        double dedx_ = -1;
        const edm::ValueMap<reco::DeDxData> dedxTrack = *DeDxTrack.product();
        dedx_ = dedxTrack[track_combo_.track_ref].dEdx();

        return dedx_;
    }


    void FillTreeStruct(utility::tree_particle& treeStruct, PhiMeson* phi)
    {
            kaon* dau1            = phi->getKaonDau(0);
            kaon* dau2            = phi->getKaonDau(1);
            treeStruct.mass       = phi->getMass();
            treeStruct.momentum = phi->getP();
            treeStruct.pt = phi->getPt();
            treeStruct.momentum_1 = dau1->getP();
            treeStruct.pt_1       = dau1->getPt();
            treeStruct.ptError_1  = dau1->getPtError();
            //treeStruct.energy_1   = dau1->getEnergy();
            treeStruct.dedx_1     = dau1->getDedx();
            treeStruct.charge_1   = dau1->getCharge();
            treeStruct.dz_1       = dau1->getDz();
            treeStruct.dzError_1  = dau1->getDzError();
            treeStruct.dxy_1      = dau1->getDxy();
            treeStruct.dxyError_1 = dau1->getDxyError();
            treeStruct.eta_1      = dau1->getEta();
            treeStruct.rapidity_1 = dau1->getRapidity();
            //treeStruct.phi_1      = dau1->getPhi();
            //treeStruct.vx_1       = dau1->getVx();
            //treeStruct.vy_1       = dau1->getVy();
            //treeStruct.vz_1       = dau1->getVz();
            //treeStruct.px_1       = dau1->getPx();
            //treeStruct.py_1       = dau1->getPy();
            //treeStruct.pz_1       = dau1->getPz();
            //treeStruct.vzFlip_1   = -dau1->getVz();
            //treeStruct.chi2_1     = dau1->getChi2();
            //treeStruct.chi2norm_1 = dau1->getChi2norm();
            //treeStruct.ndof_1     = dau1->getNdof();
            treeStruct.nhits_1    = dau1->getNhits();
            treeStruct.momentum_2 = dau2->getP();
            treeStruct.pt_2       = dau2->getPt();
            treeStruct.ptError_2  = dau2->getPtError();
            //treeStruct.energy_2   = dau2->getEnergy();
            treeStruct.dedx_2     = dau2->getDedx();
            treeStruct.charge_2   = dau2->getCharge();
            treeStruct.dz_2       = dau2->getDz();
            treeStruct.dzError_2  = dau2->getDzError();
            treeStruct.dxy_2      = dau2->getDxy();
            treeStruct.dxyError_2 = dau2->getDxyError();
            treeStruct.eta_2      = dau2->getEta();
            treeStruct.rapidity_2 = dau2->getRapidity();
            //treeStruct.phi_2      = dau2->getPhi();
            //treeStruct.vx_2       = dau2->getVx();
            //treeStruct.vy_2       = dau2->getVy();
            //treeStruct.vz_2       = dau2->getVz();
            //treeStruct.px_2       = dau2->getPx();
            //treeStruct.py_2       = dau2->getPy();
            //treeStruct.pz_2       = dau2->getPz();
            //treeStruct.vzFlip_2   = -dau2->getVz();
            //treeStruct.chi2_2     = dau2->getChi2();
            //treeStruct.chi2norm_2 = dau2->getChi2norm();
            //treeStruct.ndof_2     = dau2->getNdof();
            treeStruct.nhits_2    = dau2->getNhits();
    }
}
