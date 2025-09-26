// The Luo-Rudy Dynamic (LRd) Model of the Mammalian Ventricular Myocyte
// https://models.cellml.org/exposure/2d2ce7737b42a4f72d6bf8b67f6eb5a2
#ifndef LRCELL91_original_H
#define LRCELL91_original_H

#include <vector>
#include <iostream>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#define bcl 300   // Basic Cycle Length (ms)
#define beats 10  // Number of Beats

using namespace std;

class Luo_Rudy_91_cell_original
{
	public:

	Luo_Rudy_91_cell_original();

	int save_object(ofstream* dfile);
	int load_object(ifstream* dfile);

//	double ALGEBRAIC[25],RATES[8],STATES[8],CONSTANTS[24];
	double t;
	double dt;
	double v, vnew, vprev_1, vprev_2;
	double V_Tip_Timer_ms_ago;

    double get_current();

/*
   There are a total of 25 entries in the algebraic variable array.
   There are a total of 8 entries in each of the rate and state variable arrays.
   There are a total of 24 entries in the constant variable array.
 */

double R,T,F,C,stim_start,stim_end,stim_period,stim_duration,stim_amplitude,g_Na;
double Nao,Nai,m,h,j,E_Na,alpha_m,i_si,i_Na,i_K,i_K1,i_Kp,i_b,I_stim;

double VOI; // this is time in cellml formulation,used only for stimulation purpose

double beta_m; //beta_m in component fast_sodium_current_m_gate (per_millisecond).
double alpha_h; //alpha_h in component fast_sodium_current_h_gate (per_millisecond).
double beta_h; //beta_h in component fast_sodium_current_h_gate (per_millisecond).
double alpha_j; //alpha_j in component fast_sodium_current_j_gate (per_millisecond).
double beta_j; //beta_j in component fast_sodium_current_j_gate (per_millisecond).
double E_si; //E_si in component slow_inward_current (millivolt).
double Cai; //Cai in component intracellular_calcium_concentration (millimolar).
double d; //d in component slow_inward_current_d_gate (dimensionless).
double f; //f in component slow_inward_current_f_gate (dimensionless).
double alpha_d; //alpha_d in component slow_inward_current_d_gate (per_millisecond).
double beta_d; //beta_d in component slow_inward_current_d_gate (per_millisecond).
double alpha_f; //alpha_f in component slow_inward_current_f_gate (per_millisecond).
double beta_f; //beta_f in component slow_inward_current_f_gate (per_millisecond).
double g_K; //g_K in component time_dependent_potassium_current (milliS_per_cm2).
double E_K; //E_K in component time_dependent_potassium_current (millivolt).
double PR_NaK; //PR_NaK in component time_dependent_potassium_current (dimensionless).
double Ko; //Ko in component ionic_concentrations (millimolar).
double Ki; //Ki in component ionic_concentrations (millimolar).
double X; //X in component time_dependent_potassium_current_X_gate (dimensionless).
double Xi; //Xi in component time_dependent_potassium_current_Xi_gate (dimensionless).
double alpha_X; //alpha_X in component time_dependent_potassium_current_X_gate (per_millisecond).
double beta_X; //beta_X in component time_dependent_potassium_current_X_gate (per_millisecond).
double E_K1; //E_K1 in component time_independent_potassium_current (millivolt).
double g_K1; //g_K1 in component time_independent_potassium_current (milliS_per_cm2).
double K1_infinity; //K1_infinity in component time_independent_potassium_current_K1_gate (dimensionless).
double alpha_K1; //alpha_K1 in component time_independent_potassium_current_K1_gate (per_millisecond).
double beta_K1; //beta_K1 in component time_independent_potassium_current_K1_gate (per_millisecond).
double E_Kp; //E_Kp in component plateau_potassium_current (millivolt).
double g_Kp; //g_Kp in component plateau_potassium_current (milliS_per_cm2).
double Kp; //Kp in component plateau_potassium_current (dimensionless).
double E_b; //E_b in component background_current (millivolt).
double g_b; //g_b in component background_current (milliS_per_cm2).
double dv; //d/dt V in component membrane (millivolt).
double dm; //d/dt m in component fast_sodium_current_m_gate (dimensionless).
double dh; //d/dt h in component fast_sodium_current_h_gate (dimensionless).
double dj; //d/dt j in component fast_sodium_current_j_gate (dimensionless).
double dd; //d/dt d in component slow_inward_current_d_gate (dimensionless).
double df; //d/dt f in component slow_inward_current_f_gate (dimensionless).
double dX; //d/dt X in component time_dependent_potassium_current_X_gate (dimensionless).
double dCai; //d/dt Cai in component intracellular_calcium_concentration (millimolar).


	void initialize_variables();

	void computeVariables();
	void computeRates();

	void compute_one_step();

	// Tip
	double Timer;
	double Tip_Function;
	double Tip_Threshold;
	int Tip_Was_Here;

	// Diffusion term from surrounding cells
	double Diffusion_Coefficient;
	double Diffusion_Term;
	double Istim;

	// not used since history is handled now by grid approach on "tissue level"
	std::vector <double> Voltage_History;
	std::vector <double> Unipolar_Signal_History;

};

#endif

