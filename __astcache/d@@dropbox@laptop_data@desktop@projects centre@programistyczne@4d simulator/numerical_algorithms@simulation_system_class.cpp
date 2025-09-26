//---------------------------------------------------------------------------

#pragma hdrstop

#include "Simulation_System_Class.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
Simulation_System_Class::Simulation_System_Class()
{
	Critical_Region_Search_Mode = false;

	Which_System_Dimension = 2; // TWO_LAYER_SYSTEM_DIMENSION;

	if( Which_System_Dimension == 2 )
	{
		Dimension_X = TWO_DIM_SYSTEM_X_SIZE;
		Dimension_Y = TWO_DIM_SYSTEM_Y_SIZE;
		Dimension_Z = 1;
		Dimension_T = 1;

		Two_Dim_System.Size_X = Dimension_X;
		Two_Dim_System.Size_Y = Dimension_Y;
		Two_Dim_System.allocate_tables();
	}

	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	{
		Dimension_X = TWO_DIM_SYSTEM_X_SIZE;
		Dimension_Y = TWO_DIM_SYSTEM_Y_SIZE;
		Dimension_Z = 2;
		Dimension_T = 1;

		Two_Dim_System.Size_X = Dimension_X;
		Two_Dim_System.Size_Y = Dimension_Y;
		Two_Dim_System.allocate_tables();
	}

	if( Which_System_Dimension == 3 )
	{
		Dimension_X = THREE_DIM_SYSTEM_X_SIZE;
		Dimension_Y = THREE_DIM_SYSTEM_Y_SIZE;
		Dimension_Z = THREE_DIM_SYSTEM_Z_SIZE;
		Dimension_T = 1;

		Three_Dim_System.Size_X = Dimension_X;
		Three_Dim_System.Size_Y = Dimension_Y;
		Three_Dim_System.Size_Z = Dimension_Z;
		Three_Dim_System.allocate_tables();
	}

	Boundary_Conditions = 0; // no flux

//    Four_Dim_System.free_tables();
//    Four_Dim_System.Size_X = Dimension_X;
//    Four_Dim_System.Size_Y = Dimension_Y;
//    Four_Dim_System.Size_Z = Dimension_Z;
//    Four_Dim_System.Size_T = Dimension_T;
//    Four_Dim_System.allocate_tables();

}

//---------------------------------------------------------------------------

Simulation_System_Class::~Simulation_System_Class()
{
    //
}

//---------------------------------------------------------------------------

double Simulation_System_Class::get_current_time()
{
    if( Which_System_Dimension == 2 )
    return Two_Dim_System.Global_Time;

	if( Which_System_Dimension == 3 )
	return Three_Dim_System.Global_Time;

	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	return Two_L_System.Global_Time;

//	if( Which_System_Dimension == 4 )
//    return Four_Dim_System.Global_Time;
}

//---------------------------------------------------------------------------

void Simulation_System_Class::compute_N_steps(int N)
{
    Two_Dim_System.Boundary_Conditions = Boundary_Conditions;
	// Three_Dim_System.Boundary_Conditions = Boundary_Conditions;
//    Four_Dim_System.Boundary_Conditions = Boundary_Conditions;

	if( Which_System_Dimension == 2 )
	Two_Dim_System.compute_N_steps(N);

	if( Which_System_Dimension == 3 )
	Three_Dim_System.compute_N_steps(N);

	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	Two_L_System.compute_N_steps(N);

//    if( Which_System_Dimension == 4 )
//    Four_Dim_System.compute_N_steps(N);

}

//---------------------------------------------------------------------------

double Simulation_System_Class::get_variable(int Var_No,int X,int Y,int Z,int T)
{
	if( Var_No == 0 )
	{
		if( Which_System_Dimension == 2 )
		return Two_Dim_System.get_voltage(X,Y);

		if( Which_System_Dimension == 3 )
		return Three_Dim_System.get_voltage(X,Y,Z);

		if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
		return Two_L_System.get_voltage(X,Y,Z);
	}

	if( Var_No == 1 )
	{
		if( Which_System_Dimension == 2 )
		return Two_Dim_System.get_current(X,Y);

		if( Which_System_Dimension == 3 )
		return Three_Dim_System.get_current(X,Y,Z);

		if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
		return Two_L_System.get_current(X,Y,Z);

//		if( Which_System_Dimension == 4 )
//		return Four_Dim_System.W1[X][Y][Z][T];
	}

}

//---------------------------------------------------------------------------
void Simulation_System_Class::stimulate_node(int X,int Y,int Z,int T)
{
	if( Which_System_Dimension == 2 )
		Two_Dim_System.stimulate_node(X,Y);

	if( Which_System_Dimension == 3 )
		Three_Dim_System.stimulate_node(X,Y,Z);

	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
		Two_L_System.stimulate_node(X,Y,Z);

//    if( Which_System_Dimension == 4 )
	//    Four_Dim_System.stimulate_node(X,Y,Z,T);
}
//---------------------------------------------------------------------------

