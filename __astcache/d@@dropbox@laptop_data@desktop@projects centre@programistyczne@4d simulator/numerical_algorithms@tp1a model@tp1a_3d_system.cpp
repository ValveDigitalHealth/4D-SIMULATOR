
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <vcl.h>
#include "TP1A_3D_system.h"

#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------

TP1A_3D_system_class::~TP1A_3D_system_class()
{
    free_tables();
}
//---------------------------------------------------------------------------

TP1A_3D_system_class::TP1A_3D_system_class()
{
    Min_Voltage = 0;
    Max_Voltage = 1;

    Min_Diff = 0;
	Max_Diff_X = 0.2;
	Max_Diff_Y = 0.2;
	Max_Diff_Z = 0.2;

	a = 0.1;
	k_par = 8;
	eps = 0.01;
	mi1 = 0.2;
	mi2 = 0.2;
	b = 0.1;

  // stable spiral parameters  mi1 = 0.2;

   // breakup parameters
	mi1 = 0.02;

	dx = 0.6;
	dt = 0.02;  // normal
//	dt = 0.005;    // slow for animations

	Size_X = THREE_DIM_SYSTEM_X_SIZE;
	Size_Y = THREE_DIM_SYSTEM_Y_SIZE;
	Size_Z = THREE_DIM_SYSTEM_Z_SIZE;
	allocate_tables();

	ECG_electrode_x=-0.5*Size_X;
	ECG_electrode_y=-0.5*Size_Y;
	ECG_electrode_z=-0.5*Size_Z;

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

}

//---------------------------------------------------------------------------

int TP1A_3D_system_class::allocate_tables()
{
	vector_of_doubles D;
	D.DVector.clear();
	Middle_Row_Voltage_History.clear();
	Middle_Row_Voltage_History.assign(Size_X,D);

	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	for( int k=0; k<Size_Z; k++)
	{
		DX[i][j][k] = Max_Diff_X;
		DY[i][j][k] = Max_Diff_Y;
		DZ[i][j][k] = Max_Diff_Z;
		MI1[i][j][k] = mi1;
		MI2[i][j][k] = mi2;
		VOLTAGE_HISTORY[i][j][k].DVector.clear();
	}

	clear_system();

	return 1;
}

//---------------------------------------------------------------------------

void TP1A_3D_system_class::set_mi_distribution(double mi1_left,double mi2_left,
							 double mi1_right,double mi2_right,
							 double Ratio)
{
/*
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
*/
}

//---------------------------------------------------------------------------
void TP1A_3D_system_class::set_mi_breakup_rings(double mi1_min,double mi1_max,
							 double Radius, long Number)
{
/*
	// set all to normal value
	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	{
		MI1[i][j] = mi1_max;
	}

	//------------------------------
	// discs  xxx
	//------------------------------
	for(int i=0;i<Number;i++)
	{

	int x = random( Size_X );
	int y = random( Size_Y );

	for( int i1=x-Radius; i1<x+Radius; i1++)
	for( int j1=y-Radius; j1<y+Radius; j1++)
	if( sqrt(pow(i1-x,2)+pow(j1-y,2)) < Radius )
	if( i1 >= 0 && i1 < Size_X )
	if( j1 >= 0 && j1 < Size_Y )
	{
		MI1[i1][j1] = mi1_min;
	}

	}
*/
}

//---------------------------------------------------------------------------
void TP1A_3D_system_class::set_mi_breakup_landscape(double R, double p, double I,
		double mi1_min, double mi1_max)
{
/*
	double r;

	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	MI1[i][j] = 0.2;

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
		MI1[i1][j1] -= I*(R-r)/R;

		if( MI1[i1][j1] < mi1_min )
		MI1[i1][j1] = mi1_min;
	}

	}
*/
}

