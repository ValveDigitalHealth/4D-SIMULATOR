/* The Luo-Rudy Dynamic (LRd) Model of the Mammalian ventricular Myocyte */
/* Gregory Faber */
/* This code requires a C++ compiler */
/* Detailed list of equations and model description are provided in */
#include <iostream.h>
#include <iomanip.h>
#include <math.h>
#include <fstream.h>
#include <stdlib.h>
#include <stdio.h>
#include <vcl.h>
#include "LRdModel_cell_91.h"
/* List of variables and paramaters (this code uses all global variables) */
//************************************************************************
Luo_Rudy_91_cell::Luo_Rudy_91_cell()
{
    Istim = 0.0;

    Tip_Function = 0.0;
    Tip_Threshold = -10.0;
    Tip_Was_Here = 0;

    Diffusion_Term = 0.0;
    Diffusion_Coefficient = 0.0001;

    t = 1000.0;
    dt = 0.2;

    v = 0;
    vnew = 0;
    vprev_1 = 0;
    vprev_2 = 0;

    // Standard ionic concentrations for ventricular cells
    K_o = 5.4;                  // mmol/L
    K_i = 145;                  // mmol/L
    Na_o = 140;                 // mmol/L
    Na_i = 10;                  // mmol/L
    Ca_o = 1.8;                 // mmol/L
    Ca_i = 0.12*1000;        // mmol/L
    K_o = 5.4;                  // mmol/L

    // Physic constants
    F = 96.5;                   // Faraday constant, coulombs/mmol
    R = 8.314;                  // gas constant, J/K
    T = 273+37;                 // absolute temperature, K

    // Cell constant
    PK = 1.66*pow(10,-6);                 // permability of K
    PNa_K = 0.01833;                   // permability ratio of Na to K
    PCa_K = 0.9;                  //////////// need to be varified
    Cm = 1;                            // membrane capacitance, uF/cm^2;

    GNa = 16.0; // acc with Xi,Qu Lr1+serce psa
    ENa = 54.4; // jest bardziej zlozone wyrazenie na to

    Gk = 0.423; // acc with Xi,Qu Lr1+serce psa
    Ek = (R*T/F)*log((K_o+PNa_K*Na_o)/(K_i+PNa_K*Na_i));        // mv

    Gsi = 0.025; //0.09;
    Esi = 7.7 - 13.0287*log(Ca_i);

    Gk1 = 0.6047*sqrt(K_o/5.4);
    Ek1 = (R*T/F)*log(K_o/K_i);

    Gkp = 0.0183;
    Ekp = (R*T/F)*log(K_o/K_i);

    initialize_variables();

}
//************************************************************************
void Luo_Rudy_91_cell::initialize_variables()
{
    v = -84.522;
    vprev_1 = v;
    vprev_2 = v;
    vnew = v;
    Timer = 0.0;
    V_Tip_Timer_ms_ago = v;
    m = 0.0016732;
    h = 0.9832;
    j = 0.9894;

    d = 0.0029;
    f = 0.999;

    X = 0.0056;

    Isi = 0.0;
    INa = 0.0;
    IK = 0.0;
    IK1 = 0.0;
    Ib = 0.0;
    IKp = 0.0;
    Ca_i = 0.12*1000;        // mmol/L


}

//************************************************************************
void Luo_Rudy_91_cell::calculate_Ib()
{
    Ib = 0.03921*(v+59.87);
}
//************************************************************************
void Luo_Rudy_91_cell::calculate_INa()
{
    if(v>=-40.0)
    {
        ah = 0.0;
        aj = 0.0;
        bh = 1.0/(0.13*(1.0+exp((v+10.66)/(-11.1))));
        bj = 0.3*exp(-2.535*pow(10.0,-7.0)*v)/(1.0+exp(-0.1*(v+32.0)));
    }
    else
    {
       ah = 0.135*exp((80.0+v)/(-6.8));
       aj = (-1.2714*pow(10,5)*exp(0.2444*v)-3.474*pow(10,-5)*exp(-0.04391*v))
          *(v+37.78)/(1.0+exp(0.311*(v+79.23)));
       bh = 3.56*exp(0.079*v)+3.1*pow(10,5)*exp(0.35*v);
       bj = 0.1212*exp(-0.01052*v)/(1.0+exp(-0.1378*(v+40.14)));
    }
    am = 0.32*(v+47.13)/(1.0-exp(-0.1*(v+47.13)));
    bm = 0.08*exp(-v/11.0);

    infm = am/(am+bm);
    infh = ah/(ah+bh);
    infj = aj/(aj+bj);

    taum = 1.0/(am+bm);
    tauh = 1.0/(ah+bh);
    tauj = 1.0/(aj+bj);

    mm = infm - (infm-m)*exp(-fabs(dt)/taum);     // the hybrid method
    hh = infh - (infh-h)*exp(-fabs(dt)/tauh);
    jj = infj - (infj-j)*exp(-fabs(dt)/tauj);

    INa = GNa*m*m*m*h*j*(v-ENa);

    // update gating variables
    m = mm;
    h = hh;
    j = jj;

}
//------------------------------------------------------------------------------

