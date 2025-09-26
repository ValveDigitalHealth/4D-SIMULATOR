//---------------------------------------------------------------------------

#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <vcl.h>

#pragma hdrstop

#include "FHN_3D_system.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

FHN_3D_system_class::~FHN_3D_system_class()
{
    free_tables();
}
//---------------------------------------------------------------------------

FHN_3D_system_class::FHN_3D_system_class()
{
    Min_Voltage = -2;
    Max_Voltage = 2;

    ni = 0.3;
    gamma = 0.5;
    beta = 0.7;

    dx = 0.1;
    dt = 0.01;

    DX = 0.1;
    DY = 0.1;
    DZ = 0.1;

    Size_X = 2;
    Size_Y = 2;
    Size_Z = 2;
    allocate_tables();

    Global_Time=0;

    // Load phase vector
	ifstream dfile;
    char s[3000];
    dfile.open( "FHN_phase.txt" );
    if(dfile==NULL) ShowMessage("Unable to open FHN_phase.txt file");
    else
    {
        dfile >> FHN_APD_Length;
        FHN_Activation_Variable_APD = new double[FHN_APD_Length];
        FHN_Recovery_Variable_APD = new double[FHN_APD_Length];

        for(long i=0;i<FHN_APD_Length;i++)
        {
            dfile >> s; FHN_Activation_Variable_APD[i] = atof(s);
            dfile >> s; FHN_Recovery_Variable_APD[i] = atof(s);
        }
    }

}

//---------------------------------------------------------------------------

int FHN_3D_system_class::allocate_tables()
{

    V1 = (double***) malloc((unsigned)(Size_X)*sizeof(double**));
    V2 = (double***) malloc((unsigned)(Size_X)*sizeof(double**));
    W1 = (double***) malloc((unsigned)(Size_X)*sizeof(double**));
    W2 = (double***) malloc((unsigned)(Size_X)*sizeof(double**));
    if (!V1) exit(0);

    for( int i=0; i < Size_X; i++)
    {
        V1[i] = (double**) malloc((unsigned)(Size_Y)*sizeof(double*));
        V2[i] = (double**) malloc((unsigned)(Size_Y)*sizeof(double*));
        W1[i] = (double**) malloc((unsigned)(Size_Y)*sizeof(double*));
        W2[i] = (double**) malloc((unsigned)(Size_Y)*sizeof(double*));
        if (!V1[i]) exit(0);
    }

    for( int i=0; i<Size_X; i++)
    for( int j=0; j<Size_Y; j++)
      {
            V1[i][j] = (double*) malloc((unsigned)(Size_Z)*sizeof(double));
            V2[i][j] = (double*) malloc((unsigned)(Size_Z)*sizeof(double));
            W1[i][j] = (double*) malloc((unsigned)(Size_Z)*sizeof(double));
            W2[i][j] = (double*) malloc((unsigned)(Size_Z)*sizeof(double));
            if (!V1[i][j])
      	{
		      exit(0);
      	}
      }

     clear_system();

     return 1;
}

//---------------------------------------------------------------------------

void FHN_3D_system_class::clear_system()
{
    // Clear grid
    for( int i=0; i<Size_X; i++)
    for( int j=0; j<Size_Y; j++)
    for( int k=0; k<Size_Z; k++)
    {

    V1[i][j][k] = -1.1;
    V2[i][j][k] = -1.1;
    W1[i][j][k] = -0.6;
    W2[i][j][k] = -0.6;

    }

 }

//---------------------------------------------------------------------------

int FHN_3D_system_class::free_tables()
{
      for( int i=0; i<Size_X; i++)
      for( int j=0; j<Size_Y; j++)
      {
            free( (double*) ( V1[i][j] ) );
            free( (double*) ( V2[i][j] ) );
            free( (double*) ( W1[i][j] ) );
            free( (double*) ( W2[i][j] ) );
      }


      for( int i=0; i<Size_X; i++)
      {
            free( (double**) ( V1[i] ) );
            free( (double**) ( V2[i] ) );
            free( (double**) ( W1[i] ) );
            free( (double**) ( W2[i] ) );
      }

            free( (double***) (V1) );
            free( (double***) (V2) );
            free( (double***) (W1) );
            free( (double***) (W2) );

    return 1;

}
//---------------------------------------------------------------------------

void FHN_3D_system_class::compute_N_steps(int N)
{
    for(int n=0;n<N;n++)
    {
        calculate_V1_from_V2_fhn();
        calculate_V2_from_V1_fhn();
        Global_Time = Global_Time + dt;
    }
}

//---------------------------------------------------------------------------