//---------------------------------------------------------------------------
void TP1A_3D_system_class::clear_system()
{
	// Clear grid
	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	for( int k=0; k<Size_Z; k++)
	{
		V1[i][j][k] = 0;
		V2[i][j][k] = 0;
		W1[i][j][k] = 0;
		W2[i][j][k] = 0;
		CUSTOM_VALUE[i][j][k] = 0;
	}

	// signals history
	for(long i=0;i<Middle_Row_Voltage_History.size();i++)
	Middle_Row_Voltage_History[i].DVector.clear();

}

//---------------------------------------------------------------------------

void TP1A_3D_system_class::refresh_VW_matrices() // to avoid drop in efficiency
{
    // Clear grid
	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	for( int k=0; k<Size_Z; k++)
	if( DX[i][j][k]==0 && DY[i][j][k]==0 && DZ[i][j][k]==0)
	{
		V1[i][j][k] = 0;
		V2[i][j][k] = 0;
		W1[i][j][k] = 0;
		W2[i][j][k] = 0;
	}
}

//---------------------------------------------------------------------------

void TP1A_3D_system_class::record_ECG()
{
	double r,x,y,z,v=0;

	for(int i=1;i<(signed)Size_X-1;i++)
	for(int j=1;j<(signed)Size_Y-1;j++)
	for(int k=1;k<(signed)Size_Z-1;k++)
	{

	x =  i - ECG_electrode_x;
	y =  j - ECG_electrode_y;
	z =  k - ECG_electrode_z;

	r = std::pow( std::pow(x,2)+std::pow(y,2)+std::pow(z,2),1.5);

	v -=
	 (
	 ( V1[i+1][j][k]-V1[i-1][j][k] )*(i - ECG_electrode_x) +
	 ( V1[i][j+1][k]-V1[i][j-1][k] )*(j - ECG_electrode_y) +
	 ( V1[i][j][k+1]-V1[i][j][k-1] )*(k - ECG_electrode_z) ) / r;

	} // for all nodes

	ECG_signal.push_back(v);

}

//---------------------------------------------------------------------------

void TP1A_3D_system_class::record_potentials()
{
	int j = Size_Y/2;
	int k = Size_Z/2;
	for( int i=0; i<Size_X; i++)
		Middle_Row_Voltage_History[i].DVector.push_back(V1[i][j][k]);

	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	for( int k=0; k<Size_Z; k++)
		VOLTAGE_HISTORY[i][j][k].DVector.push_back(V1[i][j][k]);
}

//---------------------------------------------------------------------------

int TP1A_3D_system_class::free_tables()
{
	return 1;
}
//---------------------------------------------------------------------------

void TP1A_3D_system_class::compute_N_steps(int N)
{
    for(int n=0;n<N;n++)
    {
        calculate_V1_from_V2_fhn();
		calculate_V2_from_V1_fhn();
        Global_Time = Global_Time + dt;

		add_assymetric_modulation();
		add_current_modulation_1();

    }
}

//---------------------------------------------------------------------------

