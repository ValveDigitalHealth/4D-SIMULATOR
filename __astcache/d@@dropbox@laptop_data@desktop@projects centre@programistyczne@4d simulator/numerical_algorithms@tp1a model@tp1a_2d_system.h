//---------------------------------------------------------------------------
#include "Numerical_Library.h"

#ifndef TP1A_2D_systemH
#define TP1A_2D_systemH

#include "Program_Constants.h"

#include "Electrogram_class.h"

#include "Two_D_system_CR_region_class.h"


//---------------------------------------------------------------------------

/*
  PHYSICAL REVIEW E 68, 062902 2003
  Influence of nonexcitable cells on spiral breakup in two-dimensional and three-dimensional
    excitable media
    K. H. W. J. ten Tusscher1 and A. V. Panfilov1,2
*/

class TP1A_2D_system_class
{
    public:

	TP1A_2D_system_class();
	~TP1A_2D_system_class();

	int save_object(ofstream* dfile);
	int load_object(ifstream* dfile);

	Numerical_Library Numerical_Library_Obj;

	void set_timestep(double Timestep);

	void export_voltage_history(AnsiString Filename,AnsiString Signal_Type);

	// data structures
	int Size_X;
	int Size_Y;

	double V1[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	double V2[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];

	double W1[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	double W2[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];

	double DX[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	double DY[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];

	double MI1[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	double MI2[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];

	double CUSTOM_VALUE[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	double CUSTOM_VALUE_2[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	double Min_Custom_Value,Max_Custom_Value;
	void compute_min_max_custom_value();
	double get_custom_value(double Cx, double Cy);

	double MPC[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];

	vector_of_floats VOLTAGE_HISTORY[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	vector<double> VOLTAGE_HISTORY_Time_Axis_ms;
	void get_voltage(int x_coord, int y_coord,long t_ptr,double *Voltage,double *Time);
	void clear_signals_history();

	// here is stored vector field for the simulation (dynamically calculated, no need to save it)
	vector_of_doubles PHASE[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	vector_of_longs ACTIVATION_TIMINGS[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	vector_of_longs ACTIVATION_TIMINGS_DOMAIN_ID[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	vector_of_floats VELOCITY_VECTORS_DIRECTION[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	vector_of_floats VELOCITY_VECTORS_AMPLITUDE[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_Y_SIZE];
	void calculate_velocity_field(double Cycle_Length_ms, double Signal_Sampling_Timestep, int Grid_Spacing);
	long get_closest_timing(int x,int y,long t);

	void record_potentials(int Type,int Grid_Spacing);
	double get_unipolar_voltage(int x_coord, int y_coord);
	void compute_ISIs_XY(int X, int Y);

	vector<long>Activations_Positions;
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

	void compute_N_steps(int N);

	int get_color_code(int Source, int X,int Y,long t, int Grid_Spacing,double Cycle_Length_ms);
	VelocityVector_2D get_conduction_arrow(int X,int Y,long Time_Ptr);

	void stimulate_node(int X,int Y);
	void stimulate_edge(int Which);

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

	void stimulate_system(double Cx, double Cy);
	void ablate_system(double Cx, double Cy,int Direction);
	void ablate_system_xy_point(int X,int Y,int Direction);

	void set_mi_distribution(double mi1_left,double mi2_left,
							 double mi1_right,double mi2_right,
							 double Ratio);

	void set_mi_breakup_rings(double mi1_min,double mi1_max,
							 double Radius, long Number,bool Middle);

	void set_mi_breakup_landscape(double R, double p, double I,
				double min, double max, int Target_Variable);

	void add_mi1_mi2_R_disk(double mi1,double mi2,double R, int Target_Variable);

	vector <double> calculate_unipolar_voltage_from_surface(int Node_x,long Node_y, long Range);

	double get_voltage(int X, int Y);
	double get_current(int X, int Y);

	// CRITICAL REGIONS SUPPORT
	bool activity_present_check();
	void add_non_conducting_disk(int X,int Y,int Z,int R);
	void add_critical_region(Two_D_system_CR_region_class CR);

	long get_history_vector_size();
	long get_voltage_history_vector_size(int X,int Y);

	//-----------------------------------------------------
	std::vector<Electrogram> Grid_Electrograms;
	//-----------------------------------------------------
	double get_correlation_length(long Start_Ptr,long Stop_Ptr, long BCL_Ptr,
		double* CL, double* MPC,std::vector<double> *MPCs,std::vector<double> *Distances);
	void clear_grid_egms();
	void add_grid_egm(Electrogram *E);

	void phase_vs_distance_data(long Time_Ptr, long BCL_Ptr,
		std::vector<double> *Angles,std::vector<double> *Distances, int Algorithm);

	// Correlation length support
	std::vector<double> Corrs;


	long get_domain_id(int X,int Y,long Time_Ptr);

	long Line_point_A_i,Line_point_A_j;
	long Line_point_B_i,Line_point_B_j;
	void mark_line_point(int i, int j);

	void recalculate_phase(double BCL_ptr,double Signal_Sampling_ms);
	double get_phase(int X,int Y,long Time_Ptr);

};

#endif