void Simulation_System_Class::stimulate_edge(int Which)
{
	if( Which_System_Dimension == 2 )
		Two_Dim_System.stimulate_edge(Which);

}
//---------------------------------------------------------------------------

void Simulation_System_Class::clear_system()
{
	if( Which_System_Dimension == 2 )
	Two_Dim_System.clear_system();

	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	Two_L_System.clear_system();

   if( Which_System_Dimension == 3 )
   Three_Dim_System.clear_system();

//    if( Which_System_Dimension == 4 )
//    Four_Dim_System.clear_system();
}

//---------------------------------------------------------------------------

void Simulation_System_Class::refresh_VW_matrices()
{
	if( Which_System_Dimension == 2 )
	Two_Dim_System.refresh_VW_matrices();

	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	Two_L_System.refresh_VW_matrices();

	if( Which_System_Dimension == 3 )
	Three_Dim_System.refresh_VW_matrices();

//	if( Which_System_Dimension == 4 )
//    Four_Dim_System.clear_system();

}

//---------------------------------------------------------------------------

void Simulation_System_Class::create_spiral(int Slice_Plane) // 0-x,1-y,2-z,3-t
{
	if( Which_System_Dimension == 2 )
	Two_Dim_System.create_spiral(Slice_Plane);

	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	Two_L_System.create_spiral(Slice_Plane);

	if( Which_System_Dimension == 3 )
	Three_Dim_System.create_spiral(Slice_Plane);


/*

	if( Which_System_Dimension == 4 )
	Four_Dim_System.create_spiral(Slice_Plane);
*/
}

//---------------------------------------------------------------------------

int Simulation_System_Class::get_color_code(int Source, int X,int Y,int Z,int T, long Time, int Grid_Spacing,double Cycle_Length_ms)
{
	if( Which_System_Dimension == 2 )
	return Two_Dim_System.get_color_code(Source,X,Y,Time,Grid_Spacing,Cycle_Length_ms);

	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	return Two_L_System.get_color_code(Source,X,Y,Z,Time);

	if( Which_System_Dimension == 3 )
	return Three_Dim_System.get_color_code(Source,X,Y,Z);
/*

	if( Which_System_Dimension == 4 )
	return Four_Dim_System.get_color_code(X,Y,T,T);
*/
}

//---------------------------------------------------------------------------

VelocityVector_2D Simulation_System_Class::get_conduction_arrow(int X,int Y,int Z,long Time)
{
	if( Which_System_Dimension == 2 )
	return Two_Dim_System.get_conduction_arrow(X,Y,Time);
/*
	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	return Two_L_System.get_color_code(X,Y,Z,Time);

	if( Which_System_Dimension == 3 )
	return Three_Dim_System.get_color_code(X,Y,Z);
*/
}

//---------------------------------------------------------------------------

void Simulation_System_Class::record_ECG()
{
//	if( Which_System_Dimension == 2 )
//	Two_Dim_System.record_ECG();

	if( Which_System_Dimension == 3 )
	Three_Dim_System.record_ECG();
}

//---------------------------------------------------------------------------

void Simulation_System_Class::record_potentials(int Type,int Grid_Spacing)
{
	if( Which_System_Dimension == 2 )
	Two_Dim_System.record_potentials(Type,Grid_Spacing);

	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	Two_L_System.record_potentials();

	if( Which_System_Dimension == 3 )
	Three_Dim_System.record_potentials();
/*
	if( Which_System_Dimension == 4 )
	Four_Dim_System.record_potentials(Type);
*/
}

//---------------------------------------------------------------------------

vector <double> Simulation_System_Class::calculate_unipolar_voltage_from_surface(int Node_x,long Node_y, long Range)
{
	if( Which_System_Dimension == 2 )
	return Two_Dim_System.calculate_unipolar_voltage_from_surface(Node_x,Node_y,Range);

	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	return Two_L_System.calculate_unipolar_voltage_from_surface(Node_x,Node_y,Range);
}

//---------------------------------------------------------------------------
void Simulation_System_Class::export_voltage_history(AnsiString Filename,AnsiString Signal_Type)
{
	if( Which_System_Dimension == 2 )
	Two_Dim_System.export_voltage_history(Filename,Signal_Type);
/*
	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	Two_L_System.record_potentials();

	if( Which_System_Dimension == 3 )
	Three_Dim_System.record_potentials();
*/
}
//---------------------------------------------------------------------------

long Simulation_System_Class::get_voltage_history_size()
{
	if( Which_System_Dimension == 2 )
	return Two_Dim_System.get_history_vector_size();

	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	return Two_L_System.Middle_Row_Voltage_History[0].DVector.size();

	if( Which_System_Dimension == 3 )
	return Three_Dim_System.Middle_Row_Voltage_History[0].DVector.size();

/*
	if( Which_System_Dimension == 4 )
	return Four_Dim_System.Middle_Row_Voltage_History[0].DVector.size();
*/
}