void TP1A_3D_system_class::calculate_V2_from_V1_fhn()
{
//fff21
	  int i,j,k,l;
	  double e,r,k1,k2,w = 0.5,xp,yp;
	  double diff_term,s2=dx*dx;

	  // matrix V without boundaries
	  for( i = 1; i < Size_X - 1; i++ )
	  for( j = 1; j < Size_Y - 1; j++ )
	  for( k = 1; k < Size_Z - 1; k++ )
	  {
		diff_term = ( DX[i][j][k]*(V1[i-1][j][k] + V1[i+1][j][k] - 2*V1[i][j][k]) +
					  DY[i][j][k]*(V1[i][j+1][k] + V1[i][j-1][k] - 2*V1[i][j][k]) +
					  DZ[i][j][k]*(V1[i][j][k+1] + V1[i][j][k-1] - 2*V1[i][j][k])
					  ) / s2;

		if( diff_term != 0 )
		int ret=34;

		// Forward Euler
		V2[i][j][k] = V1[i][j][k] + dt*
			( -k_par*V1[i][j][k]*(V1[i][j][k]-a)*(V1[i][j][k]-1)-V1[i][j][k]*
					W1[i][j][k]+ diff_term);

		W2[i][j][k] = W1[i][j][k] + dt*
		(eps+(MI1[i][j][k]*W1[i][j][k])/(MI2[i][j][k]+V1[i][j][k]))*
			 (-W1[i][j][k]-k_par*V1[i][j][k]*(V1[i][j][k]-b-1));
      }

    //---------------------------------------------------------------------
    if( Boundary_Conditions == 0 ) // zero flux
    //---------------------------------------------------------------------
    {
      // "Z" sides
      for( i = 1; i < Size_X - 1; i++ )
      for( j = 1; j < Size_Y - 1; j++ )
      {
            V2[i][j][0] = V2[i][j][1];
            V2[i][j][Size_Z-1] = V2[i][j][Size_Z-2];

            W2[i][j][0] = W2[i][j][1];
            W2[i][j][Size_Z-1] = W2[i][j][Size_Z-2];
      }

      // "Y" sides
      for( i = 1; i < Size_X - 1; i++ )
      for( k = 1; k < Size_Z - 1; k++ )
      {
            V2[i][0][k] = V2[i][1][k];
            V2[i][Size_Y-1][k] = V2[i][Size_Y-2][k];

            W2[i][0][k] = W2[i][1][k];
            W2[i][Size_Y-1][k] = W2[i][Size_Y-2][k];
      }

      // "X" sides
      for( j = 1; j < Size_Y - 1; j++ )
      for( k = 1; k < Size_Z - 1; k++ )
      {
            V2[0][j][k] = V2[1][j][k];
            V2[Size_X-1][j][k] = V2[Size_X-2][j][k];

            W2[0][j][k] = W2[1][j][k];
            W2[Size_X-1][j][k] = W2[Size_X-2][j][k];
      }
	}

	//---------------------------------------------------------------------
    if( Boundary_Conditions == 1 ) // periodic conditions
    //---------------------------------------------------------------------
	{
	/*
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
	 */

	}

}

//------------------------------------------------------------------------------

void TP1A_3D_system_class::calculate_V1_from_V2_fhn()
{
//fff12
      int i,j,k,l;
      double e,r,k1,k2,w = 0.5,xp,yp;
      double diff_term,s2=dx*dx;

	  // matrix V without boundaries
	  for( i = 1; i < Size_X - 1; i++ )
	  for( j = 1; j < Size_Y - 1; j++ )
	  for( k = 1; k < Size_Z - 1; k++ )
	  {

		diff_term = ( DX[i][j][k]*(V2[i-1][j][k] + V2[i+1][j][k] - 2*V2[i][j][k]) +
					  DY[i][j][k]*(V2[i][j+1][k] + V2[i][j-1][k] - 2*V2[i][j][k]) +
					  DZ[i][j][k]*(V2[i][j][k+1] + V2[i][j][k-1] - 2*V2[i][j][k])
					  ) / s2;

		// Forward Euler
		V1[i][j][k] = V2[i][j][k] + dt*
			( -k_par*V2[i][j][k]*(V2[i][j][k]-a)*(V2[i][j][k]-1)-V2[i][j][k]*
				W2[i][j][k]+ diff_term);

		W1[i][j][k] = W2[i][j][k] + dt*
		(eps+(MI1[i][j][k]*W2[i][j][k])/(MI2[i][j][k]+V2[i][j][k]))*
			 (-W2[i][j][k]-k_par*V2[i][j][k]*(V2[i][j][k]-b-1));
	  }

	//---------------------------------------------------------------------
	if( Boundary_Conditions == 0 ) // zero flux
	//---------------------------------------------------------------------
	{
      // "Z" sides
      for( i = 1; i < Size_X - 1; i++ )
      for( j = 1; j < Size_Y - 1; j++ )
      {
            V1[i][j][0] = V1[i][j][1];
            V1[i][j][Size_Z-1] = V1[i][j][Size_Z-2];

            W1[i][j][0] = W1[i][j][1];
            W1[i][j][Size_Z-1] = W1[i][j][Size_Z-2];
      }

      // "Y" sides
      for( i = 1; i < Size_X - 1; i++ )
      for( k = 1; k < Size_Z - 1; k++ )
      {
            V1[i][0][k] = V1[i][1][k];
            V1[i][Size_Y-1][k] = V1[i][Size_Y-2][k];

            W1[i][0][k] = W1[i][1][k];
            W1[i][Size_Y-1][k] = W1[i][Size_Y-2][k];
      }

      // "X" sides
      for( j = 1; j < Size_Y - 1; j++ )
      for( k = 1; k < Size_Z - 1; k++ )
      {
            V1[0][j][k] = V1[1][j][k];
            V1[Size_X-1][j][k] = V1[Size_X-2][j][k];

            W1[0][j][k] = W1[1][j][k];
            W1[Size_X-1][j][k] = W1[Size_X-2][j][k];
      }

	}

	//---------------------------------------------------------------------
	if( Boundary_Conditions == 1 ) // periodic conditions
	//---------------------------------------------------------------------
	{
	/*
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
      */
	}
}

