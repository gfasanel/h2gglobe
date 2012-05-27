#include "TROOT.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TGraph.h"
#include "TLatex.h"
#include "TLine.h"

#include "RooWorkspace.h"
#include "RooRealVar.h"

#include "../interface/FMTPlots.h"

using namespace std;

FMTPlots::FMTPlots(string filename, int mHMinimum, int mHMaximum, double mHStep, double massMin, double massMax, int nDataBins, double signalRegionWidth, double sidebandWidth, int numberOfSidebands, int numberOfSidebandsForAlgos, int numberOfSidebandGaps, double massSidebandMin, double massSidebandMax, bool includeVBF, bool includeLEP, vector<string> systematics, bool rederiveOptimizedBinEdges, vector<map<int, vector<double> > > AllBinEdges, bool blind, bool verbose):

	FMTBase(mHMinimum, mHMaximum, mHStep, massMin, massMax, nDataBins, signalRegionWidth, sidebandWidth, numberOfSidebands, numberOfSidebandsForAlgos, numberOfSidebandGaps, massSidebandMin, massSidebandMax, includeVBF, includeLEP, systematics, rederiveOptimizedBinEdges, AllBinEdges, verbose),
  blind_(blind)
{
  tFile = TFile::Open(filename.c_str(),"UPDATE");
  gStyle->SetOptStat(0);
}

FMTPlots::~FMTPlots(){
  tFile->Close();
}

TH1F* FMTPlots::linearBin(TH1F* hist){
  
  int nBins = hist->GetNbinsX();
  TH1F *h = new TH1F(Form("lin_%s",hist->GetName()),"",nBins,0,nBins);
  for (int b=0; b<nBins; b++){
    h->SetBinContent(b+1,hist->GetBinContent(b+1));
    if (hist->GetBinLowEdge(b+1)<=1.) h->GetXaxis()->SetBinLabel(b+1,Form("BDT Bin %d",b+1));
    else if (hist->GetBinLowEdge(b+1)<=1.04) h->GetXaxis()->SetBinLabel(b+1,"Di-jet");
    else if (hist->GetBinLowEdge(b+1)<=1.08) h->GetXaxis()->SetBinLabel(b+1,"Di-lep");
  }
  return h;
}

void FMTPlots::makePlots(TH1F* dataH, TH1F *sigH, TH1F *bkgH, TH1F *bkgmcH, vector<TH1F*> dataSideLowH, vector<TH1F*> dataSideHighH, vector<TH1F*> bkgSideLowH, vector<TH1F*> bkgSideHighH, vector<pair<TH1F*,TH1F*> > sigSystH, TH1F* lowInterpH, TH1F* highInterpH, double mh){

  TH1F *data = linearBin(dataH);
  TH1F *sig = linearBin(sigH);
  TH1F *bkg = linearBin(bkgH);
  TH1F *bkgmc = linearBin(bkgmcH);
  TH1F *sig3 = (TH1F*)sig->Clone();
  sig3->Scale(3.);
  TH1F *sig5 = (TH1F*)sig->Clone();
  sig5->Scale(5.);
  TH1F *lowInterp = linearBin(lowInterpH);
  TH1F *highInterp = linearBin(highInterpH);

  vector<TH1F*> dataSideLow;
  vector<TH1F*> dataSideHigh;
  vector<TH1F*> bkgSideLow;
  vector<TH1F*> bkgSideHigh;
  vector<pair<TH1F*,TH1F*> > sigSyst;
  
  for (vector<TH1F*>::iterator it = dataSideLowH.begin(); it != dataSideLowH.end(); it++) dataSideLow.push_back(linearBin(*it));
  for (vector<TH1F*>::iterator it = dataSideHighH.begin(); it != dataSideHighH.end(); it++) dataSideHigh.push_back(linearBin(*it));
  for (vector<TH1F*>::iterator it = bkgSideLowH.begin(); it != bkgSideLowH.end(); it++) bkgSideLow.push_back(linearBin(*it));
  for (vector<TH1F*>::iterator it = bkgSideHighH.begin(); it != bkgSideHighH.end(); it++) bkgSideHigh.push_back(linearBin(*it));
  for (vector<pair<TH1F*,TH1F*> >::iterator it = sigSystH.begin(); it != sigSystH.end(); it++) sigSyst.push_back(pair<TH1F*,TH1F*>(linearBin(it->first),linearBin(it->second)));

  plotOutput(data,bkg,sig,sig3,sig5,mh);
  plotSidebands(bkg,dataSideLow,dataSideHigh,bkgmc,bkgSideLow,bkgSideHigh,mh);
  plotSystFracs(sig,sigSyst,mh);
  plotInterpolation(sig,lowInterp,highInterp,mh);
  delete data;
  delete sig;
  delete bkg;
  delete bkgmc;
  delete sig3;
  delete sig5;
  delete lowInterp;
  delete highInterp;
}

