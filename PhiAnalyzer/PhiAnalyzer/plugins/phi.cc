#include "PhiAnalyzer/PhiAnalyzer/interface/utility.h"
#include "PhiAnalyzer/PhiAnalyzer/interface/phi.h"

PhiMeson::PhiMeson()
{
}

PhiMeson::PhiMeson(double mass, TVector3 momentum, TLorentzVector PtEtaPhiE, bool isGen)
{
    mass_ = mass;
    isGen_ = isGen;

    momentum_ = momentum;
    PtEtaPhiE_ = PtEtaPhiE;
}

// Takes two vectors of kaons, one containing only positive and the other negative. Third option specifies if you want to throw away
// a certain percentage of the combinatorial phis. Percentage determined from maxValue and keepValue e.g. default values keeps 1%
std::vector<PhiMeson> PhiMeson::EventCombinatorialPhi(std::vector<kaon> PKp_, std::vector<kaon> PKm_, bool throwAway, int maxValue, int keepValue)
{
    std::vector<PhiMeson> phiCollection;
    for(kaon Pkp : PKp_)
    {
        for(kaon Pkm : PKm_)
        {
            if(throwAway)
            {
                int Rand = gRandom->Integer(maxValue);
                if(Rand > keepValue) continue;
            }

            PhiMeson pgf = BuildPhi(Pkp, Pkm);

            phiCollection.push_back(pgf);
        }
    }

    return phiCollection;
}

// Third parameter is to specify whether or not the phi is produced from matched
// daughter kaons
PhiMeson PhiMeson::BuildPhi(kaon Pkp, kaon Pkm, bool isGen)
{
    TVector3 dau1p(Pkp.getPx(), Pkp.getPy(), Pkp.getPz());
    TVector3 dau2p(Pkm.getPx(), Pkm.getPy(), Pkm.getPz());
    TVector3 dauPsum(dau1p + dau2p);
    double mass = sqrt(TMath::Power(Pkp.getEnergy()+Pkm.getEnergy(),2) - dauPsum.Mag2());

    double pt = dauPsum.Perp();

    TLorentzVector phiLV(dauPsum,Pkp.getEnergy() + Pkm.getEnergy());

    TLorentzVector PtEtaPhiE(pt,phiLV.Eta(),phiLV.Phi(),Pkp.getEnergy() + Pkm.getEnergy());

    PhiMeson pgf(mass,dauPsum,PtEtaPhiE,isGen);
    pgf.addKaonDau(Pkp);
    pgf.addKaonDau(Pkm);

    return pgf;
}

kaon* PhiMeson::getKaonDau(int dauID)
{
    kaon* tmpKaon = &(KaonDau_.at(dauID));
    return tmpKaon;
    //return KaonDau_.at(dauID);
}

void PhiMeson::addKaonDau(kaon dau)
{
    KaonDau_.push_back(dau);
}
