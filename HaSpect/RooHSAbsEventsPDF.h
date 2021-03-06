#ifndef ROOHSABSEVENTSPDF
#define ROOHSABSEVENTSPDF

#include "RooAbsPdf.h"
#include "RooArgSet.h"
#include "RooRealProxy.h"
#include "THSWeights.h" 
#include "TTree.h" 
#include "TChain.h" 
#include "TEntryList.h" 
#include <vector>
#include <map>

class RooHSAbsEventsPDF : public RooAbsPdf {
 public:
 RooHSAbsEventsPDF(const char *name, const char *title):RooAbsPdf(name,title){};
  RooHSAbsEventsPDF(const RooHSAbsEventsPDF& other, const char* name=0);
  
  //virtual TObject* clone(const char* newname) const { return new RooHSAbsEventsPDF(*this,newname); }

  RooHSAbsEventsPDF(){}; 
  virtual ~RooHSAbsEventsPDF();
  
 protected:
  RooHSAbsEventsPDF* fParent=nullptr;
  TChain* fEvTree=nullptr;
  TEntryList* fEntryList=nullptr;
  Double_t *fLast=nullptr;
  Long64_t fNInt=-1;
  Long64_t fNMCGen=0; //Number of generated MC events
  Int_t fNpars=0;
  Bool_t fIsIntegrating=kFALSE;
  Bool_t fIsClone=kFALSE;
  Bool_t fForceConstInt=kFALSE;
  Bool_t fUseWeightsGen=kTRUE;
  Double_t fConstInt=1;
  THSWeights* fWeights=0;
  
  vector< RooArgSet* > fVarSet;//set of variables for which integral defined
  vector< RooRealProxy* > fProxSet; //fixed variable in integration, i.e should not be in the corresponding fVarSet entry
  //RooArgList fProxSet; //fixed variable in integration, i.e should not be in the corresponding fVarSet entry
  vector<Double_t> fMCVar;
  vector< RooRealProxy* > fParSet;
  void InitSets();
  RooArgSet VarSet(Int_t iset) const;
    
  Double_t fMaxValue=0; //max value of function for accept/reject
  Long64_t fGeni=0; //index for tree generation
  TString fgenStr="gen";
 public:
  virtual Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars,const char* rangeName) const;
  virtual Double_t analyticalIntegral(Int_t code,const char* rangeName) const;

  virtual void generateEvent(Int_t code);
  virtual Int_t getGenerator(const RooArgSet& directVars, RooArgSet &generateVars, Bool_t staticInitOK) const;
  virtual void initGenerator(Int_t code);
  virtual void initIntegrator();

  //require an evaluateMC class to return same as evaluate but with
  //variables from fEvTree, it would be nicer to just use evaluate
  //but use of RooProxy variables complicates it
  virtual Double_t evaluateMC() const {return 0.;};


  virtual Bool_t CheckChange() const; //Have any fit parameters changed since last integral?
  Bool_t CheckRange(const char* rangeName) const; //only integrate EvTree over specifed variable range

  void SetNInt(Long64_t n){fNInt=n;}
  virtual Bool_t SetEvTree(TChain* tree,Long64_t ngen=0);
  void SetNMCGen(Long64_t N){fNMCGen=N;}
  TTree* GetGenTree(){fEvTree->SetEntryList(fEntryList);TTree* tree=fEvTree->CopyTree("");fEvTree->SetEntryList(0);return tree;};//whoever gets should delete
  TEntryList* GetEntryList(){return fEntryList;}
  void SetEntryList(TEntryList* elist){fEntryList=dynamic_cast<TEntryList*>(elist->Clone());}
  void SetWeights(THSWeights *wgts){fWeights=wgts;}
  void SetUseWeightsGen(Bool_t use=kTRUE){fUseWeightsGen=use;}
  Bool_t UseWeightsGen(){return fUseWeightsGen;}
  THSWeights* GetWeights(){return fWeights;}
  void SetGeni(Long64_t gi){fGeni=gi;};
  Long64_t GetGeni(){return fGeni;}
  void SetConstInt(Bool_t force=kTRUE){fForceConstInt=force;}
  void  CheckIntegralParDep(Int_t Ntests);


  ClassDef(RooHSAbsEventsPDF,1) // Your description goes here...
};
 
#endif