//------------------------------------------------------------------------------

void TP1A_3D_system_class::create_spiral(int Slice_Plane) // 0-xy,
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
	for( int k=0;k<Size_Z; k++ )
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

		V1[i][j][k] = TP1A_Activation_Variable_APD[m];
		V2[i][j][k] = TP1A_Activation_Variable_APD[m];
		W1[i][j][k] = 1*TP1A_Recovery_Variable_APD[m];
		W2[i][j][k] = 1*TP1A_Recovery_Variable_APD[m];
    }
}

//---------------------------------------------------------------------------

int TP1A_3D_system_class::get_color_code(int Source,int X,int Y,int Z)
{
	int ptr;

	if( Source == 0 )
	ptr = 255*(V1[X][Y][Z]-Min_Voltage)/(Max_Voltage-Min_Voltage);

	if( Source == 0 && DX[X][Y][Z] <= Min_Voltage && DY[X][Y][Z] <= 0.1 )
	ptr = -1;

	if( Source == 1 )
	ptr = 255-255*(DX[X][Y][Z]-Min_Diff)/(2*Max_Diff_X-Min_Diff);

	if( Source == 2 )
	ptr = 255-255*(DY[X][Y][Z]-Min_Diff)/(2*Max_Diff_Y-Min_Diff);

	double max_mi=0.4,min_mi=0;

	if( Source == 3 )
	ptr = 255-255*(MI1[X][Y][Z]-min_mi)/(max_mi-min_mi);
	if( Source == 4 )
	ptr = 255-255*(MI2[X][Y][Z]-min_mi)/(max_mi-min_mi);

	if( Source == 5 )
	ptr = 255-255*(CUSTOM_VALUE[X][Y][Z]-Min_Custom_Value)/
		(Max_Custom_Value-Min_Custom_Value);

	if( Source == 5 && DY[X][Y][Z] <= 0.1 )
	ptr = -1;

	if (ptr > 255) ptr = 255;

	return ptr;
}

//---------------------------------------------------------------------------

void TP1A_3D_system_class::stimulate_node(int X,int Y,int Z)
{
	V1[X][Y][Z]=1;
	V2[X][Y][Z]=1;
}

//---------------------------------------------------------------------------

void TP1A_3D_system_class::compute_ISIs(long Node_Position)
{
    ISIs_Node_Ptr_X = Node_Position;
    ISIs.clear();

    double Threshold = 0.5;
    double prev=0,curr=0;

    int df= Middle_Row_Voltage_History[Node_Position].DVector.size();

    if( Node_Position >= 0 && Node_Position < Middle_Row_Voltage_History.size() )
    for(long t=1;t<(signed)Middle_Row_Voltage_History[Node_Position].DVector.size()-1;t++)
    if( Middle_Row_Voltage_History[Node_Position].DVector[t-1] < Threshold )
    if( Middle_Row_Voltage_History[Node_Position].DVector[t+1] > Threshold )
	{
        curr = t;

        if( prev != 0 )
            ISIs.push_back(curr-prev);

        prev = curr;

        t+=5; // forward jump in time
    }
}

