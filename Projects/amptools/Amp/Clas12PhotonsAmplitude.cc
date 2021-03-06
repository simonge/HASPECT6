#include <cassert>
#include <iostream>
#include <string>
#include <complex>
#include <cstdlib>
#include "IUAmpTools/Kinematics.h"
#include "Clas12PhotonsAmplitude.h"

 
/* This is a derived class from AmpTools Amplitude.
It's written for the quasi-real photoproduction physics of MesonEx in CLAS12.
Basically what it does is to perform the calculation of the leptonic current, event by event, summing over the helicity of the quasi-real photon.
The spin of the initial and final state electron are two protected members of this class, that are initialized to their value by the derived amplitude from the user!
The user has to derive its own amplitude from this class, specifing the photoproduction amplitude as a function of the helicity.
*/

//the order of the particles is supposed to be:
//0: beam  
//1: target
//2: recoil
//3: electron scattered
//4: all the other particles


Clas12PhotonsAmplitude::Clas12PhotonsAmplitude(const vector<string>& args): 
  Amplitude(args)
{
  assert (args.size()==4); //helicity beam, helicity electron, (target), (recoil). Last two from the daughter amplitude only.
  m_helicity_beam=atoi(args[0].c_str());
  m_helicity_electron=atoi(args[1].c_str());

}

/*This calculates the electron-scattering 
 */

int Clas12PhotonsAmplitude::calcElectronScattering( GDouble** pKin,int Ielectron,int Iscattered,ElectronScatteringTerm &ElectronScattering) const{
  
  TLorentzVector beam;
  TLorentzVector electron;
  double E1,E2,Eg;
  double theta1,theta2,phi1,phi2;
  double Pzg;double Mg;

  
  //define here all the relevant variables
  beam.SetPxPyPzE(pKin[m_Ielectron][1],pKin[m_Ielectron][2],pKin[m_Ielectron][3],pKin[m_Ielectron][0]);
  electron.SetPxPyPzE(pKin[m_Iscattered][1],pKin[m_Iscattered][2],pKin[m_Iscattered][3],pKin[m_Iscattered][0]);
  E1=pKin[m_Ielectron][0];
  E2=pKin[m_Iscattered][0];

  theta1=beam.Theta();theta2=electron.Theta();
  phi1=beam.Phi();phi2=electron.Phi();


  Pzg=pKin[m_Ielectron][3]-pKin[m_Iscattered][3]; //quasi real photon momentum (GJ:Purely along z)
  Mg=sqrt(Pzg*Pzg-Eg*Eg);



   if ((m_helicity_beam==1)&&(m_helicity_electron==1)){

    ElectronScattering.JP=complex<GDouble>(cos(phi2),-sin(phi2));
    ElectronScattering.JP*=2*sqrt(2*E1*E2)*cos(theta1/2)*sin(theta2/2);
    
    ElectronScattering.JM=complex<GDouble>(cos(phi1),sin(phi1));
    ElectronScattering.JM*=-2*sqrt(2*E1*E2)*cos(theta2/2)*sin(theta1/2);

    ElectronScattering.J0=complex<GDouble>((Eg-Pzg)*cos(theta1/2)*cos(theta2/2)-(Eg+Pzg)*sin(theta1/2)*sin(theta2/2)*cos(phi1-phi2),-(Eg+Pzg)*sin(theta1/2)*sin(theta2/2)*sin(phi1-phi2));
    ElectronScattering.J0*=-(2*sqrt(E1*E2)/Mg);
   										  
  }
  else if ((m_helicity_beam==1)&&(m_helicity_electron==-1)){
    ElectronScattering.JP=0;
    ElectronScattering.JM=0;
    ElectronScattering.J0=0;
  }
  else if ((m_helicity_beam==-1)&&(m_helicity_electron==1)){
    ElectronScattering.JP=0;
    ElectronScattering.JM=0;
    ElectronScattering.J0=0;
  }

  else if ((m_helicity_beam==-1)&&(m_helicity_electron==-1)){
    ElectronScattering.JP=complex<GDouble>(cos(phi1),-sin(phi1));
    ElectronScattering.JP*=2*sqrt(2*E1*E2)*cos(theta2/2)*sin(theta1/2);

    ElectronScattering.JM=complex<GDouble>(cos(phi2),sin(phi2));
    ElectronScattering.JM*=-2*sqrt(2*E1*E2)*cos(theta1/2)*sin(theta2/2);

    ElectronScattering.J0=complex<GDouble>((Eg-Pzg)*cos(theta1/2)*cos(theta2/2)-(Eg+Pzg)*sin(theta1/2)*sin(theta2/2)*cos(phi2-phi1),-(Eg+Pzg)*sin(theta1/2)*sin(theta2/2)*sin(phi2-phi1));
    ElectronScattering.J0*=-(2*sqrt(E1*E2)/Mg);
  }

   
   return 0;


   
}


complex< GDouble >
Clas12PhotonsAmplitude::calcAmplitude( GDouble** pKin ) const {


  complex<GDouble> helP;
  complex<GDouble> hel0;
  complex<GDouble> helM;


 //calculate the photoproduction part of the amplitude trough the code provided by the user
  helP= calcHelicityAmplitude(1,pKin);
  hel0= calcHelicityAmplitude(0,pKin);
  helM= calcHelicityAmplitude(-1,pKin);

  //trigger the calculation of the electron scattering part.
  //This fills the JP,J0,JM complex numbers
  ElectronScatteringTerm ElectronScattering;
  calcElectronScattering(pKin,0,3,ElectronScattering);
  cout<<"es "<<ElectronScattering.JP<<" "<<ElectronScattering.J0<<" "<<ElectronScattering.JM<<endl;
  return (helP*ElectronScattering.JP+hel0*ElectronScattering.J0+helM*ElectronScattering.JM);

 
  

}

#ifdef GPU_ACCELERATION
void
Constant::launchGPUKernel( dim3 dimGrid, dim3 dimBlock, GPU_AMP_PROTO ) const {
  
  //Let's keep this as it is
  //Then:
  ElectronScatteringTerm ElectronScattering;
  calcElectronScattering(pKin,0,3,ElectronScattering);
  

  // use integers to endcode the string of daughters -- one index in each
  // decimal place
  
  /*
    1) NOTE THAT THIS DOES NOT RETURN ANYTHING, THE AMPLITUDE RESULT IS IN A "GLOBAL GPU MEMORY"
    2) ALSO, THE INPUT PARTICLES 4-vector are AGAIN IN A GLOBAL MEMORY!
    3) WE MUST PASS OUR 3 complex number (or the structure ElectronScattering) to this function. (How? Pointer/reference/value?)
    4) IN EACH SPECIFIC AMPLITUDE ON GPU we do the sum


    GPUBreitWigner_exec( dimGrid,  dimBlock, GPU_AMP_ARGS, m_module,m_phase );


  */
}
#endif //GPU_ACCELERATION
