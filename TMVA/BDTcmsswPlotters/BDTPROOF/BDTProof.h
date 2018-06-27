#include "TFile.h"
#include "TNamed.h"
#include "TH1D.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TCanvas.h"
#include "TSelector.h"

class BDTProof : public TSelector {
    public:

        TTreeReader reader;
        TTreeReaderValue<double> BDT;
        TTreeReaderValue<double> mass;

        TFile* out;
        TH1D* h_nEvt;
        //
        //TH2D **h_dedx;
        //TH2D **h_mass_pt;

        std::string pt_bin_range;
        std::string treeName;
        int num_analyze = 3;

        std::vector<TH1D*> v_hmass;

        std::vector<TH1D*> hist_containers;

        double mass_low;
        double mass_high;
        double mass_numBins;
        std::vector<double> pt_bins = {0,0.5,1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6};
        std::vector<float> BDTval = {-0.2
                                     ,-0.15
                                     ,-0.1
                                     ,-0.05
                                     ,-0.04
                                     ,-0.03
                                     ,-0.02
                                     ,-0.01
                                     ,0.0
                                     ,0.01
                                     ,0.02
                                     ,0.03
                                     ,0.04
                                     ,0.05
                                     ,0.10
                                     ,0.11
                                     ,0.12
                                     ,0.13
                                     ,0.14
                                     ,0.15
                                     ,0.16
                                     ,0.17
                                     ,0.18
                                     ,0.19
                                     ,0.20
                                     ,0.21
                                     ,0.22
                                     ,0.23
                                     ,0.24
                                     ,0.25
                                     ,0.26
                                     ,0.27
                                     ,0.28
                                     ,0.29
                                     ,0.30
                                     };

        bool doDedx;

        BDTProof(TTree * = 0) :
            /*
             * For TMVA trees
             */
            BDT(reader,"BDTresponse"),
            mass(reader,"mass")
            {
                h_nEvt = 0;
                mass_low = 1.0;
                mass_high = 1.04;
                mass_numBins = 80;
                ostringstream ostr;
                ostr << 0.5 << "-" << 1.0;
                pt_bin_range = ostr.str();
            }

        virtual ~BDTProof() {}
        virtual void Init(TTree *tree);
        virtual void SlaveBegin(TTree *tree);
        virtual bool Process(Long64_t entry);
        virtual void Terminate();
        virtual int Version() const {return 2;}

        bool FillHist(int& counter);

        template <typename T, size_t N>
            T* begin(T(&arr)[N]) {return &arr[0];}
        template <typename T, size_t N>
            T* end(T(&arr)[N]) {return &arr[0] + N;}

        ClassDef(BDTProof,0);

};
