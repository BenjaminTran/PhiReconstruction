#include <stdio.h>
#include "TGaxis.h"
#include "TNamed.h"
#include "TKey.h"
#include <TStyle.h>
#include "TF1.h"
#include "TH1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"
#include "TAxis.h"
#include "TRegexp.h"
#include "TString.h"
#include "RooFit.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooGenericPdf.h"
#include "RooPolynomial.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
#include "RooFormulaVar.h"

void MassFitPhi()
{
    std::string Pt_bin = "1-1.5";

    TH1::SetDefaultSumw2();
    RooMsgService::instance().setStreamStatus(0,kFALSE);
    RooMsgService::instance().setStreamStatus(1,kFALSE);
    TGaxis::SetMaxDigits(3);
    gStyle->SetMarkerSize(0.8);
    using namespace RooFit;
    gStyle->SetOptTitle(kFALSE);

    std::vector<std::string> file_names = {
        "/Users/btran/research/PhiReconstruction/TMVA/BDTcmsswPlotters/BDTPROOF/PhiBDT_30M_rapLT1_pt0.5-1.root"
        //"/Users/btran/research/Macros/PhiAnalysis/TMVA/BDTcmsswPlotters/BDTPROOF/Test1-1.5.root"
    };

    std::vector<TFile*> file_list;
    for(unsigned i=0; i<file_names.size(); ++i)
    {
        TFile* tmp = new TFile(file_names[i].c_str());
        file_list.push_back(tmp);
    }
    TRegexp re = "mass";
    std::map<std::string,std::map<std::string,TH1D*> > m_hist;

    // get histograms from file
    for(unsigned i=0; i<file_list.size(); i++)
    {
        TNamed* pt_bin_range_obj = (TNamed*)file_list[i]->Get("pt_range");
        std::string pt_bin_range = pt_bin_range_obj->GetTitle();
        cout << "NAMED " << pt_bin_range << endl;
        std::map <std::string,TH1D*> tmp;
        for(const auto&& object : *file_list[i]->GetListOfKeys())
        {
            TKey* key = (TKey*)object;
            TString st = key->GetName();
            if(st.Index(re) == kNPOS) continue;
            std::string obj_name = key->GetName();
            tmp[obj_name] = (TH1D*)file_list[i]->Get(obj_name.c_str());
        }
        m_hist[pt_bin_range] = tmp;
    }

    TCanvas* cc1 = new TCanvas("cc1","cc1",700,550);

    TH1D* mass_phi;

    TLatex* tex = new TLatex();
    tex->SetNDC();

    tex->SetTextSize(tex->GetTextSize()*0.75);

    for(auto const& maphist : m_hist)
    {
        std::string pt_bin_range = maphist.first;
        for(auto const& hist : maphist.second)
        {
            RooRealVar x("x","mass",1.00,1.04);
            RooPlot* xframe = x.frame(40);
            RooDataHist data("data","dataset",x,hist.second);
            data.plotOn(xframe,Name("data"));
            xframe->GetXaxis()->SetTitle("K^{+}K^{-} and charge conjugate invariant mass (GeV/c^{2})");
            xframe->GetYaxis()->SetTitle("Candidates / 1.0 MeV");
            xframe->GetXaxis()->CenterTitle(1);
            xframe->GetYaxis()->CenterTitle(1);
            xframe->GetXaxis()->SetTitleSize(xframe->GetXaxis()->GetTitleSize()*1.3);
            xframe->GetYaxis()->SetTitleSize(xframe->GetYaxis()->GetTitleSize()*1.2);
            xframe->GetYaxis()->SetTitleOffset(1);
            /*
             * For 0.5 - 2
             */
            RooRealVar mean("mean","mean",1.019,1.017,1.021);
            RooRealVar sigma1("sigma1","sigma1",0.003,0.001,0.006);
            RooRealVar sigma2("sigma2","sigma2",0.003,0.001,0.006);
            RooRealVar sig1("sig1","signal1",2200,0,100000);
            RooRealVar sig2("sig2","signal2",2200,0,100000);
            RooRealVar qsig("qsig","qsig",4000,0,4000000);
            RooGaussian gaus1("gaus1","gaus1",x,mean,sigma1);
            RooGaussian gaus2("gaus2","gaus2",x,mean,sigma2);
            RooRealVar ap("ap","ap",-0.1,-1,1);
            RooRealVar bp("bp","bp",-0.1,-1,1);
            RooRealVar cp("cp","cp",-0.1,-1,1);
            RooChebychev background("background","background",x,RooArgList(ap,bp,cp));
            RooAddPdf sum("sum","sum",RooArgList(gaus1,gaus2,background),RooArgList(sig1,sig2,qsig));

            /*
             * For 2 - 2.5
             */
            //RooRealVar mean("mean","mean",1.019,1.017,1.021);
            //RooRealVar sigma1("sigma1","sigma1",0.003,0.001,0.006);
            //RooRealVar sigma2("sigma2","sigma2",0.003,0.001,0.006);
            //RooRealVar sig1("sig1","signal1",2200,0,10000);
            //RooRealVar sig2("sig2","signal2",2200,0,10000);
            //RooRealVar qsig("qsig","qsig",2000,0,800000);
            //RooGaussian gaus1("gaus1","gaus1",x,mean,sigma1);
            //RooGaussian gaus2("gaus2","gaus2",x,mean,sigma2);
            //RooRealVar ap("ap","ap",-0.1,-1,1);
            //RooRealVar bp("bp","bp",-0.1,-1,1);
            //RooRealVar cp("cp","cp",-0.1,-1,1);
            ////RooRealVar dp("dp","dp",0.15,-1,1);
            ////RooChebychev background("background","background",x,RooArgList(ap,bp,cp,dp));
            //RooChebychev background("background","background",x,RooArgList(ap,bp,cp));
            //RooAddPdf sum("sum","sum",RooArgList(gaus1,gaus2,background),RooArgList(sig1,sig2,qsig));
            ////RooAddPdf sum("sum","sum",RooArgList(gaus1,background),RooArgList(sig1,qsig));

            /*
             * For 2.5 - 3.0
             */
            //RooRealVar mean("mean","mean",1.019,1.017,1.021);
            //RooRealVar sigma1("sigma1","sigma1",0.003,0.001,0.006);
            //RooRealVar sigma2("sigma2","sigma2",0.003,0.001,0.006);
            //RooRealVar sig1("sig1","signal1",2200,0,10000);
            //RooRealVar sig2("sig2","signal2",2200,0,10000);
            //RooRealVar qsig("qsig","qsig",2000,0,900000);
            //RooGaussian gaus1("gaus1","gaus1",x,mean,sigma1);
            //RooGaussian gaus2("gaus2","gaus2",x,mean,sigma2);
            //RooRealVar ap("ap","ap",-0.1,-1,1);
            //RooRealVar bp("bp","bp",-0.1,-1,1);
            //RooRealVar cp("cp","cp",-0.1,-1,1);
            ////RooRealVar dp("dp","dp",0.15,-1,1);
            ////RooChebychev background("background","background",x,RooArgList(ap,bp,cp,dp));
            //RooChebychev background("background","background",x,RooArgList(ap,bp,cp));
            //RooAddPdf sum("sum","sum",RooArgList(gaus1,gaus2,background),RooArgList(sig1,sig2,qsig));
            ////RooAddPdf sum("sum","sum",RooArgList(gaus1,background),RooArgList(sig1,qsig));

            /*
             * For 3.0 - 3.5
             */
            //RooRealVar mean("mean","mean",1.019,1.017,1.021);
            //RooRealVar sigma1("sigma1","sigma1",0.003,0.001,0.006);
            //RooRealVar sigma2("sigma2","sigma2",0.003,0.001,0.006);
            //RooRealVar sig1("sig1","signal1",2200,0,10000);
            //RooRealVar sig2("sig2","signal2",2200,0,10000);
            //RooRealVar qsig("qsig","qsig",2000,0,700000);
            //RooGaussian gaus1("gaus1","gaus1",x,mean,sigma1);
            //RooGaussian gaus2("gaus2","gaus2",x,mean,sigma2);
            //RooRealVar ap("ap","ap",-0.1,-1,1);
            //RooRealVar bp("bp","bp",-0.1,-1,1);
            //RooRealVar cp("cp","cp",-0.1,-1,1);
            ////RooRealVar dp("dp","dp",0.15,-1,1);
            ////RooChebychev background("background","background",x,RooArgList(ap,bp,cp,dp));
            //RooChebychev background("background","background",x,RooArgList(ap,bp,cp));
            //RooAddPdf sum("sum","sum",RooArgList(gaus1,gaus2,background),RooArgList(sig1,sig2,qsig));
            ////RooAddPdf sum("sum","sum",RooArgList(gaus1,background),RooArgList(sig1,qsig));

            /*
             * 3.5 - 4.0
             */
            //RooRealVar mean("mean","mean",1.019,1.017,1.021);
            //RooRealVar sigma1("sigma1","sigma1",0.003,0.001,0.006);
            //RooRealVar sigma2("sigma2","sigma2",0.003,0.001,0.006);
            //RooRealVar sig1("sig1","signal1",2200,0,10000);
            //RooRealVar sig2("sig2","signal2",2200,0,10000);
            //RooRealVar qsig("qsig","qsig",2000,0,700000);
            //RooGaussian gaus1("gaus1","gaus1",x,mean,sigma1);
            //RooGaussian gaus2("gaus2","gaus2",x,mean,sigma2);
            //RooRealVar ap("ap","ap",-0.1,-1,1);
            //RooRealVar bp("bp","bp",-0.1,-1,1);
            //RooRealVar cp("cp","cp",-0.1,-1,1);
            ////RooRealVar dp("dp","dp",0.15,-1,1);
            ////RooChebychev background("background","background",x,RooArgList(ap,bp,cp,dp));
            //RooChebychev background("background","background",x,RooArgList(ap,bp,cp));
            //RooAddPdf sum("sum","sum",RooArgList(gaus1,gaus2,background),RooArgList(sig1,sig2,qsig));
            ////RooAddPdf sum("sum","sum",RooArgList(gaus1,background),RooArgList(sig1,qsig));

            x.setRange("cut",1.000,1.04);
            RooFitResult* r_phi = NULL;

            r_phi = sum.fitTo(data,Save(),Minos(kTRUE),Range("cut"));

            sum.plotOn(xframe,Name("sum"),NormRange("cut"),LineWidth(2),LineColor(kRed));
            sum.plotOn(xframe,Components(background),NormRange("cut"),LineStyle(kDashed),LineWidth(2),LineColor(kRed));
            cc1->cd();
            gPad->SetTickx();
            gPad->SetTicky();
            xframe->Draw();

            //Calculation
            double Intgaus1E      = -999;
            double Intgaus2E      = -999;
            double IntbackgroundE = -999;
            double totsig         = -999;
            double sig            = -999;
            double Fsig           = -999;
            double Significance      = -999;

            double _mean = mean.getVal(  );
            double _rms  = TMath::Sqrt( 0.5*sigma1.getVal(  )*sigma1.getVal(  ) + 0.5*sigma2.getVal(  )*sigma2.getVal(  ) );
            //double _rms  = sigma1.getVal();

            double gaus1F  = sig1.getVal(  );
            double gaus2F  = sig2.getVal(  );
            double _qsig    = qsig.getVal(  );

            x.setRange("int",_mean - 2*_rms,_mean + 2*_rms);

            RooAbsReal* Intgaus1 = gaus1.createIntegral( x, x,  "int" );
            RooAbsReal* Intgaus2 = gaus2.createIntegral( x, x, "int" );

            RooAbsReal* Intbackground = background.createIntegral( x, x, "int" );

            Intgaus1E      = gaus1F*Intgaus1->getVal(  );
            Intgaus2E      = gaus2F*Intgaus2->getVal(  );
            IntbackgroundE = _qsig*Intbackground->getVal(  );
            totsig         = Intgaus1E + Intgaus2E + IntbackgroundE;
            //totsig         = Intgaus1E + IntbackgroundE;
            sig            = Intgaus1E;

            Fsig           = sig/totsig;
            Significance   = sig/(TMath::Sqrt(totsig));

            double covQual = r_phi->covQual();

            std::ostringstream ostr;
            ostr << "Mass " << _mean;
            double increment = 0.07;
            double start_y = 0.67;
            tex->DrawLatex(0.55,start_y-=increment,ostr.str().c_str());
            ostr.str(std::string());
            ostr << "Significance #frac{S}{#sqrt{S+B}} " << Significance;
            tex->DrawLatex(0.55,start_y-=increment,ostr.str().c_str());
            ostr.str(std::string());
            ostr << "CovQual: " << covQual;
            tex->DrawLatex(0.55,start_y-=increment,ostr.str().c_str());

            TLine* t1 = new TLine(mean.getVal()- 2*_rms,0,mean.getVal() - 2*_rms,gPad->GetUymax());
            TLine* t2 = new TLine(mean.getVal()+ 2*_rms,0,mean.getVal() + 2*_rms,gPad->GetUymax());
            t1->SetLineStyle(2);
            t2->SetLineStyle(2);
            t1->SetLineColor(kGreen+1);
            t2->SetLineColor(kGreen+1);

            t1->Draw();
            t2->Draw();


            cc1->Print((hist.first + "_pt_" + pt_bin_range + ".pdf").c_str());
            cout << "RMS: " << _rms << endl;
        }
    }
}
