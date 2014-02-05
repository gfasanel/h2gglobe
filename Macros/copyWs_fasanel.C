copyWs_fasanel(int i){
  double corr_norm=((double)29376/(double)14675)*(2.28/2.23);
  //correggo per come e' fatto il sample (io prendo solo gli eventi con 2 fotoni) e poi correggo mettendo il giusto BR(Hgg) con mH=125 GeV
  cout<<"corr_norm "<<corr_norm<<endl;
  RooWorkspace* cms_hgg_workspace=(RooWorkspace*) _file0->Get("cms_hgg_workspace");
  RooRealVar mass=(RooRealVar) cms_hgg_workspace->var("CMS_hgg_mass");

   RooWorkspace* newWs= new RooWorkspace("newWs");
   //   string masses[7]={"400","450","500","550","700","800","900"};
   string masses[5]={"500","600","700","800","900"};

   //cat6 
   TString name_tth="roohist_sig_tth_mass_m125_cat6";
   RooDataHist* roohist_125_tth=(RooDataHist*)cms_hgg_workspace->data(name_tth);
   TH1F* histo_125_tth=(TH1F*)roohist_125_tth->createHistogram("CMS_hgg_mass");
   //histo_125_tth->Print();
   float norm_125_tth= histo_125_tth->Integral();
   cout<<"norm_125 tth"<<norm_125_tth<<endl;
   cout<<"histo ttH lep preso"<<endl;
   //cat7 
   TString name_tth_cat7="roohist_sig_tth_mass_m125_cat7";
   RooDataHist* roohist_125_tth_cat7=(RooDataHist*)cms_hgg_workspace->data(name_tth_cat7);
   TH1F* histo_125_tth_cat7=(TH1F*)roohist_125_tth_cat7->createHistogram("CMS_hgg_mass");
   //histo_125_tth_cat7->Print();
   float norm_125_tth_cat7= histo_125_tth_cat7->Integral();
   cout<<"norm_125 tth_cat7"<<norm_125_tth_cat7<<endl;

   //   for(int i=1;i<2;i++){
     cout<<endl;
     cout<<"MASSA "<<masses[i]<<endl;
   //   for(int i=3;i<4;i++){//solo m_T=550
   norm_125_tth= histo_125_tth->Integral();
   TString name_tprime="roohist_sig_TprimeM"+masses[i]+"_mass_m120_cat6";
   RooDataHist* roohist_120_tprime=(RooDataHist*)cms_hgg_workspace->data(name_tprime);
   TH1F* histo_120_tprime=(TH1F*)roohist_120_tprime->createHistogram("CMS_hgg_mass");
   //histo_120_tprime->Print();
   histo_120_tprime->Scale(2*corr_norm);
   float norm_120_tprime= histo_120_tprime->Integral();
   cout<<"norm_120 tprime"<<norm_120_tprime<<endl;
   histo_125_tth->Scale(norm_120_tprime/norm_125_tth);
   //histo_125_tth->Draw();
   RooDataHist sig_tth_mass_m125_cat6(("roohist_sig_TprimeM"+masses[i]+"_mass_m125_cat6").c_str(),("roohist_sig_TprimeM"+masses[i]+"_mass_m125_cat6").c_str(),mass,histo_125_tth);
   newWs->import(sig_tth_mass_m125_cat6);

   norm_125_tth_cat7= histo_125_tth_cat7->Integral();
   TString name_tprime="roohist_sig_TprimeM"+masses[i]+"_mass_m120_cat7";
   RooDataHist* roohist_120_tprime=(RooDataHist*)cms_hgg_workspace->data(name_tprime);
   TH1F* histo_120_tprime=(TH1F*)roohist_120_tprime->createHistogram("CMS_hgg_mass");
   //histo_120_tprime->Print();
   histo_120_tprime->Scale(2*corr_norm);
   float norm_120_tprime= histo_120_tprime->Integral();
   cout<<"norm_120 tprime"<<norm_120_tprime<<endl;
   histo_125_tth_cat7->Scale(norm_120_tprime/norm_125_tth_cat7);
   //histo_125_tth_cat7->Draw();
   RooDataHist sig_tth_cat7_mass_m125_cat7(("roohist_sig_TprimeM"+masses[i]+"_mass_m125_cat7").c_str(),("roohist_sig_TprimeM"+masses[i]+"_mass_m125_cat7").c_str(),mass,histo_125_tth_cat7);
   newWs->import(sig_tth_cat7_mass_m125_cat7);
   newWs->writeToFile(("Tprime_ws_rescaled/CMS-HGG_fasanel_"+masses[i]+"scaled.root").c_str(),1);

   if(masses[i]=="500"){
     TFile*  f=TFile::Open("Tprime_ws_rescaled/histo_fasanel.root","recreate");
     histo_125_tth->SetName(("th1f_sig_TprimeM"+masses[i]+"_mass_m125_cat6").c_str());
     histo_125_tth->SetTitle(("th1f_sig_TprimeM"+masses[i]+"_mass_m125_cat6").c_str());
     histo_125_tth->Write();

     histo_125_tth_cat7->SetName(("th1f_sig_TprimeM"+masses[i]+"_mass_m125_cat7").c_str());
     histo_125_tth_cat7->SetTitle(("th1f_sig_TprimeM"+masses[i]+"_mass_m125_cat7").c_str());
     histo_125_tth_cat7->Write();
     f->Write();
     f->Close();
   }else {
     TFile*  f=TFile::Open("Tprime_ws_rescaled/histo_fasanel.root","update");
     histo_125_tth->SetName(("th1f_sig_TprimeM"+masses[i]+"_mass_m125_cat6").c_str());
     histo_125_tth->SetTitle(("th1f_sig_TprimeM"+masses[i]+"_mass_m125_cat6").c_str());
     histo_125_tth->Write();
     
     histo_125_tth_cat7->SetName(("th1f_sig_TprimeM"+masses[i]+"_mass_m125_cat7").c_str());
     histo_125_tth_cat7->SetTitle(("th1f_sig_TprimeM"+masses[i]+"_mass_m125_cat7").c_str());
     histo_125_tth_cat7->Write();
     f->Write();
     f->Close();
   }
   
   
   //   }
   

   
}
