
#ifndef LRTISSUE91_H
#define LRTISSUE91_H

#include "Numerical_Library.h"
#include "Program_Constants.h"
#include "Electrogram_class.h"

#include "LR_91_cell.h"

class LR_91_2D_system_class
{
	public:

	LR_91_2D_system_class();
	~LR_91_2D_system_class();

	void create_spiral();
	void load_phase_file();

	int Boundary_Conditions_Type; // 0-no flux, 1-periodic !!!!!!111 ADD TO SAVE

	//-------------------------------------------------------------------------------
	Luo_Rudy_91_cell LR_Tissue[TWO_DIM_SYSTEM_X_SIZE][TWO_DIM_SYSTEM_X_SIZE];
	//-------------------------------------------------------------------------------

	int save_object(ofstream* dfile);
	int load_object(ifstream* dfile);

	bool activity_present();

	// Dimensions of the grid
	int Size_X,Size_Y;
	double dx,dx2;

	int record_potentials();
	int compute_ISIs(long Node_Ptr_X, long Node_Ptr_Y);
    vector<long>ISIs;
	long ISIs_Node_Ptr_X,ISIs_Node_Ptr_Y;

	void calculate_one_step();
	void calculate_N_steps(int steps_number, int if_tip);

/////////////////////////////////////////
	double Tip_Timer;
	bool tip_searching;
	int** Tip_Position;
	long Tip_Counter;
	long Pos_Size;
	void find_tip();

/////////////////////////////////////////

};
#endif
