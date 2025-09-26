// The Luo-Rudy Dynamic (LRd) Model of the Mammalian Ventricular Myocyte
// https://models.cellml.org/exposure/2d2ce7737b42a4f72d6bf8b67f6eb5a2

#include "LR_91_cell_Original.h"

//************************************************************************
Luo_Rudy_91_cell_original::Luo_Rudy_91_cell_original()
{

    Istim = 0.0;   // not used. V is set to 10 to stimulate cell

    Tip_Function = 0.0;
    Tip_Threshold = -10.0;
    Tip_Was_Here = 0;

	initialize_variables();

}

//************************************************************************

void Luo_Rudy_91_cell_original::compute_one_step()
{
	computeVariables();
	computeRates();

	vnew = v + (Diffusion_Term + dv) * dt;

	v = vnew;

	t = t + dt;
}

//------------------------------------------------------------------------------

void Luo_Rudy_91_cell_original::initialize_variables()
{
	Diffusion_Term = 0.0;
	Diffusion_Coefficient = 0.0001;

	t = 0.0;
	dt = 0.2;

	v = 0;
	vnew = 0;
	vprev_1 = 0;
	vprev_2 = 0;

v = -84.3801107371;
R = 8314;
T = 310;
F = 96484.6;
C = 1;
stim_start = 100;
stim_end = 9000;
stim_period = 1000;
stim_duration = 2;
stim_amplitude = -25.5;
g_Na = 23;
Nao = 140;
Nai = 18;
m = 0.00171338077730188;
h = 0.982660523699656;
j = 0.989108212766685;
Cai = 0.00017948816388306;
d = 0.00302126301779861;
f = 0.999967936476325;
PR_NaK = 0.01833;
Ko = 5.4;
Ki = 145;
X = 0.0417603108167287;
g_Kp = 0.0183;
E_b = -59.87;
g_b = 0.03921;
E_Na =  (( R*T)/F)*log(Nao/Nai);
g_K =  0.282000* pow((Ko/5.40000), 1.0 / 2);
E_K =  (( R*T)/F)*log((Ko+ PR_NaK*Nao)/(Ki+ PR_NaK*Nai));
E_K1 =  (( R*T)/F)*log(Ko/Ki);
g_K1 =  0.604700* pow((Ko/5.40000), 1.0 / 2);
E_Kp = E_K1;

    I_stim = 0;
}

//------------------------------------------------------------------------------

void Luo_Rudy_91_cell_original::computeRates()
{

alpha_m = ( 0.320000*(v+47.1300))/(1.00000 - exp( - 0.100000*(v+47.1300)));
beta_m =  0.0800000*exp(- v/11.0000);
dm =  alpha_m*(1.00000 - m) -  beta_m*m;

alpha_h = (v<- 40.0000 ?  0.135000*exp((80.0000+v)/- 6.80000) : 0.00000);
beta_h = (v<- 40.0000 ?  3.56000*exp( 0.0790000*v)+ 310000.*exp( 0.350000*v) : 1.00000/( 0.130000*(1.00000+exp((v+10.6600)/- 11.1000))));
dh =  alpha_h*(1.00000 - h) -  beta_h*h;

alpha_j = (v<- 40.0000 ? ( ( - 127140.*exp( 0.244400*v) -  3.47400e-05*exp( - 0.0439100*v))*(v+37.7800))/(1.00000+exp( 0.311000*(v+79.2300))) : 0.00000);
beta_j = (v<- 40.0000 ? ( 0.121200*exp( - 0.0105200*v))/(1.00000+exp( - 0.137800*(v+40.1400))) : ( 0.300000*exp( - 2.53500e-07*v))/(1.00000+exp( - 0.100000*(v+32.0000))));
dj =  alpha_j*(1.00000 - j) -  beta_j*j;

alpha_d = ( 0.0950000*exp( - 0.0100000*(v - 5.00000)))/(1.00000+exp( - 0.0720000*(v - 5.00000)));
beta_d = ( 0.0700000*exp( - 0.0170000*(v+44.0000)))/(1.00000+exp( 0.0500000*(v+44.0000)));
dd =  alpha_d*(1.00000 - d) -  beta_d*d;

alpha_f = ( 0.0120000*exp( - 0.00800000*(v+28.0000)))/(1.00000+exp( 0.150000*(v+28.0000)));
beta_f = ( 0.00650000*exp( - 0.0200000*(v+30.0000)))/(1.00000+exp( - 0.200000*(v+30.0000)));
df =  alpha_f*(1.00000 - f) -  beta_f*f;

alpha_X = ( 0.000500000*exp( 0.0830000*(v+50.0000)))/(1.00000+exp( 0.0570000*(v+50.0000)));
beta_X = ( 0.00130000*exp( - 0.0600000*(v+20.0000)))/(1.00000+exp( - 0.0400000*(v+20.0000)));
dX =  alpha_X*(1.00000 - X) -  beta_X*X;

E_si = 7.70000 -  13.0287*log(Cai/1.00000);
i_si =  0.0900000*d*f*(v - E_si);

dCai =  (- 0.000100000/1.00000)*i_si+ 0.0700000*(0.000100000 - Cai);
//I_stim = (VOI>=stim_start&&VOI<=stim_end&&(VOI - stim_start) -  floor((VOI - stim_start)/stim_period)*stim_period<=stim_duration ? stim_amplitude : 0.00000);
i_Na =  g_Na*pow(m, 3.00000)*h*j*(v - E_Na);

Xi = (v>- 100.000 ? ( 2.83700*(exp( 0.0400000*(v+77.0000)) - 1.00000))/( (v+77.0000)*exp( 0.0400000*(v+35.0000))) : 1.00000);

i_K =  g_K*X*Xi*(v - E_K);

alpha_K1 = 1.02000/(1.00000+exp( 0.238500*((v - E_K1) - 59.2150)));
beta_K1 = ( 0.491240*exp( 0.0803200*((v+5.47600) - E_K1))+ 1.00000*exp( 0.0617500*(v - (E_K1+594.310))))/(1.00000+exp( - 0.514300*((v - E_K1)+4.75300)));
K1_infinity = alpha_K1/(alpha_K1+beta_K1);
i_K1 =  g_K1*K1_infinity*(v - E_K1);

Kp = 1.00000/(1.00000+exp((7.48800 - v)/5.98000));
i_Kp =  g_Kp*Kp*(v - E_Kp);
i_b =  g_b*(v - E_b);

//*****************
dv =  (- 1.00000/C)*(I_stim + i_Na + i_si + i_K + i_K1 + i_Kp + i_b);
//*****************

}

