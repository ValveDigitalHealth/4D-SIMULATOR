//---------------------------------------------------------------------------
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <vcl.h>

#pragma hdrstop

#include "FHN_2D_system.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

FHN_2D_system_class::~FHN_2D_system_class()
{
	delete[] FHN_Activation_Variable_APD;
	delete[] FHN_Recovery_Variable_APD;
}
//---------------------------------------------------------------------------

FHN_2D_system_class::FHN_2D_system_class()
{
	Min_Voltage = -1;
    Max_Voltage = 1.5;

    Min_Diff = 0;
	Max_Diff_X = 0.5;
	Max_Diff_Y = 0.5;

    ni = 0.3;
    gamma = 0.5;
    beta = 0.7;

    dx = 0.4;
    dt = 0.05;

    Size_X = TWO_DIM_SYSTEM_X_SIZE; // Assuming these are defined in the header
    Size_Y = TWO_DIM_SYSTEM_Y_SIZE;
    allocate_tables();

    Global_Time=0;

    // Load phase vector
    ifstream dfile;
	char s[3000];
    dfile.open( "FHN_phase.txt" );
    if(!dfile.is_open()) // <-- CORRECTED FILE CHECK
    {
        ShowMessage("Unable to open FHN_phase.txt file");
    }
    else
	{
        dfile >> FHN_APD_Length;
        FHN_Activation_Variable_APD = new double[FHN_APD_Length];
        FHN_Recovery_Variable_APD = new double[FHN_APD_Length];

        for(long i=0; i<FHN_APD_Length; i++)
        {
            dfile >> FHN_Activation_Variable_APD[i];
            dfile >> FHN_Recovery_Variable_APD[i];
        }
        dfile.close();
    }
}

//---------------------------------------------------------------------------

int FHN_2D_system_class::save_object(ofstream* dfile)
{
    (*dfile) << " version_1_FHN ";

    (*dfile) << Size_X << " ";
    (*dfile) << Size_Y << " ";

    for(long i=0; i<Size_X; i++)
    for(long j=0; j<Size_Y; j++)
    {
        (*dfile) << V1[i][j] << " ";
        (*dfile) << W1[i][j] << " "; // Save V1/W1 as the current state
        (*dfile) << DX[i][j] << " ";
        (*dfile) << DY[i][j] << " ";
    }

	(*dfile) << Boundary_Conditions << " ";

    // parameters
    (*dfile) << ni << " ";
    (*dfile) << gamma << " ";
    (*dfile) << beta << " ";

	(*dfile) << dx << " ";
    (*dfile) << dt << " ";

    (*dfile) << Global_Time << " ";

    return 1;
}
//---------------------------------------------------------------------------

int FHN_2D_system_class::load_object(ifstream* dfile)
{
    char string[2000];

    (*dfile) >> string;

    if( !strcmp(string,"version_1_FHN") )
    {
        long loaded_Size_X, loaded_Size_Y;
        (*dfile) >> loaded_Size_X;
        (*dfile) >> loaded_Size_Y;

        if( loaded_Size_X != Size_X || loaded_Size_Y != Size_Y )
        {
            ShowMessage("Incorrect system dimensions in the save file.");
            return -1;
        }

        allocate_tables();

        for(long i=0; i<Size_X; i++)
        for(long j=0; j<Size_Y; j++)
        {
			(*dfile) >> V1[i][j];
            (*dfile) >> W1[i][j];
            // Initialize V2/W2 as well to be safe
            V2[i][j] = V1[i][j];
            W2[i][j] = W1[i][j];
            (*dfile) >> DX[i][j];
            (*dfile) >> DY[i][j];
		}

        (*dfile) >> Boundary_Conditions;

        // parameters
        (*dfile) >> ni;
        (*dfile) >> gamma;
        (*dfile) >> beta;

        (*dfile) >> dx;
        (*dfile) >> dt;

        (*dfile) >> Global_Time;

        return 1;
    }

    ShowMessage("Not FitzHugh-Nagumo system saved in this file.");
    return -1;
}

//---------------------------------------------------------------------------