//---------------------------------------------------------------------------
void Simulation_System_Class::clear_signals_history()
{
	if( Which_System_Dimension == 2 )
	Two_Dim_System.clear_signals_history();

	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	for(long k=0;k<Two_L_System.Middle_Row_Voltage_History.size();k++)
	Two_L_System.Middle_Row_Voltage_History[k].DVector.clear();

	if( Which_System_Dimension == 3 )
	for(long k=0;k<Three_Dim_System.Middle_Row_Voltage_History.size();k++)
	Three_Dim_System.Middle_Row_Voltage_History[k].DVector.clear();

	if( Which_System_Dimension == 3 )
	for( int i=0; i<Three_Dim_System.Size_X; i++)
	for( int j=0; j<Three_Dim_System.Size_Y; j++)
	for( int k=0; k<Three_Dim_System.Size_Z; k++)
		Three_Dim_System.VOLTAGE_HISTORY[i][j][k].DVector.clear();
/*
	if( Which_System_Dimension == 3 )
	for(long k=0;k<Two_Dim_System.Middle_Row_Voltage_History.size();k++)
	// Three_Dim_System.Middle_Row_Voltage_History[k].DVector.clear();

	if( Which_System_Dimension == 4 )
	for(long k=0;k<Two_Dim_System.Middle_Row_Voltage_History.size();k++)
//    Four_Dim_System.Middle_Row_Voltage_History[k].DVector.clear();
*/

}

//---------------------------------------------------------------------------

long Simulation_System_Class::get_voltage_history_vector_size(int X,int Y)
{
	long S = 0;

	if( Which_System_Dimension == 2 )
		S = Two_Dim_System.get_voltage_history_vector_size(X,Y);

	return S;
}

//---------------------------------------------------------------------------

void Simulation_System_Class::clear_ECG_history()
{
///	if( Which_System_Dimension == 2 )
//	Two_Dim_System.ECG_signal.clear();

	if( Which_System_Dimension == 3 )
	Three_Dim_System.ECG_signal.clear();
}

//---------------------------------------------------------------------------

void Simulation_System_Class::stimulate_system(double Cx, double Cy,double Cz)
{
	if( Which_System_Dimension == 2 )
	Two_Dim_System.stimulate_system(Cx,Cy);

	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	Two_L_System.stimulate_system(Cx,Cy,Cz);

	if( Which_System_Dimension == 3 )
	Three_Dim_System.stimulate_system(Cx,Cy,Cz);
/*

	if( Which_System_Dimension == 4 )
//    Four_Dim_System.stimulate_system(Cx,Cy);
*/
}

//---------------------------------------------------------------------------

void Simulation_System_Class::ablate_system_xy_point(int X,int Y,int Direction)
{
	if( Which_System_Dimension == 2 )
	Two_Dim_System.ablate_system_xy_point(X,Y,Direction);
/*

	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	Two_L_System.ablate_system_xy_point(X,Y);

	if( Which_System_Dimension == 3 )
	Three_Dim_System.ablate_system_xy_point(X,Y);

	if( Which_System_Dimension == 4 )
//    Four_Dim_System.stimulate_system(Cx,Cy);
*/
}

//---------------------------------------------------------------------------

void Simulation_System_Class::ablate_system(double Cx, double Cy, double Cz,int Direction)
{
	if( Which_System_Dimension == 2 )
	Two_Dim_System.ablate_system(Cx,Cy,Direction);

	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	Two_L_System.ablate_system(Cx,Cy,Cz);

	if( Which_System_Dimension == 3 )
	Three_Dim_System.ablate_system(Cx,Cy,Cz);
/*

	if( Which_System_Dimension == 4 )
//    Four_Dim_System.stimulate_system(Cx,Cy);
*/
}

//---------------------------------------------------------------------------

void Simulation_System_Class::compute_ISIs_XY(int X, int Y)
{
	if( Which_System_Dimension == 2 )
	Two_Dim_System.compute_ISIs_XY(X,Y);
/*
	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	Two_L_System.compute_ISIs_XY(Node_Position);

	if( Which_System_Dimension == 3 )
	Three_Dim_System.compute_ISIs_XY(Node_Position);
*/
/*
	if( Which_System_Dimension == 4 )
	Four_Dim_System.compute_ISIs(Node_Position);
*/
}

//---------------------------------------------------------------------------

long Simulation_System_Class::get_ISIs_history_size()
{
	if( Which_System_Dimension == 2 )
	return Two_Dim_System.ISIs.size();

	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	return Two_L_System.ISIs.size();

	if( Which_System_Dimension == 3 )
	return Three_Dim_System.ISIs.size();
/*

	if( Which_System_Dimension == 4 )
	return Four_Dim_System.ISIs.size();
*/
}

//---------------------------------------------------------------------------

double Simulation_System_Class::get_ISI(long Index)
{
	if( Which_System_Dimension == 2 )
	return Two_Dim_System.ISIs[Index];

	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	return Two_L_System.ISIs[Index];

	if( Which_System_Dimension == 3 )
	return Three_Dim_System.ISIs[Index];
/*

	if( Which_System_Dimension == 4 )
	return Four_Dim_System.ISIs[Index];
*/
}