//---------------------------------------------------------------------------

int TP1A_3D_system_class::save_object(ofstream* dfile)
{
	dfile[0] << " version_3 ";

	dfile[0] << Size_X << " ";
	dfile[0] << Size_Y << " ";
	dfile[0] << Size_Z << " ";

	for(long i=0;i<Size_X;i++)
	for(long j=0;j<Size_Y;j++)
	for(long k=0;k<Size_Z;k++)
	{
		dfile[0] << V1[i][j][k] << " ";
		dfile[0] << V2[i][j][k] << " ";
		dfile[0] << W1[i][j][k] << " ";
		dfile[0] << W2[i][j][k] << " ";
		dfile[0] << DX[i][j][k] << " ";
		dfile[0] << DY[i][j][k] << " ";
		dfile[0] << MI1[i][j][k] << " ";
		dfile[0] << MI2[i][j][k] << " ";

		dfile[0] << VOLTAGE_HISTORY[i][j][k].DVector.size() << " ";
		for(long t=0;t<VOLTAGE_HISTORY[i][j][k].DVector.size();t++)
		dfile[0] << VOLTAGE_HISTORY[i][j][k].DVector[t] << " ";
	}


	dfile[0] << Middle_Row_Voltage_History.size() << " ";
	dfile[0] << Middle_Row_Voltage_History[0].DVector.size() << " ";
	for(long i=0;i<Middle_Row_Voltage_History.size();i++)
	for(long j=0;j<Middle_Row_Voltage_History[i].DVector.size();j++)
	dfile[0] <<  Middle_Row_Voltage_History[i].DVector[j] << " ";

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

int TP1A_3D_system_class::load_object(ifstream* dfile)
{
	char string[2000];

	dfile[0] >> string;

	if( !strcmp(string,"version_3") )
	{

	free_tables();
	dfile[0] >> Size_X;
	dfile[0] >> Size_Y;
	dfile[0] >> Size_Z;
	allocate_tables();

	for(long i=0;i<Size_X;i++)
	for(long j=0;j<Size_Y;j++)
	for(long k=0;k<Size_Z;k++)
	{
		dfile[0] >> V1[i][j][k];
		dfile[0] >> V2[i][j][k];
		dfile[0] >> W1[i][j][k];
		dfile[0] >> W2[i][j][k];
		dfile[0] >> DX[i][j][k];
		dfile[0] >> DY[i][j][k];
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

	} // version_3

}

//---------------------------------------------------------------------------

void TP1A_3D_system_class::add_assymetric_modulation()
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
	for(long k=0;k<Size_Z;k++)
	{
		if(V1[i][j][k] + Modulation < Max_Voltage )
		V1[i][j][k] += Modulation;
		if(V2[i][j][k] + Modulation < Max_Voltage )
		V2[i][j][k] += Modulation;
	}

	}
}

//---------------------------------------------------------------------------

void TP1A_3D_system_class::add_current_modulation_1()
{
/*
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
*/
}

//---------------------------------------------------------------------------

void TP1A_3D_system_class::set_initial_coupling()
{
	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	for( int k=0; k<Size_Z; k++)
	{
		DX[i][j][k] = Max_Diff_X;
		DY[i][j][k] = Max_Diff_Y;
		DZ[i][j][k] = Max_Diff_Z;
	}
}

//---------------------------------------------------------------------------

void TP1A_3D_system_class::add_coupling(int Version,bool Add_Rings)
{
/*
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
*/
}

//---------------------------------------------------------------------------

void TP1A_3D_system_class::stimulate_system(double Cx, double Cy,double Cz)
{
	int Radius=5;
	int Radius_Z = 1;
	int Pos_X = Cx*Size_X;
	int Pos_Y = Cy*Size_Y;
	int Pos_Z = Cz*Size_Z;

	for(int x=Pos_X-Radius;x<=Pos_X+Radius;x++)
	for(int y=Pos_Y-Radius;y<=Pos_Y+Radius;y++)
	for(int z=Pos_Z-Radius_Z;z<=Pos_Z+Radius_Z;z++)
	stimulate_node(x,y,z);
}

