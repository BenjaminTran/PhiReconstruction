#include "PhiAnalyzer/PhiAnalyzer/interface/kaon.h"

kaon::kaon(TVector3 momentum, double eta, double phi, int charge, double dedx, bool isGen, double mass)
{
    //TVector3 momentum(px,py,pz);
    double energy = sqrt(TMath::Power(mass,2) + TMath::Power(momentum.Mag(),2));
    TLorentzVector PtEtaPhiE(momentum.Perp(),eta,phi,energy);
    momentum_ = momentum;
    PtEtaPhiE_ = PtEtaPhiE;
    dedx_ = dedx;
    charge_ = charge;
    isGen_ = isGen;
    mass_ = mass;
}

kaon::kaon(TVector3 momentum, double eta, double phi, int charge, bool isGen, double mass)
{
    //TVector3 momentum(px,py,pz);
    double energy = sqrt(TMath::Power(mass,2) + TMath::Power(momentum.Mag(),2));
    TLorentzVector PtEtaPhiE(momentum.Perp(),eta,phi,energy);
    momentum_ = momentum;
    PtEtaPhiE_ = PtEtaPhiE;
    charge_ = charge;
    dedx_ = 0;
    isGen_ = isGen;
    mass_ = mass;
}

kaon::kaon(TVector3 momentum, double eta, double phi, cutVariables cutValues, int charge, double dedx, bool isGen, double mass)
{
    double energy = sqrt(TMath::Power(mass,2) + TMath::Power(momentum.Mag(),2));
    TLorentzVector PtEtaPhiE(momentum.Perp(),eta,phi,energy);
    momentum_ = momentum;
    PtEtaPhiE_ = PtEtaPhiE;
    dedx_ = dedx;
    charge_ = charge;
    isGen_ = isGen;
    mass_ = mass;
    ptError_ = cutValues.ptError;
    dz_ = cutValues.dz;
    dzError_ = cutValues.dzError;
    dxy_ = cutValues.dxy;
    dxyError_ = cutValues.dxyError;
    nhits_ = cutValues.nhits;
    chi2_ = cutValues.chi2;
    chi2norm_ = cutValues.chi2norm;
    vx_ = cutValues.vx;
    vy_ = cutValues.vy;
    vz_ = cutValues.vz;
    ndof_ = cutValues.ndof;
}