//---------------------------------------------------------------------------

int Simulation_System_Class::save_object(ofstream* dfile)
{
	dfile[0] << " version_3 " << endl;
	dfile[0] << Boundary_Conditions << " ";
	dfile[0] << Signal_Sampling_Timestep << " ";
	dfile[0] << Which_System_Dimension << " ";

	dfile[0] << TP1A_2L_system_CR_regions.size() << " ";
	for(long i=0;i<TP1A_2L_system_CR_regions.size();i++)
	{
		dfile[0] << TP1A_2L_system_CR_regions[i].Layer.size() << " ";
		for(long j=0;j<TP1A_2L_system_CR_regions[i].Layer.size();j++)
		{
			dfile[0] << TP1A_2L_system_CR_regions[i].Layer[j] << " ";
			dfile[0] << TP1A_2L_system_CR_regions[i].X[j] << " ";
			dfile[0] << TP1A_2L_system_CR_regions[i].Y[j] << " ";
			dfile[0] << TP1A_2L_system_CR_regions[i].R[j] << " ";
		}
	}
	dfile[0] << TP1A_2D_system_CR_regions.size() << " ";
	for(long i=0;i<TP1A_2D_system_CR_regions.size();i++)
	{
		dfile[0] << TP1A_2D_system_CR_regions[i].X.size() << " ";
		for(long j=0;j<TP1A_2D_system_CR_regions[i].X.size();j++)
		{
			dfile[0] << TP1A_2D_system_CR_regions[i].X[j] << " ";
			dfile[0] << TP1A_2D_system_CR_regions[i].Y[j] << " ";
			dfile[0] << TP1A_2D_system_CR_regions[i].R[j] << " ";
		}
	}
   dfile[0] << CR_Tests_Number << " ";
   dfile[0] << CR_Level << " ";
   dfile[0] << CR_Number << " ";
   dfile[0] << CR_Regions_Overlap << " ";
   dfile[0] << Diff_Sizes_CR_Search << " ";
   dfile[0] << CR_Test_Time << " ";
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if( Which_System_Dimension == 2 )
	Two_Dim_System.save_object(dfile);
	if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	Two_L_System.save_object(dfile);
	if( Which_System_Dimension == 3 )
	Three_Dim_System.save_object(dfile);

//  Four_Dim_System.save_object(dfile);
}
//-------------------------------------------------------------------