void FMTPlots::plotSystFracs(TH1F *sig, vector<pair<TH1F*,TH1F*> > sigSyst, double mh){
  
  TCanvas *canv = new TCanvas();
  canv->Print(Form("plots/png/systs_mH_%3.1f.png",mh));
  canv->Print(Form("plots/pdf/systs_mH_%3.1f.pdf",mh));
  delete canv;
}

void FMTPlots::plotInterpolation(TH1F *sig, TH1F *interpLow, TH1F *interpHigh, double mh){
 
  TCanvas *c = new TCanvas("c","c",1600,1800);
  TPad *up = new TPad("u","u",0.01,0.6,0.99,0.99);
  TPad *dp = new TPad("d","d",0.01,0.01,0.99,0.6);
  up->SetNumber(1);
  dp->SetNumber(2);
  up->Draw();
  dp->Draw();

  c->cd(1);
  sig->SetLineColor(kBlack);
  sig->SetFillColor(kGray);
  interpHigh->SetLineColor(kBlue);
  interpLow->SetLineColor(kRed);

  interpHigh->SetTitle("");
  interpHigh->Draw("e");
  sig->Draw("hist");
  interpLow->Draw("same e");

  TH1F *diffHigh = (TH1F*)interpHigh->Clone();
  diffHigh->Add(sig,-1);
  TH1F *diffLow = (TH1F*)interpLow->Clone();
  diffLow->Add(sig,-1);

  c->cd(2);
  diffHigh->SetTitle(Form("m_{H}=%3.1f",mh));
  diffHigh->Draw("e");
  diffLow->Draw("same e");
  TLine l1;
  l1.SetLineWidth(2);
  l1.DrawLine(dp->GetUxmin(),0.,dp->GetUxmax(),0.);
  TLegend *leg = new TLegend(0.7,0.8,0.89,0.89);
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  leg->AddEntry(diffHigh,"Upper template","lep");
  leg->AddEntry(diffLow,"Lower template","lep");
  leg->Draw("same");

  c->Print(Form("plots/png/interp_mH_%3.1f.png",mh));
  c->Print(Form("plots/pdf/interp_mH_%3.1f.pdf",mh));
  
  delete c; 
}

