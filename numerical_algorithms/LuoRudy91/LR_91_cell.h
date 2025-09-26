
/* The Luo-Rudy Dynamic (LRd) Model of the Mammalian Ventricular Myocyte */
/* Based on code by Gregory Faber */
/* This code requires a C++ compiler */
/* Detailed list of equations and model description are provided in */

#ifndef LRCELL91_H
#define LRCELL91_H

#include <vector>
#include <iostream>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#define bcl 300   // Basic Cycle Length (ms)
#define beats 10  // Number of Beats

using namespace std;

class Luo_Rudy_91_cell
{
	public:

	Luo_Rudy_91_cell();

	int save_object(ofstream* dfile);
	int load_object(ifstream* dfile);

	void initialize_variables();

	double t;
	double dt;
    double v, vnew, vprev_1, vprev_2;
    double dv;
    double V_Tip_Timer_ms_ago;
	double get_current();

    // Tip
    double Timer;
    double Tip_Function;
    double Tip_Threshold;
    int Tip_Was_Here;

    void calculate_Ib();
    double Ib;

    // Standard ionic concentrations for ventricular cells
    double K_o;                  // mmol/L
    double K_i;                  // mmol/L
    double Na_o;                 // mmol/L
	double Na_i;                  // mmol/L
    double Ca_o;                 // mmol/L
    double Ca_i;        // mmol/L

    // Physic constants
	double F;                   // Faraday constant, coulombs/mmol
    double R;                  // gas constant, J/K
    double T;                 // absolute temperature, K

    // Cell constant
	double PK;                 // permability of K
    double PNa_K;                   // permability ratio of Na to K
    double PCa_K;                  //////////// need to be varified
    double Cm;                            // membrane capacitance, uF/cm^2;

    void calculate_INa();
    double INa;
    double ah,bh,aj,bj,am,bm;
    double m,h,j,mm,hh,jj;
	double GNa,ENa;
    double infm, infh, infj, taum, tauh, tauj;
    void calculate_Isi();
    double Isi;
    double d,f,dd,ff;
    double Gsi,Esi,dCai;
    double ad,bd,af,bf;
    double infd,inff,taud,tauf;    
    void calculate_IK();
    double IK;
    double Gk,Ek;
    double X,XX,Xi;
    double ax, bx;
    double infx,taux;
    void calculate_IK1();
    double IK1;
    double Gk1,Ek1;
    double K1inf, infk, tauk;
    double ak1, bk1;
    void calculate_IKp();
    double IKp;
    double Kp,Gkp,Ekp;

    // Diffusion term from surrounding cells
    double Diffusion_Coefficient;
    double Diffusion_Term;
    void compute_one_step();
	double Istim;

	std::vector <double> Voltage_History;
	std::vector <double> Unipolar_Signal_History;

};

#endif