int Simulation_System_Class::load_object(ifstream* dfile)
{
	char string[2000];
	int tmpint=0;
	long Size;
	double tmp;
	dfile[0] >> string;
	if( !strcmp(string,"version_1") )
	{
		dfile[0] >> Boundary_Conditions;
		dfile[0] >> Signal_Sampling_Timestep;
		dfile[0] >> Which_System_Dimension;

		if( Which_System_Dimension == 2 )
		Two_Dim_System.load_object(dfile);
		if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
		Two_L_System.load_object(dfile);
		if( Which_System_Dimension == 3 )
		Three_Dim_System.load_object(dfile);
	//    Four_Dim_System.load_object(dfile);
		Dimension_X = Two_Dim_System.Size_X;
		Dimension_Y = Two_Dim_System.Size_Y;
/*
		Dimension_X = Four_Dim_System.Size_X;
		Dimension_Y = Four_Dim_System.Size_Y;
		Dimension_Z = Four_Dim_System.Size_Z;
		Dimension_T = Four_Dim_System.Size_T;
*/
	} // version_1

	//************************************************
	if( !strcmp(string,"version_2") )
	{
		dfile[0] >> Boundary_Conditions;
		dfile[0] >> Signal_Sampling_Timestep;
		dfile[0] >> Which_System_Dimension;

	if( Critical_Region_Search_Mode == false )
	{
	dfile[0] >> Size;
	TP1A_2L_system_CR_region_class Rr;
	TP1A_2L_system_CR_regions.clear();
	TP1A_2L_system_CR_regions.assign(Size,Rr);
	for(long i=0;i<TP1A_2L_system_CR_regions.size();i++)
	{
		dfile[0] >> Size;
		TP1A_2L_system_CR_regions[i].Layer.assign(Size,tmpint);
		TP1A_2L_system_CR_regions[i].X.assign(Size,tmpint);
		TP1A_2L_system_CR_regions[i].Y.assign(Size,tmpint);
		TP1A_2L_system_CR_regions[i].R.assign(Size,tmpint);
		for(long j=0;j<TP1A_2L_system_CR_regions[i].Layer.size();j++)
		{
			dfile[0] >> TP1A_2L_system_CR_regions[i].Layer[j];
			dfile[0] >> TP1A_2L_system_CR_regions[i].X[j];
			dfile[0] >> TP1A_2L_system_CR_regions[i].Y[j];
			dfile[0] >> TP1A_2L_system_CR_regions[i].R[j];
		}
	}
	dfile[0] >> Size;
	Two_D_system_CR_region_class Rrd;
	TP1A_2D_system_CR_regions.clear();
	TP1A_2D_system_CR_regions.assign(Size,Rrd);
	for(long i=0;i<TP1A_2D_system_CR_regions.size();i++)
	{
		dfile[0] >> Size;
		TP1A_2D_system_CR_regions[i].X.assign(Size,tmpint);
		TP1A_2D_system_CR_regions[i].Y.assign(Size,tmpint);
		TP1A_2D_system_CR_regions[i].R.assign(Size,tmpint);
		for(long j=0;j<TP1A_2D_system_CR_regions[i].X.size();j++)
		{
			dfile[0] >> TP1A_2D_system_CR_regions[i].X[j];
			dfile[0] >> TP1A_2D_system_CR_regions[i].Y[j];
			dfile[0] >> TP1A_2D_system_CR_regions[i].R[j];
		}
	}
	}
	else // fake reading CRs
	{
	long tmp,tmp2;
	dfile[0] >> Size;
	for(long i=0;i<Size;i++)
	{
		dfile[0] >> tmp;
		for(long j=0;j<tmp;j++)
		{
			dfile[0] >> tmp2;
			dfile[0] >> tmp2;
			dfile[0] >> tmp2;
			dfile[0] >> tmp2;
		}
	}
	dfile[0] >> Size;
	for(long i=0;i<Size;i++)
	{
		dfile[0] >> tmp;
		for(long j=0;j<tmp;j++)
		{
			dfile[0] >> tmp2;
			dfile[0] >> tmp2;
			dfile[0] >> tmp2;
		}
	}
	}
	if( !Critical_Region_Search_Mode )
	{
		dfile[0] >> CR_Level;
		dfile[0] >> CR_Number;
		dfile[0] >> CR_Regions_Overlap;
		dfile[0] >> Diff_Sizes_CR_Search;
		dfile[0] >> CR_Test_Time;
	}
	else
	{
		dfile[0] >> tmp;
		dfile[0] >> tmp;
		dfile[0] >> tmp;
		dfile[0] >> tmp;
		dfile[0] >> tmp;
	}
		if( Which_System_Dimension == 2 )
		Two_Dim_System.load_object(dfile);
		if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
		Two_L_System.load_object(dfile);
		if( Which_System_Dimension == 3 )
		Three_Dim_System.load_object(dfile);
		Dimension_X = Two_Dim_System.Size_X;
		Dimension_Y = Two_Dim_System.Size_Y;
	} // version_2
	//************************************************
	if( !strcmp(string,"version_3") )
	{
		dfile[0] >> Boundary_Conditions;
		dfile[0] >> Signal_Sampling_Timestep;
		dfile[0] >> Which_System_Dimension;

	if( Critical_Region_Search_Mode == false )
	{
	dfile[0] >> Size;
	TP1A_2L_system_CR_region_class Rr;
	TP1A_2L_system_CR_regions.clear();
	TP1A_2L_system_CR_regions.assign(Size,Rr);
	for(long i=0;i<TP1A_2L_system_CR_regions.size();i++)
	{
		dfile[0] >> Size;
		TP1A_2L_system_CR_regions[i].Layer.assign(Size,tmpint);
		TP1A_2L_system_CR_regions[i].X.assign(Size,tmpint);
		TP1A_2L_system_CR_regions[i].Y.assign(Size,tmpint);
		TP1A_2L_system_CR_regions[i].R.assign(Size,tmpint);
		for(long j=0;j<TP1A_2L_system_CR_regions[i].Layer.size();j++)
		{
			dfile[0] >> TP1A_2L_system_CR_regions[i].Layer[j];
			dfile[0] >> TP1A_2L_system_CR_regions[i].X[j];
			dfile[0] >> TP1A_2L_system_CR_regions[i].Y[j];
			dfile[0] >> TP1A_2L_system_CR_regions[i].R[j];
		}
	}
	dfile[0] >> Size;
	Two_D_system_CR_region_class Rrd;
	TP1A_2D_system_CR_regions.clear();
	TP1A_2D_system_CR_regions.assign(Size,Rrd);
	for(long i=0;i<TP1A_2D_system_CR_regions.size();i++)
	{
		dfile[0] >> Size;
		TP1A_2D_system_CR_regions[i].X.assign(Size,tmpint);
		TP1A_2D_system_CR_regions[i].Y.assign(Size,tmpint);
		TP1A_2D_system_CR_regions[i].R.assign(Size,tmpint);
		for(long j=0;j<TP1A_2D_system_CR_regions[i].X.size();j++)
		{
			dfile[0] >> TP1A_2D_system_CR_regions[i].X[j];
			dfile[0] >> TP1A_2D_system_CR_regions[i].Y[j];
			dfile[0] >> TP1A_2D_system_CR_regions[i].R[j];
		}
	}
	}
	else // fake reading CRs
	{
	long tmp,tmp2;
	dfile[0] >> Size;
	for(long i=0;i<Size;i++)
	{
		dfile[0] >> tmp;
		for(long j=0;j<tmp;j++)
		{
			dfile[0] >> tmp2;
			dfile[0] >> tmp2;
			dfile[0] >> tmp2;
			dfile[0] >> tmp2;
		}
	}
	dfile[0] >> Size;
	for(long i=0;i<Size;i++)
	{
		dfile[0] >> tmp;
		for(long j=0;j<tmp;j++)
		{
			dfile[0] >> tmp2;
			dfile[0] >> tmp2;
			dfile[0] >> tmp2;
		}
	}
	}
	if( !Critical_Region_Search_Mode )
	{
		dfile[0] >> CR_Tests_Number;
		dfile[0] >> CR_Level;
		dfile[0] >> CR_Number;
		dfile[0] >> CR_Regions_Overlap;
		dfile[0] >> Diff_Sizes_CR_Search;
		dfile[0] >> CR_Test_Time;
	}
	else
	{
		dfile[0] >> tmp;
		dfile[0] >> tmp;
		dfile[0] >> tmp;
		dfile[0] >> tmp;
		dfile[0] >> tmp;
	}
		if( Which_System_Dimension == 2 )
		Two_Dim_System.load_object(dfile);
		if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
		Two_L_System.load_object(dfile);
		if( Which_System_Dimension == 3 )
		Three_Dim_System.load_object(dfile);
		Dimension_X = Two_Dim_System.Size_X;
		Dimension_Y = Two_Dim_System.Size_Y;
	} // version_3
}
//-------------------------------------------------------------------

