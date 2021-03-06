/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROOHSEVENTSHISTPDF
#define ROOHSEVENTSHISTPDF

#include "RooAbsPdf.h"
#include "RooHSAbsEventsPDF.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooRealVar.h"
#include "TH2.h" 

class RooHSEventsHistPDF : public RooHSAbsEventsPDF {
public:
  RooHSEventsHistPDF() {cout<<"IN DEFAULT "<<endl;} ; 
  RooHSEventsHistPDF(const char *name, const char *title,
	      RooAbsReal& _x,
	      RooAbsReal& _offset,
	      RooAbsReal& _scale,
	      RooAbsReal& _alpha);
  RooHSEventsHistPDF(const RooHSEventsHistPDF& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooHSEventsHistPDF(*this,newname); }
  inline virtual ~RooHSEventsHistPDF();

protected:
  Double_t fMCx;

  RooRealProxy x ;
  RooRealProxy offset ;
  RooRealProxy scale ;
  RooRealProxy alpha ;
  
  Double_t evaluate() const ;
  Double_t evaluateMC() const ;
  Double_t evaluateMC(Double_t mcx) const ;
  void MakeSets();
  
  RooDataHist* fHist=0;
  TH2D* fRHist=0;
  Double_t fVarMax=0;
  
  RooRealVar* fx_off=0; //variables for hist
  RooRealVar* falpha=0;

private:
  RooGaussian *fAlphaConstr=0;
  RooGaussian *fOffConstr=0;
  RooGaussian *fScaleConstr=0;
  
 public:

  virtual Bool_t SetEvTree(TChain* tree,Long64_t ngen=0);
  void CreateHistPdf();

  virtual Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars,const char* rangeName) const;
  Double_t analyticalIntegral(Int_t code,const char* rangeName) const;
  
  RooGaussian* AlphaConstraint() {return fAlphaConstr;};
  RooGaussian* OffConstraint() {return fOffConstr;};
  RooGaussian* ScaleConstraint() {return fScaleConstr;};

  ClassDef(RooHSEventsHistPDF,1) // Your description goes here...
};
 
#endif