//------------------------------------------------------------------------------

double Luo_Rudy_91_cell_original::get_current()
{
	return I_stim + i_Na + i_si + i_K + i_K1 + i_Kp + i_b;
}

//------------------------------------------------------------------------------

void Luo_Rudy_91_cell_original::computeVariables()
{
alpha_m = ( 0.320000*(v+47.1300))/(1.00000 - exp( - 0.100000*(v+47.1300)));
beta_m =  0.0800000*exp(- v/11.0000);

alpha_h = (v<- 40.0000 ?  0.135000*exp((80.0000+v)/- 6.80000) : 0.00000);
beta_h = (v<- 40.0000 ?  3.56000*exp( 0.0790000*v)+ 310000.*exp( 0.350000*v) : 1.00000/( 0.130000*(1.00000+exp((v+10.6600)/- 11.1000))));

alpha_j = (v<- 40.0000 ? ( ( - 127140.*exp( 0.244400*v) -  3.47400e-05*exp( - 0.0439100*v))*(v+37.7800))/(1.00000+exp( 0.311000*(v+79.2300))) : 0.00000);
beta_j = (v<- 40.0000 ? ( 0.121200*exp( - 0.0105200*v))/(1.00000+exp( - 0.137800*(v+40.1400))) : ( 0.300000*exp( - 2.53500e-07*v))/(1.00000+exp( - 0.100000*(v+32.0000))));

alpha_d = ( 0.0950000*exp( - 0.0100000*(v - 5.00000)))/(1.00000+exp( - 0.0720000*(v - 5.00000)));
beta_d = ( 0.0700000*exp( - 0.0170000*(v+44.0000)))/(1.00000+exp( 0.0500000*(v+44.0000)));

alpha_f = ( 0.0120000*exp( - 0.00800000*(v+28.0000)))/(1.00000+exp( 0.150000*(v+28.0000)));
beta_f = ( 0.00650000*exp( - 0.0200000*(v+30.0000)))/(1.00000+exp( - 0.200000*(v+30.0000)));

alpha_X = ( 0.000500000*exp( 0.0830000*(v+50.0000)))/(1.00000+exp( 0.0570000*(v+50.0000)));
beta_X = ( 0.00130000*exp( - 0.0600000*(v+20.0000)))/(1.00000+exp( - 0.0400000*(v+20.0000)));

E_si = 7.70000 -  13.0287*log(Cai/1.00000);
i_si =  0.0900000*d*f*(v - E_si);

//I_stim = (VOI>=stim_start&&VOI<=stim_end&&(VOI - stim_start) -  floor((VOI - stim_start)/stim_period)*stim_period<=stim_duration ? stim_amplitude : 0.00000);

i_Na =  g_Na*pow(m, 3.00000)*h*j*(v - E_Na);

Xi = (v>- 100.000 ? ( 2.83700*(exp( 0.0400000*(v+77.0000)) - 1.00000))/( (v+77.0000)*exp( 0.0400000*(v+35.0000))) : 1.00000);

i_K =  g_K*X*Xi*(v - E_K);
alpha_K1 = 1.02000/(1.00000+exp( 0.238500*((v - E_K1) - 59.2150)));
beta_K1 = ( 0.491240*exp( 0.0803200*((v+5.47600) - E_K1))+ 1.00000*exp( 0.0617500*(v - (E_K1+594.310))))/(1.00000+exp( - 0.514300*((v - E_K1)+4.75300)));

K1_infinity = alpha_K1/(alpha_K1+beta_K1);
i_K1 =  g_K1*K1_infinity*(v - E_K1);

Kp = 1.00000/(1.00000+exp((7.48800 - v)/5.98000));
i_Kp =  g_Kp*Kp*(v - E_Kp);
i_b =  g_b*(v - E_b);

}

//------------------------------------------------------------------------------

int Luo_Rudy_91_cell_original::save_object(ofstream* dfile)
{
/*
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
*/
}
//---------------------------------------------------------------------------

int Luo_Rudy_91_cell_original::load_object(ifstream* dfile)
{
/*
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
*/
}
//---------------------------------------------------------------------------

