#ifndef Normalization_8TeV_h
#define Normalization_8TeV_h

#include <vector>
#include <map>
#include <iostream>

#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TString.h"
#include "TROOT.h"
#include "TLegend.h"
#include "TPython.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooSpline1D.h"

using namespace std;

class Normalization_8TeV {

  public:
	Normalization_8TeV();

	void Init(int sqrtS);
	
	double GetBR(double);
	// double GetBR(int);
	double GetXsection(double,TString);
	double GetXsection(double);
	// double GetXsection(int);
	double GetNorm(double,TH1F*,double, TH1F*,double);
	double GetMass(int);
	double GetVBFCorrection(double);
	TString GetProcess(int);
	void CheckNorm(double,double,double,TString);
	void FillSignalTypes();
	void PlotExpected(double ,double);	
	void PlotBR(double ,double);	
	void PlotXS(double ,double);	

	TGraph * GetSigmaGraph(TString process);
	TGraph * GetBrGraph();
	
	std::map<int,std::pair<TString,double > > & SignalType() { return SignalTypeMap; }
 private:
	std::map<double,double> BranchingRatioMap;
	std::map<double,double> XSectionMap_ggh;
	std::map<double,double> XSectionMap_vbf;
	std::map<double,double> XSectionMap_vbfold;
	std::map<double,double> XSectionMap_wh;
	std::map<double,double> XSectionMap_zh;
	std::map<double,double> XSectionMap_wzh;
	std::map<double,double> XSectionMap_tth;
  	std::map<double,double> XSectionMap_sm;

	std::map<double,double> XSectionMap_TprimeM400;
	std::map<double,double> XSectionMap_TprimeM450;
	std::map<double,double> XSectionMap_TprimeM500;
	std::map<double,double> XSectionMap_TprimeM550;
	std::map<double,double> XSectionMap_TprimeM600;
	std::map<double,double> XSectionMap_TprimeM650;
	std::map<double,double> XSectionMap_TprimeM700;
	std::map<double,double> XSectionMap_TprimeM750;
	std::map<double,double> XSectionMap_TprimeM800;
	std::map<double,double> XSectionMap_TprimeM850;
	std::map<double,double> XSectionMap_TprimeM900;

	std::map<double,double> XSectionMap_THWBM500;
	std::map<double,double> XSectionMap_THWBM600;
	std::map<double,double> XSectionMap_THWBM700;
	std::map<double,double> XSectionMap_THWBM800;
	std::map<double,double> XSectionMap_THWBM900;
	std::map<double,double> XSectionMap_THtZM500;
	std::map<double,double> XSectionMap_THtZM600;
	std::map<double,double> XSectionMap_THtZM700;
	std::map<double,double> XSectionMap_THtZM800;
	std::map<double,double> XSectionMap_THtZM900;

	std::map<int,std::pair<TString,double > > SignalTypeMap;
	
	bool is2011_;

};
#endif