void Luo_Rudy_91_cell::calculate_Isi()
{
    Esi = 7.7 - 13.0287*log(Ca_i);

    ad = 0.095*exp(-0.01*(v-5.0))/(1.0+exp(-0.072*(v-5.0)));
    bd = 0.07*exp(-0.017*(v+44.0))/(1.0+exp(0.05*(v+44.0)));

    af = 0.012*exp(-0.008*(v+28.0))/(1.0+exp(0.15*(v+28.0)));
    bf = 0.0065*exp(-0.02*(v+30.0))/(1.0+exp(-0.2*(v+30.0)));

    infd = ad/(ad+bd);
    inff = af/(af+bf);
    taud = 1.0/(ad+bd);
    tauf = 1.0/(af+bf);
    taud = 0.5*taud;        // acc with Xi,Qu Lr1+serce psa
    tauf = 0.5*tauf;
    
    dd = infd - (infd-d)*exp(-fabs(dt)/taud);
    ff = inff - (inff-f)*exp(-fabs(dt)/tauf);

    dCai = -pow(10.0,-4.0)*Isi + 0.07*(pow(10.0,-4.0)-Ca_i);
    Ca_i = Ca_i + dCai*dt;

    Isi = Gsi*d*f*(v-Esi);

    d = dd;
    f = ff;
}
//------------------------------------------------------------------------------

void Luo_Rudy_91_cell::calculate_IK()
{
    if (v > -100.0)
    {
        if ( (v+77.0)<pow(10,-6) )      // singularity // ????????????
            Xi = 0.7;
        else
            Xi = 2.837*(exp(0.04*(v+77.0))-1.0)/((v+77.0)*exp(0.04*(v+35.0)));
    }
    else
      Xi = 1.0;


    ax = 0.0005*exp(0.083*(v+50.0))/(1.0+exp(0.057*(v+50.0)));
    bx = 0.0013*exp(-0.06*(v+20.0))/(1.0+exp(-0.04*(v+20.0)));

    infx = ax/(ax+bx);
    taux = 1.0/(ax+bx);

    XX = infx - (infx-X)*exp(-fabs(dt)/taux);

    IK = Gk*X*Xi*(v-Ek);

    X = XX;

}
//------------------------------------------------------------------------------

void Luo_Rudy_91_cell::calculate_IK1()
{
    ak1 = 1.02/(1.0+exp(0.2385*(v-Ek1-59.215)));
    bk1 = (0.49124*exp(0.08032*(v-Ek1+5.476)) +
            exp(0.06175*(v-Ek1-594.31)))/
          (1.0+exp(-0.5143*(v-Ek1+4.753)));

    infk = ak1/(ak1+bk1);
    tauk = 1.0/(ak1+bk1);

    K1inf = ak1/(ak1+bk1);

    IK1 = Gk1*K1inf*(v-Ek1);

}
//***********************************************************************
void Luo_Rudy_91_cell::calculate_IKp()
{
    Kp = 1.0/(1.0+exp((7.488-v)/5.98));
    IKp = Gkp*Kp*(v-Ekp);
}
//------------------------------------------------------------------------------
void Luo_Rudy_91_cell::compute_one_step()
{
    calculate_Ib();
    calculate_INa();
    calculate_Isi();
    calculate_IK();
    calculate_IK1();
    calculate_IKp();

    dv = -( Ib + INa + Isi + IK + IK1 + IKp );
     vnew = v + (Diffusion_Term + dv) * dt;

//    vnew = vprev_2 + 2.0*(Diffusion_Term + dv) * dt;
//    vprev_2 = vprev_1;
//    vprev_1 = vnew;


    if( fabs(vnew) > 1000.0 ) // to prevent overflow
        vnew = 0.0;

    v = vnew;
    t = t + dt;

}