void Simulation_System_Class::set_initial_coupling()
{
		if( Which_System_Dimension == 2 )
		Two_Dim_System.set_initial_coupling();
		if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
		Two_L_System.set_initial_coupling();
		if( Which_System_Dimension == 3 )
		Three_Dim_System.set_initial_coupling();
}
//-------------------------------------------------------------------
bool Simulation_System_Class::activity_present_check()
{
		if( Which_System_Dimension == 2 )
		return Two_Dim_System.activity_present_check();
		if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
		return Two_L_System.activity_present_check();
		if( Which_System_Dimension == 3 )
		return Three_Dim_System.activity_present_check();
	return false;
}
//-------------------------------------------------------------------
void Simulation_System_Class::add_non_conducting_disk(int X,int Y,int Z,int R)
{
		if( Which_System_Dimension == 2 )
		return Two_Dim_System.add_non_conducting_disk(X,Y,Z,R);
		if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
		return Two_L_System.add_non_conducting_disk(X,Y,Z,R);
//		if( Which_System_Dimension == 3 )
//		return Three_Dim_System.add_non_conducting_disk(int Layer,int X,int Y,int R)
}
//-------------------------------------------------------------------
void Simulation_System_Class::compute_min_max_custom_value()
{
		if( Which_System_Dimension == 2 )
			Two_Dim_System.compute_min_max_custom_value();
}
//-------------------------------------------------------------------
double Simulation_System_Class::get_custom_value(double Cx, double Cy)
{
		if( Which_System_Dimension == 2 )
			return Two_Dim_System.get_custom_value(Cx,Cy);
		return 0;
}
//-------------------------------------------------------------------
void Simulation_System_Class::add_mi1_mi2_R_disk(double mi1,double mi2,double R, int Target_Variable)
{
		if( Which_System_Dimension == 2 )
		return Two_Dim_System.add_mi1_mi2_R_disk(mi1,mi2,R,Target_Variable);
		if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
		return Two_L_System.add_mi1_mi2_R_disk(mi1,mi2,R,Target_Variable);
}
//-------------------------------------------------------------------
long Simulation_System_Class::get_history_vector_size()
{
		if( Which_System_Dimension == 2 )
		return Two_Dim_System.get_history_vector_size();
		if( Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
		return Two_L_System.get_history_vector_size();
}
//-------------------------------------------------------------------

void Simulation_System_Class::get_voltage(int x_coord, int y_coord,long t_ptr,double *Voltage,double *Time)
{
	if( Which_System_Dimension == 2 )
		Two_Dim_System.get_voltage(x_coord, y_coord,t_ptr,Voltage,Time);
}

//-------------------------------------------------------------------

void Simulation_System_Class::clear_grid_egms()
{
	if( Which_System_Dimension == 2 )
		Two_Dim_System.clear_grid_egms();
}

//-------------------------------------------------------------------

void Simulation_System_Class::add_grid_egm(Electrogram *E)
{
	if( Which_System_Dimension == 2 )
		Two_Dim_System.add_grid_egm(E);
}

//-------------------------------------------------------------------

void Simulation_System_Class::get_correlation_length(long Start_Ptr,long Stop_Ptr,
	long BCL_Ptr, double* CL, double* MPC,std::vector<double> *MPCs,std::vector<double> *Distances)
{
	if( Which_System_Dimension == 2 )
		Two_Dim_System.get_correlation_length(Start_Ptr,Stop_Ptr,BCL_Ptr,CL,MPC,MPCs,Distances);

}

//-------------------------------------------------------------------

void Simulation_System_Class::phase_vs_distance_data(long Time_Ptr, long BCL_Ptr,
		std::vector<double> *Angles,std::vector<double> *Distances, int Algorithm)
{
	if( Which_System_Dimension == 2 )
		Two_Dim_System.phase_vs_distance_data(Time_Ptr,BCL_Ptr,Angles,Distances,Algorithm);
}

//-------------------------------------------------------------------

void Simulation_System_Class::calculate_velocity_field(double Cycle_Length_ms, int Grid_Spacing)
{
	if( Which_System_Dimension == 2 )
		Two_Dim_System.calculate_velocity_field(Cycle_Length_ms,Signal_Sampling_Timestep,Grid_Spacing);
}

//-------------------------------------------------------------------

domain_structure Simulation_System_Class::perform_domain_walk(int ci,int cj,long ct,
	double Direction_Tolerance_Degrees, int Grid_Spacing)
{
	bool sth_done = true;
	domain_structure Domain;
	int i_c,j_c,t_c;
	double Direction_Tolerance_Rad = Direction_Tolerance_Degrees/360.0*(2*M_PI);

	// find max id
	long Domain_Id = get_max_activation_time_id() + 1;
	if( Domain_Id == 0 ) Domain_Id = 1; // to start from 1

	// mark seed Activation as visited
	Two_Dim_System.ACTIVATION_TIMINGS_DOMAIN_ID[ci][cj].DVector[ct] = Domain_Id;

	// push seed to Domain
	Domain.i_coordinates.push_back(ci);
	Domain.j_coordinates.push_back(cj);
	Domain.t_coordinates.push_back(ct);
	Domain.Domain_Id = Domain_Id;

	while(sth_done)
	{

	sth_done = false;

	for(long k=0;k<Domain.i_coordinates.size();k++)
	{

	i_c = Domain.i_coordinates[k];
	j_c = Domain.j_coordinates[k];
	t_c = Domain.t_coordinates[k];

	// 1. check surrounding nodes for not visited nodes
	for(int x=-1;x<=1;x++)
	for(int y=-1;y<=1;y++)
	if( !(x==0 && y==0) ) // except center
	if( i_c+x*Grid_Spacing >= 0 && i_c+x*Grid_Spacing < TWO_DIM_SYSTEM_X_SIZE )
	if( j_c+y*Grid_Spacing >= 0 && j_c+y*Grid_Spacing < TWO_DIM_SYSTEM_Y_SIZE )
	if( Two_Dim_System.ACTIVATION_TIMINGS_DOMAIN_ID[i_c + x*Grid_Spacing][j_c + y*Grid_Spacing].DVector[t_c] == NOT_VISITED_FLAG )
	if( Numerical_Library_Obj.directions_similarity_test(
		Two_Dim_System.VELOCITY_VECTORS_DIRECTION[i_c + x*Grid_Spacing][j_c + y*Grid_Spacing].DVector[t_c],
		Two_Dim_System.VELOCITY_VECTORS_DIRECTION[i_c                 ][j_c                 ].DVector[t_c], Direction_Tolerance_Rad) )
	{
		// assign id
		Two_Dim_System.ACTIVATION_TIMINGS_DOMAIN_ID[i_c + x*Grid_Spacing][j_c + y*Grid_Spacing].DVector[t_c] = Domain_Id;

		// add to domain
		Domain.i_coordinates.push_back(i_c + x*Grid_Spacing);
		Domain.j_coordinates.push_back(j_c + y*Grid_Spacing);
		Domain.t_coordinates.push_back(t_c);

		sth_done = true;
	}

	// 2. check back and forth in time for not visited nodes
	int time_shift;
	for(int m=1;m<=2;m++)
	{
		if( m== 1)
		time_shift = -1;
		if( m== 2)
		time_shift = +1;

	if( t_c + time_shift >= 0 && t_c + time_shift < Two_Dim_System.VELOCITY_VECTORS_DIRECTION[i_c][j_c].DVector.size() )
	if( Two_Dim_System.ACTIVATION_TIMINGS_DOMAIN_ID[i_c][j_c].DVector[t_c + time_shift ] == NOT_VISITED_FLAG )
	if( Numerical_Library_Obj.directions_similarity_test(
		Two_Dim_System.VELOCITY_VECTORS_DIRECTION[i_c][j_c].DVector[t_c + time_shift ],
		Two_Dim_System.VELOCITY_VECTORS_DIRECTION[i_c][j_c].DVector[t_c], Direction_Tolerance_Rad ) )
	{
		// assign id
		Two_Dim_System.ACTIVATION_TIMINGS_DOMAIN_ID[i_c][j_c].DVector[t_c + time_shift ] = Domain_Id;

		// add to domain
		Domain.i_coordinates.push_back(i_c);
		Domain.j_coordinates.push_back(j_c);
		Domain.t_coordinates.push_back(t_c + time_shift);

		sth_done = true;
	}

	} // back and forth in time

	} // goingh through all existing elements in domain

	} // while sth done

	return Domain;
}

//-------------------------------------------------------------------

void Simulation_System_Class::calculate_laminar_events(double Direction_Tolerance_Degrees, int Grid_Spacing)
{
	bool sth_done = true;
	domain_structure Domain;
	Domains.clear();

	// assign IDs to have same length as activations
	for(int i=0;i<TWO_DIM_SYSTEM_X_SIZE;i++)
	for(int j=0;j<TWO_DIM_SYSTEM_Y_SIZE;j++)
	Two_Dim_System.ACTIVATION_TIMINGS_DOMAIN_ID[i][j].DVector = Two_Dim_System.ACTIVATION_TIMINGS[i][j].DVector;

	// clear flags
	for(int i=0;i<TWO_DIM_SYSTEM_X_SIZE;i++)
	for(int j=0;j<TWO_DIM_SYSTEM_Y_SIZE;j++)
	for(int t=0;t<Two_Dim_System.ACTIVATION_TIMINGS_DOMAIN_ID[i][j].DVector.size();t++)
	Two_Dim_System.ACTIVATION_TIMINGS_DOMAIN_ID[i][j].DVector[t] = NOT_VISITED_FLAG;

	while(sth_done)
	{
		sth_done = false;

		for(int i=0;i<TWO_DIM_SYSTEM_X_SIZE;i++)
		for(int j=0;j<TWO_DIM_SYSTEM_Y_SIZE;j++)
		for(int t=0;t<Two_Dim_System.ACTIVATION_TIMINGS_DOMAIN_ID[i][j].DVector.size();t++)
		if( Two_Dim_System.ACTIVATION_TIMINGS_DOMAIN_ID[i][j].DVector[t] == NOT_VISITED_FLAG )
		{
			Domain = perform_domain_walk(i,j,t,Direction_Tolerance_Degrees,Grid_Spacing);
			Domains.push_back(Domain);
			sth_done = true;
		}
	}

	// delete small domains???

	std::vector<RGBColor> distinctColors = Numerical_Library_Obj.generateDistinctColors(Domains.size());

	// calculate stats
	for(int d=0;d<Domains.size();d++)
	{

	// derived parameters
	Domains[d].xyt_Volume = Domains[d].i_coordinates.size();
	Domains[d].RGB_Color = distinctColors[d];

	/*
	Conescutive_Areas.clear();
	dobule area;
	for(long t=0;t<t_coordinates.size();t++)
	{
		area = 0;
		for(long t=0;t<t_coordinates.size();t++)

		Conescutive_Areas.push_back(area);
	}
	*/
	}

}

//-------------------------------------------------------------------

long Simulation_System_Class::get_max_activation_time_id()
{
	long max_id=-1;

	for(int i=0;i<TWO_DIM_SYSTEM_X_SIZE;i++)
	for(int j=0;j<TWO_DIM_SYSTEM_Y_SIZE;j++)
	for(int t=0;t<Two_Dim_System.ACTIVATION_TIMINGS_DOMAIN_ID[i][j].DVector.size();t++)
	if( Two_Dim_System.ACTIVATION_TIMINGS_DOMAIN_ID[i][j].DVector[t] > max_id )
	max_id = Two_Dim_System.ACTIVATION_TIMINGS_DOMAIN_ID[i][j].DVector[t];

	return max_id;
}

//-------------------------------------------------------------------

long Simulation_System_Class::get_domain_id(int X,int Y,int Z,long Time_Ptr)
{
	if( Which_System_Dimension == 2 )
	return Two_Dim_System.get_domain_id(X,Y,Time_Ptr);

}

//-------------------------------------------------------------------

long Simulation_System_Class::get_domain_ptr(int X,int Y,int Z,long Time_Ptr)
{
	long domain_ptr = -1;

	if( Which_System_Dimension == 2 )
	{

	long domain_id = Two_Dim_System.get_domain_id(X,Y,Time_Ptr);

	for(long d=0;d<Domains.size();d++)
	if( Domains[d].Domain_Id == domain_id )
	domain_ptr = d;

	}

	return domain_ptr;
}

//-------------------------------------------------------------------

void Simulation_System_Class::set_timestep(double Timestep)
{
	if( Which_System_Dimension == 2 )
	Two_Dim_System.set_timestep(Timestep);

}

//-------------------------------------------------------------------

void Simulation_System_Class::mark_line_point(int i, int j)
{
	if( Which_System_Dimension == 2 )
	Two_Dim_System.mark_line_point(i,j);
}

//---------------------------------------------------------------------------

void Simulation_System_Class::recalculate_phase(double BCL_ptr,double Signal_Sampling_ms)
{
	if( Which_System_Dimension == 2 )
	Two_Dim_System.recalculate_phase(BCL_ptr,Signal_Sampling_ms);
}

//---------------------------------------------------------------------------

double Simulation_System_Class::get_phase(int X,int Y,long Time_Ptr)
{
	if( Which_System_Dimension == 2 )
	return Two_Dim_System.get_phase(X,Y,Time_Ptr);
}

//---------------------------------------------------------------------------


