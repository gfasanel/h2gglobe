{
  gROOT->ProcessLine(".L copyWs_fasanel.C");
  copyWs_fasanel(0);//500
  copyWs_fasanel(1);//600
  copyWs_fasanel(2);//700
  copyWs_fasanel(3);//800
  copyWs_fasanel(4);//900
  //  copyWs_fasanel(5);
  //  copyWs_fasanel(6);
  gROOT->ProcessLine(".q");
}