void FHN_3D_system_class::calculate_V2_from_V1_fhn()
{
//fff21
      int i,j,k,l;
      double e,r,k1,k2,w = 0.5,xp,yp;
      double D1 = DX/(dx*dx),diff_term;

      // matrix V without boundaries
      for( i = 1; i < Size_X - 1; i++ )
      for( j = 1; j < Size_Y - 1; j++ )
      for( k = 1; k < Size_Z - 1; k++ )
      {
        diff_term = D1*(
            V1[i+1][j][k] + V1[i-1][j][k] +
            V1[i][j+1][k] + V1[i][j-1][k] +
            V1[i][j][k+1] + V1[i][j][k-1] +
            - 6*V1[i][j][k] );

        // Runge Kutta for e variable
        k1 = (V1[i][j][k]-0.333*std::pow(V1[i][j][k],3)-W1[i][j][k])/ni;
        e = V1[i][j][k] + k1*dt/(0.5*w);
        r = W1[i][j][k] + dt/(0.5*w);
        k2 = (e-0.333*pow(e,3)-r)/ni;
        V2[i][j][k] = V1[i][j][k] +
            + dt*( (1-w)*k1 + w*k2 + diff_term);

        // Runge Kutta for r variable
        k1 = ni*(V1[i][j][k] + beta - gamma*W1[i][j][k]);
        k2 = ni*(e + beta - gamma*r);
        W2[i][j][k] = W1[i][j][k] + dt*( (1-w)*k1 + w*k2 );

      }

      //  zero flux boundary conditions

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

//------------------------------------------------------------------------------

void FHN_3D_system_class::calculate_V1_from_V2_fhn()
{
//fff12
      int i,j,k,l;
      double e,r,k1,k2,w = 0.5,xp,yp;
      double D1 = DX/(dx*dx),diff_term;

      // matrix V without boundaries
      for( i = 1; i < Size_X - 1; i++ )
      for( j = 1; j < Size_Y - 1; j++ )
      for( k = 1; k < Size_Z - 1; k++ )
      {
        diff_term = D1*(
            V2[i+1][j][k] + V2[i-1][j][k] +
            V2[i][j+1][k] + V2[i][j-1][k] +
            V2[i][j][k+1] + V2[i][j][k-1] +
            - 6*V2[i][j][k] );

        // Runge Kutta for e variable
        k1 = (V2[i][j][k]-0.333*std::pow(V2[i][j][k],3)-W2[i][j][k])/ni;
        e = V2[i][j][k] + k1*dt/(0.5*w);
        r = W2[i][j][k] + dt/(0.5*w);
        k2 = (e-0.333*pow(e,3)-r)/ni;
        V1[i][j][k] = V2[i][j][k] +
            + dt*( (1-w)*k1 + w*k2 + diff_term);

        // Runge Kutta for r variable
        k1 = ni*(V2[i][j][k] + beta - gamma*W2[i][j][k]);
        k2 = ni*(e + beta - gamma*r);
        W1[i][j][k] = W2[i][j][k] + dt*( (1-w)*k1 + w*k2 );

      }

      //  zero flux boundary conditions

      //  zero flux boundary conditions

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

//------------------------------------------------------------------------------


void FHN_3D_system_class::create_spiral(int Slice_Plane) // 0-xy,
{
    double wn,r,fi,phase,x,y,d;
    double cx,cy;
    long n;

    // winding number
    wn = 15.0;

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
        int m = (int)((1.0-phase)*(FHN_APD_Length-2));

        V1[i][j][k] = FHN_Activation_Variable_APD[m];
        V2[i][j][k] = FHN_Activation_Variable_APD[m];
        W1[i][j][k] = FHN_Recovery_Variable_APD[m];
        W2[i][j][k] = FHN_Recovery_Variable_APD[m];
    }


}

//---------------------------------------------------------------------------
int FHN_3D_system_class::get_color_code(int X,int Y, int Z)
{
    return 255*(V1[X][Y][Z]-Min_Voltage)/(Max_Voltage-Min_Voltage);
}

//---------------------------------------------------------------------------

void FHN_3D_system_class::stimulate_node(int X,int Y,int Z)
{
    V1[X][Y][Z]=1;
    V2[X][Y][Z]=1;
}

//---------------------------------------------------------------------------

void FHN_3D_system_class::record_potentials()
{
    int j = Size_Y/2;
    int k = Size_Z/2;
    for( int i=0; i<Size_X; i++)
        Middle_Row_Voltage_History[i].DVector.push_back(V1[i][j][k]);
}

//---------------------------------------------------------------------------

void FHN_3D_system_class::compute_ISIs(long Node_Position)
{
    ISIs_Node_Ptr_X = Node_Position;
    ISIs.clear();
    double Threshold = 0.5;
    double prev=0,curr=0;
    if( Node_Position >= 0 && Node_Position < Size_X )
    for(long t=1;t<Middle_Row_Voltage_History[Node_Position].DVector.size()-1;t++)
    if( Middle_Row_Voltage_History[Node_Position].DVector[t-1] < Threshold )
    if( Middle_Row_Voltage_History[Node_Position].DVector[t+1] > Threshold )
    {
        curr = t;
        if( prev != 0 )
            ISIs.push_back(curr-prev);
        prev = curr;
        t+=10; // jump in time forward
    }

}

//---------------------------------------------------------------------------

int FHN_3D_system_class::save_object(ofstream* dfile)
{
    dfile[0] << " version_1 ";

    dfile[0] << Size_X << " ";
    dfile[0] << Size_Y << " ";
    dfile[0] << Size_Z << " ";


}
//---------------------------------------------------------------------------

int FHN_3D_system_class::load_object(ifstream* dfile)
{
    char string[2000];

    dfile[0] >> string;
    if( !strcmp(string,"version_1") )
    {
    free_tables();
    dfile[0] >> Size_X;
    dfile[0] >> Size_Y;
    dfile[0] >> Size_Z;
    allocate_tables();

    }

}

//---------------------------------------------------------------------------