void FMTPlots::plotSidebands(TH1F *bkg, vector<TH1F*> dataSideLow, vector<TH1F*> dataSideHigh, TH1F* bkgmc, vector<TH1F*> bkgSideLow, vector<TH1F*> bkgSideHigh, double mh){

  int color[6] = {kGreen+4,kGreen-1,kGreen,kRed,kRed-2,kRed+4};

  TCanvas *c1 = new TCanvas();
  TCanvas *c2 = new TCanvas();
  TLegend *leg1 = new TLegend(0.6,0.6,0.85,0.85);
  TLegend *leg2 = new TLegend(0.6,0.6,0.85,0.85);
  leg1->SetLineColor(0);
  leg1->SetFillColor(0);
  leg2->SetLineColor(0);
  leg2->SetFillColor(0);
  TLatex *text = new TLatex();
  text->SetTextSize(0.04);
  text->SetNDC();

  c1->cd();
  bkg->SetMarkerStyle(22);
  bkg->SetMarkerColor(1);
  bkg->SetLineColor(1);
  leg1->AddEntry(bkg,"Bkg model in signal","lep");
  bkg->SetTitle(Form("Background model from data in sidebands m_{H} = %3.1f",mh));
  bkg->Draw("lep");

  c2->cd();
  bkgmc->SetMarkerStyle(22);
  bkgmc->SetMarkerColor(1);
  bkgmc->SetLineColor(1);
  bkgmc->SetTitle(Form("Background mc m_{H} = %3.1f",mh));
  leg2->AddEntry(bkgmc,"Bkg mc in signal","lep");
  bkgmc->Draw("lep");
 
  for (int h=0; h<dataSideHigh.size(); h++){
    c1->cd();
    dataSideHigh[h]->SetLineColor(color[h]);
    dataSideHigh[h]->SetMarkerStyle(20);
    dataSideHigh[h]->SetMarkerColor(color[h]);
    dataSideHigh[h]->Scale(bkg->Integral()/dataSideHigh[0]->Integral());
    leg1->AddEntry(dataSideHigh[h],Form("High sideband %d",h+1),"lep");
    if (!blind_) dataSideHigh[h]->Draw("same lep");
    c2->cd();
    bkgSideHigh[h]->SetLineColor(color[h]);
    bkgSideHigh[h]->SetMarkerStyle(20);
    bkgSideHigh[h]->SetMarkerColor(color[h]);
    bkgSideHigh[h]->Scale(bkg->Integral()/dataSideHigh[0]->Integral());
    leg2->AddEntry(bkgSideHigh[h],Form("High sideband %d",h+1),"lep");
    bkgSideHigh[h]->Draw("same lep");
  }
  for (int l=0; l<dataSideLow.size(); l++){
    c1->cd();
    dataSideLow[l]->SetLineColor(color[l]);
    dataSideLow[l]->SetMarkerStyle(20);
    dataSideLow[l]->SetMarkerColor(color[l]);
    dataSideLow[l]->Scale(bkg->Integral()/dataSideLow[0]->Integral());
    leg1->AddEntry(dataSideLow[l],Form("Low sideband %d",l+1),"lep");
    if (!blind_) dataSideLow[l]->Draw("same lep");
    c2->cd();
    bkgSideLow[l]->SetLineColor(color[l]);
    bkgSideLow[l]->SetMarkerStyle(20);
    bkgSideLow[l]->SetMarkerColor(color[l]);
    bkgSideLow[l]->Scale(bkgmc->Integral()/dataSideLow[0]->Integral());
    leg2->AddEntry(bkgSideLow[l],Form("Low sideband %d",l+1),"lep");
    bkgSideLow[l]->Draw("same lep");
  }
  c1->cd();
  leg1->Draw("same");
  text->DrawLatex(0.13,0.30,"CMS preliminary");
  text->DrawLatex(0.20,0.23,"#sqrt{s} = 7 TeV");
  text->DrawLatex(0.18,0.15,"#int L = 5.1 fb^{-1}");
  c2->cd();
  leg2->Draw("same");
  text->DrawLatex(0.13,0.25,"CMS preliminary");
  text->DrawLatex(0.20,0.18,"#sqrt{s} = 7 TeV");
  text->DrawLatex(0.18,0.11,"#int L = 5.1 fb^{-1}");

  c1->SetLogy();
  c1->Print(Form("plots/png/dat_sideband_mH_%3.1f.png",mh));
  c1->Print(Form("plots/pdf/dat_sideband_mH_%3.1f.pdf",mh));
  c2->SetLogy();
  c2->Print(Form("plots/png/bkg_sideband_mH_%3.1f.png",mh));
  c2->Print(Form("plots/pdf/bkg_sideband_mH_%3.1f.pdf",mh));

  delete c1;
  delete c2;
}

