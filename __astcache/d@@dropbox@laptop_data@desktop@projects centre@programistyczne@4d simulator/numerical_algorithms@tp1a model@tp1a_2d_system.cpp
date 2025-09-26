#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <vcl.h>
#include "TP1A_2D_system.h"

#include "Two_D_system_CR_region_class.h"


#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------

TP1A_2D_system_class::~TP1A_2D_system_class()
{

}
//---------------------------------------------------------------------------

TP1A_2D_system_class::TP1A_2D_system_class()
{
	Min_Voltage = 0;
	Max_Voltage = 1;

	Min_Diff = 0;
	Max_Diff_X = 0.2;
	Max_Diff_Y = 0.2;

// breakup
	a = 0.1;
	k_par = 8;
	eps = 0.01;
	mi1 = 0.05;
	mi2 = 0.3;
	b = 0.1;

  // stable spiral parameters
//	mi1 = 0.2;

/* self-termination, zero-flux, 80x80
	a = 0.1;
	k_par = 8;
	eps = 0.01;
	mi1 = 0.1;
	mi2 = 0.3;
	b = 0.1;
*/

// standard parameters
/*	a = 0.1;
	k_par = 8;
	eps = 0.01;
	mi1 = 0.2;
	mi2 = 0.3;
	b = 0.1;
*/
	dx = 0.6;
	dt = 0.02;  // normal
//	dt = 0.002;    // slow for animations

	Size_X = TWO_DIM_SYSTEM_X_SIZE;
	Size_Y = TWO_DIM_SYSTEM_Y_SIZE;

	allocate_tables();

	Global_Time=0;

	// Load phase vector
	ifstream dfile;
	char s[3000];
	dfile.open( "TP1A_phase.txt" );
	if(dfile==NULL) ShowMessage("Unable to open TP1A_phase.txt file");
	else
	{
		dfile >> TP1A_APD_Length;
		TP1A_Activation_Variable_APD = new double[TP1A_APD_Length];
		TP1A_Recovery_Variable_APD = new double[TP1A_APD_Length];

		for(long i=0;i<TP1A_APD_Length;i++)
		{
			dfile >> s; TP1A_Activation_Variable_APD[i] = atof(s);
			dfile >> s; TP1A_Recovery_Variable_APD[i] = atof(s);
		}
	}

	Min_Custom_Value = 0;
	Max_Custom_Value = 1;

	Line_point_A_i = -1;
	Line_point_A_j = -1;
	Line_point_B_i = -1;
	Line_point_B_j = -1;
}

//---------------------------------------------------------------------------

int TP1A_2D_system_class::allocate_tables()
{
	vector_of_doubles D;
	D.DVector.clear();

	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	{
		DX[i][j] = Max_Diff_X;
		DY[i][j] = Max_Diff_Y;
		MI1[i][j] = mi1;
		MI2[i][j] = mi2;
		VOLTAGE_HISTORY[i][j].DVector.clear();
		CUSTOM_VALUE[i][j]=0.5;
	}

	clear_system();

	return 1;
}

//---------------------------------------------------------------------------