int FHN_2D_system_class::allocate_tables()
{
    vector_of_doubles D;
    D.DVector.clear();
    Middle_Row_Voltage_History.clear();
    Middle_Row_Voltage_History.assign(Size_X,D);

    for( int i=0; i<Size_X; i++)
    for( int j=0; j<Size_Y; j++)
	{
        DX[i][j] = Max_Diff_X;
        DY[i][j] = Max_Diff_Y;
        CUSTOM_VALUE[i][j]=0.5;
    }

    clear_system();

    return 1;
}

//---------------------------------------------------------------------------

void FHN_2D_system_class::clear_system()
{
    // Clear grid
    for( int i=0; i<Size_X; i++)
    for( int j=0; j<Size_Y; j++)
    {
        V1[i][j] = -1.1;
        V2[i][j] = -1.1;
        W1[i][j] = -0.6;
        W2[i][j] = -0.6;
        CUSTOM_VALUE[i][j]=0;
    }
}

//---------------------------------------------------------------------------

void FHN_2D_system_class::compute_N_steps(int N)
{
    for(int n=0; n<N; n++)
    {
        // Alternate between V1->V2 and V2->V1
        if (n % 2 == 0) {
            calculate_fhn_step(V1, W1, V2, W2);
        } else {
            calculate_fhn_step(V2, W2, V1, W1);
        }
		Global_Time += dt;
    }

    // Ensure V1 holds the final state if N is odd
    if (N % 2 != 0) {
        for(int i = 0; i < Size_X; ++i) {
            for(int j = 0; j < Size_Y; ++j) {
				V1[i][j] = V2[i][j];
                W1[i][j] = W2[i][j];
            }
        }
    }
}

//---------------------------------------------------------------------------
void FHN_2D_system_class::calculate_fhn_step(
	double V_in[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE], double W_in[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE],
	double V_out[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE], double W_out[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE])
{
	int i, j;

	// --- Main loop for interior points ---
	for( i = 1; i < Size_X - 1; i++ )
	for( j = 1; j < Size_Y - 1; j++ )
	{
		_compute_fhn_update(i, j, V_in, W_in, V_out, W_out, i-1, i+1, j-1, j+1);
    }

    // --- Boundary Conditions ---
    if( Boundary_Conditions == 0 ) // zero flux (Neumann)
    {
        // Set ghost cells by copying interior values to boundaries
        // This makes the derivative (flux) zero
        for( i = 1; i < Size_X - 1; i++ ) // Top and bottom rows
        {
            V_out[i][0] = V_out[i][1];
            V_out[i][Size_Y-1] = V_out[i][Size_Y-2];
            W_out[i][0] = W_out[i][1];
            W_out[i][Size_Y-1] = W_out[i][Size_Y-2];
		}
        for( j = 0; j < Size_Y; j++ ) // Left and right columns (incl. corners)
        {
            V_out[0][j] = V_out[1][j];
            V_out[Size_X-1][j] = V_out[Size_X-2][j];
            W_out[0][j] = W_out[1][j];
            W_out[Size_X-1][j] = W_out[Size_X-2][j];
		}
    }
    else if( Boundary_Conditions == 1 ) // periodic conditions
    {
        // Edges
        for( i = 1; i < Size_X - 1; i++ ) // Top (j=0) and Bottom (j=Size_Y-1)
        {
             _compute_fhn_update(i, 0, V_in, W_in, V_out, W_out, i-1, i+1, Size_Y-1, 1);
             _compute_fhn_update(i, Size_Y-1, V_in, W_in, V_out, W_out, i-1, i+1, Size_Y-2, 0);
		}
        for( j = 1; j < Size_Y - 1; j++ ) // Left (i=0) and Right (i=Size_X-1)
        {
             _compute_fhn_update(0, j, V_in, W_in, V_out, W_out, Size_X-1, 1, j-1, j+1);
             _compute_fhn_update(Size_X-1, j, V_in, W_in, V_out, W_out, Size_X-2, 0, j-1, j+1);
        }
        // Corners
        _compute_fhn_update(0, 0, V_in, W_in, V_out, W_out, Size_X-1, 1, Size_Y-1, 1);
        _compute_fhn_update(Size_X-1, 0, V_in, W_in, V_out, W_out, Size_X-2, 0, Size_Y-1, 1);
		_compute_fhn_update(0, Size_Y-1, V_in, W_in, V_out, W_out, Size_X-1, 1, Size_Y-2, 0);
        _compute_fhn_update(Size_X-1, Size_Y-1, V_in, W_in, V_out, W_out, Size_X-2, 0, Size_Y-2, 0);
    }
}