//------------------------------------------------------------------------------
int Luo_Rudy_91_cell::save_object(ofstream* dfile)
{
    dfile[0] << "version_1" << endl;
    dfile[0] << t<< " ";
    dfile[0] << dt<< " ";
    dfile[0] << v << " " << vnew<< " ";
    dfile[0] << dv << " ";
    dfile[0] << V_Tip_Timer_ms_ago << " ";

    // Tip
    dfile[0] << Timer<< " ";
    dfile[0] << Tip_Function<< " ";
    dfile[0] << Tip_Threshold<< " ";
    dfile[0] << Tip_Was_Here<< " ";

    dfile[0] << Ib<< " ";

    // Standard ionic concentrations for ventricular cells
    dfile[0] << K_o<< " ";                  // mmol/L
    dfile[0] << K_i<< " ";                  // mmol/L
    dfile[0] << Na_o<< " ";                 // mmol/L
    dfile[0] << Na_i<< " ";                  // mmol/L
    dfile[0] << Ca_o<< " ";                 // mmol/L
    dfile[0] << Ca_i<< " ";        // mmol/L

    // Physic constants
    dfile[0] << F<< " ";                   // Faraday constant, coulombs/mmol
    dfile[0] << R<< " ";                  // gas constant, J/K
    dfile[0] << T<< " ";                 // absolute temperature, K

    // Cell constant
    dfile[0] << PK<< " ";                 // permability of K
    dfile[0] << PNa_K<< " ";                   // permability ratio of Na to K
    dfile[0] << PCa_K<< " ";                  //////////// need to be varified
    dfile[0] << Cm<< " ";                            // membrane capacitance, uF/cm^2<< " ";

    dfile[0] << INa<< " ";
    dfile[0] << ah<< " " <<bh<< " " <<aj<< " " <<bj<< " " <<am<< " " <<bm<< " ";
    dfile[0] << m<< " " <<h<< " " <<j<< " " <<mm<< " " <<hh<< " " <<jj<< " ";
    dfile[0] << GNa<< " " <<ENa<< " ";
    dfile[0] << infm<< " " << infh<< " " << infj<< " " << taum<< " " << tauh<< " " << tauj<< " ";
    dfile[0] << Isi<< " ";
    dfile[0] << d << " " << f << " " << dd << " " << ff<< " ";
    dfile[0] << Gsi << " " << Esi << " " << dCai<< " ";
    dfile[0] << ad << " " << bd << " " << af << " " << bf<< " ";
    dfile[0] << infd << " " << inff << " " << taud << " " << tauf<< " ";
    dfile[0] << IK<< " ";
    dfile[0] << Gk << " " << Ek<< " ";
    dfile[0] << X << " " << XX << " " << Xi<< " ";
    dfile[0] << ax << " " <<  bx<< " ";
    dfile[0] << infx << " " << taux<< " ";
    dfile[0] << IK1<< " ";
    dfile[0] << Gk1 << " " << Ek1<< " ";
    dfile[0] << K1inf << " " <<  infk << " " <<  tauk<< " ";
    dfile[0] << ak1 << " " <<  bk1<< " ";
    dfile[0] << IKp<< " ";
    dfile[0] << Kp << " " << Gkp << " " << Ekp<< " ";

    dfile[0] << Diffusion_Coefficient<< " ";
    dfile[0] << Diffusion_Term<< " ";
    dfile[0] << Istim << " ";
    dfile[0] << Voltage_History.size() << " ";
    for(long k=0;k<Voltage_History.size();k++)
    dfile[0] << Voltage_History[k] << " ";
    dfile[0] << Unipolar_Signal_History.size() << " ";
    for(long k=0;k<Unipolar_Signal_History.size();k++)
    dfile[0] << Unipolar_Signal_History[k] << " ";
}
//---------------------------------------------------------------------------
int Luo_Rudy_91_cell::load_object(ifstream* dfile)
{
    char string[2000];
    dfile[0] >> string;
    if( !strcmp(string,"version_1") )
    {
    dfile[0] >> t;
    dfile[0] >> dt;
    dfile[0] >> v ; dfile[0] >>  vnew;
    dfile[0] >> dv ;
    dfile[0] >> V_Tip_Timer_ms_ago ;

    // Tip
    dfile[0] >> Timer;
    dfile[0] >> Tip_Function;
    dfile[0] >> Tip_Threshold;
    dfile[0] >> Tip_Was_Here;

    dfile[0] >> Ib;

    // Standard ionic concentrations for ventricular cells
    dfile[0] >> K_o;                  // mmol/L
    dfile[0] >> K_i;                  // mmol/L
    dfile[0] >> Na_o;                 // mmol/L
    dfile[0] >> Na_i;                  // mmol/L
    dfile[0] >> Ca_o;                 // mmol/L
    dfile[0] >> Ca_i;        // mmol/L

    // Physic constants
    dfile[0] >> F;                   // Faraday constant, coulombs/mmol
    dfile[0] >> R;                  // gas constant, J/K
    dfile[0] >> T;                 // absolute temperature, K

    // Cell constant
    dfile[0] >> PK;                 // permability of K
    dfile[0] >> PNa_K;                   // permability ratio of Na to K
    dfile[0] >> PCa_K;                  //////////// need to be varified
    dfile[0] >> Cm;                            // membrane capacitance, uF/cm^2;

    dfile[0] >> INa;
    dfile[0] >> ah; dfile[0] >> bh; dfile[0] >> aj; dfile[0] >> bj; dfile[0] >> am; dfile[0] >> bm;
    dfile[0] >> m; dfile[0] >> h; dfile[0] >> j; dfile[0] >> mm; dfile[0] >> hh; dfile[0] >> jj;
    dfile[0] >> GNa; dfile[0] >> ENa;
    dfile[0] >> infm; dfile[0] >>  infh; dfile[0] >>  infj; dfile[0] >>  taum; dfile[0] >>  tauh; dfile[0] >>  tauj;
    dfile[0] >> Isi;
    dfile[0] >> d ; dfile[0] >>  f ; dfile[0] >>  dd ; dfile[0] >>  ff;
    dfile[0] >> Gsi ; dfile[0] >>  Esi ; dfile[0] >>  dCai;
    dfile[0] >> ad ; dfile[0] >>  bd ; dfile[0] >>  af ; dfile[0] >>  bf;
    dfile[0] >> infd ; dfile[0] >>  inff ; dfile[0] >>  taud ; dfile[0] >>  tauf;
    dfile[0] >> IK;
    dfile[0] >> Gk ; dfile[0] >>  Ek;
    dfile[0] >> X ; dfile[0] >>  XX ; dfile[0] >>  Xi;
    dfile[0] >> ax ; dfile[0] >>   bx;
    dfile[0] >> infx ; dfile[0] >>  taux;
    dfile[0] >> IK1;
    dfile[0] >> Gk1 ; dfile[0] >>  Ek1;
    dfile[0] >> K1inf ; dfile[0] >>   infk ; dfile[0] >>   tauk;
    dfile[0] >> ak1 ; dfile[0] >>   bk1;
    dfile[0] >> IKp;
    dfile[0] >> Kp ; dfile[0] >>  Gkp ; dfile[0] >>  Ekp;

    dfile[0] >> Diffusion_Coefficient;
    dfile[0] >> Diffusion_Term;
    dfile[0] >> Istim ;
    long Size;
    double tmp;
    dfile[0] >> Size;
    Voltage_History.clear();
    for(long k=0;k<Size;k++)
    {
        dfile[0] >> tmp;
        Voltage_History.push_back(tmp);
    }
    dfile[0] >> Size;
    Unipolar_Signal_History.clear();
    for(long k=0;k<Size;k++)
    {
        dfile[0] >> tmp;
        Unipolar_Signal_History.push_back(tmp);
    }
    } // version_1
}
//---------------------------------------------------------------------------
