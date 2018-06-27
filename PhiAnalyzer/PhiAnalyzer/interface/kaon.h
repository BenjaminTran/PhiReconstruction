#ifndef PHIANALYZER__KAON_H
#define PHIANALYZER__KAON_H

#include "PhiAnalyzer/PhiAnalyzer/interface/utility.h"
#include "PhiAnalyzer/PhiAnalyzer/interface/particle.h"

class kaon : public Particle{
    private:
        double dedx_;
        int charge_;
        double ptError_ = -999;
        double dz_ = -999;
        double dzError_ = -999;
        double dxy_ = -999;
        double dxyError_ = -999;
        int nhits_ = -999;
        double chi2_ = -999;
        double chi2norm_ = -999;
        double vx_ = -999;
        double vy_ = -999;
        double vz_ = -999;
        double ndof_ = -999;

    public:
        struct cutVariables{
            double ptError;
            double dz;
            double dzError;
            double dxy;
            double dxyError;
            int nhits;
            double chi2;
            double chi2norm;
            double vx;
            double vy;
            double vz;
            double ndof;
        };
        kaon(TVector3 momentum, double eta, double phi, int charge = -9, double dedx = 0, bool isGen = false, double mass = utility::kaonMass);
        kaon(TVector3 momentum, double eta, double phi, int charge = -9, bool isGen = false, double mass = utility::kaonMass);
        kaon(TVector3 momentum, double eta, double phi, cutVariables cutValues, int charge = -9, double dedx = 0, bool isGen = false, double mass = utility::kaonMass);

        double getPtError() {return ptError_;};
        double getDz() {return dz_;};
        double getDzError() {return dzError_;};
        double getDxy() {return dxy_;};
        double getDxyError() {return dxyError_;};
        int getNhits() {return nhits_;};
        double getChi2() {return chi2_;};
        double getChi2norm() {return chi2norm_;};
        double getVx() {return vx_;};
        double getVy() {return vy_;};
        double getVz() {return vz_;};
        double getNdof() {return ndof_;};
        double getDedx() {return dedx_;};
        double getCharge() {return charge_;};

        void setDedx(double dedx) {dedx_ = dedx;};
        void setCharge(int charge) {charge_ = charge;};
};
#endif