//------------------------------------------------------------------------------


//--- Private Helper Function to avoid code duplication ---
void FHN_2D_system_class::_compute_fhn_update(int i, int j,
	double V_in[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE], double W_in[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE],
	double V_out[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE], double W_out[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE],
	int i_prev, int i_next, int j_prev, int j_next)
{
	double s2 = (dx * dx);

	// Diffusion term (Laplacian)
	double diff_term = ( DX[i][j] * (V_in[i_prev][j] + V_in[i_next][j] - 2 * V_in[i][j]) +
						 DY[i][j] * (V_in[i][j_prev] + V_in[i][j_next] - 2 * V_in[i][j]) ) / s2;

	// --- Corrected RK2 (Heun's Method) Implementation ---
	// 1. Calculate slopes at the initial point (k1)
	double k1_V = (V_in[i][j] - std::pow(V_in[i][j], 3) / 3.0 - W_in[i][j]) / ni;
	double k1_W = ni * (V_in[i][j] + beta - gamma * W_in[i][j]);

	// 2. Calculate intermediate values e, r using a full Euler step
	double e = V_in[i][j] + k1_V * dt;
	double r = W_in[i][j] + k1_W * dt;

	// 3. Calculate slopes at the intermediate point (k2)
	double k2_V = (e - std::pow(e, 3) / 3.0 - r) / ni;
	double k2_W = ni * (e + beta - gamma * r);

	// 4. Update using the average of the slopes
	V_out[i][j] = V_in[i][j] + (dt / 2.0) * (k1_V + k2_V) + dt * diff_term;
	W_out[i][j] = W_in[i][j] + (dt / 2.0) * (k1_W + k2_W);
}


//---------------------------------------------------------------------------

void FHN_2D_system_class::create_spiral(int Slice_Plane)
{
    double wn,r,fi,phase,x,y,d;
    double cx,cy;
    long n;

    wn = 15.0; // winding number
	clear_system();
    cx = (Size_X-1) / 2.0;
    cy = (Size_Y-1) / 2.0;

    if( Slice_Plane == 0 ) // spiral wave in XY plane
    for( int i=0; i<Size_X; i++ )
    for( int j=0; j<Size_Y; j++ )
    {
        r = std::sqrt( std::pow(i-cx,2) + std::pow(j-cy,2)); // distance
        x = i-cx;
        y = j-cy;

        fi = atan2(y, x); // atan2 handles all quadrants correctly
        if (fi < 0) fi += 2.0 * M_PI; // map to [0, 2pi]

        fi += 0.55;

        n = floor( (r - wn*fi) / (2.0 * M_PI * wn) );
        phase = fabs(r - wn*fi - 2.0*wn*M_PI*n) / (2.0 * wn * M_PI);
        int m = (int)((1.0 - phase)*(FHN_APD_Length-1));
        if (m < 0) m = 0;
        if (m >= FHN_APD_Length) m = FHN_APD_Length - 1;

        V1[i][j] = FHN_Activation_Variable_APD[m];
        V2[i][j] = FHN_Activation_Variable_APD[m];
        W1[i][j] = FHN_Recovery_Variable_APD[m];
        W2[i][j] = FHN_Recovery_Variable_APD[m];
    }
}

//---------------------------------------------------------------------------

int FHN_2D_system_class::get_color_code(int X,int Y)
{
    return 255 * (V1[X][Y] - Min_Voltage) / (Max_Voltage - Min_Voltage);
}

//---------------------------------------------------------------------------

void FHN_2D_system_class::stimulate_node(int X,int Y)
{
    if (X >= 0 && X < Size_X && Y >= 0 && Y < Size_Y) {
        V1[X][Y] = 2.0;
        V2[X][Y] = 2.0;
    }
}

