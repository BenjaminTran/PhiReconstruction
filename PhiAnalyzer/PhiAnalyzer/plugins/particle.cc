#include "PhiAnalyzer/PhiAnalyzer/interface/particle.h"

bool Particle::matched(Particle* genParticle)
{
    double genPt = -999;
    double trkPt = -999;
    //check which kaon is gen level. If neither then throw exception
    if(genParticle->getIsGen())
    {
        genPt = genParticle->getPt();
        trkPt = momentum_.Perp();
    }
    else if(isGen_)
    {
        genPt = momentum_.Perp();
        trkPt = genParticle->getPt();
    }
    else
        throw std::invalid_argument("Neither of these is a gen level particle");

    double dpt = genPt - trkPt;

    bool isMatched = false;

    if(deltaR(genParticle->getLorentzVect()) < 0.1 && fabs(dpt/genPt) < 0.1)
        isMatched = true;

    return isMatched;
}
