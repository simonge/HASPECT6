//Class to compute various common kinematic quantities

#ifndef THSKINEMATICS_h
#define THSKINEMATICS_h


#include "THSParticle.h"
#include <TLorentzRotation.h>



class THSKinematics{
  
 private :
  TLorentzVector fElin;
  TLorentzVector fElsc;
  TLorentzVector fGamma;
  TLorentzVector fCM;
  TLorentzVector fTar;
  TLorentzVector fMes;
  TLorentzVector fBar;
  TLorentzVector fMes_d1;
  TLorentzVector fMes_d2;
  TLorentzVector fBar_d1;
  TLorentzVector fBar_d2;
  
  
  Double_t fCosTh=0;
  Double_t fPhi=0;
  
 public :
  THSKinematics(){};
  ~THSKinematics(){};
  
  Double_t CosTheta(){return fCosTh;}
  Double_t Phi(){return fPhi;}

  void SetTarget(TLorentzVector tar){fTar=tar;};
  void SetElecsTarget(TLorentzVector in,TLorentzVector sc,TLorentzVector tar){
    fElin=in;
    fElsc=sc;
    fGamma=fElin-fElsc;
    fTar=tar;
    fCM=fGamma+fTar;
  
   }
  void SetGammaTarget(TLorentzVector gamma,TLorentzVector tar){
    fGamma=gamma;
    fTar=tar;
    fCM=gamma+tar;
  }
  void SetMesonBaryon(TLorentzVector mes,TLorentzVector bar){
    fMes=mes;
    fBar=bar;
  }
  void SetMesonDecay(TLorentzVector d1,TLorentzVector d2){
    fMes_d1=d1;
    fMes_d2=d2;
  }
  void SetBaryonDecay(TLorentzVector d1,TLorentzVector d2){
    fBar_d1=d1;
    fBar_d2=d2;
  }

  TLorentzVector Gamma(){return fGamma;}
  Double_t W(){ return fCM.M();}
  Double_t Q2() {return -fGamma.M2();}
  Double_t GammaE() {return fGamma.E();}
  Double_t GammaTh() {return fGamma.Theta();}
  Double_t ElScatTh() {return fElsc.Theta();}
  Double_t GammaPol(){return 1./(1+2*(1+GammaE()*GammaE()/Q2())*TMath::Tan(ElScatTh()/2)*TMath::Tan(ElScatTh()/2));}
  Double_t GammaFlux(){return(1./137/4/TMath::Pi()/TMath::Pi())/fElin.E()*fElsc.E()*((W()*W()-fTar.M2())/fTar.M())/Q2()/(1-GammaPol()); }

  
  Double_t t(){return t(fGamma,fMes);}//default gamma-meson
  Double_t t(TLorentzVector p0,TLorentzVector p1){
    TLorentzVector transfer=p0-p1;
    return transfer.M2();
  }
  Double_t t0(){return t0(fGamma,fMes);}//default gamma-meson
  Double_t t0(TLorentzVector p0,TLorentzVector p1);
  
  //Decay angles
  void MesonDecayHelicity();
  void MesonDecayGJ();
  void ElectroCMDecay();
  void PhotoCMDecay();
};

inline Double_t THSKinematics::t0(TLorentzVector p0,TLorentzVector p1){   
  TLorentzVector cm0=p0;
  cm0.Boost(-fCM.BoostVector());
  TLorentzVector cm1=p1;
  cm1.Boost(-fCM.BoostVector());
  Double_t P0CM=cm0.Rho();
  Double_t P1CM=cm1.Rho();
  
  Double_t t0=(p1.M2()/2/fCM.M())*(p1.M2()/2/fCM.M())-(P0CM-P1CM)*(P0CM-P1CM);
  return t(p0,p1) - t0;
}

inline void THSKinematics::ElectroCMDecay(){
  //CM frame defined by e-scattering
  /* TLorentzRotation CMBoost(-fCM.BoostVector()); */
  /* TLorentzVector CMBeam=CMBoost*fElin; */
  /* TLorentzVector CMScat=CMBoost*fElsc; */
  /* TLorentzVector CMMes=CMBoost*fMes; */
  /* TLorentzVector CMGamma=CMBoost*fGamma; */
  TVector3 CMBoost(-fCM.BoostVector());
  TLorentzVector CMBeam=fElin;
  CMBeam.Boost(CMBoost);
  TLorentzVector CMScat=fElsc;
  CMScat.Boost(CMBoost);
  TLorentzVector CMMes=fMes;
  CMMes.Boost(CMBoost);
  TLorentzVector CMGamma=fGamma;
  CMGamma.Boost(CMBoost);

  
  // TVector3 zV=CMBeam.Vect().Unit();
  //TVector3 yV=CMBeam.Vect().Cross(CMGamma.Vect()).Unit();
  TVector3 zV=CMGamma.Vect().Unit();
  TVector3 yV=CMBeam.Vect().Cross(CMGamma.Vect()).Unit();
  TVector3 xV=yV.Cross(zV).Unit();
  
  TVector3 angles(CMMes.Vect().Dot(xV),CMMes.Vect().Dot(yV),CMMes.Vect().Dot(zV));
  fCosTh=angles.CosTheta();
  fPhi=angles.Phi();
}
inline void THSKinematics::PhotoCMDecay(){
  //CM frame defined by CM boost. No rotation to lab phi
  TLorentzRotation CMBoost(-fCM.BoostVector());
  TLorentzVector CMMes=CMBoost*fMes;
  fCosTh=CMMes.CosTheta();
  fPhi=CMMes.Phi();
}

inline void THSKinematics::MesonDecayHelicity(){
  //z-axis along -baryon in meson rest frame
  TLorentzRotation decBoost(-fMes.BoostVector());
  TLorentzVector decBar=decBoost*fBar;
  TLorentzVector decGamma=decBoost*fGamma;
  TVector3 zV=-decBar.Vect().Unit();
  TVector3 yV=decBar.Vect().Cross(decGamma.Vect()).Unit();
  TVector3 xV=yV.Cross(zV).Unit();
   
  TLorentzVector decD1=decBoost*fMes_d1;
  
  TVector3 angles(decD1.Vect().Dot(xV),decD1.Vect().Dot(yV),decD1.Vect().Dot(zV));
  fCosTh=angles.CosTheta();
  fPhi=angles.Phi();
}

inline void THSKinematics::MesonDecayGJ(){
  //z-axis along gamma direction in meson decay frame
  TLorentzRotation decBoost(-fMes.BoostVector());
  TLorentzVector decGamma=decBoost*fGamma;
  TLorentzVector decBar=decBoost*fBar;
  TVector3 zV=decGamma.Vect().Unit();
  TVector3 yV=decBar.Vect().Cross(decGamma.Vect()).Unit();
  TVector3 xV=yV.Cross(zV).Unit();

  TLorentzVector decD1=decBoost*fMes_d1;

  TVector3 angles(decD1.Vect().Dot(xV),decD1.Vect().Dot(yV),decD1.Vect().Dot(zV));
  fCosTh=angles.CosTheta();
  fPhi=angles.Phi();
}

#endif //ifdef THKinematics
