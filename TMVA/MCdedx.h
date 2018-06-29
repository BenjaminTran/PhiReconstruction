#include "TFile.h"
#include "TNamed.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TCanvas.h"
#include "TSelector.h"

class MCdedx : public TSelector {
    public:

        TTreeReader reader;
        TTreeReaderValue<float> momentum_1;
        TTreeReaderValue<float> momentum_2;
        TTreeReaderValue<float> dedx_1;
        TTreeReaderValue<float> dedx_2;

        TFile* out;
        TH1D* h_nEvt;

        TH2D* h_dedx;

        MCdedx(TTree * = 0) :
            momentum_1(reader,"momentum_1"),
            momentum_2(reader,"momentum_2"),
            dedx_1(reader,"dedx_1"),
            dedx_2(reader,"dedx_2")
            {
                h_nEvt = 0;
                h_dedx = 0;
            }

        virtual ~MCdedx() {}
        virtual void Init(TTree *tree);
        virtual void SlaveBegin(TTree *tree);
        virtual bool Process(Long64_t entry);
        virtual void Terminate();
        virtual int Version() const {return 2;}

        ClassDef(MCdedx,0);

};
