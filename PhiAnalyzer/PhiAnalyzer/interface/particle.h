#ifndef PHIANALYZER__PARTICLE_H
#define PHIANALYZER__PARTICLE_H

#include "TVector3.h"
#include "TLorentzVector.h"

class Particle{
    public:
        TVector3 momentum_;
        TLorentzVector PtEtaPhiE_;
        bool isGen_;
        double mass_;

        //Getters
        double getMass() { return mass_; };
        double getP() { return sqrt(momentum_.Mag2()); };
        double getPt() { return momentum_.Perp(); };
        double getPx() { return momentum_.X(); };
        double getPy() { return momentum_.Y(); };
        double getPz() { return momentum_.Z(); };
        double getEnergy() { return PtEtaPhiE_.E(); };
        TVector3 getMomentumVect() { return momentum_; };
        TLorentzVector getLorentzVect() { return PtEtaPhiE_; };
        double getRapidity() {
            TLorentzVector LV(momentum_.X(), momentum_.Y(), momentum_.Z(), PtEtaPhiE_.E());
            return LV.Rapidity();
        };
        double getEta() { return PtEtaPhiE_.Eta(); };
        double getPhi() { return PtEtaPhiE_.Phi(); };
        bool getIsGen() { return isGen_; };

        //Calculations
        double deltaR(TLorentzVector otherLV) {return PtEtaPhiE_.DeltaR(otherLV);};
        bool matched(Particle* genParticle);

        //Setters
        void setMass(double mass) {mass_ = mass;};
        void setMomentumVect(TVector3 momentum) {momentum_ = momentum;};
        void setLorentzVect(TLorentzVector PtEtaPhiE) {PtEtaPhiE_ = PtEtaPhiE;};
        void setIsGen(bool isGen) {isGen_ = isGen;};
};

#endif
