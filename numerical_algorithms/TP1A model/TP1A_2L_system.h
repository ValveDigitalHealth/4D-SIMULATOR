//---------------------------------------------------------------------------
#include "Numerical_Library.h"

#ifndef TP1A_2L_systemH
#define TP1A_2L_systemH

#include "Program_Constants.h"

//---------------------------------------------------------------------------
/*
  PHYSICAL REVIEW E 68, 062902 ~2003!
  Influence of nonexcitable cells on spiral breakup in two-dimensional and three-dimensional
	excitable media
	K. H. W. J. ten Tusscher1 and A. V. Panfilov1,2
*/

class TP1A_2L_system_CR_region_class
{
	public:
		vector <int> Layer; // 0 or 1
		vector <int> X,Y; // coordinates
		vector <int> R; // radius
		void clear();
};

//---------------------------------------------------------------------------

class TP1A_2L_system_class
{
	public:

    TP1A_2L_system_class();
	~TP1A_2L_system_class();

	int save_object(ofstream* dfile);
    int load_object(ifstream* dfile);

    // data structures
	int Size_X;
	int Size_Y;

	// 1st Layer
	double V1_A[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	double V2_A[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];

	double W1_A[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	double W2_A[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];

	double DX_A[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	double DY_A[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];

	double MI1_A[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	double MI2_A[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];

	// 2nd Layer
	double V1_B[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	double V2_B[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];

	double W1_B[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	double W2_B[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];

	double DX_B[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	double DY_B[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];

	double MI1_B[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	double MI2_B[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];

	// coupling between layers
	double COUPLING[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	double Coupling_Init;

	double CUSTOM_VALUE[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	double Min_Custom_Value,Max_Custom_Value;

	vector_of_doubles VOLTAGE_HISTORY_A[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	vector_of_doubles VOLTAGE_HISTORY_B[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];

	vector <vector_of_doubles> Middle_Row_Voltage_History;
	void record_potentials();
	void compute_ISIs(long Node_Position);
	vector<long>ISIs;
    long ISIs_Node_Ptr_X;

	double Min_Voltage,Max_Voltage;

	double Min_Diff,Max_Diff_X,Max_Diff_Y;
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

    int get_color_code(int Source, int X,int Y,int Z,long t);

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

	// inter-layer coupling
	double Min_Layer_Coupling;
	double Max_Layer_Coupling;
	double Layer_Coupling_Disks_Number;
	double Layer_Coupling_Disks_Radius;
	void update_inter_layer_coupling();

	double get_voltage(int X, int Y, int L);
	double get_current(int X, int Y, int L);


	void stimulate_system(double Cx, double Cy, double Cz);
	void ablate_system(double Cx, double Cy, double Cz);

	void set_mi_distribution(double mi1_left,double mi2_left,
							 double mi1_right,double mi2_right,
							 double Ratio);

	void set_mi_breakup_rings(double mi1_min,double mi1_max,
							 double Radius, long Number,bool Middle);

	void set_mi_breakup_landscape(double R, double p, double I,
				double min, double max, int Target_Variable);


	vector <double> calculate_unipolar_voltage_from_surface(int Node_x,long Node_y, long Range);

	// CRITICAL REGIONS SUPPORT
	bool activity_present_check();
	void add_non_conducting_disk(int X,int Y,int Z,int R);
	void add_critical_region(TP1A_2L_system_CR_region_class CR);

	void add_mi1_mi2_R_disk(double mi1,double mi2,double R, int Target_Variable);

	long get_history_vector_size();
};

#endif
