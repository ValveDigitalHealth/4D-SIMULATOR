//---------------------------------------------------------------------------
#include "Numerical_Library.h"

#ifndef TP1A_3D_systemH
#define TP1A_3D_systemH

#include "Program_Constants.h"

//---------------------------------------------------------------------------
/*
  PHYSICAL REVIEW E 68, 062902 ~2003!
  Influence of nonexcitable cells on spiral breakup in two-dimensional and three-dimensional
    excitable media
    K. H. W. J. ten Tusscher1 and A. V. Panfilov1,2
*/

class TP1A_3D_system_class
{
    public:

    TP1A_3D_system_class();
    ~TP1A_3D_system_class();

	int save_object(ofstream* dfile);
    int load_object(ifstream* dfile);

    // data structures
	int Size_X;
    int Size_Y;
	int Size_Z;

	double V1[THREE_DIM_SYSTEM_X_SIZE][THREE_DIM_SYSTEM_Y_SIZE][THREE_DIM_SYSTEM_Z_SIZE];
	double V2[THREE_DIM_SYSTEM_X_SIZE][THREE_DIM_SYSTEM_Y_SIZE][THREE_DIM_SYSTEM_Z_SIZE];

	double W1[THREE_DIM_SYSTEM_X_SIZE][THREE_DIM_SYSTEM_Y_SIZE][THREE_DIM_SYSTEM_Z_SIZE];
	double W2[THREE_DIM_SYSTEM_X_SIZE][THREE_DIM_SYSTEM_Y_SIZE][THREE_DIM_SYSTEM_Z_SIZE];

	double DX[THREE_DIM_SYSTEM_X_SIZE][THREE_DIM_SYSTEM_Y_SIZE][THREE_DIM_SYSTEM_Z_SIZE];
	double DY[THREE_DIM_SYSTEM_X_SIZE][THREE_DIM_SYSTEM_Y_SIZE][THREE_DIM_SYSTEM_Z_SIZE];
	double DZ[THREE_DIM_SYSTEM_X_SIZE][THREE_DIM_SYSTEM_Y_SIZE][THREE_DIM_SYSTEM_Z_SIZE];

	double MI1[THREE_DIM_SYSTEM_X_SIZE][THREE_DIM_SYSTEM_Y_SIZE][THREE_DIM_SYSTEM_Z_SIZE];
	double MI2[THREE_DIM_SYSTEM_X_SIZE][THREE_DIM_SYSTEM_Y_SIZE][THREE_DIM_SYSTEM_Z_SIZE];

	double CUSTOM_VALUE[THREE_DIM_SYSTEM_X_SIZE][THREE_DIM_SYSTEM_Y_SIZE][THREE_DIM_SYSTEM_Z_SIZE];
	double Min_Custom_Value,Max_Custom_Value;

	vector_of_doubles VOLTAGE_HISTORY[THREE_DIM_SYSTEM_X_SIZE][THREE_DIM_SYSTEM_Y_SIZE][THREE_DIM_SYSTEM_Z_SIZE];

	vector <vector_of_doubles> Middle_Row_Voltage_History;
	void record_potentials();
	void compute_ISIs(long Node_Position);
	vector<long>ISIs;
    long ISIs_Node_Ptr_X;

	void record_ECG();
	vector <double> ECG_signal;
	double ECG_electrode_x,ECG_electrode_y,ECG_electrode_z;

	double Min_Voltage,Max_Voltage;

	double Min_Diff,Max_Diff_X,Max_Diff_Y,Max_Diff_Z;
	void set_initial_coupling();

    int Boundary_Conditions;

    void clear_system();
	void refresh_VW_matrices(); // to avoid drop in efficiency

    void create_spiral(int Slice_Plane); // 0-x,1-y,2-z,3-t

    double* TP1A_Activation_Variable_APD;
    double* TP1A_Recovery_Variable_APD;
    int TP1A_APD_Length;

    // parameters
    double k_par,a,eps,mi1,mi2,b;

    double dx,dt;

	double Global_Time;

    void calculate_V1_from_V2_fhn();
	void calculate_V2_from_V1_fhn();

    int allocate_tables();
    int free_tables();

    void compute_N_steps(int N);

    int get_color_code(int Source, int X,int Y,int Z);

    void stimulate_node(int X,int Y,int Z);

	// ASSYMETRIC VOLTAGE MODULATION
	void add_assymetric_modulation();
	bool Add_Assymetric_Modulation;
	double Asm_Length;
	double Asm_Amplitude;
	double Asm_Ratio;

	// SINUSOIDAL CURRENT MODULATION
	void add_current_modulation_1();
	bool Add_Current_Modulation_1;
	double CM1_Length;
	double CM1_Amplitude;

	double Central_Region_Relative_Size;


	// coupling stuff
    double Diff_v1_Number;
	double Diff_v1_Radius;
	double Ring_v1_Width;
	double Ring_v1_PercD;
	double Lines_No_v1;
	double Line_Perc_Leng_v1;
	double Line_Perc_D;
	double Disk_Radius;

	void add_coupling(int Version,bool Add_Rings);

	void stimulate_system(double Cx, double Cy,double Cz);
	void ablate_system(double Cx, double Cy,double Cz);


	void set_mi_distribution(double mi1_left,double mi2_left,
							 double mi1_right,double mi2_right,
							 double Ratio);

	void set_mi_breakup_rings(double mi1_min,double mi1_max,
							 double Radius, long Number);

	void set_mi_breakup_landscape(double R, double p, double I,
				double mi1_min, double mi1_max);


	vector <double> calculate_unipolar_voltage_from_surface(
			int Node_x,long Node_y, long Node_z, long Range);

	void set_Z_D_gradient(double Min_D, double Max_D);

	bool activity_present_check();

	double get_voltage(int X, int Y, int Z);
	double get_current(int X, int Y, int Z);

};

#endif