void FMTPlots::plotOutput(TH1F* data, TH1F* bkg, TH1F* sig, TH1F* sig3, TH1F* sig5, double mh){

  bkg->SetLineColor(kBlue);
  bkg->SetFillColor(kBlue-9);
  sig->SetLineColor(kRed);
  sig3->SetLineColor(kRed);
  sig3->SetLineStyle(3);
  sig5->SetLineColor(kRed);
  sig5->SetLineStyle(7);
  data->SetMarkerStyle(20);

  TCanvas *canv = new TCanvas();
  bkg->SetTitle(Form("m_{H} = %3.1f",mh));
  bkg->Draw("e2");
  sig->Draw("same hist");
  sig3->Draw("same hist");
  sig5->Draw("same hist");
  if (!blind_) data->Draw("same e");
  TLegend *leg = new TLegend(0.45,0.6,0.85,0.85);
  leg->SetLineColor(0);
  leg->SetFillColor(0);
  leg->AddEntry(bkg,"Background","f");
  if (!blind_) leg->AddEntry(data,"Data","lep");
  leg->AddEntry(sig,"Signal (1, 3, 5 #times SM)","l");
  TPaveText *txt = new TPaveText(0.2,0.1,0.4,0.35,"NDC");
  txt->SetFillColor(0);
  txt->SetLineColor(0);
  txt->AddText("#int L = 5.09 fb^{-1}");
  leg->Draw("same");
  txt->Draw("same");
  canv->SetLogy();
  canv->SaveAs(Form("plots/pdf/output_mH_%3.1f.pdf",mh));
  canv->SaveAs(Form("plots/png/output_mH_%3.1f.png",mh));

  TH1F *diff = (TH1F*)data->Clone();
  diff->Add(bkg,-1.);
  sig->SetTitle(Form("m_{H} = %3.1f",mh));
  sig->Draw("hist");
  sig3->Draw("same hist");
  sig5->Draw("same hist");
  if (!blind_) diff->Draw("same e2");
  TLegend *leg2 = new TLegend(0.45,0.6,0.85,0.85);
  leg2->SetLineColor(0);
  leg2->SetFillColor(0);
  if (!blind_) leg2->AddEntry(diff,"Data - background","lep");
  leg2->AddEntry(sig,"Signal (1, 3, 5 #times SM)","l");
  leg2->Draw("same");
  txt->Draw("same");
  canv->SetLogy();
  canv->SaveAs(Form("plots/pdf/diff_output_mH_%3.1f.pdf",mh));
  canv->SaveAs(Form("plots/png/diff_output_mH_%3.1f.png",mh));

  delete canv;
  delete leg;
  delete leg2;
  delete txt;
}