//---------------------------------------------------------------------------

void TP1A_3D_system_class::ablate_system(double Cx, double Cy,double Cz)
{

	int Radius=4;
	int Pos_X = Cx*Size_X;
	int Pos_Y = Cy*Size_Y;
	int Pos_Z = Cz*Size_Z;
	double r;

	for(int x=Pos_X-Radius;x<=Pos_X+Radius;x++)
	for(int y=Pos_Y-Radius;y<=Pos_Y+Radius;y++)
	for(int z=Pos_Z-Radius;z<=Pos_Z+Radius;z++)
	{
		r = sqrt( pow(x-Pos_X,2) + pow(y-Pos_Y,2) + pow(z-Pos_Z,2) );
		if( r < Radius )
		{
			DX[x][y][z]=0;
			DY[x][y][z]=0;
			DZ[x][y][z]=0;
		}
	}

}

//---------------------------------------------------------------------------

vector <double> TP1A_3D_system_class::calculate_unipolar_voltage_from_surface(
		int Node_x,long Node_y, long Node_z,long Range)
{
	vector <double> unipolar_voltage;
	double voltage,v,x,y,z,r;

	for(long t=0;t<VOLTAGE_HISTORY[0][0][0].DVector.size();t++)
	{

	voltage=0;
	for(int i=Node_x-Range;i<(signed)(Node_x+Range);i++)
	for(int j=Node_y-Range;j<(signed)(Node_y+Range);j++)
	for(int k=Node_z-Range;k<(signed)(Node_z+Range);k++)
	if( i > 0 && i < Size_X-1 )
	if( j > 0 && j < Size_Y-1 )
	if( k > 0 && k < Size_Z-1 )
	{

	v = VOLTAGE_HISTORY[i][j][k].DVector[t];
	x =  i - Node_x;
	y =  j - Node_y;
	z =  k - Node_z;
	r = std::pow( std::pow(x,2)+std::pow(y,2)+std::pow(z,2),1.5);

	if( r >= 2 )
	voltage -=
	 (  (VOLTAGE_HISTORY[i+1][j][k].DVector[t]-
		 VOLTAGE_HISTORY[i-1][j][k].DVector[t] )*x
		+

		(VOLTAGE_HISTORY[i][j+1][k].DVector[t]-
		 VOLTAGE_HISTORY[i][j-1][k].DVector[t] )*y
		+
		(VOLTAGE_HISTORY[i][j][k+1].DVector[t]-
		 VOLTAGE_HISTORY[i][j][k-1].DVector[t] )*z
	 ) / r;

	} // for all nodes

	unipolar_voltage.push_back(voltage);

	} // through time

	return unipolar_voltage;
}

//------------------------------------------------------------------------------

void TP1A_3D_system_class::set_Z_D_gradient(double Min_D, double Max_D)
{
	double D;

	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	for( int k=0; k<Size_Z; k++)
	{
		D = Min_D*(double)k/(double)Size_Z + Max_D*(1-(double)k/(double)Size_Z);

		DX[i][j][k] = D;
		DY[i][j][k] = D;
		DZ[i][j][k] = D;
	}

}

//------------------------------------------------------------------------------

bool TP1A_3D_system_class::activity_present_check()
{
	bool Activity_Present = false;
	double Threshold = 0.5;

	for( int i=0; i<Size_X; i++)
	for( int j=0; j<Size_Y; j++)
	for( int k=0; k<Size_Z; k++)
	if( V1[i][j][k] > Threshold )
		Activity_Present = true;

	return Activity_Present;
}

//------------------------------------------------------------------------------

double TP1A_3D_system_class::get_voltage(int X, int Y, int Z)
{
	return V1[X][Y][Z];
}

//------------------------------------------------------------------------------

double TP1A_3D_system_class::get_current(int X, int Y, int Z)
{
	return W1[X][Y][Z]; // STATES_1[X][Y][0];
}

//------------------------------------------------------------------------------