//---------------------------------------------------------------------------
void FHN_2D_system_class::record_potentials()
{
    int j = Size_Y/2;
    for( int i=0; i<Size_X; i++)
        Middle_Row_Voltage_History[i].DVector.push_back(V1[i][j]);
}

//---------------------------------------------------------------------------

void FHN_2D_system_class::compute_ISIs(long Node_Position)
{
    ISIs_Node_Ptr_X = Node_Position;
    ISIs.clear();
    double Threshold = 0.5;
    double prev_t=0, curr_t=0;

    if( Node_Position >= 0 && Node_Position < Size_X )
    {
        // Find all upstrokes
        for(long t=1; t < Middle_Row_Voltage_History[Node_Position].DVector.size(); t++)
        {
            if( Middle_Row_Voltage_History[Node_Position].DVector[t-1] < Threshold &&
                Middle_Row_Voltage_History[Node_Position].DVector[t] >= Threshold )
            {
                curr_t = t;
                if( prev_t != 0 )
                {
                    ISIs.push_back( (curr_t - prev_t) * dt ); // Store ISI in time units
                }
                prev_t = curr_t;
			}
        }
    }
}

//---------------------------------------------------------------------------

double FHN_2D_system_class::get_MI_parameter(int Which,int X, int Y)
{
    return 0; // Placeholder
}

//---------------------------------------------------------------------------

void FHN_2D_system_class::ablate_system_xy_point(int X,int Y,int Direction)
{
    if (X >= 0 && X < Size_X && Y >= 0 && Y < Size_Y) {
        if( Direction > 0 ) // Ablate
        {
            DX[X][Y]=0;
            DY[X][Y]=0;
        }
        else // Restore
        {
            DX[X][Y]=Max_Diff_X;
            DY[X][Y]=Max_Diff_Y;
        }
    }
}

//---------------------------------------------------------------------------

void FHN_2D_system_class::ablate_system(double Cx, double Cy, int Direction)
{
    int Radius = 4;
    int Pos_X = Cx * Size_X;
    int Pos_Y = Cy * Size_Y;
    double r;

	for(int x = Pos_X - Radius; x <= Pos_X + Radius; x++)
    for(int y = Pos_Y - Radius; y <= Pos_Y + Radius; y++)
    {
        if(x >= 0 && y >= 0 && x < Size_X && y < Size_Y)
        {
            r = sqrt( pow(x-Pos_X, 2) + pow(y-Pos_Y, 2) );
            if( r <= Radius )
            {
                // CORRECTED LOGIC: use loop variables x and y
                if( Direction > 0 ) // Ablate
                {
                    DX[x][y] = 0;
                    DY[x][y] = 0;
                }
                else // Restore
                {
                    DX[x][y] = Max_Diff_X;
                    DY[x][y] = Max_Diff_Y;
                }
            }
        }
    }
}

//------------------------------------------------------------------------------

void FHN_2D_system_class::set_initial_coupling()
{
    for( int i=0; i<Size_X; i++)
    for( int j=0; j<Size_Y; j++)
    {
        DX[i][j] = Max_Diff_X;
        DY[i][j] = Max_Diff_Y;
	}

	update_timestep();
}

//---------------------------------------------------------------------------

void FHN_2D_system_class::update_timestep()
{
    // Find maximum diffusion coefficient in the grid
    double Dmax = 0.0;
    for (int i = 0; i < Size_X; i++)
    for (int j = 0; j < Size_Y; j++)
    {
        if (DX[i][j] > Dmax) Dmax = DX[i][j];
        if (DY[i][j] > Dmax) Dmax = DY[i][j];
    }

    // Stability limit for 2D explicit scheme
    double dt_max = (dx * dx) / (4.0 * Dmax);

    // Safety factor (important in nonlinear systems)
    double safety = 0.9;
    dt = safety * dt_max;
}

//---------------------------------------------------------------------------

bool FHN_2D_system_class::activity_present_check()
{
    double Threshold = 0.5;
    for( int i=0; i<Size_X; i++)
    for( int j=0; j<Size_Y; j++)
    {
        if( V1[i][j] > Threshold )
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
