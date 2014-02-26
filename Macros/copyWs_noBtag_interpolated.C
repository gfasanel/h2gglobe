copyWs_noBtag_interpolated(i){

  double corr_norm=((double)29376/(double)14675)*(2.28/2.23);
  double corr_sigma=1.;//considering the last x-sections available
  if(i==0){//500
    corr_sigma=0.590/0.571;
  }else if(i==1){//600
    corr_sigma=0.174/0.170;
  }else if(i==2){//700
    corr_sigma=0.0585/0.0569;
  }else if(i==3){//800
    corr_sigma=0.0213/0.0208;
  }else if(i==4){//900
    corr_sigma=0.00828/0.00809;
  }
  corr_norm*=corr_sigma;

  cout<<"corr_norm "<<corr_norm<<endl;

  RooWorkspace* cms_hgg_workspace=(RooWorkspace*) _file0->Get("cms_hgg_workspace");
  RooRealVar mass=(RooRealVar) cms_hgg_workspace->var("CMS_hgg_mass");
  
  RooWorkspace* newWs= new RooWorkspace("newWs");
  string masses[5]={"500","600","700","800","900"};
   

  //125.6

  string Cat[2];
  Cat[0]="cat6"; //Tprime leptonico
  Cat[1]="cat7"; //Tprime adronico

  string THY[3];
  THY[0]="Tprime";
  THY[1]="THWB";
  THY[2]="THtZ";

  double BR_Hgg_factor[3];
  BR_Hgg_factor[0]=2.;
  BR_Hgg_factor[1]=1.;
  BR_Hgg_factor[2]=1.;

  for(int k=0;k<2;k++){
    for(int decay=0;decay<3;decay++){
  TString name_tth="roohist_sig_tth_mass_m125.6_"+Cat[k];
  cout<<name_tth<<endl;
  RooDataHist* roohist_125_tth=(RooDataHist*)cms_hgg_workspace->data(name_tth);
  TH1F* histo_125_tth=(TH1F*)roohist_125_tth->createHistogram("CMS_hgg_mass");
  float norm_125_tth= histo_125_tth->Integral();
  cout<<"**********************"<<endl<<endl;
  cout<<"MASS POINT "<<masses[i]<<endl;
  cout<<"Categoria "<<Cat[k]<<endl;
  cout<<"Processo "<<THY[decay]<<endl<<endl;
  cout<<"norm_125 tth "<<norm_125_tth<<endl;

  norm_125_tth= histo_125_tth->Integral();
  TString name_tprime="roohist_sig_"+THY[decay]+"M"+masses[i]+"_mass_m120_"+Cat[k];
  TString name_tprime_final="roohist_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k];
  RooDataHist* roohist_120_tprime=(RooDataHist*)cms_hgg_workspace->data(name_tprime);
  TH1F* histo_120_tprime=(TH1F*)roohist_120_tprime->createHistogram("CMS_hgg_mass");
  cout<<"NORM_TPRIME_PRIMA "<<histo_120_tprime->Integral()<<endl;
  histo_120_tprime->Scale(BR_Hgg_factor[decay]*corr_norm);
  cout<<"Riscalamento totale "<<BR_Hgg_factor[decay]*corr_norm<<endl;
  float norm_120_tprime= histo_120_tprime->Integral();
  cout<<"norm_120 tprime riscalata "<<norm_120_tprime<<endl;
  histo_125_tth->Scale(norm_120_tprime/norm_125_tth);
  RooDataHist sig_tprime_mass_m125(("roohist_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]).c_str(),("roohist_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]).c_str(),mass,histo_125_tth);
  newWs->import(sig_tprime_mass_m125);


  /*************SYSTEMATICS**************/

  RooWorkspace* cms_hgg_workspace_noBtag=(RooWorkspace*) _file0->Get("cms_hgg_workspace");
  RooDataHist* roohist_125_tth_syst_down[6];
  TH1F* histo_125_tth_syst_down[6];
  RooDataHist* roohist_125_tth_syst_up[6];
  TH1F* histo_125_tth_syst_up[6];

  RooDataHist* roohist_125_tth_syst_down[0]=(RooDataHist*)cms_hgg_workspace_noBtag->data(name_tth+"_E_resDown01_sigma");
  histo_125_tth_syst_down[0]=(TH1F*)roohist_125_tth_syst_down[0]->createHistogram("CMS_hgg_mass");
  histo_125_tth_syst_down[0]->Scale(norm_120_tprime/norm_125_tth);
  RooDataHist sig_tprime_mass_m125_E_resDown01_sigma(name_tprime_final+"_E_resDown01_sigma",name_tprime_final+"_E_resDown01_sigma",mass,histo_125_tth_syst_down[0]);
  newWs->import(sig_tprime_mass_m125_E_resDown01_sigma);



  RooDataHist* roohist_125_tth_syst_up[0]=(RooDataHist*)cms_hgg_workspace_noBtag->data(name_tth+"_E_resUp01_sigma");
  histo_125_tth_syst_up[0]=(TH1F*)roohist_125_tth_syst_up[0]->createHistogram("CMS_hgg_mass");
  histo_125_tth_syst_up[0]->Scale(norm_120_tprime/norm_125_tth);
  RooDataHist sig_tprime_mass_m125_E_resUp01_sigma(name_tprime_final+"_E_resUp01_sigma",name_tprime_final+"_E_resUp01_sigma",mass,histo_125_tth_syst_up[0]);
  newWs->import(sig_tprime_mass_m125_E_resUp01_sigma);

  RooDataHist* roohist_125_tth_syst_down[1]=(RooDataHist*)cms_hgg_workspace_noBtag->data(name_tth+"_E_scaleDown01_sigma");
  histo_125_tth_syst_down[1]=(TH1F*)roohist_125_tth_syst_down[1]->createHistogram("CMS_hgg_mass");
  histo_125_tth_syst_down[1]->Scale(norm_120_tprime/norm_125_tth);
  RooDataHist sig_tprime_mass_m125_E_scaleDown01_sigma(name_tprime_final+"_E_scaleDown01_sigma",name_tprime_final+"_E_scaleDown01_sigma",mass,histo_125_tth_syst_down[1]);
  newWs->import(sig_tprime_mass_m125_E_scaleDown01_sigma);


  RooDataHist* roohist_125_tth_syst_up[1]=(RooDataHist*)cms_hgg_workspace_noBtag->data(name_tth+"_E_scaleUp01_sigma");
  histo_125_tth_syst_up[1]=(TH1F*)roohist_125_tth_syst_up[1]->createHistogram("CMS_hgg_mass");
  histo_125_tth_syst_up[1]->Scale(norm_120_tprime/norm_125_tth);
  RooDataHist sig_tprime_mass_m125_E_scaleUp01_sigma(name_tprime_final+"_E_scaleUp01_sigma",name_tprime_final+"_E_scaleUp01_sigma",mass,histo_125_tth_syst_up[1]);
  newWs->import(sig_tprime_mass_m125_E_scaleUp01_sigma);

  RooDataHist* roohist_125_tth_syst_down[2]=(RooDataHist*)cms_hgg_workspace_noBtag->data(name_tth+"_idEffDown01_sigma");
  histo_125_tth_syst_down[2]=(TH1F*)roohist_125_tth_syst_down[2]->createHistogram("CMS_hgg_mass");
  histo_125_tth_syst_down[2]->Scale(norm_120_tprime/norm_125_tth);
  RooDataHist sig_tprime_mass_m125_idEffDown01_sigma(name_tprime_final+"_idEffDown01_sigma",name_tprime_final+"_idEffDown01_sigma",mass,histo_125_tth_syst_down[2]);
  newWs->import(sig_tprime_mass_m125_idEffDown01_sigma);


  RooDataHist* roohist_125_tth_syst_up[2]=(RooDataHist*)cms_hgg_workspace_noBtag->data(name_tth+"_idEffUp01_sigma");
  //cout<<"histo syst"<<roohist_125_tth_syst_up[2]->Print();
  histo_125_tth_syst_up[2]=(TH1F*)roohist_125_tth_syst_up[2]->createHistogram("CMS_hgg_mass");
  histo_125_tth_syst_up[2]->Scale(norm_120_tprime/norm_125_tth);
  RooDataHist sig_tprime_mass_m125_idEffUp01_sigma(name_tprime_final+"_idEffUp01_sigma",name_tprime_final+"_idEffUp01_sigma",mass,histo_125_tth_syst_up[2]);
  newWs->import(sig_tprime_mass_m125_idEffUp01_sigma);

  RooDataHist* roohist_125_tth_syst_down[3]=(RooDataHist*)cms_hgg_workspace_noBtag->data(name_tth+"_r9EffDown01_sigma");
  histo_125_tth_syst_down[3]=(TH1F*)roohist_125_tth_syst_down[3]->createHistogram("CMS_hgg_mass");
  histo_125_tth_syst_down[3]->Scale(norm_120_tprime/norm_125_tth);
  RooDataHist sig_tprime_mass_m125_r9EffDown01_sigma(name_tprime_final+"_r9EffDown01_sigma",name_tprime_final+"_r9EffDown01_sigma",mass,histo_125_tth_syst_down[3]);
  newWs->import(sig_tprime_mass_m125_r9EffDown01_sigma);


  RooDataHist* roohist_125_tth_syst_up[3]=(RooDataHist*)cms_hgg_workspace_noBtag->data(name_tth+"_r9EffUp01_sigma");
  //cout<<"histo_125 syst"<<roohist_125_tth_syst_up[3]->Print();
  histo_125_tth_syst_up[3]=(TH1F*)roohist_125_tth_syst_up[3]->createHistogram("CMS_hgg_mass");
  histo_125_tth_syst_up[3]->Scale(norm_120_tprime/norm_125_tth);
  RooDataHist sig_tprime_mass_m125_r9EffUp01_sigma(name_tprime_final+"_r9EffUp01_sigma",name_tprime_final+"_r9EffUp01_sigma",mass,histo_125_tth_syst_up[3]);
  newWs->import(sig_tprime_mass_m125_r9EffUp01_sigma);

  RooDataHist* roohist_125_tth_syst_down[4]=(RooDataHist*)cms_hgg_workspace_noBtag->data(name_tth+"_triggerEffDown01_sigma");
  histo_125_tth_syst_down[4]=(TH1F*)roohist_125_tth_syst_down[4]->createHistogram("CMS_hgg_mass");
  histo_125_tth_syst_down[4]->Scale(norm_120_tprime/norm_125_tth);
  RooDataHist sig_tprime_mass_m125_triggerEffDown01_sigma(name_tprime_final+"_triggerEffDown01_sigma",name_tprime_final+"_triggerEffDown01_sigma",mass,histo_125_tth_syst_down[4]);
  newWs->import(sig_tprime_mass_m125_triggerEffDown01_sigma);


  RooDataHist* roohist_125_tth_syst_up[4]=(RooDataHist*)cms_hgg_workspace_noBtag->data(name_tth+"_triggerEffUp01_sigma");
  histo_125_tth_syst_up[4]=(TH1F*)roohist_125_tth_syst_up[4]->createHistogram("CMS_hgg_mass");
  histo_125_tth_syst_up[4]->Scale(norm_120_tprime/norm_125_tth);
  RooDataHist sig_tprime_mass_m125_triggerEffUp01_sigma(name_tprime_final+"_triggerEffUp01_sigma",name_tprime_final+"_triggerEffUp01_sigma",mass,histo_125_tth_syst_up[4]);
  newWs->import(sig_tprime_mass_m125_triggerEffUp01_sigma);

  RooDataHist* roohist_125_tth_syst_down[5]=(RooDataHist*)cms_hgg_workspace_noBtag->data(name_tth+"_vtxEffDown01_sigma");
  histo_125_tth_syst_down[5]=(TH1F*)roohist_125_tth_syst_down[5]->createHistogram("CMS_hgg_mass");
  histo_125_tth_syst_down[5]->Scale(norm_120_tprime/norm_125_tth);
  RooDataHist sig_tprime_mass_m125_vtxEffDown01_sigma(name_tprime_final+"_vtxEffDown01_sigma",name_tprime_final+"_vtxEffDown01_sigma",mass,histo_125_tth_syst_down[5]);
  newWs->import(sig_tprime_mass_m125_vtxEffDown01_sigma);


  RooDataHist* roohist_125_tth_syst_up[5]=(RooDataHist*)cms_hgg_workspace_noBtag->data(name_tth+"_vtxEffUp01_sigma");
  histo_125_tth_syst_up[5]=(TH1F*)roohist_125_tth_syst_up[5]->createHistogram("CMS_hgg_mass");
  histo_125_tth_syst_up[5]->Scale(norm_120_tprime/norm_125_tth);
  RooDataHist sig_tprime_mass_m125_vtxEffUp01_sigma(name_tprime_final+"_vtxEffUp01_sigma",name_tprime_final+"_vtxEffUp01_sigma",mass,histo_125_tth_syst_up[5]);
  newWs->import(sig_tprime_mass_m125_vtxEffUp01_sigma);
  
  if(masses[i]=="500" && Cat[k]=="cat6" && THY[decay]=="Tprime"){
    TFile*  f=TFile::Open("Tprime_ws_rescaled_new/histo_fasanel.root","recreate");
    histo_125_tth->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]).c_str());
    histo_125_tth->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]).c_str());

    //I also need to write the histos for syst
    histo_125_tth_syst_down[0]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_E_resDown01_sigma").c_str());
    histo_125_tth_syst_down[0]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_E_resDown01_sigma").c_str());
    histo_125_tth_syst_down[1]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_E_scaleDown01_sigma").c_str());
    histo_125_tth_syst_down[1]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_E_scaleDown01_sigma").c_str());
    histo_125_tth_syst_down[2]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_idEffDown01_sigma").c_str());
    histo_125_tth_syst_down[2]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_idEffDown01_sigma").c_str());
    histo_125_tth_syst_down[3]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_r9EffDown01_sigma").c_str());
    histo_125_tth_syst_down[3]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_r9EffDown01_sigma").c_str());
    histo_125_tth_syst_down[4]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_triggerEffDown01_sigma").c_str());
    histo_125_tth_syst_down[4]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_triggerEffDown01_sigma").c_str());
    histo_125_tth_syst_down[5]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_vtxEffDown01_sigma").c_str());
    histo_125_tth_syst_down[5]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_vtxEffDown01_sigma").c_str());

    histo_125_tth_syst_down[0]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_E_resUp01_sigma").c_str());
    histo_125_tth_syst_down[0]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_E_resUp01_sigma").c_str());
    histo_125_tth_syst_down[1]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_E_scaleUp01_sigma").c_str());
    histo_125_tth_syst_down[1]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_E_scaleUp01_sigma").c_str());
    histo_125_tth_syst_down[2]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_idEffUp01_sigma").c_str());
    histo_125_tth_syst_down[2]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_idEffUp01_sigma").c_str());
    histo_125_tth_syst_down[3]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_r9EffUp01_sigma").c_str());
    histo_125_tth_syst_down[3]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_r9EffUp01_sigma").c_str());
    histo_125_tth_syst_down[4]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_triggerEffUp01_sigma").c_str());
    histo_125_tth_syst_down[4]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_triggerEffUp01_sigma").c_str());
    histo_125_tth_syst_down[5]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_vtxEffUp01_sigma").c_str());
    histo_125_tth_syst_down[5]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_vtxEffUp01_sigma").c_str());

    histo_125_tth->Write();
    histo_125_tth_syst_down[0]->Write();
    histo_125_tth_syst_down[1]->Write();
    histo_125_tth_syst_down[2]->Write();
    histo_125_tth_syst_down[3]->Write();
    histo_125_tth_syst_down[4]->Write();
    histo_125_tth_syst_down[5]->Write();

    histo_125_tth_syst_up[0]->Write();
    histo_125_tth_syst_up[1]->Write();
    histo_125_tth_syst_up[2]->Write();
    histo_125_tth_syst_up[3]->Write();
    histo_125_tth_syst_up[4]->Write();
    histo_125_tth_syst_up[5]->Write();


    f->Write();
    f->Close();
  }else {
    TFile*  f=TFile::Open("Tprime_ws_rescaled_new/histo_fasanel.root","update");
    histo_125_tth->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]).c_str());
    histo_125_tth->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]).c_str());
    
    //I also need to write the histos for syst
    histo_125_tth_syst_down[0]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_E_resDown01_sigma").c_str());
    histo_125_tth_syst_down[0]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_E_resDown01_sigma").c_str());
    histo_125_tth_syst_down[1]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_E_scaleDown01_sigma").c_str());
    histo_125_tth_syst_down[1]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_E_scaleDown01_sigma").c_str());
    histo_125_tth_syst_down[2]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_idEffDown01_sigma").c_str());
    histo_125_tth_syst_down[2]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_idEffDown01_sigma").c_str());
    histo_125_tth_syst_down[3]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_r9EffDown01_sigma").c_str());
    histo_125_tth_syst_down[3]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_r9EffDown01_sigma").c_str());
    histo_125_tth_syst_down[4]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_triggerEffDown01_sigma").c_str());
    histo_125_tth_syst_down[4]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_triggerEffDown01_sigma").c_str());
    histo_125_tth_syst_down[5]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_vtxEffDown01_sigma").c_str());
    histo_125_tth_syst_down[5]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_vtxEffDown01_sigma").c_str());

    histo_125_tth_syst_down[0]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_E_resUp01_sigma").c_str());
    histo_125_tth_syst_down[0]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_E_resUp01_sigma").c_str());
    histo_125_tth_syst_down[1]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_E_scaleUp01_sigma").c_str());
    histo_125_tth_syst_down[1]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_E_scaleUp01_sigma").c_str());
    histo_125_tth_syst_down[2]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_idEffUp01_sigma").c_str());
    histo_125_tth_syst_down[2]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_idEffUp01_sigma").c_str());
    histo_125_tth_syst_down[3]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_r9EffUp01_sigma").c_str());
    histo_125_tth_syst_down[3]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_r9EffUp01_sigma").c_str());
    histo_125_tth_syst_down[4]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_triggerEffUp01_sigma").c_str());
    histo_125_tth_syst_down[4]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_triggerEffUp01_sigma").c_str());
    histo_125_tth_syst_down[5]->SetName(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_vtxEffUp01_sigma").c_str());
    histo_125_tth_syst_down[5]->SetTitle(("th1f_sig_"+THY[decay]+"M"+masses[i]+"_mass_m125.6_"+Cat[k]+"_vtxEffUp01_sigma").c_str());

    histo_125_tth->Write();
    histo_125_tth_syst_down[0]->Write();
    histo_125_tth_syst_down[1]->Write();
    histo_125_tth_syst_down[2]->Write();
    histo_125_tth_syst_down[3]->Write();
    histo_125_tth_syst_down[4]->Write();
    histo_125_tth_syst_down[5]->Write();

    histo_125_tth_syst_up[0]->Write();
    histo_125_tth_syst_up[1]->Write();
    histo_125_tth_syst_up[2]->Write();
    histo_125_tth_syst_up[3]->Write();
    histo_125_tth_syst_up[4]->Write();
    histo_125_tth_syst_up[5]->Write();

    f->Write();
    f->Close();
  }
  
  
    }//chiude il for sui decays
  }//chiude il for sulle categorie

  newWs->writeToFile(("Tprime_ws_rescaled_new/CMS-HGG_fasanel_"+masses[i]+"scaled.root").c_str(),1);
}//chiude la macro