void TP1A_2D_system_class::set_mi_distribution(double mi1_left,double mi2_left,
							 double mi1_right,double mi2_right,
							 double Ratio)
{
	if( Ratio >= 0 & Ratio <= 1 )
	{

	for( int i=0; i<Ratio*Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	{
		MI1[i][j] = mi1_left;
		MI2[i][j] = mi2_left;
	}

	for( int i=Ratio*Size_X; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	{
		MI1[i][j] = mi1_right;
		MI2[i][j] = mi2_right;
	}
	}
}

//---------------------------------------------------------------------------
void TP1A_2D_system_class::set_mi_breakup_rings(double mi1_min,double mi1_max,
							 double Radius, long Number,bool Middle)
{
	// set all to normal value
	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	{
		MI1[i][j] = mi1_max;
	}

	//------------------------------
	// discs
	//------------------------------
	for(int i=0;i<Number;i++)
	{

	int x = random( Size_X );
	int y = random( Size_Y );

	if( Middle )
	{
		x = 0.5*Size_X;
		y = 0.5*Size_Y;
	}

	for( int i1=x-Radius; i1<x+Radius; i1++)
	for( int j1=y-Radius; j1<y+Radius; j1++)
	if( sqrt(pow(i1-x,2)+pow(j1-y,2)) < Radius )
	if( i1 >= 0 && i1 < Size_X )
	if( j1 >= 0 && j1 < Size_Y )
	{
		MI1[i1][j1] = mi1_min;
	}

	}

}

//---------------------------------------------------------------------------
void TP1A_2D_system_class::set_mi_breakup_landscape(double R, double p, double I,
		double min, double max, int Target_Variable)
{
	double r;

	if( Target_Variable == 0 )
		set_initial_coupling();
    else
	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	{
		if( Target_Variable == 1 ) 	MI1[i][j] = max;
		if( Target_Variable == 2 ) 	MI2[i][j] = max;
	}

	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	{

	if( random( 100000 ) / 100000.0 < p )
	for( int i1=i;i1<= i+2*R;i1++)
	for( int j1=j;j1<= j+2*R;j1++)
	if( i1 < Size_X && j1 < Size_Y )
	{
		r = sqrt( std::pow(i1-R-i,2) + std::pow(j1-R-j,2) );
		if( r <= R )
		{
			if( Target_Variable == 0 ) 	{ DX[i1][j1] -= I*(R-r)/R; DY[i1][j1] -= I*(R-r)/R; }
			if( Target_Variable == 1 ) 	MI1[i1][j1] -= I*(R-r)/R;
			if( Target_Variable == 2 ) 	MI2[i1][j1] -= I*(R-r)/R;
		}

			if( Target_Variable == 0 ) 	{ if( DX[i1][j1] < min ) DX[i1][j1] = min; if( DY[i1][j1] < min ) DY[i1][j1] = min; }
			if( Target_Variable == 1 ) 	if( MI1[i1][j1] < min ) MI1[i1][j1] = min;
			if( Target_Variable == 2 ) 	if( MI2[i1][j1] < min ) MI2[i1][j1] = min;
	}

	}
}

//---------------------------------------------------------------------------
void TP1A_2D_system_class::clear_system()
{
	// Clear grid
	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	{
		V1[i][j] = 0;
		V2[i][j] = 0;
		W1[i][j] = 0;
		W2[i][j] = 0;
		CUSTOM_VALUE[i][j] = 0;
		VOLTAGE_HISTORY[i][j].DVector.clear();
	}

	VOLTAGE_HISTORY_Time_Axis_ms.clear();
}

//---------------------------------------------------------------------------

void TP1A_2D_system_class::clear_signals_history()
{
	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
		VOLTAGE_HISTORY[i][j].DVector.clear();

	VOLTAGE_HISTORY_Time_Axis_ms.clear();
}

//---------------------------------------------------------------------------

void TP1A_2D_system_class::refresh_VW_matrices() // to avoid drop in efficiency
{
    // Clear grid
	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	if( DX[i][j]==0 && DY[i][j]==0 )
	{
		V1[i][j] = 0;
		V2[i][j] = 0;
		W1[i][j] = 0;
		W2[i][j] = 0;
	}
}

//---------------------------------------------------------------------------

void TP1A_2D_system_class::record_potentials(int Type,int Grid_Spacing)
{
	double v;

	for( int i=1; i<Size_X; i+=Grid_Spacing)
	for( int j=1; j<Size_Y; j+=Grid_Spacing)
	{
		if( Type == 0 )
		v = V1[i][j];

		if( Type == 1 )
		v = get_unipolar_voltage(i,j);

		if( Type == 2 )
		v = 100*(get_unipolar_voltage(i,j) - get_unipolar_voltage(i,j-Grid_Spacing));

		VOLTAGE_HISTORY[i][j].DVector.push_back(v);
	}

	VOLTAGE_HISTORY_Time_Axis_ms.push_back(Global_Time);
}

//---------------------------------------------------------------------------

double TP1A_2D_system_class::get_unipolar_voltage(int x_coord, int y_coord)
{
	double r,x,y,z,v=0;
	int Grid_Sampling=2;
	int Range = 20;

	for(int i=x_coord-Range; i<(signed)x_coord+Range; i+=Grid_Sampling)
	for(int j=y_coord-Range; j<(signed)y_coord+Range; j+=Grid_Sampling)
	if( i>0 && i<Size_X-1 )
	if( j>0 && j<Size_Y-1 )
	{

	x =  i - x_coord;
	y =  j - y_coord;

	r = std::pow( std::pow(x,2)+std::pow(y,2),1.5);

	if( r!=0 )
	v =
	 (
	 ( V1[i+1][j]-V1[i-1][j] )*(i - x_coord) +
	 ( V1[i][j+1]-V1[i][j-1] )*(j - y_coord)
	  ) / r;

	} // for all nodes

	return v;
}

//---------------------------------------------------------------------------

void TP1A_2D_system_class::get_voltage(int x_coord, int y_coord,long t_ptr,double *Voltage,double *Time)
{
	Time[0] = 0;
	Voltage[0] = 0;

	if( t_ptr > 0 && t_ptr < VOLTAGE_HISTORY_Time_Axis_ms.size() )
	Time[0] = VOLTAGE_HISTORY_Time_Axis_ms[t_ptr];

	if( x_coord >= 0 && x_coord < TWO_DIM_SYSTEM_X_SIZE &&
		y_coord >= 0 && y_coord < TWO_DIM_SYSTEM_Y_SIZE )
	if( t_ptr > 0 && t_ptr < VOLTAGE_HISTORY[x_coord][y_coord].DVector.size() )
	Voltage[0] = VOLTAGE_HISTORY[x_coord][y_coord].DVector[t_ptr];
}

//---------------------------------------------------------------------------

double TP1A_2D_system_class::get_voltage(int X, int Y)
{
	return V1[X][Y];
}

//------------------------------------------------------------------------------

vector <double> TP1A_2D_system_class::calculate_unipolar_voltage_from_surface(int Node_x,long Node_y, long Range)
{
	vector <double> unipolar_voltage;
	double voltage,v,x,y,r;

	for(long t=0;t<VOLTAGE_HISTORY[0][0].DVector.size()-1;t++)
	{

	voltage=0;
	for(int i=Node_x-Range;i<(signed)(Node_x+Range);i++)
	for(int j=Node_y-Range;j<(signed)(Node_y+Range);j++)
	if( i > 0 && i < Size_X-1 )
	if( j > 0 && j < Size_Y-1 )
/// comment out below line if repolarization wave is to be removed from electrogram
	if( VOLTAGE_HISTORY[i][j].DVector[t+1]-VOLTAGE_HISTORY[i][j].DVector[t] > 0 )
	{

	v = VOLTAGE_HISTORY[i][j].DVector[t];
	x =  i - Node_x;
	y =  j - Node_y;
	r = std::pow( std::pow(x,2)+std::pow(y,2),1.5);

	if( r >= 2 )
	voltage +=
	 (  (VOLTAGE_HISTORY[i+1][j].DVector[t]-
		 VOLTAGE_HISTORY[i-1][j].DVector[t] )*x
		+

		(VOLTAGE_HISTORY[i][j+1].DVector[t]-
		 VOLTAGE_HISTORY[i][j-1].DVector[t] )*y
	 ) / r;

	} // for all nodes

	unipolar_voltage.push_back(voltage);

	} // through time

	return unipolar_voltage;
}

//---------------------------------------------------------------------------

void TP1A_2D_system_class::compute_N_steps(int N)
{
    for(int n=0;n<N;n++)
    {
        calculate_V1_from_V2_fhn();
        calculate_V2_from_V1_fhn();
		Global_Time += dt;

		add_assymetric_modulation();
		add_current_modulation_1();

    }
}

//---------------------------------------------------------------------------

void TP1A_2D_system_class::calculate_V2_from_V1_fhn()
{
//fff21
      int i,j,k,l;
      double e,r,k1,k2,w = 0.5,xp,yp;
      double diff_term,s2=dx*dx;

      // matrix V without boundaries
      for( i = 1; i < Size_X - 1; i++ )
	  for( j = 1; j < Size_Y - 1; j++ )
      {
        diff_term = ( DX[i][j]*(V1[i-1][j] + V1[i+1][j] - 2*V1[i][j]) +
                      DY[i][j]*(V1[i][j+1] + V1[i][j-1] - 2*V1[i][j]) ) / s2;

        // Forward Euler
        V2[i][j] = V1[i][j] + dt*
            ( -k_par*V1[i][j]*(V1[i][j]-a)*(V1[i][j]-1)-V1[i][j]*W1[i][j]+ diff_term);

        W2[i][j] = W1[i][j] + dt*
        (eps+(MI1[i][j]*W1[i][j])/(MI2[i][j]+V1[i][j]))*
             (-W1[i][j]-k_par*V1[i][j]*(V1[i][j]-b-1));
      }

    //---------------------------------------------------------------------
    if( Boundary_Conditions == 0 ) // zero flux
    //---------------------------------------------------------------------
    {
      // "X" side
      for( i = 1; i < Size_X - 1; i++ )
      {
			V2[i][0] = V2[i][1];
            V2[i][Size_Y-1] = V2[i][Size_Y-2];

            W2[i][0] = W2[i][1];
            W2[i][Size_Y-1] = W2[i][Size_Y-2];
      }

      // "Y" side
      for( j = 1; j < Size_Y - 1; j++ )
      {
            V2[0][j] = V2[1][j];
            V2[Size_X-1][j] = V2[Size_Y-2][j];

            W2[0][j] = W2[j][1];
            W2[Size_X-1][j] = W2[Size_Y-2][j];
      }
    }

    //---------------------------------------------------------------------
	if( Boundary_Conditions == 1 ) // periodic conditions
    //---------------------------------------------------------------------
    {
      // "X" 0 side
      i = 0;
      for( j = 1; j < Size_Y - 1; j++ )
      {
        diff_term = ( DX[i][j]*(V1[Size_X-1][j] + V1[1][j] -   2*V1[i][j]) +
                      DY[i][j]*(V1[i][j+1]      + V1[i][j-1] - 2*V1[i][j]) )/s2;

        // Forward Euler
        V2[i][j] = V1[i][j] + dt*
            ( -k_par*V1[i][j]*(V1[i][j]-a)*(V1[i][j]-1)-V1[i][j]*W1[i][j]+ diff_term);

        W2[i][j] = W1[i][j] + dt*
		(eps+(MI1[i][j]*W1[i][j])/(MI2[i][j]+V1[i][j]))*
             (-W1[i][j]-k_par*V1[i][j]*(V1[i][j]-b-1));
      }

      // "X" Size_X-1 side
      i = Size_X-1;
      for( j = 1; j < Size_Y - 1; j++ )
      {
        diff_term = ( DX[i][j]*(V1[Size_X-2][j] + V1[0][j] -  2*V1[i][j]) +
                      DY[i][j]*(V1[i][j+1]      + V1[i][j-1]- 2*V1[i][j]) )/s2;

        // Forward Euler
        V2[i][j] = V1[i][j] + dt*
            ( -k_par*V1[i][j]*(V1[i][j]-a)*(V1[i][j]-1)-V1[i][j]*W1[i][j]+ diff_term);

        W2[i][j] = W1[i][j] + dt*
        (eps+(MI1[i][j]*W1[i][j])/(MI2[i][j]+V1[i][j]))*
             (-W1[i][j]-k_par*V1[i][j]*(V1[i][j]-b-1));
	  }

      // "Y" 0 side
      j = 0;
      for( i = 1; i < Size_X - 1; i++ )
      {
        diff_term = ( DX[i][j]*(V1[i-1][j]       + V1[i+1][j] -  2*V1[i][j]) +
                      DY[i][j]*(V1[i][Size_Y-1]  + V1[i][1]   - 2*V1[i][j]) )/s2;

        // Forward Euler
        V2[i][j] = V1[i][j] + dt*
            ( -k_par*V1[i][j]*(V1[i][j]-a)*(V1[i][j]-1)-V1[i][j]*W1[i][j]+ diff_term);

        W2[i][j] = W1[i][j] + dt*
		(eps+(MI1[i][j]*W1[i][j])/(MI2[i][j]+V1[i][j]))*
			 (-W1[i][j]-k_par*V1[i][j]*(V1[i][j]-b-1));
      }

      // "Y" Size_Y-1 side
      j = Size_Y-1;
      for( i = 1; i < Size_X - 1; i++ )
      {
        diff_term = ( DX[i][j]*(V1[i-1][j]       + V1[i+1][j] -  2*V1[i][j]) +
                      DY[i][j]*(V1[i][Size_Y-2]  + V1[i][0]   - 2*V1[i][j]) )/s2;

        // Forward Euler
        V2[i][j] = V1[i][j] + dt*
            ( -k_par*V1[i][j]*(V1[i][j]-a)*(V1[i][j]-1)-V1[i][j]*W1[i][j]+ diff_term);

        W2[i][j] = W1[i][j] + dt*
        (eps+(MI1[i][j]*W1[i][j])/(MI2[i][j]+V1[i][j]))*
             (-W1[i][j]-k_par*V1[i][j]*(V1[i][j]-b-1));
      }

	}

	// corners
	V2[0][0]=V2[1][1];
	V2[Size_X-1][0]=V2[Size_X-2][1];
	V2[0][Size_Y-1]=V2[1][Size_Y-2];
	V2[Size_X-1][Size_X-1]=V2[Size_X-2][Size_X-2];
}

//------------------------------------------------------------------------------

void TP1A_2D_system_class::calculate_V1_from_V2_fhn()
{
//fff12
      int i,j,k,l;
      double e,r,k1,k2,w = 0.5,xp,yp;
      double diff_term,s2=dx*dx;

	  // matrix V without boundaries
      for( i = 1; i < Size_X - 1; i++ )
      for( j = 1; j < Size_Y - 1; j++ )
      {

        diff_term = ( DX[i][j]*(V2[i-1][j] + V2[i+1][j] - 2*V2[i][j]) +
                      DY[i][j]*(V2[i][j+1] + V2[i][j-1] - 2*V2[i][j]) ) / s2;

		// Forward Euler
		V1[i][j] = V2[i][j] + dt*
			( -k_par*V2[i][j]*(V2[i][j]-a)*(V2[i][j]-1)-V2[i][j]*W2[i][j]+ diff_term);

		W1[i][j] = W2[i][j] + dt*
		(eps+(MI1[i][j]*W2[i][j])/(MI2[i][j]+V2[i][j]))*
			 (-W2[i][j]-k_par*V2[i][j]*(V2[i][j]-b-1));
	  }

    //---------------------------------------------------------------------
    if( Boundary_Conditions == 0 ) // zero flux
    //---------------------------------------------------------------------
    {
      // "X" side
      for( i = 1; i < Size_X - 1; i++ )
      {
            V1[i][0] = V1[i][1];
            V1[i][Size_Y-1] = V1[i][Size_Y-2];

            W1[i][0] = W1[i][1];
            W1[i][Size_Y-1] = W1[i][Size_Y-2];
      }

      // "Y" side
      for( j = 1; j < Size_Y - 1; j++ )
      {
            V1[0][j] = V1[1][j];
            V1[Size_X-1][j] = V1[Size_Y-2][j];

            W1[0][j] = W1[j][1];
            W1[Size_X-1][j] = W1[Size_Y-2][j];
      }
    }

    //---------------------------------------------------------------------
    if( Boundary_Conditions == 1 ) // periodic conditions
    //---------------------------------------------------------------------
    {
      // "X" 0 side
      i = 0;
	  for( j = 1; j < Size_Y - 1; j++ )
      {

        diff_term = ( DX[i][j]*(V2[Size_X-1][j] + V2[1][j] -   2*V2[i][j]) +
					  DY[i][j]*(V2[i][j+1]      + V2[i][j-1] - 2*V2[i][j]) )/s2;

        // Forward Euler
        V1[i][j] = V2[i][j] + dt*
            ( -k_par*V2[i][j]*(V2[i][j]-a)*(V2[i][j]-1)-V2[i][j]*W2[i][j]+ diff_term);

        W1[i][j] = W2[i][j] + dt*
        (eps+(MI1[i][j]*W2[i][j])/(MI2[i][j]+V2[i][j]))*
             (-W2[i][j]-k_par*V2[i][j]*(V2[i][j]-b-1));
      }

      // "X" Size_X-1 side
      i = Size_X-1;
	  for( j = 1; j < Size_Y - 1; j++ )
      {

        diff_term = ( DX[i][j]*(V2[Size_X-2][j] + V2[0][j] -  2*V2[i][j]) +
                      DY[i][j]*(V2[i][j+1]      + V2[i][j-1]- 2*V2[i][j]) )/s2;

        // Forward Euler
        V1[i][j] = V2[i][j] + dt*
            ( -k_par*V2[i][j]*(V2[i][j]-a)*(V2[i][j]-1)-V2[i][j]*W2[i][j]+ diff_term);

        W1[i][j] = W2[i][j] + dt*
        (eps+(MI1[i][j]*W2[i][j])/(MI2[i][j]+V2[i][j]))*
             (-W2[i][j]-k_par*V2[i][j]*(V2[i][j]-b-1));
	  }

      // "Y" 0 side
      j = 0;
      for( i = 1; i < Size_X - 1; i++ )
      {
        diff_term = ( DX[i][j]*(V2[i-1][j]       + V2[i+1][j] -  2*V2[i][j]) +
                      DY[i][j]*(V2[i][Size_Y-1]  + V2[i][1]   - 2*V2[i][j]) )/s2;

        // Forward Euler
        V1[i][j] = V2[i][j] + dt*
            ( -k_par*V2[i][j]*(V2[i][j]-a)*(V2[i][j]-1)-V2[i][j]*W2[i][j]+ diff_term);

        W1[i][j] = W2[i][j] + dt*
        (eps+(MI1[i][j]*W2[i][j])/(MI2[i][j]+V2[i][j]))*
             (-W2[i][j]-k_par*V2[i][j]*(V2[i][j]-b-1));
      }

      // "Y" Size_Y-1 side
      j = Size_Y-1;
      for( i = 1; i < Size_X - 1; i++ )
      {
        diff_term = ( DX[i][j]*(V2[i-1][j]       + V2[i+1][j] -  2*V2[i][j]) +
                      DY[i][j]*(V2[i][Size_Y-2]  + V2[i][0]   - 2*V2[i][j]) )/s2;

        // Forward Euler
		V1[i][j] = V2[i][j] + dt*
            ( -k_par*V2[i][j]*(V2[i][j]-a)*(V2[i][j]-1)-V2[i][j]*W2[i][j]+ diff_term);

        W1[i][j] = W2[i][j] + dt*
		(eps+(MI1[i][j]*W2[i][j])/(MI2[i][j]+V2[i][j]))*
             (-W2[i][j]-k_par*V2[i][j]*(V2[i][j]-b-1));
      }

	}

	// corners
	V1[0][0]=V1[1][1];
	V1[Size_X-1][0]=V1[Size_X-2][1];
	V1[0][Size_Y-1]=V1[1][Size_Y-2];
	V1[Size_X-1][Size_X-1]=V1[Size_X-2][Size_X-2];

}

//------------------------------------------------------------------------------

void TP1A_2D_system_class::create_spiral(int Slice_Plane) // 0-xy,
{
    double wn,r,fi,phase,x,y,d;
    double cx,cy;
    long n;

    // winding number
    wn = 245.0;

    clear_system();

    cx = Size_X/2;
    cy = Size_Y/2;

	if( Slice_Plane == 0 ) // spiral wave in XY plane
    for( int i=0;i<Size_X; i++ )
    for( int j=0;j<Size_Y; j++ )
    {
        // distance from center of the spiral in dx units
        r = std::sqrt( std::pow(i-cx,2) + std::pow(j-cy,2));

        x = i-cx;
        y = j-cy;

        if( i-cx != 0.0 )
        {
            d = atan(fabs(y/x));

            // 1st quarter
            if( x >= 0 && y >= 0 )
                fi = d;

			// 2nd quarter
            if( x <= 0 && y >= 0 )
                fi = M_PI - d;

            // 3rd quarter
            if( x <= 0 && y <= 0 )
                fi = M_PI + d;

            // 4th quarter
            if( x >= 0 && y <= 0 )
                fi = 2.0*M_PI - d;
        }
		else
		{
            if( j-0.5*Size_Y > 0.0 )
                 fi = M_PI_2;
            else
                 fi = 3.0*M_PI_2;
        }

        fi += 0.55;

        // find the greatest n such that r > k*fi + 2pi*k*n
        if(wn*fi!=0)
            n = floor( (r-wn*fi)/(2.0*M_PI*wn) );
        else
            n = 0;

        phase = fabs(r-wn*fi-2.0*wn*M_PI*n)/(2.0*wn*M_PI);
        int m = (int)((1.0-phase)*(TP1A_APD_Length-2));

        V1[i][j] = TP1A_Activation_Variable_APD[m];
        V2[i][j] = TP1A_Activation_Variable_APD[m];
        W1[i][j] = 1*TP1A_Recovery_Variable_APD[m];
        W2[i][j] = 1*TP1A_Recovery_Variable_APD[m];
    }
}

//---------------------------------------------------------------------------

int TP1A_2D_system_class::get_color_code(int Source,int X,int Y,long t,int Grid_Spacing,double Cycle_Length_ms)
{
	int ptr=-1;

	if( Source == 0 )
	ptr = 255*(V1[X][Y]-Min_Voltage)/(Max_Voltage-Min_Voltage);

	if( Source == 0 && DX[X][Y] <= Min_Voltage && DY[X][Y] <= 0.1 )
	ptr = -1;

	if( Source == 1 )
	ptr = 255-255*(DX[X][Y]-Min_Diff)/(2*Max_Diff_X-Min_Diff);

	if( Source == 2 )
	ptr = 255-255*(DY[X][Y]-Min_Diff)/(2*Max_Diff_Y-Min_Diff);

	double max_mi=0.4,min_mi=0;

	if( Source == 3 )
	ptr = 255-255*(MI1[X][Y]-min_mi)/(max_mi-min_mi);
	if( Source == 4 )
	ptr = 255-255*(MI2[X][Y]-min_mi)/(max_mi-min_mi);

	if( Source == 5 )
	if( Max_Custom_Value-Min_Custom_Value != 0 )
	ptr = 255-255*(CUSTOM_VALUE[X][Y]-Min_Custom_Value)/(Max_Custom_Value-Min_Custom_Value);
	else
	ptr = 0;

	if( Source == 5 && DY[X][Y] <= 0.1 )
	ptr = -1;

	if( Source == 6 ) // between layers coupling (here not applicable)
	ptr = 0;

	if( Source == 7 )
	{
		// correct X Y so that it is closest node with recorded potentials
		X = X - X%Grid_Spacing+1;
		Y = Y - Y%Grid_Spacing+1;

		if( t >= 0 && t < VOLTAGE_HISTORY[X][Y].DVector.size() )
		ptr = 255*(VOLTAGE_HISTORY[X][Y].DVector[t] - Min_Voltage)/(Max_Voltage - Min_Voltage);
	}

	std::vector <double> S1;
	double ts = 1;
	if( VOLTAGE_HISTORY_Time_Axis_ms.size() > 3 )
	ts = VOLTAGE_HISTORY_Time_Axis_ms[2]-VOLTAGE_HISTORY_Time_Axis_ms[1];
	int BCL_Ptr = Cycle_Length_ms/ts;
	int Electrogram_Type = 2;

	if( Source == 8 ) // phase map
	{
		if( t >= 0 && t < VOLTAGE_HISTORY[X][Y].DVector.size() )
		{

		// calculate phase if not present xxx
		if( VOLTAGE_HISTORY[X][Y].DVector.size() != PHASE[X][Y].DVector.size() )
		{
			for(long i=0;i<(signed)TWO_DIM_SYSTEM_X_SIZE;i++)
			for(long j=0;j<(signed)TWO_DIM_SYSTEM_Y_SIZE;j++)
			{
				// phase
				//S1 = Numerical_Library_Obj.recompose_signal( &VOLTAGE_HISTORY[i][j].DVector,BCL_Ptr,Electrogram_Type);
				//PHASE[i][j].DVector = Numerical_Library_Obj.get_Hilbert_phase_using_convolution_double(&S1,ts);

				PHASE[i][j].DVector = Numerical_Library_Obj.get_electrogram_phase(
					&VOLTAGE_HISTORY[i][j].DVector,Electrogram_Type,BCL_Ptr,ts);
			}
		}

		ptr = 255*(PHASE[X][Y].DVector[t] + M_PI)/(2*M_PI);

		}
	}

	if( Source == 9 ) // MPC
	ptr = 255*MPC[X][Y];  // mpc goes from 0 to 1, so no normalization required

	if (ptr > 255) ptr = 255;
	return ptr;
}

//---------------------------------------------------------------------------

void TP1A_2D_system_class::stimulate_node(int X,int Y)
{
	if( X >= 0 && X < TWO_DIM_SYSTEM_X_SIZE )
	if( Y >= 0 && Y < TWO_DIM_SYSTEM_Y_SIZE )
	{
		V1[X][Y]=1;
		V2[X][Y]=1;
	}
}

//---------------------------------------------------------------------------

void TP1A_2D_system_class::compute_ISIs_XY(int X, int Y)
{
/*
	ISIs.clear();
	Activations_Positions.clear();
	double Threshold = 0.5;
	double prev=0,curr=0;
	if( X >= 0 && X < Size_X )
	if( Y >= 0 && Y < Size_Y )
	for(long t=1;t<(signed)VOLTAGE_HISTORY[X][Y].DVector.size()-1;t++)
	if( VOLTAGE_HISTORY[X][Y].DVector[t-1] < Threshold )
	if( VOLTAGE_HISTORY[X][Y].DVector[t+1] > Threshold )
	{
		curr = t;
		Activations_Positions.push_back(curr);
		if( prev != 0 )
			ISIs.push_back(curr-prev);
		prev = curr;
		t+=5; // forward jump in time
	}
*/
}

//---------------------------------------------------------------------------

int TP1A_2D_system_class::save_object(ofstream* dfile)
{
	dfile[0] << " version_4 ";

	dfile[0] << Size_X << " ";
	dfile[0] << Size_Y << " ";

	for(long i=0;i<Size_X;i++)
	for(long j=0;j<Size_Y;j++)
	{
		dfile[0] << V1[i][j] << " ";
		dfile[0] << V2[i][j] << " ";
		dfile[0] << W1[i][j] << " ";
		dfile[0] << W2[i][j] << " ";
		dfile[0] << DX[i][j] << " ";
		dfile[0] << DY[i][j] << " ";
		dfile[0] << MI1[i][j] << " ";
		dfile[0] << MI2[i][j] << " ";

		dfile[0] << VOLTAGE_HISTORY[i][j].DVector.size() << " ";
		for(long t=0;t<VOLTAGE_HISTORY[i][j].DVector.size();t++)
		dfile[0] << VOLTAGE_HISTORY[i][j].DVector[t] << " ";

		dfile[0] << PHASE[i][j].DVector.size() << " ";
		for(long t=0;t<PHASE[i][j].DVector.size();t++)
		dfile[0] << PHASE[i][j].DVector[t] << " ";
	}

	dfile[0] << VOLTAGE_HISTORY_Time_Axis_ms.size() << " ";
	for(long i=0;i<VOLTAGE_HISTORY_Time_Axis_ms.size();i++)
	dfile[0] <<  VOLTAGE_HISTORY_Time_Axis_ms[i] << " ";

	dfile[0] << ISIs.size() << " ";
	for(long i=0;i<ISIs.size();i++)
	dfile[0] <<  ISIs[i] << " ";

    dfile[0] << ISIs_Node_Ptr_X << " ";

    dfile[0] << Min_Voltage << " ";
    dfile[0] << Max_Voltage << " ";
	dfile[0] << Boundary_Conditions << " ";

    // parameters
	dfile[0] << k_par << " ";
    dfile[0] << a << " ";
    dfile[0] << eps << " ";
    dfile[0] << mi1 << " ";
    dfile[0] << mi2 << " ";
	dfile[0] << b << " ";

    dfile[0] << dx << " ";
	dfile[0] << dt << " ";

	dfile[0] << Global_Time << " ";

}

//---------------------------------------------------------------------------

int TP1A_2D_system_class::load_object(ifstream* dfile)
{
	char string[2000];
	long S1;
	double tmp=0;

	dfile[0] >> string;

	//--------------------------------------------------------------------
	if( !strcmp(string,"version_4") )
	//--------------------------------------------------------------------
	{
	dfile[0] >> Size_X;
	dfile[0] >> Size_Y;

	if( Size_X != TWO_DIM_SYSTEM_X_SIZE ||
		Size_Y != TWO_DIM_SYSTEM_Y_SIZE )
	{
		ShowMessage("Incorrect system dimensions");
		return -1;
	}


	allocate_tables();
	long S;
	double v;

	for(long i=0;i<Size_X;i++)
	for(long j=0;j<Size_Y;j++)
	{
		dfile[0] >> V1[i][j];
		dfile[0] >> V2[i][j];
		dfile[0] >> W1[i][j];
		dfile[0] >> W2[i][j];
		dfile[0] >> DX[i][j];
		dfile[0] >> DY[i][j];
		dfile[0] >> MI1[i][j];
		dfile[0] >> MI2[i][j];

		dfile[0] >> S;
		for(long t=0;t<S;t++)
		{
			dfile[0] >> v;
			VOLTAGE_HISTORY[i][j].DVector.push_back(v);
		}

		dfile[0] >> S;
		for(long t=0;t<S;t++)
		{
			dfile[0] >> v;
			PHASE[i][j].DVector.push_back(v);
		}
	}

	VOLTAGE_HISTORY_Time_Axis_ms.clear();
	dfile[0] >> S1;
	VOLTAGE_HISTORY_Time_Axis_ms.assign(S1,tmp);
	for(long i=0;i<VOLTAGE_HISTORY_Time_Axis_ms.size();i++)
	dfile[0] >> VOLTAGE_HISTORY_Time_Axis_ms[i];

    ISIs.clear();
	dfile[0] >> S1;
	ISIs.assign(S1,tmp);
	for(long i=0;i<ISIs.size();i++)
	dfile[0] >>  ISIs[i];

    dfile[0] >> ISIs_Node_Ptr_X;

    dfile[0] >> Min_Voltage;
    dfile[0] >> Max_Voltage;
	dfile[0] >> Boundary_Conditions;

	// parameters
	dfile[0] >> k_par;
	dfile[0] >> a;
	dfile[0] >> eps;
	dfile[0] >> mi1;
	dfile[0] >> mi2;
	dfile[0] >> b;

	dfile[0] >> dx;
	dfile[0] >> dt;

	dfile[0] >> Global_Time;

	///////////////

	return 1;

	} // version_4

	//--------------------------------------------------------------------
	if( !strcmp(string,"version_3") )
	//--------------------------------------------------------------------
	{
	dfile[0] >> Size_X;
	dfile[0] >> Size_Y;

	if( Size_X != TWO_DIM_SYSTEM_X_SIZE ||
		Size_Y != TWO_DIM_SYSTEM_Y_SIZE )
	{
		ShowMessage("Incorrect system dimensions");
		return -1;
	}


	allocate_tables();
	long S;
	double v;

	for(long i=0;i<Size_X;i++)
	for(long j=0;j<Size_Y;j++)
	{
		dfile[0] >> V1[i][j];
		dfile[0] >> V2[i][j];
		dfile[0] >> W1[i][j];
		dfile[0] >> W2[i][j];
		dfile[0] >> DX[i][j];
		dfile[0] >> DY[i][j];
		dfile[0] >> MI1[i][j];
		dfile[0] >> MI2[i][j];

		dfile[0] >> S;
		for(long t=0;t<S;t++)
		{
			dfile[0] >> v;
			VOLTAGE_HISTORY[i][j].DVector.push_back(v);
		}
	}

	VOLTAGE_HISTORY_Time_Axis_ms.clear();
	dfile[0] >> S1;
	VOLTAGE_HISTORY_Time_Axis_ms.assign(S1,tmp);
	for(long i=0;i<VOLTAGE_HISTORY_Time_Axis_ms.size();i++)
	dfile[0] >> VOLTAGE_HISTORY_Time_Axis_ms[i];

    ISIs.clear();
	dfile[0] >> S1;
	ISIs.assign(S1,tmp);
	for(long i=0;i<ISIs.size();i++)
	dfile[0] >>  ISIs[i];

    dfile[0] >> ISIs_Node_Ptr_X;

    dfile[0] >> Min_Voltage;
    dfile[0] >> Max_Voltage;
	dfile[0] >> Boundary_Conditions;

	// parameters
	dfile[0] >> k_par;
	dfile[0] >> a;
	dfile[0] >> eps;
	dfile[0] >> mi1;
	dfile[0] >> mi2;
	dfile[0] >> b;

	dfile[0] >> dx;
	dfile[0] >> dt;

	dfile[0] >> Global_Time;

	///////////////

	return 1;

	} // version_3

/*
	//--------------------------------------------------------------------
	if( !strcmp(string,"version_1") )
	//--------------------------------------------------------------------
	{
	dfile[0] >> Size_X;
	dfile[0] >> Size_Y;
	allocate_tables();

	for(long i=0;i<Size_X;i++)
	for(long j=0;j<Size_Y;j++)
	{
		dfile[0] >> V1[i][j];
		dfile[0] >> V2[i][j];
		dfile[0] >> W1[i][j];
		dfile[0] >> W2[i][j];
		dfile[0] >> DX[i][j];
		dfile[0] >> DY[i][j];
	}

	long S1,S2;
	double tmp;
	dfile[0] >> S1;
	dfile[0] >> S2;

	vector_of_doubles D;
	D.DVector.clear();
	D.DVector.assign(S2,tmp);
	Middle_Row_Voltage_History.clear();
	Middle_Row_Voltage_History.assign(S1,D);
	for(long i=0;i<Middle_Row_Voltage_History.size();i++)
	for(long j=0;j<Middle_Row_Voltage_History[i].DVector.size();j++)
	dfile[0] >>  Middle_Row_Voltage_History[i].DVector[j];

	dfile[0] >> S1;
    ISIs.assign(S1,tmp);
    for(long i=0;i<ISIs.size();i++)
	dfile[0] >>  ISIs[i];


    dfile[0] >> ISIs_Node_Ptr_X;

    dfile[0] >> Min_Voltage;
    dfile[0] >> Max_Voltage;
    dfile[0] >> Boundary_Conditions;

	// parameters
    dfile[0] >> k_par;
	dfile[0] >> a;
	dfile[0] >> eps;
	dfile[0] >> mi1;
	dfile[0] >> mi2;
	dfile[0] >> b;

	dfile[0] >> dx;
	dfile[0] >> dt;

	dfile[0] >> Global_Time;

	///////////////

	return 1;

	} // version_2

	//--------------------------------------------------------------------
	if( !strcmp(string,"version_2") )
	//--------------------------------------------------------------------
	{
	dfile[0] >> Size_X;
	dfile[0] >> Size_Y;

	if( Size_X != TWO_DIM_SYSTEM_X_SIZE ||
		Size_Y != TWO_DIM_SYSTEM_Y_SIZE )
	{
		ShowMessage("Incorrect system dimensions");
		return -1;
	}


	allocate_tables();
	long S;
	double v;

	for(long i=0;i<Size_X;i++)
	for(long j=0;j<Size_Y;j++)
	{
		dfile[0] >> V1[i][j];
		dfile[0] >> V2[i][j];
		dfile[0] >> W1[i][j];
		dfile[0] >> W2[i][j];
		dfile[0] >> DX[i][j];
		dfile[0] >> DY[i][j];
		dfile[0] >> MI1[i][j];
		dfile[0] >> MI2[i][j];

		dfile[0] >> S;
		for(long t=0;t<S;t++)
		{
			dfile[0] >> v;
			VOLTAGE_HISTORY[i][j].DVector.push_back(v);
		}
	}

	long S1,S2;
	double tmp;
	dfile[0] >> S1;
	dfile[0] >> S2;

	vector_of_doubles D;
	D.DVector.clear();
	D.DVector.assign(S2,tmp);
	Middle_Row_Voltage_History.clear();
	Middle_Row_Voltage_History.assign(S1,D);
	for(long i=0;i<Middle_Row_Voltage_History.size();i++)
	for(long j=0;j<Middle_Row_Voltage_History[i].DVector.size();j++)
	dfile[0] >>  Middle_Row_Voltage_History[i].DVector[j];

    dfile[0] >> S1;
	ISIs.assign(S1,tmp);
    for(long i=0;i<ISIs.size();i++)
    dfile[0] >>  ISIs[i];


    dfile[0] >> ISIs_Node_Ptr_X;

    dfile[0] >> Min_Voltage;
    dfile[0] >> Max_Voltage;
	dfile[0] >> Boundary_Conditions;

    // parameters
    dfile[0] >> k_par;
    dfile[0] >> a;
    dfile[0] >> eps;
    dfile[0] >> mi1;
    dfile[0] >> mi2;
    dfile[0] >> b;

    dfile[0] >> dx;
    dfile[0] >> dt;

    dfile[0] >> Global_Time;

    ///////////////

    return 1;

	} // version_2
*/
}

//---------------------------------------------------------------------------

void TP1A_2D_system_class::add_assymetric_modulation()
{
	if( Add_Assymetric_Modulation )
	{
	double Modulation;

	double t1 = floor(Global_Time/Asm_Length)*Asm_Length;
	double t2 = (floor(Global_Time/Asm_Length)+1)*Asm_Length;

	if( Global_Time < t1 + Asm_Ratio*Asm_Length)
	Modulation = Asm_Amplitude*(Global_Time-t1)/(Asm_Ratio*Asm_Length);
	else
	Modulation = Asm_Amplitude*(t2-Global_Time)/((1-Asm_Ratio)*Asm_Length);

	long XL = 0.5*Size_X - Central_Region_Relative_Size*Size_X/2;
	long XR = 0.5*Size_X + Central_Region_Relative_Size*Size_X/2;

	long YL = 0.5*Size_Y - Central_Region_Relative_Size*Size_Y/2;
	long YR = 0.5*Size_Y + Central_Region_Relative_Size*Size_Y/2;

	for(long i=XL;i<XR;i++)
	for(long j=YL;j<YR;j++)
	{
		if(V1[i][j] + Modulation < Max_Voltage )
		V1[i][j] += Modulation;
		if(V2[i][j] + Modulation < Max_Voltage )
		V2[i][j] += Modulation;
	}

	}
}

//---------------------------------------------------------------------------

void TP1A_2D_system_class::add_current_modulation_1()
{
	if( Add_Current_Modulation_1 )
	{
	double Modulation = CM1_Amplitude*sin(2*M_PI/CM1_Length*Global_Time);
	if( Modulation > 0 )
	Modulation = 0;

	long XL = 0.5*Size_X - Central_Region_Relative_Size*Size_X/2;
	long XR = 0.5*Size_X + Central_Region_Relative_Size*Size_X/2;

	long YL = 0.5*Size_Y - Central_Region_Relative_Size*Size_Y/2;
	long YR = 0.5*Size_Y + Central_Region_Relative_Size*Size_Y/2;

	for(long i=XL;i<XR;i++)
	for(long j=YL;j<YR;j++)
	{
		if(V1[i][j] + Modulation < Max_Voltage )
		V1[i][j] += Modulation;
		if(V2[i][j] + Modulation < Max_Voltage )
		V2[i][j] += Modulation;
	}

	}
}

//---------------------------------------------------------------------------

void TP1A_2D_system_class::set_initial_coupling()
{
	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	{
		DX[i][j] = Max_Diff_X;
		DY[i][j] = Max_Diff_Y;
	}
}

//---------------------------------------------------------------------------

void TP1A_2D_system_class::add_coupling(int Version,bool Add_Rings)
{
	//------------------------------
	// discs
	//------------------------------
	if( Version == 1 )
	{
	int Curr_No = 0;
	while( Curr_No < Diff_v1_Number )
	{

	Curr_No++;
	int x = random( Size_X );
	int y = random( Size_Y );

	for( int i1=x-Diff_v1_Radius; i1<x+Diff_v1_Radius; i1++)
	for( int j1=y-Diff_v1_Radius; j1<y+Diff_v1_Radius; j1++)
	if( sqrt(pow(i1-x,2)+pow(j1-y,2)) < Diff_v1_Radius )
	if( i1 >= 0 && i1 < Size_X )
	if( j1 >= 0 && j1 < Size_Y )
	{
		DX[i1][j1] = 0;
		DY[i1][j1] = 0;
	}

	if(Add_Rings)
	for( int i1=x-Diff_v1_Radius-Ring_v1_Width; i1<x+Diff_v1_Radius+Ring_v1_Width; i1++)
	for( int j1=y-Diff_v1_Radius-Ring_v1_Width; j1<y+Diff_v1_Radius+Ring_v1_Width; j1++)
	if( sqrt(pow(i1-x,2)+pow(j1-y,2)) > Diff_v1_Radius )
	if( sqrt(pow(i1-x,2)+pow(j1-y,2)) < Diff_v1_Radius+Ring_v1_Width )
	if( i1 >= 0 && i1 < Size_X )
	if( j1 >= 0 && j1 < Size_Y )
	{
		DX[i1][j1] = (1.+Ring_v1_PercD/100.)*Max_Diff_X;
		DY[i1][j1] = (1.+Ring_v1_PercD/100.)*Max_Diff_Y;
	}

	}


	} // discs

	//------------------------------
	// lines
	//------------------------------
	if( Version == 2 )
	{

	for(long k=0;k<Lines_No_v1;k++)
	for(long ypos=Size_X*(100-Line_Perc_Leng_v1)*0.01*0.5;
			 ypos<Size_X-Size_X*(100-Line_Perc_Leng_v1)*0.01*0.5;
			 ypos++)
	{
		int xpos = (k+1)*Size_Y/Lines_No_v1;

		if( xpos > 0 && xpos < Size_X )
		if( ypos > 0 && ypos < Size_Y )
		{
			DX[xpos][ypos] = Line_Perc_D/100.*Max_Diff_X;
			DY[xpos][ypos] = Line_Perc_D/100.*Max_Diff_Y;
		}
	}

	} // discs

	//------------------------------
	// central disk
	//------------------------------
	if( Version == 3 )
	{

	for( int i1=Size_X/2-Disk_Radius; i1<Size_X/2+Disk_Radius; i1++)
	for( int j1=Size_Y/2-Disk_Radius; j1<Size_Y/2+Disk_Radius; j1++)
	if( sqrt(pow(i1-Size_X/2,2)+pow(j1-Size_Y/2,2)) < Disk_Radius )
	if( i1 >= 0 && i1 < Size_X )
	if( j1 >= 0 && j1 < Size_Y )
	{
		DX[i1][j1] = 0;
		DY[i1][j1] = 0;
	}

	} //  central disk

}

//---------------------------------------------------------------------------

void TP1A_2D_system_class::stimulate_system(double Cx, double Cy)
{
	int Radius=5;
	int Pos_X = Cx*Size_X;
	int Pos_Y = Cy*Size_Y;

	for(int x=Pos_X-Radius;x<=Pos_X+Radius;x++)
	for(int y=Pos_Y-Radius;y<=Pos_Y+Radius;y++)
	stimulate_node(x,y);
}
//---------------------------------------------------------------------------

void TP1A_2D_system_class::stimulate_edge(int Which)
{
	if( Which == 1 )
	for(int x=1;x<Size_X-1;x++)
	for(int y=1;y<=5;y++)
		stimulate_node(x,y);

	if( Which == 2 )
	for(int x=1;x<Size_X-1;x++)
	for(int y=Size_Y-5;y<Size_Y;y++)
		stimulate_node(x,y);

}

//---------------------------------------------------------------------------
void TP1A_2D_system_class::ablate_system_xy_point(int X,int Y,int Direction)
{
	if( Direction > 0 )
	{
		DX[X][Y]=0;
		DY[X][Y]=0;
	}
	else
	{
		DX[X][Y]=Max_Diff_X;
		DY[X][Y]=Max_Diff_Y;
	}
}

//---------------------------------------------------------------------------

void TP1A_2D_system_class::ablate_system(double Cx, double Cy,int Direction)
{
	int Radius=4;
	int Pos_X = Cx*Size_X;
	int Pos_Y = Cy*Size_Y;
	double r;

	for(int x=Pos_X-Radius;x<=Pos_X+Radius;x++)
	for(int y=Pos_Y-Radius;y<=Pos_Y+Radius;y++)
	if(x>=0 && y>=0 && x<Size_X && y<Size_Y)
	{
		r = sqrt( pow(x-Pos_X,2) + pow(y-Pos_Y,2) );
		if( r < Radius )
		{
			if( Direction > 0 )
			{
				DX[Pos_X][Pos_Y]=0;
				DY[Pos_X][Pos_Y]=0;
			}
		else
		{
			DX[Pos_X][Pos_Y]=Max_Diff_X;
			DY[Pos_X][Pos_Y]=Max_Diff_Y;
		}
		}
	}
}

//------------------------------------------------------------------------------

bool TP1A_2D_system_class::activity_present_check()
{
	bool Activity_Present = false;
	double Threshold = 0.5;

	for( int i=1; i<Size_X-1; i++)
	for( int j=1; j<Size_Y-1; j++)
	if( V1[i][j] > Threshold )
		Activity_Present = true;

	return Activity_Present;
}

//------------------------------------------------------------------------------

void TP1A_2D_system_class::add_non_conducting_disk(int X,int Y,int Z,int R)
{
	double r;

	for(int x=X-R;x<=X+R;x++)
	for(int y=Y-R;y<=Y+R;y++)
	if(x>=0 && y>=0 && x<Size_X && y<Size_Y)
	{
		r = sqrt( pow(x-X,2) + pow(y-Y,2) );
		if( 1) // r <= R )
		{
				DX[x][y]=0;
				DY[x][y]=0;
		}
	}
}

//------------------------------------------------------------------------------

void TP1A_2D_system_class::add_critical_region(Two_D_system_CR_region_class CR)
{
	for(int r=0;r<CR.X.size();r++)
		add_non_conducting_disk(CR.X[r],CR.Y[r],0,CR.R[r]);
}

//------------------------------------------------------------------------------

double TP1A_2D_system_class::get_current(int X, int Y)
{
	return W1[X][Y]; // STATES_1[X][Y][0];
}

//------------------------------------------------------------------------------

void TP1A_2D_system_class::compute_min_max_custom_value()
{
	Min_Custom_Value = 100000000;
	Max_Custom_Value = -100000000;

	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	{
		if( CUSTOM_VALUE[i][j] < Min_Custom_Value ) Min_Custom_Value = CUSTOM_VALUE[i][j];
		if( CUSTOM_VALUE[i][j] > Max_Custom_Value ) Max_Custom_Value = CUSTOM_VALUE[i][j];
	}

}

//------------------------------------------------------------------------------

double TP1A_2D_system_class::get_custom_value(double Cx, double Cy)
{
	int X = Cx*Size_X;
	int Y = Cy*Size_Y;
	if( X>=0 && X < Size_X && Y>=0 && Y < Size_Y )
	return CUSTOM_VALUE[X][Y];
	else
	return 0;
}
//-------------------------------------------------------------------

void TP1A_2D_system_class::export_voltage_history(AnsiString Filename,AnsiString Signal_Type)
{
	long Signal_Length = VOLTAGE_HISTORY[0][0].DVector.size();
	double Spatial_Sampling = 2;

	AnsiString FF = Filename + " " + FloatToStr(TWO_DIM_SYSTEM_X_SIZE/Spatial_Sampling) + "x"+ FloatToStr(TWO_DIM_SYSTEM_Y_SIZE/Spatial_Sampling)+"x"+ FloatToStr(Signal_Length) + ".csv";
	ofstream df(FF.c_str());

	// write size of the 3D matrix
	if( Signal_Type == "voltage variable" )
	{
		for(int x=0;x<(double)TWO_DIM_SYSTEM_X_SIZE;x+=Spatial_Sampling)
		for(int y=0;y<(double)TWO_DIM_SYSTEM_Y_SIZE;y+=Spatial_Sampling)
		for(int t=0;t<Signal_Length;t++)
		{
			df << (double)((int)(VOLTAGE_HISTORY[x][y].DVector[t]*100))/100 << ",";
		}
	}

	df.close();
}

//-------------------------------------------------------------------

void TP1A_2D_system_class::add_mi1_mi2_R_disk(double mi1,double mi2,double R, int Target_Variable)
{
	// mi1 inner disk area
	// mi2 outer disk area
	if( Target_Variable == 0 )
		set_initial_coupling();
	else
	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	{
		if( Target_Variable == 1 ) 	MI1[i][j] = mi2;
		if( Target_Variable == 2 ) 	MI2[i][j] = mi2;
	}

	double r;
	int X = 0.5*Size_X;
	int Y = 0.5*Size_Y;

	for(int x=X-R;x<=X+R;x++)
	for(int y=Y-R;y<=Y+R;y++)
	if(x>=0 && y>=0 && x<Size_X && y<Size_Y)
	{
		r = sqrt( pow(x-X,2) + pow(y-Y,2) );
		if( r <= R )
		{
			if( Target_Variable == 0 ) 	{ DX[x][y] = mi1; DY[x][y] = mi1; }
			if( Target_Variable == 1 ) 	MI1[x][y] = mi1;
			if( Target_Variable == 2 ) 	MI2[x][y] = mi1;
		}
	}
}
//-------------------------------------------------------------------
long TP1A_2D_system_class::get_history_vector_size()
{
	return VOLTAGE_HISTORY_Time_Axis_ms.size();
}
//-------------------------------------------------------------------

long TP1A_2D_system_class::get_voltage_history_vector_size(int X,int Y)
{
	long S;

	if(X>=0 && X<Size_X && Y >=0 && Y < Size_Y)
	return VOLTAGE_HISTORY[X][Y].DVector.size();

}
//-------------------------------------------------------------------

void TP1A_2D_system_class::clear_grid_egms()
{
	Grid_Electrograms.clear();
}

//-------------------------------------------------------------------

void TP1A_2D_system_class::add_grid_egm(Electrogram *E)
{
	Grid_Electrograms.push_back(E[0]);
}
//-------------------------------------------------------------------

double TP1A_2D_system_class::get_correlation_length(
		long Start_Ptr,long Stop_Ptr, long BCL_Ptr,
		double* CL, double* MPC,std::vector<double> *MPCs,std::vector<double> *Distances)
{
	if( Start_Ptr >= 0 && Start_Ptr < VOLTAGE_HISTORY_Time_Axis_ms.size() &&
		Stop_Ptr >= 0 &&  Stop_Ptr  < VOLTAGE_HISTORY_Time_Axis_ms.size() &&
		VOLTAGE_HISTORY_Time_Axis_ms.size() > 50 )
	{

	double dist,mpc,Min, Max;
	double ts = VOLTAGE_HISTORY_Time_Axis_ms[2]-VOLTAGE_HISTORY_Time_Axis_ms[1];
	double slope_Pearson, intercept_Pearson, Correlation_Length_Pearson=0;
	double slope_MPC,intercept_MPC,Correlation_Length_MPC=0;
	long Min_Ptr, Max_Ptr;
	std::vector<double> Correlation_Vector,logCorr;
	std::vector <double> Data_Vec1,Data_Vec2;
	bool Take_80_Perc_Flag = true; // for MPC calcuations
	int Electrogram_Type = 2; // transmembrane voltage
	int Delay = 0;

	if( ts != 0 )
	{

	double Corr_Step_Size = 2; // parameter used in calculating cross-correlation
	double Corr_Steps_Number = 1.0 / Corr_Step_Size * 90.0/ts; // parameter used in calculating cross-correlation

	// if Phase vector is empty, calcualte phases for all egms AND filtered egm for classic correlation
	std::vector <double> S1;
	if( Grid_Electrograms[0].Phase.size() == 0 )
	{

	for(long d1=0;d1<(signed)Grid_Electrograms.size();d1++)
	{
		// phase
		//S1 = Numerical_Library_Obj.recompose_signal( &Grid_Electrograms[d1].Egm,BCL_Ptr,Electrogram_Type);
		//Grid_Electrograms[d1].Phase = Numerical_Library_Obj.get_Hilbert_phase_using_convolution_double(&S1,ts);

		Grid_Electrograms[d1].Phase = Numerical_Library_Obj.
			get_electrogram_phase(&Grid_Electrograms[d1].Egm,Electrogram_Type,BCL_Ptr,ts);
	}

	}

	Corrs.clear();
	MPCs[0].clear();
	Distances[0].clear();

	//---------------------------------------------------------------
	for(long d1=0;d1<(signed)Grid_Electrograms.size();d1++)
	for(long d2=0;d2<(signed)Grid_Electrograms.size();d2++)
	if( d1<d2 )
	//---------------------------------------------------------------
	{
		dist = std::sqrt(std::pow(Grid_Electrograms[d1].x - Grid_Electrograms[d2].x,2 ) +
						 std::pow(Grid_Electrograms[d1].y - Grid_Electrograms[d2].y,2 ) +
						 std::pow(Grid_Electrograms[d1].z - Grid_Electrograms[d2].z,2 ) );

		Distances[0].push_back(dist);

		// get MPC in current window
		Data_Vec1.clear();
		Data_Vec2.clear();
		Data_Vec1.assign(Grid_Electrograms[d1].Phase.begin() + Start_Ptr, Grid_Electrograms[d1].Phase.begin() + Stop_Ptr);
		Data_Vec2.assign(Grid_Electrograms[d2].Phase.begin() + Start_Ptr, Grid_Electrograms[d2].Phase.begin() + Stop_Ptr);
		mpc = Numerical_Library_Obj.get_phase_synchronization(&Data_Vec1,&Data_Vec2,1,1,Take_80_Perc_Flag,Delay);
		MPCs[0].push_back(mpc);
/*
		// get Pearson Correlation in current window
		Data_Vec1.clear();
		Data_Vec2.clear();
		Data_Vec1.assign(Grid_Electrograms[d1].Egm.begin() + Start_Ptr, Grid_Electrograms[d1].Egm.begin() + Stop_Ptr);
		Data_Vec2.assign(Grid_Electrograms[d2].Egm.begin() + Start_Ptr, Grid_Electrograms[d2].Egm.begin() + Stop_Ptr);
		Correlation_Vector = Numerical_Library_Obj.get_correlation_vector_custom_range(&Data_Vec1,&Data_Vec2, Corr_Steps_Number, Corr_Step_Size);
		Numerical_Library_Obj.find_min_max(&Correlation_Vector,&Min, &Max, &Min_Ptr, &Max_Ptr);
		Corrs.push_back(Max);
*/
	}

	MPC[0] = Numerical_Library_Obj.get_average_from_vector(MPCs);

	// fit linear log curve: MPC
	logCorr.clear();
	for(long k=0;k<MPCs[0].size();k++)
	if( MPCs[0][k] > 0 )
	logCorr.push_back( std::log(MPCs[0][k]) );
	else
	logCorr.push_back(0);

	Numerical_Library_Obj.fitLine(Distances, &logCorr, &slope_MPC, &intercept_MPC);

	if( slope_MPC != 0 )
	Correlation_Length_MPC = -1.0/slope_MPC;
	else
	Correlation_Length_MPC = 0;

	CL[0] = Correlation_Length_MPC;


/*
	// fit linear log curve: Pearson
	logCorr.clear();
	for(long k=0;k<Corrs.size();k++)
	if( Corrs[k] > 0 )
	logCorr.push_back( std::log(Corrs[k]) );
	else
	logCorr.push_back(0);

	Numerical_Library_Obj.fitLine(&Distances, &logCorr, &slope_Pearson, &intercept_Pearson);

	if( slope_Pearson != 0 )
	Correlation_Length_Pearson = -1.0/slope_Pearson;
	else
	Correlation_Length_Pearson = 0;

	// !!!!!!!!!!!!!!!! I dont know why, sometimes its in millions. I guess it's becasue of
	// lack of dependency on distance + random effect
	if( fabs(Correlation_Length_Pearson)>1000 )
	Correlation_Length_Pearson = 0;

	return Correlation_Length_Pearson;
*/


	} // if ts != 0
	else
	ShowMessage("Timestep equal to zero!");
	}

}
//---------------------------------------------------------------------------
/*
void __fastcall TNL_Analysis_Form::Sliding_Window_Corr_L_ButtonClick(TObject *Sender)
{
	Ask_For_Single_Item_Form1->Caption = "Window length [ms]: ";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "3000";
	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	int Corr_L_W_Size_ms = Ask_For_Single_Item_Form1->Edit_Box->Text.ToInt();

	Ask_For_Single_Item_Form1->Caption = "Sliding step [ms]: ";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "100";
	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{
		int Corr_L_step_ms = Ask_For_Single_Item_Form1->Edit_Box->Text.ToInt();

		sliding_window_correlation_length_analysis(Corr_L_W_Size_ms, Corr_L_step_ms);
	}
	}
}
//---------------------------------------------------------------------------
*/
long TP1A_2D_system_class::get_closest_timing(int x,int y,long t)
{
	long diff=10000,timing=-1;

	for(long tt=0;tt<(signed)ACTIVATION_TIMINGS[x][y].DVector.size();tt++)
	{
		if( fabs( ACTIVATION_TIMINGS[x][y].DVector[tt] - t ) < diff )
		{
			diff = fabs( ACTIVATION_TIMINGS[x][y].DVector[tt] - t );
			timing = ACTIVATION_TIMINGS[x][y].DVector[tt];
		}

	}
	return timing;
}

//---------------------------------------------------------------------------

long TP1A_2D_system_class::get_domain_id(int X,int Y,long Time_Ptr)
{
	long domain_id=-1;

	if( X > 1 && X < Size_X && Y > 1 && Y < Size_Y )
	{
		// find which activation ptr corresponds with time Time
		long tptr = -1;
		for(long i=0;i<ACTIVATION_TIMINGS[X][Y].DVector.size();i++)
		if( ACTIVATION_TIMINGS[X][Y].DVector[i] <= Time_Ptr && Time_Ptr <= ACTIVATION_TIMINGS[X][Y].DVector[i+1] )
		tptr = i;

		if( tptr >= 0 && tptr < ACTIVATION_TIMINGS_DOMAIN_ID[X][Y].DVector.size() )
		{
			domain_id = ACTIVATION_TIMINGS_DOMAIN_ID[X][Y].DVector[tptr];
		}
	}

	if( domain_id == -1 )
	int ere=34;

	return domain_id;
}

//-------------------------------------------------------------------

void TP1A_2D_system_class::calculate_velocity_field(double Cycle_Length_ms,
	double Signal_Sampling_Timestep, int Grid_Spacing)
{
	int Algorithm = 3;

	long History_Size = get_history_vector_size();
	Electrogram E;
	double ts = Signal_Sampling_Timestep,Voltage,Time;
	if(ts==0) ts = 0.25;

	int BCL_Ptr = Cycle_Length_ms/ts;
	int Electrogram_Type = 2;
	std::vector<double> S1;
	VelocityVector_2D CV;
	Point_2D grid[3][3];
	double timings[3][3];

	// calculate activation timings for subset of simulation mesh elements xxx
	for(int s1=0;s1<Size_X;s1++)
	for(int s2=0;s2<Size_Y;s2++)
	if( VOLTAGE_HISTORY[s1][s2].DVector.size() > 0 )
	{

		if( PHASE[s1][s2].DVector.size() == 0 )
		{
			E.Egm.clear();

			for(long t=2;t<History_Size;t++)
			{
				get_voltage(s1,s2,t,&Voltage,&Time);
				E.Egm.push_back(Voltage);
			}

			//S1 = Numerical_Library_Obj.recompose_signal( &E.Egm,BCL_Ptr,Electrogram_Type);
			//E.Phase = Numerical_Library_Obj.get_Hilbert_phase_using_convolution_double(&S1,ts);
			PHASE[s1][s2].DVector = Numerical_Library_Obj.get_electrogram_phase(&E.Egm,Electrogram_Type,BCL_Ptr,ts);
		}

		ACTIVATION_TIMINGS[s1][s2].DVector =
			Numerical_Library_Obj.get_activation_timings(&PHASE[s1][s2].DVector);
	}

	double tmp=0;
	for(int s1=0;s1<Size_X;s1++)
	for(int s2=0;s2<Size_Y;s2++)
	{
		VELOCITY_VECTORS_DIRECTION[s1][s2].DVector.assign( ACTIVATION_TIMINGS[s1][s2].DVector.size(), tmp );
		VELOCITY_VECTORS_AMPLITUDE[s1][s2].DVector.assign( ACTIVATION_TIMINGS[s1][s2].DVector.size(), tmp );
	}


	//---------------------------------------------------------------------
	if( Algorithm == 1) // based on local activation times obtained from phase mapping
	//---------------------------------------------------------------------
	{


	// calculate CV
	for(int s1=0;s1<Size_X;s1++)
	for(int s2=0;s2<Size_Y;s2++)
	if( VOLTAGE_HISTORY[s1][s2].DVector.size() > 0 )
	{

	for(long t=0;t<(signed)ACTIVATION_TIMINGS[s1][s2].DVector.size();t++)
	{
		grid[0][0] = Point_2D(s1-Grid_Spacing,s2-Grid_Spacing);
		grid[0][1] = Point_2D(s1-Grid_Spacing,s2);
		grid[0][2] = Point_2D(s1-Grid_Spacing,s2+Grid_Spacing);

		grid[1][0] = Point_2D(s1,s2-Grid_Spacing);
		grid[1][1] = Point_2D(s1,s2);
		grid[1][2] = Point_2D(s1,s2+Grid_Spacing);

		grid[2][0] = Point_2D(s1+Grid_Spacing,s2-Grid_Spacing);
		grid[2][1] = Point_2D(s1+Grid_Spacing,s2);
		grid[2][2] = Point_2D(s1+Grid_Spacing,s2+Grid_Spacing);

		timings[0][0] = get_closest_timing(s1-Grid_Spacing,s2-Grid_Spacing,ACTIVATION_TIMINGS[s1][s2].DVector[t]);
		timings[0][1] = get_closest_timing(s1-Grid_Spacing,s2,ACTIVATION_TIMINGS[s1][s2].DVector[t]);
		timings[0][2] = get_closest_timing(s1-Grid_Spacing,s2+Grid_Spacing,ACTIVATION_TIMINGS[s1][s2].DVector[t]);

		timings[1][0] = get_closest_timing(s1,s2-Grid_Spacing,ACTIVATION_TIMINGS[s1][s2].DVector[t]);
		timings[1][1] = get_closest_timing(s1,s2,ACTIVATION_TIMINGS[s1][s2].DVector[t]);
		timings[1][2] = get_closest_timing(s1,s2+Grid_Spacing,ACTIVATION_TIMINGS[s1][s2].DVector[t]);

		timings[2][0] = get_closest_timing(s1+Grid_Spacing,s2-Grid_Spacing,ACTIVATION_TIMINGS[s1][s2].DVector[t]);
		timings[2][1] = get_closest_timing(s1+Grid_Spacing,s2,ACTIVATION_TIMINGS[s1][s2].DVector[t]);
		timings[2][2] = get_closest_timing(s1+Grid_Spacing,s2+Grid_Spacing,ACTIVATION_TIMINGS[s1][s2].DVector[t]);


		// check if all timings valid
		bool invalid_found = false;
		for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
		if( timings[i][j] < 0 )
		invalid_found = true;

		if( !invalid_found )
		{
			CV = Numerical_Library_Obj.calculateVelocityVector_2D_Grid(grid,timings);

			VELOCITY_VECTORS_DIRECTION[s1][s2].DVector[t] = CV.direction;
			VELOCITY_VECTORS_AMPLITUDE[s1][s2].DVector[t] = CV.amplitude;
		}
	}
	}

	} // algorithm 1
/*
	// randomize (to test how it works for random distrubution
	double v;
	for(int s1=0;s1<Size_X;s1++)
	for(int s2=0;s2<Size_Y;s2++)
	for(int t=0;t<VELOCITY_VECTORS_DIRECTION[s1][s2].DVector.size();t++)
	{
		v = (double)Numerical_Library_Obj.getRandomNumber(10000)/10000.;
		VELOCITY_VECTORS_DIRECTION[s1][s2].DVector[t] = v*(2*M_PI) - M_PI;
	}
*/
	//---------------------------------------------------------------------
	if( Algorithm == 2) // based on gradient of voltage
	//---------------------------------------------------------------------
	{

	int gradient_range = 3;
	double vx,vy,time_ptr;

	// calculate CV
	for(int s1=gradient_range;s1<Size_X-gradient_range;s1++)
	for(int s2=gradient_range;s2<Size_Y-gradient_range;s2++)
	if( VOLTAGE_HISTORY[s1][s2].DVector.size() > 0 )
	{
		for(long t=0;t<(signed)ACTIVATION_TIMINGS[s1][s2].DVector.size();t++)
		{
			time_ptr = ACTIVATION_TIMINGS[s1][s2].DVector[t];

			if( time_ptr > 0 &&
			time_ptr < VOLTAGE_HISTORY[s1+gradient_range][s2].DVector.size() &&
			time_ptr < VOLTAGE_HISTORY[s1][s2+gradient_range].DVector.size() &&
			time_ptr < VOLTAGE_HISTORY[s1-gradient_range][s2].DVector.size() &&
			time_ptr < VOLTAGE_HISTORY[s1][s2-gradient_range].DVector.size() )
			{
				vx = 0.5*(VOLTAGE_HISTORY[s1+gradient_range][s2].DVector[time_ptr] - VOLTAGE_HISTORY[s1-gradient_range][s2].DVector[time_ptr]);
				vy = 0.5*(VOLTAGE_HISTORY[s1][s2+gradient_range].DVector[time_ptr] - VOLTAGE_HISTORY[s1][s2-gradient_range].DVector[time_ptr]);

				VELOCITY_VECTORS_DIRECTION[s1][s2].DVector[t] = std::atan2(vy,vx);
				VELOCITY_VECTORS_AMPLITUDE[s1][s2].DVector[t] = std::sqrt( vx*vx + vy*vy );
			}
		}
	}

	} // algorithm 2

}

//---------------------------------------------------------------------------

VelocityVector_2D TP1A_2D_system_class::get_conduction_arrow(int X,int Y,long Time_Ptr)
{
	double amplitude=0, direction=0;

	if( X > 1 && X < Size_X && Y > 1 && Y < Size_Y )
	{
		// find which activation ptr corresponds with time Time
		long tptr = -1;
		for(long i=0;i<(signed long)ACTIVATION_TIMINGS[X][Y].DVector.size()-1;i++)
		if( ACTIVATION_TIMINGS[X][Y].DVector[i] <= Time_Ptr && Time_Ptr <= ACTIVATION_TIMINGS[X][Y].DVector[i+1] )
		tptr = i;

		if( tptr >= 0 )
		{
			direction = VELOCITY_VECTORS_DIRECTION[X][Y].DVector[tptr];
			amplitude = VELOCITY_VECTORS_AMPLITUDE[X][Y].DVector[tptr];
		}
	}

	return { amplitude, direction };
}

//---------------------------------------------------------------------------

void TP1A_2D_system_class::set_timestep(double Timestep)
{
	dt = Timestep;
}

//-------------------------------------------------------------------

void TP1A_2D_system_class::mark_line_point(int i, int j)
{
	// no A B points marked yet
	if( Line_point_A_i == -1 &&	Line_point_B_i == -1 )
	{
	Line_point_A_i = i;
	Line_point_A_j = j;
	}

	// A marked and B is not
	if( Line_point_A_i > 0 && Line_point_B_i == -1 )
	{
	Line_point_B_i = i;
	Line_point_B_j = j;
	}

	// both A B points marked - just eras them and mark ij as A
	if( Line_point_A_i > 0 && Line_point_B_i > 0 )
	{
	Line_point_A_i = i;
	Line_point_A_j = j;
	Line_point_B_i = -1;
	Line_point_B_j = -1;
	}

}

//---------------------------------------------------------------------------

void TP1A_2D_system_class::phase_vs_distance_data(long Time_Ptr, long BCL_Ptr,
		std::vector<double> *Angles,std::vector<double> *Distances, int Algorithm)
{
	double distance;
	VelocityVector_2D CV_Vector_1,CV_Vector_2;
	double Angle_1_Rad, Angle_2_Rad, MPC;
	long i1,j1,i2,j2,Comparisons_Number = 100000;
	long MPC_Comparisons_Number = 100000;
	bool Take_80_Perc_Flag = true;
	int Delay = 0;

	Angles[0].clear();
	Distances[0].clear();

	if( Algorithm == 0 ) // CV vector direction
	for(int k=1;k<Comparisons_Number;k++)
	{
		i1 = Numerical_Library_Obj.getRandomNumber(TWO_DIM_SYSTEM_X_SIZE);
		j1 = Numerical_Library_Obj.getRandomNumber(TWO_DIM_SYSTEM_Y_SIZE);

		i2 = Numerical_Library_Obj.getRandomNumber(TWO_DIM_SYSTEM_X_SIZE);
		j2 = Numerical_Library_Obj.getRandomNumber(TWO_DIM_SYSTEM_Y_SIZE);

		CV_Vector_1 = get_conduction_arrow(i1,j1,Time_Ptr);
		Angle_1_Rad = CV_Vector_1.direction;

		CV_Vector_2 = get_conduction_arrow(i2,j2,Time_Ptr);
		Angle_2_Rad = CV_Vector_2.direction;

		distance = std::sqrt( std::pow(i1-i2,2) + std::pow(j1-j2,2) );

		if( CV_Vector_1.amplitude > 0 && CV_Vector_2.amplitude > 0 )
		{
			Angles[0].push_back( cos(Angle_1_Rad - Angle_2_Rad) );
			Distances[0].push_back(distance/TWO_DIM_SYSTEM_X_SIZE);
		}
	}

	if( Algorithm == 1 ) // activation phase
	for(int k=1;k<Comparisons_Number;k++)
	{
		i1 = Numerical_Library_Obj.getRandomNumber(TWO_DIM_SYSTEM_X_SIZE);
		j1 = Numerical_Library_Obj.getRandomNumber(TWO_DIM_SYSTEM_Y_SIZE);

		i2 = Numerical_Library_Obj.getRandomNumber(TWO_DIM_SYSTEM_X_SIZE);
		j2 = Numerical_Library_Obj.getRandomNumber(TWO_DIM_SYSTEM_Y_SIZE);

		if( Time_Ptr > 0 && Time_Ptr < PHASE[i1][j1].DVector.size() && Time_Ptr < PHASE[i2][j2].DVector.size() )
		{

		Angle_1_Rad = PHASE[i1][j1].DVector[Time_Ptr];
		Angle_2_Rad = PHASE[i2][j2].DVector[Time_Ptr];

		distance = std::sqrt( std::pow(i1-i2,2) + std::pow(j1-j2,2) );

		if( Angle_1_Rad != 0 && Angle_2_Rad != 0 )
		{
			Angles[0].push_back( cos(Angle_1_Rad - Angle_2_Rad) );
			Distances[0].push_back(distance/TWO_DIM_SYSTEM_X_SIZE);
		}
		}
	}

	if( Algorithm == 2 ) // MPC of whole signal
	for(int k=1;k < MPC_Comparisons_Number;k++)
	{
		i1 = Numerical_Library_Obj.getRandomNumber(TWO_DIM_SYSTEM_X_SIZE);
		j1 = Numerical_Library_Obj.getRandomNumber(TWO_DIM_SYSTEM_Y_SIZE);

		i2 = Numerical_Library_Obj.getRandomNumber(TWO_DIM_SYSTEM_X_SIZE);
		j2 = Numerical_Library_Obj.getRandomNumber(TWO_DIM_SYSTEM_Y_SIZE);

		if( PHASE[i1][j1].DVector.size() > 0 && PHASE[i2][j2].DVector.size() > 0 &&
			PHASE[i1][j1].DVector.size() == PHASE[i2][j2].DVector.size() &&
			DX[i1][j1] != 0 && DY[i1][j1] != 0 )
		{

		MPC = Numerical_Library_Obj.get_phase_synchronization(
			&PHASE[i1][j1].DVector,&PHASE[i2][j2].DVector,1,1,Take_80_Perc_Flag,Delay);

		if(MPC<0.4)
		int df=34;

		distance = std::sqrt( std::pow(i1-i2,2) + std::pow(j1-j2,2) );

		Angles[0].push_back( MPC );
		Distances[0].push_back(distance/TWO_DIM_SYSTEM_X_SIZE);

		}
	}
}

//-------------------------------------------------------------------

void TP1A_2D_system_class::recalculate_phase(double BCL_ptr,double Signal_Sampling_ms)
{
	long History_Size = get_history_vector_size();
	Electrogram E;
	std::vector<double> S1;
	double ts = Signal_Sampling_ms,Voltage,Time;
	if(ts==0) ts = 0.25;
	int Electrogram_Type = 2;

	// calculate activation timings for subset of simulation mesh elements xxx
	for(int s1=0;s1<Size_X;s1++)
	for(int s2=0;s2<Size_Y;s2++)
	if( VOLTAGE_HISTORY[s1][s2].DVector.size() > 0 )
	{
		E.Egm.clear();

		for(long t=2;t<History_Size;t++)
		{
			get_voltage(s1,s2,t,&Voltage,&Time);
			E.Egm.push_back(Voltage);
		}

		//S1 = Numerical_Library_Obj.recompose_signal( &E.Egm,BCL_ptr,Electrogram_Type);
		//E.Phase = Numerical_Library_Obj.get_Hilbert_phase_using_convolution_double(&S1,ts);

		PHASE[s1][s2].DVector = Numerical_Library_Obj.get_electrogram_phase(&E.Egm,Electrogram_Type,BCL_ptr,ts);

		ACTIVATION_TIMINGS[s1][s2].DVector =
			Numerical_Library_Obj.get_activation_timings(&PHASE[s1][s2].DVector);
	}
}

//---------------------------------------------------------------------------

double TP1A_2D_system_class::get_phase(int X,int Y,long Time_Ptr)
{
	if( X>=0 && X<Size_X && Y>=0 && Y<Size_Y )
	if( Time_Ptr >= 0 && Time_Ptr < PHASE[X][Y].DVector.size() )
	return PHASE[X][Y].DVector[Time_Ptr];
}

//---------------------------------------------------------------------------