void FMTPlots::plotAll(double mh){
  
  TH1F *bkg = (TH1F*)tFile->Get(Form("th1f_bkg_grad_%3.1f_fitsb_biascorr",mh));
  TH1F *bkgmc = (TH1F*)tFile->Get(Form("th1f_bkg_mc_grad_%3.1f",mh));
  TH1F *data = (TH1F*)tFile->Get(Form("th1f_data_grad_%3.1f",mh));

  TH1F *sig = (TH1F*)((TH1F*)tFile->Get(Form("th1f_sig_grad_ggh_%3.1f",mh)))->Clone();
  sig->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_vbf_%3.1f",mh)));
  sig->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_wzh_%3.1f",mh)));
  sig->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_tth_%3.1f",mh)));
  sig->SetName(Form("th1f_sig_grad_%3.1f",mh));
  write(tFile,sig);

  vector<pair<TH1F*,TH1F*> > sigSyst;
  vector<string> theSystematics = getsystematics();
  for (vector<string>::iterator syst = theSystematics.begin(); syst != theSystematics.end(); syst++){
    TH1F *up = (TH1F*)((TH1F*)tFile->Get(Form("th1f_sig_grad_ggh_%3.1f_%sUp01_sigma",mh,syst->c_str())))->Clone();
    up->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_vbf_%3.1f_%sUp01_sigma",mh,syst->c_str())));
    up->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_wzh_%3.1f_%sUp01_sigma",mh,syst->c_str())));
    up->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_tth_%3.1f_%sUp01_sigma",mh,syst->c_str())));
    up->SetName(Form("th1f_sig_grad_%3.1f_%sUp01_sigma",mh,syst->c_str()));
    write(tFile,up);
    TH1F *down = (TH1F*)((TH1F*)tFile->Get(Form("th1f_sig_grad_ggh_%3.1f_%sUp01_sigma",mh,syst->c_str())))->Clone();
    down->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_vbf_%3.1f_%sDown01_sigma",mh,syst->c_str())));
    down->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_wzh_%3.1f_%sDown01_sigma",mh,syst->c_str())));
    down->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_tth_%3.1f_%sDown01_sigma",mh,syst->c_str())));
    down->SetName(Form("th1f_sig_grad_%3.1f_%sDown01_sigma",mh,syst->c_str()));
    write(tFile,down);
    sigSyst.push_back(pair<TH1F*,TH1F*>(up,down));
  }

  int nearest = (getInterpMasses(mh)).first;
  int nextNear = (getInterpMasses(mh)).second;
  int binningMass, lowInterpMass, highInterpMass;
  if (nearest-nextNear>0) {
    binningMass=nearest;
    lowInterpMass=nextNear;
    highInterpMass=nearest;
  }
  else {
    binningMass=nearest;
    lowInterpMass=nearest;
    highInterpMass=nextNear;
  }

  TH1F *lowInterp = (TH1F*)((TH1F*)tFile->Get(Form("th1f_sig_grad_ggh_%d.0_%d.0",binningMass,lowInterpMass)))->Clone();
  lowInterp->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_vbf_%d.0_%d.0",binningMass,lowInterpMass)));
  lowInterp->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_wzh_%d.0_%d.0",binningMass,lowInterpMass)));
  lowInterp->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_tth_%d.0_%d.0",binningMass,lowInterpMass)));

  TH1F *highInterp = ((TH1F*)tFile->Get(Form("th1f_sig_grad_ggh_%d.0_%d.0",binningMass,highInterpMass))->Clone());
  highInterp->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_vbf_%d.0_%d.0",binningMass,highInterpMass)));
  highInterp->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_wzh_%d.0_%d.0",binningMass,highInterpMass)));
  highInterp->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_tth_%d.0_%d.0",binningMass,highInterpMass)));

  int nLower = (getNsidebandsUandD(mh)).first;
  int nHigher = (getNsidebandsUandD(mh)).second;

  vector<TH1F*> dataSideLow;
  vector<TH1F*> dataSideHigh;
  vector<TH1F*> bkgSideLow;
  vector<TH1F*> bkgSideHigh;
  // loop lower
  for (int sideband_i=getnumberOfSidebandGaps()+1; sideband_i<=getnumberOfSidebandGaps()+nLower; sideband_i++){
    dataSideLow.push_back((TH1F*)tFile->Get(Form("th1f_bkg_%dlow_grad_%3.1f",sideband_i,mh)));
    bkgSideLow.push_back((TH1F*)tFile->Get(Form("th1f_bkg_mc_%dlow_grad_%3.1f",sideband_i,mh)));
  }

  // loop higher
  for (int sideband_i=getnumberOfSidebandGaps()+1; sideband_i<=getnumberOfSidebandGaps()+nHigher; sideband_i++){
    dataSideHigh.push_back((TH1F*)tFile->Get(Form("th1f_bkg_%dhigh_grad_%3.1f",sideband_i,mh)));
    bkgSideHigh.push_back((TH1F*)tFile->Get(Form("th1f_bkg_mc_%dhigh_grad_%3.1f",sideband_i,mh)));
  }
  
  makePlots(data,sig,bkg,bkgmc,dataSideLow,dataSideHigh,bkgSideLow,bkgSideHigh,sigSyst,lowInterp,highInterp,mh);
}

void FMTPlots::makeNormPlot(){
 
  RooWorkspace *outWS = (RooWorkspace*)tFile->Get("cms_hgg_workspace");
  TGraph *normG = new TGraph(getNumMHMasses());  
  vector<double> allMasses = getAllMH();
  int i=0;
  for (vector<double>::iterator mh=allMasses.begin(); mh!=allMasses.end(); mh++){
    RooRealVar *val = (RooRealVar*)outWS->var(Form("NBkgInSignal_mH%3.1f",*mh));
    normG->SetPoint(i,*mh,val->getVal());
    i++;
  }
  TCanvas *canv = new TCanvas();
  normG->SetMarkerSize(2);
  normG->SetLineWidth(2);
  normG->SetTitle("Fit normalisations");
  normG->GetYaxis()->SetTitle("Background events in signal region");
  normG->GetYaxis()->SetTitleOffset(1.2);
  normG->GetXaxis()->SetTitle("m_{H} (GeV)");
  normG->Draw("ALP");
  canv->SaveAs("plots/pdf/fit_all_norm.pdf");
  canv->SaveAs("plots/png/fit_all_norm.png");
	delete normG;
	delete canv;
}
