//---------------------------------------------------------------------------
#include "Main_Screen.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma hdrstop

#pragma resource "*.dfm"
TMain_Screen_Form *Main_Screen_Form;

using namespace std;
using namespace stdcomb;

//---------------------------------------------------------------------------
__fastcall TMain_Screen_Form::TMain_Screen_Form(TComponent* Owner)
	: TForm(Owner)
{
//	OpenGL_Panel_1.Simulation_System = Simulation_System;

	// Find application dierctory and store it
	Application_Directory = ExtractFilePath(ParamStr(0));

	Current_Z_Slice=0;
    Current_T_Slice=0;

    Z_Slice_TrackBar->Max = Simulation_System.Dimension_Z-1;
    T_Slice_TrackBar->Max = Simulation_System.Dimension_T-1;

    Current_Z_Slice = Simulation_System.Dimension_Z / 2;
	Z_Slice_TrackBar->Position = Current_Z_Slice;

    Current_T_Slice = Simulation_System.Dimension_T / 2;
    T_Slice_TrackBar->Position = Current_T_Slice;

	Signal_Display_ScrollBar->Max = Simulation_System.Dimension_X;

	randomize();

	Echo = true;

	save_interface_parameters_to_sim_system();
}
//---------------------------------------------------------------------------
void __fastcall TMain_Screen_Form::Exit1Click(TObject *Sender)
{
    Simulation_Flag = false;
    Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::FormResize(TObject *Sender)
{
	Panel_1->Left = 1;
	Panel_1->Top = 1;
	Panel_1->Width = Main_Screen_Form->Width / 2. - 20;
	Panel_1->Height = Main_Screen_Form->Height - 100;
	OpenGL_Panel_1.opengl_panel_resized();

	Main_PageControl->Left = Panel_1->Left+Panel_1->Width+5;
	Main_PageControl->Top = 1;
	Main_PageControl->Width = Main_Screen_Form->Width-Panel_1->Width-10;
	Main_PageControl->Height = Panel_1->Height;

	Single_Signal_Chart->Width = Main_PageControl->Width - 30;
	ISIs_Chart->Width = Single_Signal_Chart->Width;
	Signal_Display_ScrollBar->Width = Single_Signal_Chart->Width;
	ISIs_Parameters_Chart->Width = Single_Signal_Chart->Width;
	ECG_Chart->Width = Main_PageControl->Width - 30;

	Signals_Tab_Chart->Width = Main_PageControl->Width - 30;
	Signals_Tab_Chart->Height = 0.6*(Main_PageControl->Height - 200);

	MPC_Dist_Chart->Top = Signals_Tab_Chart->Height + Signals_Tab_Chart->Top + 10;
	MPC_Dist_Chart->Left = 3;
	MPC_Dist_Chart->Width = 0.4*(Main_PageControl->Height - 200);
	MPC_Dist_Chart->Height = 0.4*(Main_PageControl->Height - 200);

	CL_Chart->Top = MPC_Dist_Chart->Top;
	CL_Chart->Left = MPC_Dist_Chart->Left + MPC_Dist_Chart->Width + 5;
	CL_Chart->Width = Main_PageControl->Width - MPC_Dist_Chart->Width - 10;
	CL_Chart->Height = 0.2*(Main_PageControl->Height - 200);

	Progress_Form->Left = Main_Screen_Form->Width/2 - Progress_Form->Width/2;
	Progress_Form->Top = Main_Screen_Form->Height/2 - Progress_Form->Height/2;
}

//---------------------------------------------------------------------------
	//-----------------------------------------------------------
	// Function returns ON if given (int tag) element of Map_Panel_PopupMenu
    // is checked. Used to check which action type is taken when
    // dragging occurs on OpenGL panel
	// Tag types (as in atms_program_parameters.h)
    // MAP_POPUP_ITEM_SELECT_MODE
    // MAP_POPUP_ITEM_ROTATE
    // MAP_POPUP_ITEM_ZOOM
	// MAP_POPUP_ITEM_PAN
	// MAP_POPUP_ITEM_ZOOM_CLIPPING_PLANE
    // MAP_POPUP_ITEM_ENABLE_CLIPPING_PLANE
	//-----------------------------------------------------------

int __fastcall TMain_Screen_Form::is_Map_Panel_PopupMenu_item_tag_checked(int tag)
{
	for(int i=0;i<EP_Mapping_3D_Window_PopupMenu->Items[0].Count;i++)
	if(EP_Mapping_3D_Window_PopupMenu->Items[0].Items[i]->Tag == tag)
	if(EP_Mapping_3D_Window_PopupMenu->Items[0].Items[i]->Checked == cbChecked)
	return ON;

	return OFF;
}

//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::repaint_all_controls()
{
//
}

//---------------------------------------------------------------------------

	//-----------------------------------------------------------
	// Function controlling repainting of the OpenGL panel. Called by
	// paint event of OpenGL panel
	// Calls paint_map() function which actually paints something.
	//-----------------------------------------------------------

void __fastcall TMain_Screen_Form::opengl_panel_paint()
{
	map_display_panels_paint(Display_Type_RadioGroup->ItemIndex);
}

//--------------------------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Start_Sim_ButtonClick(TObject *Sender)
{
	clock_t clock_start, clock_end;
	double sample_time_start,sample_time_end,Eff;
	double prev_time_s,prev_time_ecg;

	Simulation_Flag = true;

	Display_Value_RadioGroup->ItemIndex = 0;

	Simulation_System.set_timestep(Timestep_Edit->Text.ToDouble());

	prev_time_s = Simulation_System.get_current_time();
	prev_time_ecg = Simulation_System.get_current_time();

	Simulation_System.Signal_Sampling_Timestep = Sampling_Edit->Text.ToDouble();

	// configure modulation
	// Add_Assymetric_Modulation_CheckBoxClick(this);

	while( Simulation_Flag )
	{
		Simulation_System.Boundary_Conditions = Boundary_Conditions_RadioGroup->ItemIndex;

		if(Fixed_Simulation_Time_Flag)
		if( Simulation_System.get_current_time() > Max_Time )
		Simulation_Flag = false;

		Application->ProcessMessages();

		clock_start = clock();
		sample_time_start = Simulation_System.get_current_time();

		Simulation_System.compute_N_steps(20);
		Simulation_System.refresh_VW_matrices();

		double Pause = Pause_Edit->Text.ToDouble();

		if( Display_Sim_CheckBox->State )
		opengl_panel_paint();

		clock_end = clock();
		sample_time_end = Simulation_System.get_current_time();

		// calculate efficiency
		if(clock_end-clock_start!=0)
		Eff = (sample_time_end-sample_time_start)*
				CLK_TCK/(clock_end-clock_start);
		else
		Eff = -1;

		Time_Label->Caption = "Time: "+IntToStr((int)sample_time_end)+
				"   Efficiency: "+FloatToStrF(Eff,ffGeneral,3,3);

		if( Plot_Variables_CheckBox->Checked )
		{

		   if( Plot_Source_RadioGroup->ItemIndex == 0 )
		   {
			Variables_Plot_Chart->Series[0]->AddXY(sample_time_end,
						Simulation_System.get_variable(0,1,1,1,1));
			Variables_Plot_Chart->Series[1]->AddXY(sample_time_end,
						Simulation_System.get_variable(1,1,1,1,1));
		   }

		   //-----------------------------------------------------
		   if( Plot_Source_RadioGroup->ItemIndex == 1 )
		   //-----------------------------------------------------
		   {
			double Counter=0;
			for( int i=0; i<Simulation_System.Dimension_X; i++)
			for( int j=0; j<Simulation_System.Dimension_Y; j++)
			if( Simulation_System.get_variable(0,i,j,0,0) > 0.5 )
			Counter++;

			Counter /= ((double)Simulation_System.Dimension_X*
						(double)Simulation_System.Dimension_Y);
						Variables_Plot_Chart->Series[0]->AddXY(sample_time_end,Counter);

		   }
		}

		//-----------------------------------------------------
		// record potentials
		//-----------------------------------------------------
		if( Record_Potentials_CheckBox->Checked == cbChecked)
		if( Simulation_System.get_current_time() > prev_time_s +
			Simulation_System.Signal_Sampling_Timestep )
		{
			Simulation_System.record_potentials(Signal_Type_To_Record_RadioGroup->ItemIndex,Grid_Spacing_Edit->Text.ToInt());
			prev_time_s = Simulation_System.get_current_time();
		}

		//-----------------------------------------------------
		// record ecg
		//-----------------------------------------------------
		if( Record_ECG_CheckBox->Checked == cbChecked)
		if( Simulation_System.get_current_time() > prev_time_ecg +
			Simulation_System.Signal_Sampling_Timestep )
		{
			Simulation_System.record_ECG();
			ECG_Chart->Series[0]->AddXY(Simulation_System.get_current_time(),
				Simulation_System.Three_Dim_System.ECG_signal[
				Simulation_System.Three_Dim_System.ECG_signal.size()-1] );
			prev_time_ecg = Simulation_System.get_current_time();
		}

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Stop_Sim_ButtonClick(TObject *Sender)
{
	Simulation_Flag = false;
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Stim_ButtonClick(TObject *Sender)
{

    for( int i=0; i<0.5*Simulation_System.Dimension_X; i++)
    for( int j=0; j<0.5*Simulation_System.Dimension_Y; j++)
    for( int k=0; k<Simulation_System.Dimension_Z; k++)
    for( int l=0; l<Simulation_System.Dimension_T; l++)
    {

    Simulation_System.stimulate_node(i,j,k,l);

    }

	opengl_panel_paint();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Clear_ButtonClick(TObject *Sender)
{
    Simulation_System.clear_system();
	opengl_panel_paint();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Clear_Variables_Plot_ButtonClick(TObject *Sender)

{
    Variables_Plot_Chart->Series[0]->Clear();
    Variables_Plot_Chart->Series[1]->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Z_Slice_TrackBarChange(TObject *Sender)
{
	Current_Z_Slice = Z_Slice_TrackBar->Position;

	Z_Slice_Label->Caption = "Z slice: "+IntToStr(Current_Z_Slice+1)+"/"+
							 IntToStr(Simulation_System.Dimension_Z);

	opengl_panel_paint();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::T_Slice_TrackBarChange(TObject *Sender)
{
    Current_T_Slice = T_Slice_TrackBar->Position;

    T_Slice_Label->Caption = "T slice: "+IntToStr(Current_T_Slice+1)+"/"+
                             IntToStr(Simulation_System.Dimension_T);

	opengl_panel_paint();

}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Make_Spiral_In_Current_Slice_ButtonClick(TObject *Sender)

{
	Simulation_System.create_spiral(0); // 0-x,1-y,2-z,3-t
	opengl_panel_paint();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::System_Dim_RadioGroupClick(TObject *Sender)
{
	if( System_Dim_RadioGroup->ItemIndex == 0 )
	Simulation_System.Which_System_Dimension = 2;

	if( System_Dim_RadioGroup->ItemIndex == 1 )
	Simulation_System.Which_System_Dimension = 3;

	if( System_Dim_RadioGroup->ItemIndex == 2 )
	Simulation_System.Which_System_Dimension = 4;

	if( System_Dim_RadioGroup->ItemIndex == 3 )
	Simulation_System.Which_System_Dimension = TWO_LAYER_SYSTEM_DIMENSION;

	opengl_panel_paint();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Export_Sig_Plot_ButtonClick(TObject *Sender)
{
    if( SaveDialog1->Execute() )
    {

    ofstream df;
    df.open(SaveDialog1->FileName.c_str());

	df << Variables_Plot_Chart->Series[0]->Count() << endl;

    for(long i=0;i<Variables_Plot_Chart->Series[0]->Count();i++)
    {
        df << Variables_Plot_Chart->Series[0]->YValue[i] << endl;
//        df << Variables_Plot_Chart->Series[0]->YValue[i] << " ";
//        df << Variables_Plot_Chart->Series[1]->YValue[i] << endl;
    }

    df.close();

    }

}
//---------------------------------------------------------------------------

void TMain_Screen_Form::display_single_signal_from_history_vector(long Node_Ptr)
{
/*
	Single_Signal_Chart->Series[0]->Clear();
	ISIs_Chart->Series[0]->Clear();

	if( Node_Ptr < 0 ) Node_Ptr = 0;
	if( Node_Ptr == Simulation_System.Dimension_X )
	Node_Ptr = Simulation_System.Dimension_X-1;

	for(long i=0;i<Simulation_System.get_voltage_history_size();i++)
	Single_Signal_Chart->Series[0]->AddY(
		Simulation_System.get_voltage_from_history(Node_Ptr,i));

	Simulation_System.compute_ISIs(Node_Ptr);
	for(long i=0;i<Simulation_System.get_ISIs_history_size();i++)
	ISIs_Chart->Series[0]->AddY(Simulation_System.get_ISI(i) );
*/
}

//---------------------------------------------------------------------------

void TMain_Screen_Form::display_signals_from_history_array()
{
	long History_Size = Simulation_System.get_history_vector_size();
	Electrogram E,E1,E2;

	if( History_Size > 0 )
	{

	for(int i=0;i<80;i++)
	Signals_Tab_Chart->Series[i]->Clear();

	//------------------------------------------------------------
	// point grid around pointed element
	//------------------------------------------------------------
	double Grid_Spacing = Grid_Spacing_Edit->Text.ToInt();
	double VS = VS_Gap_Edit->Text.ToDouble();
	double cx,cy,Voltage,Time;
	double ts = Simulation_System.Signal_Sampling_Timestep;
	if(ts==0) ts = 0.25;
	int BCL_Ptr = 1;
	if( ts != 0 ) BCL_Ptr = Basic_CL_Edit->Text.ToDouble()/ts;
	int Electrogram_Type = 2;
	std::vector<double> S1;

	double DX = Simulation_System.Dimension_X;
	double DY = Simulation_System.Dimension_Y;

	Simulation_System.clear_grid_egms();

	// calculate phase and local activation times
	int Counter = 0;
	for(int s1=0;s1<HD_GRID_SIZE;s1++)
	for(int s2=0;s2<HD_GRID_SIZE;s2++)
	{
		cx = Pointed_Spot_i + s1*Grid_Spacing;
		cy = Pointed_Spot_j + s2*Grid_Spacing;

		E.Egm.clear();
		E.x = cx/DX;      // so that size of system for egm is normalized to [0-1]
		E.y = cy/DY;
		E.z = 0;

		for(long t=2;t<History_Size;t++)
		{
			Simulation_System.get_voltage(cx,cy,t,&Voltage,&Time);
			E.Egm.push_back(Voltage);
		}

		E.Phase = NumAlg.get_electrogram_phase(&E.Egm,Electrogram_Type,BCL_Ptr,ts);
		E.Activation_Timings_Ptr = NumAlg.get_activation_timings(&E.Phase);

		if(s1==0 && s2==0)
		E1.Phase = E.Phase;

		if(s1==0 && s2== HD_GRID_SIZE -1)
		E2.Phase = E.Phase;

		if( SC_RadioGroup->ItemIndex == 0 )
		for(long t=2;t<E.Egm.size();t++)
			Signals_Tab_Chart->Series[Counter]->AddXY(t*ts,E.Egm[t]+VS*Counter);

		if( SC_RadioGroup->ItemIndex == 1 )
		for(long t=2;t<E.Phase.size();t++)
			Signals_Tab_Chart->Series[Counter]->AddXY(t*ts,E.Phase[t]/7.0+Counter);

		if( SC_RadioGroup->ItemIndex == 2 )
		{
		for(long t=2;t<E.Egm.size();t++)
			Signals_Tab_Chart->Series[Counter]->AddXY(t*ts,E.Egm[t]+VS*Counter);

		for(long t=2;t<E.Phase.size();t++)
			Signals_Tab_Chart->Series[30+Counter]->AddXY(t*ts,E.Phase[t]/7.0+VS*Counter);
		}

		if( SC_RadioGroup->ItemIndex == 3 )
		for(long t=2;t<E.Activation_Timings_Ptr.size();t++)
			Signals_Tab_Chart->Series[60+Counter]->AddXY(E.Activation_Timings_Ptr[t]*ts,VS*Counter);

		Counter++;
		Simulation_System.add_grid_egm(&E);
	}

	ofstream df("phase1.csv");
	for(long t=0;t<E1.Phase.size();t++)
	df << E1.Phase[t] << "," << E2.Phase[t] << endl;
	df.close();

	double mpc = NumAlg.get_phase_synchronization(&E1.Phase,&E2.Phase,1,1,true,0);
    ShowMessage("MPC= " + FloatToStr(mpc));

	//------------------------------------------------------------
	// calculate and paint sliding window correlation length plot
	//------------------------------------------------------------
	if( Corr_L_CheckBox->State == cbChecked )
	Recalculate_CL_ButtonClick(this);

	}
}

//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::SaveData1Click(TObject *Sender)
{
	if( SaveDialog1->Execute() )
	{
		Simulation_System.Saved_Study_Filename = SaveDialog1->FileName;
		ofstream df(Simulation_System.Saved_Study_Filename.c_str() );
		Simulation_System.save_object(&df);
		df.close();

		opengl_panel_paint();
		ShowMessage("Done");
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Update_Par_ButtonClick(TObject *Sender)
{
/*
	vector <double> CLs;
	vector <double> Base_Signal;
	vector <double> Current_Signal;

	Custom_Global_Value = 0;
	long Counter=0;

	ISIs_Parameters_Chart->Series[0]->Clear();

	int Selected_Node_Ptr = (double)Signal_Display_ScrollBar->Position/
				   (double)Signal_Display_ScrollBar->Max *
                   (double)Simulation_System.Dimension_X;
    if( Selected_Node_Ptr < 0 ) Selected_Node_Ptr = 0;
    if( Selected_Node_Ptr == Simulation_System.Dimension_X )
    Selected_Node_Ptr = Simulation_System.Dimension_X-1;

	if( Simulation_System.Two_Dim_System.DX[Selected_Node_Ptr]
            [(int)(0.5*Simulation_System.Two_Dim_System.Size_Y)] != 0 &&
        Simulation_System.Two_Dim_System.DY[Selected_Node_Ptr]
            [(int)(0.5*Simulation_System.Two_Dim_System.Size_Y)] != 0 )
    {

    Base_Signal.clear();
	for(long i=0;i<Simulation_System.get_voltage_history_size();i++)
    Base_Signal.push_back( Simulation_System.get_voltage_from_history(Selected_Node_Ptr,i) );

	for(int X=0;X<Simulation_System.Dimension_X;X++)
    {

    if( Simulation_System.Two_Dim_System.DX[X]
            [(int)(0.5*Simulation_System.Two_Dim_System.Size_Y)] != 0 &&
        Simulation_System.Two_Dim_System.DY[X]
            [(int)(0.5*Simulation_System.Two_Dim_System.Size_Y)] != 0 )
    {

    Simulation_System.compute_ISIs(X);

    CLs.clear();
    for(long i=1;i<Simulation_System.get_ISIs_history_size();i++)
    CLs.push_back(Simulation_System.get_ISI(i));

    //------------------------------------------------------
    // Mean
    //------------------------------------------------------
    if( Paramter_Selection_RadioGroup->ItemIndex == 0 )
    {
        double Mean,SD;
        NumAlg.stdev_cor_vec(&CLs, &Mean, &SD);
        ISIs_Parameters_Chart->Series[0]->AddY(Mean);

        if( Mean > 0 )
        {
			Custom_Global_Value += Mean;
            Counter++;
        }
    }

    //------------------------------------------------------
    // SD
    //------------------------------------------------------
    if( Paramter_Selection_RadioGroup->ItemIndex == 1 )
    {
		double Mean,SD;
        NumAlg.stdev_cor_vec(&CLs, &Mean, &SD);
        ISIs_Parameters_Chart->Series[0]->AddY(SD);

        if( SD > 0 )
        {
            Custom_Global_Value += SD;
            Counter++;
        }
    }

    //------------------------------------------------------
    // Porta Index
    //------------------------------------------------------
    if( Paramter_Selection_RadioGroup->ItemIndex == 2 )
    {
		long N_plus=0,N_minus=0;
        double Assymetry;

        for(long k=1;k<CLs.size();k++)
        {
            if( CLs[k]-CLs[k-1] >= 0 )
            N_plus++;
            if( CLs[k]-CLs[k-1] <= 0 )
            N_minus++;
        }

        if( N_minus + N_plus != 0 )
        Assymetry = 100*N_minus/(N_minus + N_plus);
        else
        Assymetry = 0;

        ISIs_Parameters_Chart->Series[0]->AddY(Assymetry);

        if( Assymetry > 0 )
        {
            Custom_Global_Value += Assymetry;
            Counter++;
        }
	}

	}    // if X node D!=0
	else
	ISIs_Parameters_Chart->Series[0]->AddY(0);

	} // going through all nodes in X direction

	if( Counter != 0 )
	Custom_Global_Value /= Counter;
	else
	Custom_Global_Value = -1000;

	} // if selected node has D!=0
*/
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Paramter_Selection_RadioGroupClick(TObject *Sender)
{
    Update_Par_ButtonClick(this);
}

//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Add_Assymetric_Modulation_CheckBoxClick(TObject *Sender)
{
/*
	// configure modulation
	if( Add_Assymetric_Modulation_CheckBox->State == cbChecked)
	Simulation_System.Two_Dim_System.Add_Assymetric_Modulation = true;
	else
	Simulation_System.Two_Dim_System.Add_Assymetric_Modulation = false;

	Simulation_System.Two_Dim_System.Asm_Length = Asm_Len_Edit->Text.ToDouble();
	Simulation_System.Two_Dim_System.Asm_Amplitude = Asm_Amp_Edit->Text.ToDouble();
	Simulation_System.Two_Dim_System.Asm_Ratio = Ass_Ratio_Edit->Text.ToDouble();

	Simulation_System.Two_Dim_System.Central_Region_Relative_Size = Ass_Region_Size_Edit->Text.ToDouble();
*/
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Displaymodulation1Click(TObject *Sender)
{
/*
	double Modulation;
	double Asm_Length = Asm_Len_Edit->Text.ToDouble();
	double Asm_Amplitude = Asm_Amp_Edit->Text.ToDouble();
	double Asm_Ratio = Ass_Ratio_Edit->Text.ToDouble();

	Variables_Plot_Chart->Series[0]->Clear();

	for (double Global_Time=0; Global_Time < 200; Global_Time++)
	{

	double t1 = floor(Global_Time/Asm_Length)*Asm_Length;
	double t2 = (floor(Global_Time/Asm_Length)+1)*Asm_Length;

	if( Global_Time < t1 + Asm_Ratio*Asm_Length)
	Modulation = Asm_Amplitude*(Global_Time-t1)/(Asm_Ratio*Asm_Length);
	else
	Modulation = Asm_Amplitude*(t2-Global_Time)/((1-Asm_Ratio)*Asm_Length);

	Variables_Plot_Chart->Series[0]->AddXY(Global_Time,Modulation);

	}
*/
}
//---------------------------------------------------------------------------


void __fastcall TMain_Screen_Form::Generate_Coupling_v1_ButtonClick(TObject *Sender)
{
	Simulation_System.Two_Dim_System.Max_Diff_X = DX_Edit->Text.ToDouble();
	Simulation_System.Two_Dim_System.Max_Diff_Y = DY_Edit->Text.ToDouble();
	Simulation_System.Two_Dim_System.set_initial_coupling();

	if( Lines_CheckBox->State == cbChecked )
	{
	Simulation_System.Two_Dim_System.Lines_No_v1 = Lines_No_Edit->Text.ToDouble();
	Simulation_System.Two_Dim_System.Line_Perc_Leng_v1 = Perc_Len_Edit->Text.ToDouble();
	Simulation_System.Two_Dim_System.Line_Perc_D = PL_Edit->Text.ToDouble();
	Simulation_System.Two_Dim_System.add_coupling(2,false);

	}

	if( Disks_CheckBox->State == cbChecked )
	{

	if( ARings_CheckBox->State == cbChecked )
	{
	Simulation_System.Two_Dim_System.Diff_v1_Number = NDiffv1_Edit->Text.ToDouble();
	Simulation_System.Two_Dim_System.Diff_v1_Radius = DRadius_Edit->Text.ToDouble();
	Simulation_System.Two_Dim_System.Ring_v1_Width = AR_Radius_Edit->Text.ToDouble();
	Simulation_System.Two_Dim_System.Ring_v1_PercD = AR_Perc_Edit->Text.ToDouble();
	Simulation_System.Two_Dim_System.add_coupling(1,true);
	}
	else
	{
	Simulation_System.Two_Dim_System.Diff_v1_Number = NDiffv1_Edit->Text.ToDouble();
	Simulation_System.Two_Dim_System.Diff_v1_Radius = DRadius_Edit->Text.ToDouble();
	Simulation_System.Two_Dim_System.add_coupling(1,false);
	}

	}

	if( CDCB_CheckBox->State == cbChecked )
	{
	Simulation_System.Two_Dim_System.Disk_Radius = CD_R_Edit->Text.ToDouble();
	Simulation_System.Two_Dim_System.add_coupling(3,false);
	}

	opengl_panel_paint();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Display_Type_RadioGroupClick(TObject *Sender)
{
	set_initial_world_position();
	rotate_world_using_angles(M_PI,M_PI,-M_PI_2); // M_PI

	opengl_panel_paint();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Display_Value_RadioGroupClick(TObject *Sender)
{
    opengl_panel_paint();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Gettotalcrosscorelationpowerinfnofprobofablationcouplingv11Click(TObject *Sender)
{
/*
	ShowMessage("4DAMS 200x200 AlievPanf BASELINE file required!!!");

	Ask_For_Single_Item_Form1->Caption = "Number of repetitions";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "20";
	Ask_For_Single_Item_Form1->ShowModal();
	int Repetition_Number = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

    Ask_For_Single_Item_Form1->Caption = "Min Number of Lesions";
    Ask_For_Single_Item_Form1->Edit_Box->Text = "0";
    Ask_For_Single_Item_Form1->ShowModal();
    double Min_N = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

    Ask_For_Single_Item_Form1->Caption = "Max Number of Lesions";
    Ask_For_Single_Item_Form1->Edit_Box->Text = "100";
    Ask_For_Single_Item_Form1->ShowModal();
    double Max_N = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

    Ask_For_Single_Item_Form1->Caption = "Number of Lesions step";
    Ask_For_Single_Item_Form1->Edit_Box->Text = "10";
    Ask_For_Single_Item_Form1->ShowModal();
    double Step_N = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

    Ask_For_Single_Item_Form1->Caption = "Lesion Radius";
    Ask_For_Single_Item_Form1->Edit_Box->Text = "10";
	Ask_For_Single_Item_Form1->ShowModal();
    double R = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();
    Simulation_System.Two_Dim_System.Diff_v1_Radius = R;

    Ask_For_Single_Item_Form1->Caption = "Number of ms for initial simulation";
    Ask_For_Single_Item_Form1->Edit_Box->Text = "500";
    Ask_For_Single_Item_Form1->ShowModal();
    double Length_Initial = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

    Ask_For_Single_Item_Form1->Caption = "Number of ms to simulate (simulation to get results)";
    Ask_For_Single_Item_Form1->Edit_Box->Text = "500";
    Ask_For_Single_Item_Form1->ShowModal();
    double Length_Proper = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

    SaveDialog1->FileName = "Cross_Corr Ablation MASS ANALYSIS "+
    FloatToStr(Min_N)+" "+
    FloatToStr(Max_N)+" "+
    FloatToStr(Step_N)+" "+
    FloatToStr(R)+" "+
    IntToStr(Repetition_Number)+" "+
    FloatToStr(Length_Initial)+" "+
    FloatToStr(Length_Proper)+".csv";

    if( SaveDialog1->Execute() )
    {

    ofstream df;
    df.open(SaveDialog1->FileName.c_str());
	df << "#,N,Mean_Total_Cross_Corr,Mean_CL,Mean_CL_SD,Porta_Index,Distant_correlation" << endl;

    for(double n=Min_N;n<=Max_N;n+=Step_N)
    for(int Rep=1;Rep<=Repetition_Number;Rep++)
    {

    df << Rep << "," << n << ",";

    double it=(n-Min_N)/Step_N;
    double max_it = (Max_N-Min_N)/Step_N;
    Main_Screen_Form->Caption = IntToStr((int)(it)) + "/" + IntToStr((int)(max_it));
    Application->ProcessMessages();

    // load Baseline simulation
    ifstream DF("4DAMS 200x200 AlievPanf BASELINE");
    Simulation_System.load_object(&DF);
    DF.close();

    // generate ablation spots
    NDiffv1_Edit->Text = FloatToStr(n);
    DRadius_Edit->Text = FloatToStr(R);
    Generate_Coupling_v1_ButtonClick(this);

    // initial simulation to get to steady state
    Fixed_Simulation_Time_Flag = true;
	Record_Potentials_CheckBox->State = cbUnchecked;
    Max_Time = Simulation_System.get_current_time() + Length_Initial;
    Start_Sim_ButtonClick(this);

    // clear plots
    Clear_Sig_History_ButtonClick(this);
	Record_Potentials_CheckBox->State = cbChecked;

    // proper simulation
    Max_Time = Simulation_System.get_current_time() + Length_Proper;
    Start_Sim_ButtonClick(this);

    //----------------------------------------------------------------------
    // compute mean cross-correlation
    //----------------------------------------------------------------------
    Paramter_Selection_RadioGroup->ItemIndex = 3;
    double Counter=0,Mean_Total=0;
    for(long pos=Signal_Display_ScrollBar->Min;pos<Signal_Display_ScrollBar->Max;pos++)
    {
        Signal_Display_ScrollBar->Position = pos;
        Update_Par_ButtonClick(this);

		if( Custom_Global_Value > 0 ) //!!!!!!!!!!!!!!
		{
            Mean_Total += Custom_Global_Value;
            Counter++;
        }
    }

    if( Counter != 0 )
    Mean_Total /= Counter;

    df << Mean_Total << ",";

    //----------------------------------------------------------------------
    // compute mean CL mean
    //----------------------------------------------------------------------
    Paramter_Selection_RadioGroup->ItemIndex = 0;
    Counter=0,Mean_Total=0;
    for(long pos=Signal_Display_ScrollBar->Min;pos<Signal_Display_ScrollBar->Max;pos++)
    {
        Signal_Display_ScrollBar->Position = pos;
        Update_Par_ButtonClick(this);

        if( Custom_Global_Value > 0 ) //!!!!!!!!!!!!!!
        {
            Mean_Total += Custom_Global_Value;
            Counter++;
		}
    }

    if( Counter != 0 )
    Mean_Total /= Counter;

    df << Mean_Total << ",";

    //----------------------------------------------------------------------
    // compute mean CL SD
    //----------------------------------------------------------------------
    Paramter_Selection_RadioGroup->ItemIndex = 1;
    Counter=0,Mean_Total=0;
    for(long pos=Signal_Display_ScrollBar->Min;pos<Signal_Display_ScrollBar->Max;pos++)
    {
        Signal_Display_ScrollBar->Position = pos;
        Update_Par_ButtonClick(this);

        if( Custom_Global_Value > 0 ) //!!!!!!!!!!!!!!
        {
            Mean_Total += Custom_Global_Value;
            Counter++;
        }
    }

	if( Counter != 0 )
    Mean_Total /= Counter;

    df << Mean_Total << ",";

    //----------------------------------------------------------------------
    // compute mean Porta index
    //----------------------------------------------------------------------
    Paramter_Selection_RadioGroup->ItemIndex = 2;
    Counter=0,Mean_Total=0;
    for(long pos=Signal_Display_ScrollBar->Min;pos<Signal_Display_ScrollBar->Max;pos++)
    {
        Signal_Display_ScrollBar->Position = pos;
        Update_Par_ButtonClick(this);

        if( Custom_Global_Value > 0 ) //!!!!!!!!!!!!!!
        {
            Mean_Total += Custom_Global_Value;
            Counter++;
        }
    }

    if( Counter != 0 )
    Mean_Total /= Counter;

	df << Mean_Total << ",";

    //----------------------------------------------------------------------
    // compute distant correlatoin
    //----------------------------------------------------------------------
    vector <double> Base_Signal;
    vector <double> Current_Signal;
    int X = Simulation_System.Two_Dim_System.Size_X;
    int Y = Simulation_System.Two_Dim_System.Size_Y;

    if( Simulation_System.Two_Dim_System.DX[(int)(0.25*X)][(int)(0.5*Y)] != 0 &&
        Simulation_System.Two_Dim_System.DY[(int)(0.75*X)][(int)(0.5*Y)] != 0 )
    {
    Base_Signal.clear();
    for(long i=0;i<Simulation_System.get_voltage_history_size();i++)
    Base_Signal.push_back( Simulation_System.get_voltage_from_history(0.25*X,i) );

    Current_Signal.clear();
    for(long i=0;i<Simulation_System.get_voltage_history_size();i++)
    Current_Signal.push_back( Simulation_System.get_voltage_from_history(0.75*X,i) );

    double Corr,Lag;
    long Max_Delay = 50;
    long Max_Delay_Step=1;
    NumAlg.get_cross_correlation(
				&Base_Signal,&Current_Signal,Max_Delay,Max_Delay_Step,&Corr,&Lag);

	df << Corr << ",";
	}
	else
	df << ",";



	df << endl;

	} // main fors

	df.close();

	} // save execute
*/
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Add_Current_Modulation_CheckBoxClick(TObject *Sender)
{
/*
	// configure modulation
	if( Add_Current_Modulation_CheckBox->State == cbChecked)
	Simulation_System.Two_Dim_System.Add_Current_Modulation_1 = true;
	else
	Simulation_System.Two_Dim_System.Add_Current_Modulation_1 = false;

	Simulation_System.Two_Dim_System.CM1_Length = CML_Edit->Text.ToDouble();
	Simulation_System.Two_Dim_System.CM1_Amplitude = CMA_Edit->Text.ToDouble();

	Simulation_System.Two_Dim_System.Central_Region_Relative_Size = Ass_Region_Size_Edit->Text.ToDouble();
*/
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Signals_Tab_Refresh_ButtonClick(TObject *Sender)
{
/*
	int Slots_Number = 30;
	double Electrograms_Number = Grid_Size_Edit->Text.ToInt();
	double v,Spacing = Grid_Spacing_Edit->Text.ToDouble();
	long Node_Ptr;

	if( Electrograms_Number > Slots_Number )
	Electrograms_Number = Slots_Number;

	// clear plot
	for(int i=0;i<Slots_Number;i++)
	Signals_Tab_Chart->Series[i]->Clear();

	// voltage
	if( SC_RadioGroup->ItemIndex == 0 )
	for(double e=0;e<Electrograms_Number;e++)
	for(long i=0;i<Simulation_System.get_voltage_history_size()-1;i++)
	{
	Node_Ptr = e/Electrograms_Number*Simulation_System.Dimension_X;
	v = Simulation_System.get_voltage_from_history(Node_Ptr,i);
	Signals_Tab_Chart->Series[e]->AddXY(
		Simulation_System.Signal_Sampling_Timestep*i, e*Spacing + v);
	}

	// voltage gradient
	if( SC_RadioGroup->ItemIndex == 1 )
	for(double e=1;e<Electrograms_Number;e++)
	for(long i=0;i<Simulation_System.get_voltage_history_size()-1;i++)
	{
	Node_Ptr = e/Electrograms_Number*Simulation_System.Dimension_X;
	v = Simulation_System.get_voltage_from_history(Node_Ptr,i)-
		Simulation_System.get_voltage_from_history(Node_Ptr-1,i);
	Signals_Tab_Chart->Series[e]->AddXY(
		Simulation_System.Signal_Sampling_Timestep*i, e*Spacing + v);
	}
	// unipolar elelctrogram
	vector <double> El;
	if( SC_RadioGroup->ItemIndex == 2 )
	for(double e=1;e<Electrograms_Number;e++)
	{
	Node_Ptr = e/Electrograms_Number*Simulation_System.Dimension_X;
	El = Simulation_System.calculate_unipolar_voltage_from_surface(
		Node_Ptr,0.5*Simulation_System.Dimension_Y,
		Options_Form->Range_for_Uni_El_Edit->Text.ToInt() );
	for(long i=0;i<El.size();i++)
	{
	Signals_Tab_Chart->Series[e]->AddXY(
		Simulation_System.Signal_Sampling_Timestep*i, e*Spacing + El[i] );
	}
	}

	int Bip_Spacing = Options_Form->Bip_Spacing_Edit->Text.ToInt();

	// bipolar elelctrogram
	vector <double> El1,El2;
	if( SC_RadioGroup->ItemIndex == 3 )
	for(double e=1;e<Electrograms_Number;e++)
	{
	Node_Ptr = e/Electrograms_Number*Simulation_System.Dimension_X;
	El1 = Simulation_System.calculate_unipolar_voltage_from_surface(
		Node_Ptr,0.5*Simulation_System.Dimension_Y,
		Options_Form->Range_for_Uni_El_Edit->Text.ToInt() );
	El2 = Simulation_System.calculate_unipolar_voltage_from_surface(
		Node_Ptr+Bip_Spacing,0.5*Simulation_System.Dimension_Y,
		Options_Form->Range_for_Uni_El_Edit->Text.ToInt() );
	for(long i=0;i<El1.size();i++)
	{
	Signals_Tab_Chart->Series[e]->AddXY(
		Simulation_System.Signal_Sampling_Timestep*i, e*Spacing + El1[i]-El2[i] );
	}
	}

	// bipolar elelctrograms from corners
	int Corner_Loc = 10;
	if( SC_RadioGroup->ItemIndex == 4 )
	for(double e=0;e<4;e++)
	{

	if(e==0)
	{
	El1 = Simulation_System.calculate_unipolar_voltage_from_surface(
		Corner_Loc,
		Corner_Loc,
		Options_Form->Range_for_Uni_El_Edit->Text.ToInt() );
	El2 = Simulation_System.calculate_unipolar_voltage_from_surface(
		Corner_Loc+Bip_Spacing,
		Corner_Loc,
		Options_Form->Range_for_Uni_El_Edit->Text.ToInt() );
	}

	if(e==1)
	{
	El1 = Simulation_System.calculate_unipolar_voltage_from_surface(
		Simulation_System.Dimension_X-Corner_Loc,
		Corner_Loc,
		Options_Form->Range_for_Uni_El_Edit->Text.ToInt() );
	El2 = Simulation_System.calculate_unipolar_voltage_from_surface(
		Simulation_System.Dimension_X-Corner_Loc+Bip_Spacing,
		Corner_Loc,
		Options_Form->Range_for_Uni_El_Edit->Text.ToInt() );
	}

	if(e==2)
	{
	El1 = Simulation_System.calculate_unipolar_voltage_from_surface(
		Corner_Loc,
		Simulation_System.Dimension_Y-Corner_Loc,
		Options_Form->Range_for_Uni_El_Edit->Text.ToInt() );
	El2 = Simulation_System.calculate_unipolar_voltage_from_surface(
		Corner_Loc+Bip_Spacing,
		Simulation_System.Dimension_Y-Corner_Loc,
		Options_Form->Range_for_Uni_El_Edit->Text.ToInt() );
	}

	if(e==3)
	{
	El1 = Simulation_System.calculate_unipolar_voltage_from_surface(
		Simulation_System.Dimension_X-Corner_Loc,
		Simulation_System.Dimension_Y-Corner_Loc,
		Options_Form->Range_for_Uni_El_Edit->Text.ToInt() );
	El2 = Simulation_System.calculate_unipolar_voltage_from_surface(
		Simulation_System.Dimension_X-Corner_Loc+Bip_Spacing,
		Simulation_System.Dimension_Y-Corner_Loc,
		Options_Form->Range_for_Uni_El_Edit->Text.ToInt() );
	}

	for(long i=0;i<El1.size();i++)
	{
	Signals_Tab_Chart->Series[e]->AddXY(
		Simulation_System.Signal_Sampling_Timestep*i, e*Spacing + El1[i]-El2[i] );
	}

	}
*/
}
//---------------------------------------------------------------------------


void __fastcall TMain_Screen_Form::Reduce_ButtonClick(TObject *Sender)
{
	CD_R_Edit->Text = IntToStr(CD_R_Edit->Text.ToInt()-1);
	Generate_Coupling_v1_ButtonClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::SET_MI_ButtonClick(TObject *Sender)
{
/*
	Simulation_System.Two_Dim_System.set_mi_distribution(
		mi1_left_Edit->Text.ToDouble(),
		mi2_left_Edit->Text.ToDouble(),
		mi1_right_Edit->Text.ToDouble(),
		mi2_right_Edit->Text.ToDouble(),
		Cut_ratio_mi_Edit->Text.ToDouble() );

	opengl_panel_paint();
*/
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Generate_Random_Breakup_Rings_ButtonClick(TObject *Sender)
{
/*
	Simulation_System.Two_Dim_System.set_mi_breakup_rings(
		mi_min_Edit->Text.ToDouble(),
		mi_max_Edit->Text.ToDouble(),
		DR_Edit->Text.ToDouble(),
		DN_Edit->Text.ToDouble(), false );

	opengl_panel_paint();
*/
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::ComputeMPC1Click(TObject *Sender)
{
/*
	// ask for radius
	Ask_For_Single_Item_Form1->Caption = "Radius of electrode taken to get MPC with";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "4";
	Ask_For_Single_Item_Form1->ShowModal();
	int Radius = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

	// ask for BCL
	Ask_For_Single_Item_Form1->Caption = "Basic CL";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "50";
	Ask_For_Single_Item_Form1->ShowModal();
	int BCL = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

	long Delay = 0;

	int El_Type = 2;

	Progress_Form->ProgressBar1->Min = 0;
	Progress_Form->ProgressBar1->Max = Simulation_System.Two_Dim_System.Size_X;
	Progress_Form->Show();

	for(int i=0;i<Simulation_System.Two_Dim_System.Size_X;i++)
	for(int j=0;j<Simulation_System.Two_Dim_System.Size_Y;j++)
	Simulation_System.Two_Dim_System.CUSTOM_VALUE[i][j] = 0;

	for(int i=Radius;i<Simulation_System.Two_Dim_System.Size_X-Radius;i++)
	{

	Progress_Form->ProgressBar1->Position = i;
	Progress_Form->Show();
	Application->ProcessMessages();

	for(int j=Radius;j<Simulation_System.Two_Dim_System.Size_Y-Radius;j++)
	{

	double Mean_Coherence = 0;

		if( Simulation_System.Two_Dim_System.VOLTAGE_HISTORY[i][j].DVector.size() > 0 &&
			Simulation_System.Two_Dim_System.VOLTAGE_HISTORY[i+Radius][j].DVector.size() > 0 &&
			Simulation_System.Two_Dim_System.VOLTAGE_HISTORY[i-Radius][j].DVector.size() > 0 &&
			Simulation_System.Two_Dim_System.VOLTAGE_HISTORY[i][j-Radius].DVector.size() > 0 &&
			Simulation_System.Two_Dim_System.VOLTAGE_HISTORY[i][j+Radius].DVector.size() > 0 )
		{
		Mean_Coherence += NumAlg.get_MPC_between_electrograms(
			&Simulation_System.Two_Dim_System.VOLTAGE_HISTORY[i][j].DVector,
			&Simulation_System.Two_Dim_System.VOLTAGE_HISTORY[i-Radius][j].DVector,
			BCL,El_Type,Delay);
		Mean_Coherence += NumAlg.get_MPC_between_electrograms(
			&Simulation_System.Two_Dim_System.VOLTAGE_HISTORY[i][j].DVector,
			&Simulation_System.Two_Dim_System.VOLTAGE_HISTORY[i+Radius][j].DVector,
			BCL,El_Type,Delay);
		Mean_Coherence += NumAlg.get_MPC_between_electrograms(
			&Simulation_System.Two_Dim_System.VOLTAGE_HISTORY[i][j].DVector,
			&Simulation_System.Two_Dim_System.VOLTAGE_HISTORY[i][j-Radius].DVector,
			BCL,El_Type,Delay);
		Mean_Coherence += NumAlg.get_MPC_between_electrograms(
			&Simulation_System.Two_Dim_System.VOLTAGE_HISTORY[i][j].DVector,
			&Simulation_System.Two_Dim_System.VOLTAGE_HISTORY[i][j+Radius].DVector,
			BCL,El_Type,Delay);
		}

	Simulation_System.Two_Dim_System.CUSTOM_VALUE[i][j] = Mean_Coherence/4.0;

	}
	} // thru all nodes

	Progress_Form->Close();
	opengl_panel_paint();
	ShowMessage("Done");
*/
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Set_Custom_Color_Range_ButtonClick(TObject *Sender)
{
	Simulation_System.Two_Dim_System.Min_Custom_Value = Min_Disp_Val_Edit->Text.ToDouble();
	Simulation_System.Two_Dim_System.Max_Custom_Value = Max_Disp_Val_Edit->Text.ToDouble();
	opengl_panel_paint();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Landscape_mi1_ButtonClick(TObject *Sender)
{
/*
	if( Simulation_System.Which_System_Dimension == 2 )
	Simulation_System.Two_Dim_System.set_mi_breakup_landscape(
		RL_Edit->Text.ToDouble(),
		ppL_Edit->Text.ToDouble(),
		IL_Edit->Text.ToDouble(),
		mi_min_Edit->Text.ToDouble(),
		mi_max_Edit->Text.ToDouble(),
		InhVar_RadioGroup->ItemIndex);

	if( Simulation_System.Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	Simulation_System.Two_L_System.set_mi_breakup_landscape(
		RL_Edit->Text.ToDouble(),
		ppL_Edit->Text.ToDouble(),
		IL_Edit->Text.ToDouble(),
		mi_min_Edit->Text.ToDouble(),
		mi_max_Edit->Text.ToDouble(),
		InhVar_RadioGroup->ItemIndex);

	opengl_panel_paint();
*/
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Options2Click(TObject *Sender)
{
	Options_Form->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TMain_Screen_Form::Rotate2Click(TObject *Sender)
{
    set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_SELECT_MODE,cbUnchecked);
    set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_ROTATE,cbChecked);
    set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_ZOOM,cbUnchecked);
    set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_PAN,cbUnchecked);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Zoom2Click(TObject *Sender)
{
    set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_SELECT_MODE,cbUnchecked);
    set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_ROTATE,cbUnchecked);
    set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_ZOOM,cbChecked);
    set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_PAN,cbUnchecked);

}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Pan2Click(TObject *Sender)
{
    set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_SELECT_MODE,cbUnchecked);
    set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_ROTATE,cbUnchecked);
    set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_ZOOM,cbUnchecked);
    set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_PAN,cbChecked);

}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Poiningmode1Click(TObject *Sender)
{
    set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_SELECT_MODE,cbChecked);
    set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_ROTATE,cbUnchecked);
    set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_ZOOM,cbUnchecked);
    set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_PAN,cbUnchecked);
}
//---------------------------------------------------------------------------

void TMain_Screen_Form::set_Map_Panel_PopupMenu_item_checked_state(int tag,bool checked)
{
    for(int i=0;i<EP_Mapping_3D_Window_PopupMenu->Items[0].Count;i++)
	if(EP_Mapping_3D_Window_PopupMenu->Items[0].Items[i]->Tag == tag)
		EP_Mapping_3D_Window_PopupMenu->Items[0].Items[i]->Checked = checked;
}

//---------------------------------------------------------------------------

void TMain_Screen_Form::set_initial_world_position()
{

ThisRot.M[0] = 1;
ThisRot.M[1] = 0;
ThisRot.M[2] = 0;

ThisRot.M[3] = 0;
ThisRot.M[4] = 1;
ThisRot.M[5] = 0;

ThisRot.M[6] = 0;
ThisRot.M[7] = 0;
ThisRot.M[8] = 1;

Transform.M[0] = 1;
Transform.M[1] = 0;
Transform.M[2] = 0;
Transform.M[3] = 0;

Transform.M[4] = 0;
Transform.M[5] = 1;
Transform.M[6] = 0;
Transform.M[7] = 0;

Transform.M[8] = 0;
Transform.M[9] = 0;
Transform.M[10] = 1;
Transform.M[11] = 0;

Transform.M[12] = 0;
Transform.M[13] = 0;
Transform.M[14] = 0;
Transform.M[15] = 1;

}

//---------------------------------------------------------------------------

void TMain_Screen_Form::rotate_world_using_angles(double heading, double attitude, double bank)
{
    Quat4fT     ThisQuat;

    double c1 = cos(heading/2);
    double s1 = sin(heading/2);
    double c2 = cos(attitude/2);
    double s2 = sin(attitude/2);
    double c3 = cos(bank/2);
    double s3 = sin(bank/2);
    double c1c2 = c1*c2;
    double s1s2 = s1*s2;

    ThisQuat.s.X = c1c2*s3 + s1s2*c3;
    ThisQuat.s.Y = s1*c2*c3 + c1*s2*s3;
    ThisQuat.s.Z = c1*s2*c3 - s1*c2*s3;
	ThisQuat.s.W = c1c2*c3 - s1s2*s3;

    LastRot = ThisRot;
    Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);
	Matrix3fMulMatrix3f(&ThisRot, &LastRot);
    Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);
    LastRot = ThisRot;

}

//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Clear_ECG_ButtonClick(TObject *Sender)
{
	Simulation_System.clear_ECG_history();
	ECG_Chart->Series[0]->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::FormShow(TObject *Sender)
{
	Simulation_System.Signal_Sampling_Timestep = Sampling_Edit->Text.ToDouble();

	Main_Screen_Form->Caption = "4D simulator. Size: " + FloatToStr(TWO_DIM_SYSTEM_X_SIZE) +
		+ "x" + FloatToStr(TWO_DIM_SYSTEM_Y_SIZE);

	ECG_Chart->Series[0]->Pen->Width = 2;
	System_Dim_RadioGroupClick(this);

}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Export_ButtonClick(TObject *Sender)
{
	SaveDialog1->FileName = "ECG_export.csv";

	if( SaveDialog1->Execute() )
	{
	ofstream df;
	df.open(SaveDialog1->FileName.c_str());
	for(long t=0;t<Simulation_System.Three_Dim_System.ECG_signal.size();t++)
	df << Simulation_System.Three_Dim_System.ECG_signal[t] << endl;
	df.close();
	}

}

//---------------------------------------------------------------------------


void __fastcall TMain_Screen_Form::Z_D_Grad_ButtonClick(TObject *Sender)
{
	Simulation_System.Three_Dim_System.set_Z_D_gradient(
		Min_Z_D_Edit->Text.ToDouble(),Max_Z_D_Edit->Text.ToDouble());
	opengl_panel_paint();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Gen2_ButtonClick(TObject *Sender)
{
/*
	Simulation_System.Two_Dim_System.set_mi_breakup_rings(
		mi_max_Edit->Text.ToDouble(),
		mi_min_Edit->Text.ToDouble(),
		DR_Edit->Text.ToDouble(),
		DN_Edit->Text.ToDouble(),true );

	opengl_panel_paint();
*/
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Set_Layers_Coupling_ButtonClick(TObject *Sender)
{
	Simulation_System.Two_L_System.Min_Layer_Coupling = mincp_Edit->Text.ToDouble();
	Simulation_System.Two_L_System.Max_Layer_Coupling = maxcp_Edit->Text.ToDouble();
	Simulation_System.Two_L_System.Layer_Coupling_Disks_Number = DCLN_Edit->Text.ToDouble();
	Simulation_System.Two_L_System.Layer_Coupling_Disks_Radius = DCLR_Edit->Text.ToDouble();
	Simulation_System.Two_L_System.update_inter_layer_coupling();

	opengl_panel_paint();

}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Layer_RadioGroupClick(TObject *Sender)
{
	Current_Z_Slice = Layer_RadioGroup->ItemIndex;
	opengl_panel_paint();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::CR_Level_ComboBoxChange(TObject *Sender)
{
	save_interface_parameters_to_sim_system();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::CR_Number_ComboBoxChange(TObject *Sender)
{
	save_interface_parameters_to_sim_system();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::CR_Regions_Overlap_ComboBoxChange(TObject *Sender)

{
	save_interface_parameters_to_sim_system();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Diff_Sizes_CR_Search_CheckBoxClick(TObject *Sender)

{
	save_interface_parameters_to_sim_system();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::CR_Test_Time_EditChange(TObject *Sender)
{
	save_interface_parameters_to_sim_system();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::LoadData1Click(TObject *Sender)
{
	bool Result = true;
/*
	if( Echo )
	ShowMessage("Make sure correct system is set (the same as in the file). App is set to: ["
		+ FloatToStr(Simulation_System.Dimension_X) + ","+ FloatToStr(Simulation_System.Dimension_Y) + "]" );
*/
	if( Echo )
	{
		bool Result = OpenDialog1->Execute();
		Simulation_System.Saved_Study_Filename = OpenDialog1->FileName;
	}

	if( Result )
	{
		// Simulation_System.Saved_Study_Filename = OpenDialog1->FileName;
		ifstream df(Simulation_System.Saved_Study_Filename.c_str() );
		Simulation_System.load_object(&df);
		df.close();

		if( Echo )
		load_interface_parameters_from_sim_system();

		// update controls
		Signal_Display_ScrollBar->Max = Simulation_System.Dimension_X;

		if( Simulation_System.Which_System_Dimension == 2 )
		System_Dim_RadioGroup->ItemIndex = 0;

		if( Simulation_System.Which_System_Dimension == 3 )
		System_Dim_RadioGroup->ItemIndex = 1;

		if( Simulation_System.Which_System_Dimension == 4 )
		System_Dim_RadioGroup->ItemIndex = 2;

		if( Simulation_System.Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
		System_Dim_RadioGroup->ItemIndex = 3;

		Boundary_Conditions_RadioGroup->ItemIndex = Simulation_System.Boundary_Conditions;

		Sampling_Edit->Text = FloatToStr(Simulation_System.Signal_Sampling_Timestep);

		opengl_panel_paint();
		// ShowMessage("Done");
	}

}

//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Load_Seed_Study_ButtonClick(TObject *Sender)
{
	LoadData1Click(this);
/*
	if( OpenDialog1->Execute() )
	{
		Simulation_System.Saved_Study_Filename = OpenDialog1->FileName;
		Seed_Study_Filename_Edit->Text = OpenDialog1->FileName;

		ifstream df(Simulation_System.Saved_Study_Filename.c_str() );
		Simulation_System.load_object(&df);
		df.close();

		// update controls
		Signal_Display_ScrollBar->Max = Simulation_System.Dimension_X;

		if( Simulation_System.Which_System_Dimension == 2 )
		System_Dim_RadioGroup->ItemIndex = 0;

		if( Simulation_System.Which_System_Dimension == 3 )
		System_Dim_RadioGroup->ItemIndex = 1;

		if( Simulation_System.Which_System_Dimension == 4 )
		System_Dim_RadioGroup->ItemIndex = 2;

		if( Simulation_System.Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
		System_Dim_RadioGroup->ItemIndex = 3;

		Boundary_Conditions_RadioGroup->ItemIndex = Simulation_System.Boundary_Conditions;
		Sampling_Edit->Text = FloatToStr(Simulation_System.Signal_Sampling_Timestep);

		opengl_panel_paint();
		ShowMessage("Done");
	}
*/
}
//---------------------------------------------------------------------------

void TMain_Screen_Form::save_interface_parameters_to_sim_system()
{
	if( Echo )
	if( !Simulation_System.Critical_Region_Search_Mode )
	{
	Simulation_System.CR_Tests_Number = TSTN_Edit->Text.ToInt();
	Simulation_System.CR_Level =  CR_Level_ComboBox->Text.ToInt();
	Simulation_System.CR_Number = CR_Number_ComboBox->Text.ToInt();
	Simulation_System.CR_Regions_Overlap = CR_Regions_Overlap_ComboBox->Text.ToInt();

	Simulation_System.Diff_Sizes_CR_Search = Diff_Sizes_CR_Search_CheckBox->State;
	Simulation_System.CR_Test_Time = CR_Test_Time_Edit->Text.ToInt();
	}
}

//---------------------------------------------------------------------------

void TMain_Screen_Form::load_interface_parameters_from_sim_system()
{
	TSTN_Edit->Text = IntToStr(Simulation_System.CR_Tests_Number);

	CR_Level_ComboBox->ItemIndex =  CR_Level_ComboBox->Items[0].IndexOf(IntToStr(Simulation_System.CR_Level));
	CR_Number_ComboBox->ItemIndex = CR_Number_ComboBox->Items[0].IndexOf(IntToStr(Simulation_System.CR_Number));
	CR_Regions_Overlap_ComboBox->ItemIndex = CR_Regions_Overlap_ComboBox->Items[0].IndexOf(IntToStr(Simulation_System.CR_Regions_Overlap));

	if(Simulation_System.Diff_Sizes_CR_Search)
	Diff_Sizes_CR_Search_CheckBox->State = cbChecked;
	else
	Diff_Sizes_CR_Search_CheckBox->State = cbUnchecked;

	CR_Test_Time_Edit->Text = IntToStr((int)Simulation_System.CR_Test_Time);

		CRs_ListBox->Items[0].Clear();
		if( Simulation_System.Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
		for(int i=0;i<Simulation_System.TP1A_2L_system_CR_regions.size();i++)
			CRs_ListBox->Items[0].Add(i);
		if( Simulation_System.Which_System_Dimension == 2 )
		for(int i=0;i<Simulation_System.TP1A_2D_system_CR_regions.size();i++)
			CRs_ListBox->Items[0].Add(i);
}

//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Begin_CR_Protocol_ButtonClick(TObject *Sender)
{
	// clear CR list
	CRs_ListBox->Items[0].Clear();

	save_interface_parameters_to_sim_system();

	Simulation_System.Critical_Region_Search_Mode = true;

	// list of regions to test
	vector <TP1A_2L_system_CR_region_class> TP1A_2L_system_test_regions;
	vector <Two_D_system_CR_region_class> TP1A_2D_system_test_regions;

	// currently tested CR
	TP1A_2L_system_CR_region_class TP1A_2L_system_CR_region;
	Two_D_system_CR_region_class TP1A_2D_system_CR_region;

	bool AF_Terminated_Flag;
	long Iteration = 0;
	double radius,rounded_r;
	vector<int> cb;
	vector<int> ca;
	double x1,y1;
	int Level;

	Simulation_System.TP1A_2L_system_CR_regions.clear();
	Simulation_System.TP1A_2D_system_CR_regions.clear();

	//-----------------------------------------
	// GO THROUGH ALL LEVELS
	// for(int Level=2;Level<=Simulation_System.CR_Level;Level++)
	if( Level==Simulation_System.CR_Level )
	//-----------------------------------------
	{
		// 0. PREPARE LIST OF CRs to test
		TP1A_2L_system_test_regions.clear();
		TP1A_2D_system_test_regions.clear();

		radius = ((double)Simulation_System.Dimension_X/(2.*(double)Level));
		rounded_r = floor(radius+0.5);

		//-------------------------------------------------------
		// 2D SYSTEM CR TEST REGIONS GENERATION
		//-------------------------------------------------------
		if( Simulation_System.Which_System_Dimension == 2 )

		{
		// generate set to pick combinations from
		long counter=1;
		ca.clear();
		for(int x1=0;x1<Level;x1++)
		for(int y1=0;y1<Level;y1++)
		{
			ca.push_back (counter);
			counter++;
		}

		// Single region combinations
		if( Simulation_System.CR_Number == 1 )
		{
		  cb.clear();
		  cb.push_back (1);

		  do
			{
			y1 = cb[0] % Level;
			if( y1 == 0 ) y1 = Level;
			x1 = std::floor((double)(cb[0]-y1)/(double)Level)+1;
			x1--;
			y1--;
			  TP1A_2D_system_CR_region.clear();

			  TP1A_2D_system_CR_region.X.push_back(radius+radius*2*x1);
			  TP1A_2D_system_CR_region.Y.push_back(radius+radius*2*y1);
			  TP1A_2D_system_CR_region.R.push_back(rounded_r);
			  TP1A_2D_system_test_regions.push_back(TP1A_2D_system_CR_region);

			}
			while(next_combination(ca.begin (),ca.end (),cb.begin (),cb.end()) );
		}

		// two region combinations
		if( Simulation_System.CR_Number == 2 )
		{
		  cb.clear();
		  cb.push_back (1);
		  cb.push_back (2);

		  do
			{
			TP1A_2D_system_CR_region.clear();

			y1 = cb[0] % Level;
			if( y1 == 0 ) y1 = Level;
			x1 = std::floor((double)(cb[0]-y1)/(double)Level)+1;
			x1--;
			y1--;
			TP1A_2D_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2D_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2D_system_CR_region.R.push_back(rounded_r);

			y1 = cb[1] % Level;
			if( y1 == 0 ) y1 = Level;
			x1 = std::floor((double)(cb[1]-y1)/(double)Level)+1;
			x1--;
			y1--;
			TP1A_2D_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2D_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2D_system_CR_region.R.push_back(rounded_r);

			TP1A_2D_system_test_regions.push_back(TP1A_2D_system_CR_region);

			}
			while(next_combination(ca.begin (),ca.end (),cb.begin (),cb.end()) );
		}

		// three region combinations
		if( Simulation_System.CR_Number == 3 )
		{
		  cb.clear();
		  cb.push_back (1);
		  cb.push_back (2);
		  cb.push_back (3);

		  do
			{
			TP1A_2D_system_CR_region.clear();

			y1 = cb[0] % Level;
			if( y1 == 0 ) y1 = Level;
			x1 = std::floor((double)(cb[0]-y1)/(double)Level)+1;
			x1--;
			y1--;
			TP1A_2D_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2D_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2D_system_CR_region.R.push_back(rounded_r);

			y1 = cb[1] % Level;
			if( y1 == 0 ) y1 = Level;
			x1 = std::floor((double)(cb[1]-y1)/(double)Level)+1;
			x1--;
			y1--;
			TP1A_2D_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2D_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2D_system_CR_region.R.push_back(rounded_r);

			y1 = cb[2] % Level;
			if( y1 == 0 ) y1 = Level;
			x1 = std::floor((double)(cb[2]-y1)/(double)Level)+1;
			x1--;
			y1--;
			TP1A_2D_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2D_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2D_system_CR_region.R.push_back(rounded_r);

			TP1A_2D_system_test_regions.push_back(TP1A_2D_system_CR_region);

			}
			while(next_combination(ca.begin (),ca.end (),cb.begin (),cb.end()) );
		}   // triple

		// four region combinations
		if( Simulation_System.CR_Number == 4 )
		{
		  cb.clear();
		  cb.push_back (1);
		  cb.push_back (2);
		  cb.push_back (3);
		  cb.push_back (4);

		  do
			{
			TP1A_2D_system_CR_region.clear();

			y1 = cb[0] % Level;
			if( y1 == 0 ) y1 = Level;
			x1 = std::floor((double)(cb[0]-y1)/(double)Level)+1;
			x1--;
			y1--;
			TP1A_2D_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2D_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2D_system_CR_region.R.push_back(rounded_r);

			y1 = cb[1] % Level;
			if( y1 == 0 ) y1 = Level;
			x1 = std::floor((double)(cb[1]-y1)/(double)Level)+1;
			x1--;
			y1--;
			TP1A_2D_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2D_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2D_system_CR_region.R.push_back(rounded_r);

			y1 = cb[2] % Level;
			if( y1 == 0 ) y1 = Level;
			x1 = std::floor((double)(cb[2]-y1)/(double)Level)+1;
			x1--;
			y1--;
			TP1A_2D_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2D_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2D_system_CR_region.R.push_back(rounded_r);

			y1 = cb[3] % Level;
			if( y1 == 0 ) y1 = Level;
			x1 = std::floor((double)(cb[3]-y1)/(double)Level)+1;
			x1--;
			y1--;
			TP1A_2D_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2D_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2D_system_CR_region.R.push_back(rounded_r);

			TP1A_2D_system_test_regions.push_back(TP1A_2D_system_CR_region);

			}
			while(next_combination(ca.begin (),ca.end (),cb.begin (),cb.end()) );
		} // qudruple

		} // 2D SYSTEM

		//-------------------------------------------------------
		// 2L SYSTEM CR TEST REGIONS GENERATION
		//-------------------------------------------------------
		int Layer;

		if( Simulation_System.Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
		{

		// generate set to pick combinations from
		ca.clear();
		int counter=1;
		for(int x1=0;x1<2*Level;x1++)   // side-by-side arrangement
		for(int y1=0;y1<Level;y1++)
		{
			ca.push_back (counter);
			counter++;
		}

		// Single region combinations
		if( Simulation_System.CR_Number == 1 )
		{
		  cb.clear();
		  cb.push_back (1);

		  do
			{

			TP1A_2L_system_CR_region.clear();

			x1 = cb[0] % (int)(2*Level);
			Layer = 0;
			if( x1 == 0 )
			{
				x1 = Level;
				Layer = 1;
			}
			if( x1 > Level )
			{
				x1 -= Level;
				Layer = 1;
			}

			y1 = (double)(cb[0])/(double)(2*Level)+1;
			if( (int)(y1) == y1 )
				y1--;
			y1 = (int)(y1);

			x1--;
			y1--;

			TP1A_2L_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2L_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2L_system_CR_region.Layer.push_back(Layer);
			TP1A_2L_system_CR_region.R.push_back(rounded_r);
			TP1A_2L_system_test_regions.push_back(TP1A_2L_system_CR_region);

			}
			while(next_combination(ca.begin (),ca.end (),cb.begin (),cb.end()) );
		}  // single

		// Double region combinations
		if( Simulation_System.CR_Number == 2 )
		{
		  cb.clear();
		  cb.push_back (1);
		  cb.push_back (2);

		  do
			{

			TP1A_2L_system_CR_region.clear();

			x1 = cb[0] % (int)(2*Level);
			Layer = 0;
			if( x1 == 0 )
			{
				x1 = Level;
				Layer = 1;
			}
			if( x1 > Level )
			{
				x1 -= Level;
				Layer = 1;
			}

			y1 = (double)(cb[0])/(double)(2*Level)+1;
			if( (int)(y1) == y1 )
				y1--;
			y1 = (int)(y1);

			x1--;
			y1--;

			TP1A_2L_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2L_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2L_system_CR_region.Layer.push_back(Layer);
			TP1A_2L_system_CR_region.R.push_back(rounded_r);

			x1 = cb[1] % (int)(2*Level);
			Layer = 0;
			if( x1 == 0 )
			{
				x1 = Level;
				Layer = 1;
			}
			if( x1 > Level )
			{
				x1 -= Level;
				Layer = 1;
			}

			y1 = (double)(cb[1])/(double)(2*Level)+1;
			if( (int)(y1) == y1 )
				y1--;
			y1 = (int)(y1);

			x1--;
			y1--;

			TP1A_2L_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2L_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2L_system_CR_region.Layer.push_back(Layer);
			TP1A_2L_system_CR_region.R.push_back(rounded_r);

			TP1A_2L_system_test_regions.push_back(TP1A_2L_system_CR_region);

			}
			while(next_combination(ca.begin (),ca.end (),cb.begin (),cb.end()) );
		} // double

		// Triple region combinations
		if( Simulation_System.CR_Number == 3 )
		{
		  cb.clear();
		  cb.push_back (1);
		  cb.push_back (2);
		  cb.push_back (3);

		  do
			{

			TP1A_2L_system_CR_region.clear();

			// 1
			x1 = cb[0] % (int)(2*Level);
			Layer = 0;
			if( x1 == 0 )
			{
				x1 = Level;
				Layer = 1;
			}
			if( x1 > Level )
			{
				x1 -= Level;
				Layer = 1;
			}

			y1 = (double)(cb[0])/(double)(2*Level)+1;
			if( (int)(y1) == y1 )
				y1--;
			y1 = (int)(y1);

			x1--;
			y1--;

			TP1A_2L_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2L_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2L_system_CR_region.Layer.push_back(Layer);
			TP1A_2L_system_CR_region.R.push_back(rounded_r);

			// 2
			x1 = cb[1] % (int)(2*Level);
			Layer = 0;
			if( x1 == 0 )
			{
				x1 = Level;
				Layer = 1;
			}
			if( x1 > Level )
			{
				x1 -= Level;
				Layer = 1;
			}

			y1 = (double)(cb[1])/(double)(2*Level)+1;
			if( (int)(y1) == y1 )
				y1--;
			y1 = (int)(y1);

			x1--;
			y1--;

			TP1A_2L_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2L_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2L_system_CR_region.Layer.push_back(Layer);
			TP1A_2L_system_CR_region.R.push_back(rounded_r);

			// 3
			x1 = cb[2] % (int)(2*Level);
			Layer = 0;
			if( x1 == 0 )
			{
				x1 = Level;
				Layer = 1;
			}
			if( x1 > Level )
			{
				x1 -= Level;
				Layer = 1;
			}

			y1 = (double)(cb[2])/(double)(2*Level)+1;
			if( (int)(y1) == y1 )
				y1--;
			y1 = (int)(y1);

			x1--;
			y1--;

			TP1A_2L_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2L_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2L_system_CR_region.Layer.push_back(Layer);
			TP1A_2L_system_CR_region.R.push_back(rounded_r);

			TP1A_2L_system_test_regions.push_back(TP1A_2L_system_CR_region);

			}
			while(next_combination(ca.begin (),ca.end (),cb.begin (),cb.end()) );
		} // triple

		// Quadruple region combinations
		if( Simulation_System.CR_Number == 4 )
		{
		  cb.clear();
		  cb.push_back (1);
		  cb.push_back (2);
		  cb.push_back (3);
		  cb.push_back (4);

		  do
			{

			TP1A_2L_system_CR_region.clear();

			// 1
			x1 = cb[0] % (int)(2*Level);
			Layer = 0;
			if( x1 == 0 )
			{
				x1 = Level;
				Layer = 1;
			}
			if( x1 > Level )
			{
				x1 -= Level;
				Layer = 1;
			}

			y1 = (double)(cb[0])/(double)(2*Level)+1;
			if( (int)(y1) == y1 )
				y1--;
			y1 = (int)(y1);

			x1--;
			y1--;

			TP1A_2L_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2L_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2L_system_CR_region.Layer.push_back(Layer);
			TP1A_2L_system_CR_region.R.push_back(rounded_r);

			// 2
			x1 = cb[1] % (int)(2*Level);
			Layer = 0;
			if( x1 == 0 )
			{
				x1 = Level;
				Layer = 1;
			}
			if( x1 > Level )
			{
				x1 -= Level;
				Layer = 1;
			}

			y1 = (double)(cb[1])/(double)(2*Level)+1;
			if( (int)(y1) == y1 )
				y1--;
			y1 = (int)(y1);

			x1--;
			y1--;

			TP1A_2L_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2L_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2L_system_CR_region.Layer.push_back(Layer);
			TP1A_2L_system_CR_region.R.push_back(rounded_r);

			// 3
			x1 = cb[2] % (int)(2*Level);
			Layer = 0;
			if( x1 == 0 )
			{
				x1 = Level;
				Layer = 1;
			}
			if( x1 > Level )
			{
				x1 -= Level;
				Layer = 1;
			}

			y1 = (double)(cb[2])/(double)(2*Level)+1;
			if( (int)(y1) == y1 )
				y1--;
			y1 = (int)(y1);

			x1--;
			y1--;

			TP1A_2L_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2L_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2L_system_CR_region.Layer.push_back(Layer);
			TP1A_2L_system_CR_region.R.push_back(rounded_r);

			// 4
			x1 = cb[3] % (int)(2*Level);
			Layer = 0;
			if( x1 == 0 )
			{
				x1 = Level;
				Layer = 1;
			}
			if( x1 > Level )
			{
				x1 -= Level;
				Layer = 1;
			}

			y1 = (double)(cb[3])/(double)(2*Level)+1;
			if( (int)(y1) == y1 )
				y1--;
			y1 = (int)(y1);

			x1--;
			y1--;

			TP1A_2L_system_CR_region.X.push_back(radius+radius*2*x1);
			TP1A_2L_system_CR_region.Y.push_back(radius+radius*2*y1);
			TP1A_2L_system_CR_region.Layer.push_back(Layer);
			TP1A_2L_system_CR_region.R.push_back(rounded_r);

			TP1A_2L_system_test_regions.push_back(TP1A_2L_system_CR_region);

			}
			while(next_combination(ca.begin (),ca.end (),cb.begin (),cb.end()) );
		} // quadruple

		} // 2L SYSTEM


		//-----------------------------------------
		//-----------------------------------------
		for(long tr=0;tr<max(TP1A_2L_system_test_regions.size(),TP1A_2D_system_test_regions.size());tr++)
		//-----------------------------------------
		//-----------------------------------------
		{

		int Test_Number = 0;
		bool Test_Passed = true;

		while( Test_Passed && Test_Number < TSTN_Edit->Text.ToInt() )
		{

		Test_Passed = false;

		// update progress label
		Iteration++;
		CR_Progress_Label->Caption = "Level: "+IntToStr(Level)+" Iteration: " + IntToStr((int)Iteration);

		// 1. Load initial study seed (with ongoing AF!!!)

		//Echo = false;
		//LoadData1Click(this); //Load_Seed_Study_ButtonClick(this);
		//Echo = true;

		Simulation_System.set_initial_coupling();
		Initiate_AF_ButtonClick(this);

		Simulation_System.set_initial_coupling();

		// 2. Introduce test CR lesions
		if( Simulation_System.Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
		{
			TP1A_2L_system_CR_region = TP1A_2L_system_test_regions[tr];

			for(long r=0;r<TP1A_2L_system_CR_region.Layer.size();r++)
			Simulation_System.add_non_conducting_disk(
						TP1A_2L_system_CR_region.X[r],
						TP1A_2L_system_CR_region.Y[r],
						TP1A_2L_system_CR_region.Layer[r],
						TP1A_2L_system_CR_region.R[r]);
		}
		if( Simulation_System.Which_System_Dimension == 2 )
		{
			TP1A_2D_system_CR_region = TP1A_2D_system_test_regions[tr];

			for(long r=0;r<TP1A_2D_system_CR_region.X.size();r++)
			Simulation_System.add_non_conducting_disk(
						TP1A_2D_system_CR_region.X[r],
						TP1A_2D_system_CR_region.Y[r],
						0,
						TP1A_2D_system_CR_region.R[r]);
		}


		// 3. Run simulation testing for termination
		AF_Terminated_Flag = false;

		long End_Time = Simulation_System.get_current_time() + Simulation_System.CR_Test_Time;

		while( Simulation_System.get_current_time() < End_Time && AF_Terminated_Flag==false )
		{
			Simulation_System.compute_N_steps(20);
			Simulation_System.refresh_VW_matrices();
			Simulation_System.compute_N_steps(20);
			Simulation_System.refresh_VW_matrices();
			Simulation_System.compute_N_steps(20);
			Simulation_System.refresh_VW_matrices();
			Simulation_System.compute_N_steps(20);
			Simulation_System.refresh_VW_matrices();
			Simulation_System.compute_N_steps(20);
			Simulation_System.refresh_VW_matrices();

			opengl_panel_paint(); // ????

			Application->ProcessMessages();

			// test if terminated
			AF_Terminated_Flag = !Simulation_System.activity_present_check();
		}

		// 4. If terminated update flag
		if( AF_Terminated_Flag )
		{
			Test_Passed = true;
		}

		Test_Number ++;

		} // through all tests iterations

		// if all tests passed, add CR to the list
		if( Test_Number == TSTN_Edit->Text.ToInt() && Test_Passed )
		{
			if( Simulation_System.Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
				Simulation_System.TP1A_2L_system_CR_regions.push_back(TP1A_2L_system_CR_region);
			if( Simulation_System.Which_System_Dimension == 2 )
				Simulation_System.TP1A_2D_system_CR_regions.push_back(TP1A_2D_system_CR_region);
		}

		// 5. Update list on screen
		CRs_ListBox->Items[0].Clear();
		if( Simulation_System.Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
		for(int i=0;i<Simulation_System.TP1A_2L_system_CR_regions.size();i++)
			CRs_ListBox->Items[0].Add(i);
		if( Simulation_System.Which_System_Dimension == 2 )
		for(int i=0;i<Simulation_System.TP1A_2D_system_CR_regions.size();i++)
			CRs_ListBox->Items[0].Add(i);

		} // going through all test regions

	} // master for

	// save the study
//	ofstream df(Simulation_System.Saved_Study_Filename.c_str() );
// /	Simulation_System.save_object(&df);
//	df.close();

	Simulation_System.Critical_Region_Search_Mode = false;
	if( Echo )
	ShowMessage("Done");
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::CRs_ListBoxClick(TObject *Sender)
{
/*
	int Selected = CRs_ListBox->ItemIndex;

	Simulation_System.set_initial_coupling();

	// Load CR from the list into the simulation
	if( Simulation_System.Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	Simulation_System.Two_L_System.add_critical_region(Simulation_System.TP1A_2L_system_CR_regions[Selected] );

	if( Simulation_System.Which_System_Dimension == 2 )
	Simulation_System.Two_Dim_System.add_critical_region(Simulation_System.TP1A_2D_system_CR_regions[Selected] );

	opengl_panel_paint();
*/
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Boundary_Conditions_RadioGroupClick(TObject *Sender)
{
	Simulation_System.Boundary_Conditions = Boundary_Conditions_RadioGroup->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::InhVar_RadioGroupClick(TObject *Sender)
{
	if( InhVar_RadioGroup->ItemIndex == 0 ) // D
	{
		Display_Value_RadioGroup->ItemIndex = 1;
		mi_min_Edit->Text = "0";
		mi_max_Edit->Text = "0.2";
	}

	if( InhVar_RadioGroup->ItemIndex == 1 ) // mi1
	{
		Display_Value_RadioGroup->ItemIndex = 3;
		mi_min_Edit->Text = "0.02";
		mi_max_Edit->Text = "0.2";
	}

	if( InhVar_RadioGroup->ItemIndex == 2 ) // mi2
	{
		Display_Value_RadioGroup->ItemIndex = 4;
		mi_min_Edit->Text = "0.02";
		mi_max_Edit->Text = "0.2";
	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::GeneratesetofstudieswihtstableAF1Click(TObject *Sender)
{
/*
	int Number_of_Studies_To_Generate = 1;
	int Stability_Check_Time = 2000;
	int Tests_Number = 3;

	for(int n=0;n<Number_of_Studies_To_Generate;n++)
	{

	// generate landscape

	// mi1
	InhVar_RadioGroup->ItemIndex = 1;
	RL_Edit->Text = "10";
	ppL_Edit->Text = "0.01";
	IL_Edit->Text = "0.04";
	mi_min_Edit->Text = "0.02";
	mi_max_Edit->Text = "0.1";
	Landscape_mi1_ButtonClick(this);

	Display_Value_RadioGroup->ItemIndex = 0;

	bool Test_Passed = true;
	int Test_try = 0;

	while( Test_try < Tests_Number && Test_Passed )
	{

	Test_try++;

	// initiate AF
	Initiate_AF_ButtonClick(this);

	long End_Time = Simulation_System.get_current_time() + Stability_Check_Time;

	while( Simulation_System.get_current_time() < End_Time && Test_Passed )
	{
			Simulation_System.compute_N_steps(20);
			Simulation_System.refresh_VW_matrices();
			Simulation_System.compute_N_steps(20);
			Simulation_System.refresh_VW_matrices();
			Simulation_System.compute_N_steps(20);
			Simulation_System.refresh_VW_matrices();
			Simulation_System.compute_N_steps(20);
			Simulation_System.refresh_VW_matrices();
			Simulation_System.compute_N_steps(20);
			Simulation_System.refresh_VW_matrices();

			opengl_panel_paint(); // ????

			Application->ProcessMessages();

			// test if terminated
			Test_Passed = Simulation_System.activity_present_check();
	}

	}

	// If terminated, save
	if( Test_try == Tests_Number && Test_Passed )
	{
		AnsiString AS = "4Dstudy "+IntToStr(TWO_DIM_SYSTEM_X_SIZE)+"x"+IntToStr(TWO_DIM_SYSTEM_Y_SIZE)+" ";
		AS += IntToStr(InhVar_RadioGroup->ItemIndex)+ " ";
		AS += RL_Edit->Text + " ";
		AS += ppL_Edit->Text + " ";
		AS += IL_Edit->Text + " ";
		AS += mi_min_Edit->Text + " ";
		AS += mi_max_Edit->Text + " ";

		AS += FloatToStr(Simulation_System.Two_Dim_System.a) + " ";
		AS += FloatToStr(Simulation_System.Two_Dim_System.k_par) + " ";
		AS += FloatToStr(Simulation_System.Two_Dim_System.eps) + " ";
		AS += FloatToStr(Simulation_System.Two_Dim_System.mi1) + " ";
		AS += FloatToStr(Simulation_System.Two_Dim_System.mi2) + " ";
		AS += FloatToStr(Simulation_System.Two_Dim_System.b) + " ";

		AS += "n="+IntToStr(n)+" ";

		Simulation_System.Saved_Study_Filename = AS;
		ofstream df(Simulation_System.Saved_Study_Filename.c_str() );
		Simulation_System.save_object(&df);
		df.close();

	}

	} // all studies
*/
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Initiate_AF_ButtonClick(TObject *Sender)
{
	int Stim_Time = 500;
	double Stim_Probability = 0.25;

	long End_Time = Simulation_System.get_current_time() + Stim_Time;

	Simulation_System.clear_system();

	while( Simulation_System.get_current_time() < End_Time )
	{
			if( random( 100000 ) / 100000.0 < Stim_Probability )
			Simulation_System.stimulate_system(
						 random(100)/100.,
						 random(100)/100.,
						 double(Current_Z_Slice)/double(Simulation_System.Dimension_Z));

			Simulation_System.compute_N_steps(20);
			Simulation_System.refresh_VW_matrices();
			Simulation_System.compute_N_steps(20);
			Simulation_System.refresh_VW_matrices();
			Simulation_System.compute_N_steps(20);
			Simulation_System.refresh_VW_matrices();

			opengl_panel_paint(); // ????
			Application->ProcessMessages();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Test_CR_Button1Click(TObject *Sender)
{
	// set initial coupling
	Simulation_System.Two_Dim_System.set_initial_coupling();

	Initiate_AF_ButtonClick(this);

	CRs_ListBoxClick(this);

	Start_Sim_ButtonClick(this);

}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::ProcessfolderandcomputeCRs1Click(TObject *Sender)

{
	// Input: folder with saved studies

	// Output: files are re-saved with computed CR's

	// Parameters:
	ShowMessage("Set parameters in the source code.");

	// set CR search paremters (FURTHER DOWN %%%%%%%%%%%%%%%%%%%%%%)

	String Data_Files_Path;
	AnsiString caption;

	if( SelectDirectory("Select folder with saved studies", "", Data_Files_Path) )
	{

	Data_FileListBox->Clear();
	Data_FileListBox->Update();
	Data_FileListBox->ApplyFilePath( Data_Files_Path );
	Data_FileListBox->Update();
	Echo = false;

	//***************************************************************
	for(long f=0; f<Data_FileListBox->Items[0].Capacity; f++ )
//	if( !UT.is_substring_present((AnsiString)Data_FileListBox->Items[0].Strings[f] ,".csv") )
	//***************************************************************
	{

	caption = "Processing (" +IntToStr((int)f+1)+"/";
	caption += IntToStr(Data_FileListBox->Items[0].Capacity);
	caption += ")   " + Data_FileListBox->Items[0].Strings[f];
	Main_Screen_Form->Caption = caption;
	Application->ProcessMessages();

	Simulation_System.Saved_Study_Filename = Data_FileListBox->Items[0].Strings[f];
	LoadData1Click(this);

	// set CR search paremters (%%%%%%%%%%%%%%%%%%%%%%)
	Simulation_System.CR_Tests_Number = 3;
	Simulation_System.CR_Level = 3;
	Simulation_System.CR_Number = 2;
	Simulation_System.CR_Test_Time = 2000;
	load_interface_parameters_from_sim_system();

	// compute CR regions
	Begin_CR_Protocol_ButtonClick(this);

	int CR_No;
	if( Simulation_System.Which_System_Dimension == TWO_LAYER_SYSTEM_DIMENSION )
	CR_No = Simulation_System.TP1A_2L_system_CR_regions.size();
	if( Simulation_System.Which_System_Dimension == 2 )
	CR_No = Simulation_System.TP1A_2D_system_CR_regions.size();

	// save
	ofstream df((Simulation_System.Saved_Study_Filename + " CR_no="+IntToStr(CR_No)).c_str() );
	Simulation_System.save_object(&df);
	df.close();


	} // thru all files

	Echo = true;
	ShowMessage("Done");

  } // if dir selected


}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::N3Computeunipolarelectrograms1Click(TObject *Sender)

{
	long Recording_Time = 2000;

	// Input: folder with saved studies

	// Output: files are re-saved with computed electrograms

	double prev_time_s;

	// Parameters:
	ShowMessage("Set parameters in the source code.");

	// set CR search paremters (FURTHER DOWN %%%%%%%%%%%%%%%%%%%%%%)

	String Data_Files_Path;
	AnsiString caption;

	if( SelectDirectory("Select folder with saved studies", "", Data_Files_Path) )
	{

	Data_FileListBox->Clear();
	Data_FileListBox->Update();
	Data_FileListBox->ApplyFilePath( Data_Files_Path );
	Data_FileListBox->Update();
	Echo = false;

	//***************************************************************
	for(long f=0; f<Data_FileListBox->Items[0].Capacity; f++ )
	//***************************************************************
	{

	caption = "Processing (" +IntToStr((int)f+1)+"/";
	caption += IntToStr(Data_FileListBox->Items[0].Capacity);
	caption += ")   " + Data_FileListBox->Items[0].Strings[f];
	Main_Screen_Form->Caption = caption;
	Application->ProcessMessages();

	Simulation_System.Saved_Study_Filename = Data_FileListBox->Items[0].Strings[f];
	LoadData1Click(this);

	// set coupling to normal
	Simulation_System.Two_Dim_System.set_initial_coupling();

	// initiate AF
	Initiate_AF_ButtonClick(this);

	prev_time_s = Simulation_System.get_current_time();

	// simulate for specified length of time and record electrograms
	long End_Time = Simulation_System.get_current_time() + Recording_Time;
	bool Activity_Present = true;
	while( Activity_Present && Simulation_System.get_current_time() < End_Time )
	{
		Simulation_System.compute_N_steps(20);
		Simulation_System.refresh_VW_matrices();

		//-----------------------------------------------------
		// record potentials
		//-----------------------------------------------------
		if( Simulation_System.get_current_time() > prev_time_s +
			Simulation_System.Signal_Sampling_Timestep )
		{
			Simulation_System.record_potentials(Signal_Type_To_Record_RadioGroup->ItemIndex,Grid_Spacing_Edit->Text.ToInt());
			prev_time_s = Simulation_System.get_current_time();
		}

		// test if terminated
		Activity_Present = Simulation_System.activity_present_check();

		opengl_panel_paint(); // ????
		Application->ProcessMessages();
	}

	// save
	if(Activity_Present)
	{
		ofstream df((Simulation_System.Saved_Study_Filename + " with_electrograms").c_str() );
		Simulation_System.save_object(&df);
		df.close();
	}
	else
	{
		ofstream df((Simulation_System.Saved_Study_Filename + " SELF_TERMINATED").c_str() );
		Simulation_System.save_object(&df);
		df.close();
	}


	} // thru all files

	Echo = true;
	ShowMessage("Done");

  } // if dir selected

}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::N4Computeelectrogramparameters1Click(TObject *Sender)

{
	String Data_Files_Path;
	AnsiString caption;
	vector <double> Vmean_CR,Vmean_nCR;
	vector <double> VSD_CR,VSD_nCR;
	vector <double> Causality_CR,Causality_nCR;

	int Grid_Sampling = 4; // every 4th electrogram will be taken

	// attention! causality will take time to compute!!!
	bool Include_Causality = true;
	int Word_Length = 3;

	if( SelectDirectory("Select folder with saved studies", "", Data_Files_Path) )
	{

	Data_FileListBox->Clear();
	Data_FileListBox->Update();
	Data_FileListBox->ApplyFilePath( Data_Files_Path );
	Data_FileListBox->Update();
	Echo = false;

	double ISI_mean,ISI_SD;

	ofstream df("Electrogram_analysis.csv");
	df << "Filename,CR_ISI_Mean,CR_ISI_SD,nCR_ISI_Mean,nCR_ISI_SD,CR_Causality_Score,nCR_Causality_Score" << endl;

	//***************************************************************
	for(long f=0; f<Data_FileListBox->Items[0].Capacity; f++ )
	//***************************************************************
	{

	caption = "Processing (" +IntToStr((int)f+1)+"/";
	caption += IntToStr(Data_FileListBox->Items[0].Capacity);
	caption += ")   " + Data_FileListBox->Items[0].Strings[f];
	Main_Screen_Form->Caption = caption;
	Application->ProcessMessages();

	Simulation_System.Saved_Study_Filename = Data_FileListBox->Items[0].Strings[f];
	LoadData1Click(this);

	Vmean_CR.clear();
	VSD_CR.clear();
	Vmean_nCR.clear();
	VSD_nCR.clear();
	Causality_CR.clear();
	Causality_nCR.clear();

	if( Include_Causality )
	compute_global_causality_map(Grid_Sampling,Word_Length);

	for(int i=Grid_Sampling;i<Simulation_System.Two_Dim_System.Size_X;i+=Grid_Sampling)
	for(int j=Grid_Sampling;j<Simulation_System.Two_Dim_System.Size_Y;j+=Grid_Sampling)
	{
		Simulation_System.compute_ISIs_XY(i,j);
		NumAlg.stdev_cor_vec_long(&Simulation_System.Two_Dim_System.ISIs,&ISI_mean,&ISI_SD);

		// check if point in CR region
		int CR_Region_Flag = 0;
		for(int r=0;r<Simulation_System.TP1A_2D_system_CR_regions.size();r++)
		for(int rr=0;rr<Simulation_System.TP1A_2D_system_CR_regions[r].X.size();rr++)
		if( std::abs(i - Simulation_System.TP1A_2D_system_CR_regions[r].X[rr])  < Simulation_System.TP1A_2D_system_CR_regions[r].R[rr])
		if( std::abs(j - Simulation_System.TP1A_2D_system_CR_regions[r].Y[rr])  < Simulation_System.TP1A_2D_system_CR_regions[r].R[rr])
		CR_Region_Flag = 1;

		if( CR_Region_Flag == 1 )
		{
			Vmean_CR.push_back(ISI_mean);
			  VSD_CR.push_back(ISI_SD);
			 Causality_CR.push_back(Simulation_System.Two_Dim_System.CUSTOM_VALUE[i][j]);

		}
		else
		{
			Vmean_nCR.push_back(ISI_mean);
			  VSD_nCR.push_back(ISI_SD);
			Causality_nCR.push_back(Simulation_System.Two_Dim_System.CUSTOM_VALUE[i][j]);
		}

	}

	df << Simulation_System.Saved_Study_Filename.c_str() << ",";

	NumAlg.stdev_cor_vec_double(&Vmean_CR,&ISI_mean,&ISI_SD);
	df << ISI_mean << ",";

	NumAlg.stdev_cor_vec_double(&VSD_CR,&ISI_mean,&ISI_SD);
	df << ISI_mean << ",";

	NumAlg.stdev_cor_vec_double(&Vmean_nCR,&ISI_mean,&ISI_SD);
	df << ISI_mean << ",";

	NumAlg.stdev_cor_vec_double(&VSD_nCR,&ISI_mean,&ISI_SD);
	df << ISI_mean << ",";

	NumAlg.stdev_cor_vec_double(&Causality_CR,&ISI_mean,&ISI_SD);
	df << ISI_mean << ",";

	NumAlg.stdev_cor_vec_double(&Causality_nCR,&ISI_mean,&ISI_SD);
	df << ISI_mean << ",";

	df << endl;

	} // thru all files

	df.close();

	Echo = true;
	ShowMessage("Done");

  } // if dir selected
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::ComputeElectrogramParameters1Click(TObject *Sender)
{

	double ISI_mean,ISI_SD;
	int Grid_Sampling = 1;

	for(int i=0;i<Simulation_System.Two_Dim_System.Size_X;i+=Grid_Sampling)
	for(int j=0;j<Simulation_System.Two_Dim_System.Size_Y;j+=Grid_Sampling)
	{
		Simulation_System.compute_ISIs_XY(i,j);
		NumAlg.stdev_cor_vec_long(&Simulation_System.Two_Dim_System.ISIs,&ISI_mean,&ISI_SD);

		Simulation_System.Two_Dim_System.CUSTOM_VALUE[i][j] = ISI_mean;

	}

}

//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::ComputeSDofcyclelength1Click(TObject *Sender)
{

	double ISI_mean,ISI_SD;
	int Grid_Sampling = 1;

	for(int i=0;i<Simulation_System.Two_Dim_System.Size_X;i+=Grid_Sampling)
	for(int j=0;j<Simulation_System.Two_Dim_System.Size_Y;j+=Grid_Sampling)
	{
		Simulation_System.compute_ISIs_XY(i,j);
		NumAlg.stdev_cor_vec_long(&Simulation_System.Two_Dim_System.ISIs,&ISI_mean,&ISI_SD);

		Simulation_System.Two_Dim_System.CUSTOM_VALUE[i][j] = ISI_SD;

	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Causalitytest1Click(TObject *Sender)
{

	vector <long> SenderS,Receiver;
	int Word_Length = 3;

	//  + - + + - - + - -
	SenderS.push_back(10);
	SenderS.push_back(21);
	SenderS.push_back(33);
	SenderS.push_back(40);
	SenderS.push_back(50);
	SenderS.push_back(61);
	SenderS.push_back(70);
	SenderS.push_back(78);
	SenderS.push_back(90);
	SenderS.push_back(100);
	SenderS.push_back(109);

	//  + + - + + - + - -
	Receiver.push_back(10);
	Receiver.push_back(20);
	Receiver.push_back(31);
	Receiver.push_back(43);
	Receiver.push_back(50);
	Receiver.push_back(60);
	Receiver.push_back(71);
	Receiver.push_back(80);
	Receiver.push_back(96);
	Receiver.push_back(100);
	Receiver.push_back(103);

	int Words_Send = NumAlg.get_number_of_words_causality_alphabet_method(&SenderS,
				&Receiver,Word_Length);

	ShowMessage("Words_Send: " + IntToStr(Words_Send));

}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Pushas1stsignal1Click(TObject *Sender)
{
	// display_single_signal_from_history_array(Ratio_X,Ratio_Y,1);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Pushas2ndsignal1Click(TObject *Sender)
{
	// display_single_signal_from_history_array(Ratio_X,Ratio_Y,2);

	// compute causality

}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Computecausalitymap1Click(TObject *Sender)
{
//	ShowMessage("Remember to point the origin point (right mouse click + push as...) ");

	Ask_For_Single_Item_Form1->Caption = "Word length";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "3";
	Ask_For_Single_Item_Form1->ShowModal();
	int Word_Length = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

	vector <long> SenderS,Receiver;

	Simulation_System.compute_ISIs_XY(
		Simulation_System.Dimension_X*Ratio_X,
		Simulation_System.Dimension_Y*Ratio_Y);
	SenderS = Simulation_System.Two_Dim_System.Activations_Positions;

	for(int i=0;i<Simulation_System.Two_Dim_System.Size_X;i++)
	for(int j=0;j<Simulation_System.Two_Dim_System.Size_Y;j++)
	{

	Simulation_System.compute_ISIs_XY(i,j);
	Receiver = Simulation_System.Two_Dim_System.Activations_Positions;

	Simulation_System.Two_Dim_System.CUSTOM_VALUE[i][j] =
		NumAlg.get_number_of_words_causality_alphabet_method(&SenderS,
			&Receiver,Word_Length);

	}

	Simulation_System.compute_min_max_custom_value();

	ShowMessage("Done. Max words send: " + IntToStr((int)Simulation_System.Two_Dim_System.Max_Custom_Value));

	opengl_panel_paint();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Computeglobalcausalitymaplongcomputations1Click(TObject *Sender)

{
	int Word_Length = 3;
	int Grid_Sampling = 5;

	if( Echo )
	{
		Ask_For_Single_Item_Form1->Caption = "Word length";
		Ask_For_Single_Item_Form1->Edit_Box->Text = "3";
		Ask_For_Single_Item_Form1->ShowModal();
		Word_Length = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();
	}

	if( Echo )
	{
		Ask_For_Single_Item_Form1->Caption = "Grid sampling";
		Ask_For_Single_Item_Form1->Edit_Box->Text = "5";
		Ask_For_Single_Item_Form1->ShowModal();
		Grid_Sampling = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();
	}

	compute_global_causality_map(Grid_Sampling,Word_Length);


	if( Echo )
	ShowMessage("Done. Max value: " + IntToStr((int)Simulation_System.Two_Dim_System.Max_Custom_Value));

}

//---------------------------------------------------------------------------

void TMain_Screen_Form::compute_global_causality_map(int Grid_Sampling, int Word_Length)
{

	vector <long> SenderS,Receiver;

	// clear custom map (with global causality parameter)
	for(int i=0;i<Simulation_System.Two_Dim_System.Size_X;i++)
	for(int j=0;j<Simulation_System.Two_Dim_System.Size_Y;j++)
	Simulation_System.Two_Dim_System.CUSTOM_VALUE[i][j] = 0;

	//----------------------------------------------------------------
	// MASTER LOOP
	//----------------------------------------------------------------
	for(int ci=0;ci<Simulation_System.Two_Dim_System.Size_X;ci+=Grid_Sampling)
	for(int cj=0;cj<Simulation_System.Two_Dim_System.Size_Y;cj+=Grid_Sampling)
	{

	Main_Screen_Form->Caption =  "Processing (" +IntToStr((int)ci)+"/"+IntToStr((int)Simulation_System.Two_Dim_System.Size_X)+")";
	// Application->ProcessMessages();

	// clear custom map
	for(int i=0;i<Simulation_System.Two_Dim_System.Size_X;i++)
	for(int j=0;j<Simulation_System.Two_Dim_System.Size_Y;j++)
	Simulation_System.Two_Dim_System.CUSTOM_VALUE_2[i][j] = 0;

	// prepare sender
	Simulation_System.compute_ISIs_XY(ci,cj);
	SenderS = Simulation_System.Two_Dim_System.Activations_Positions;

	// go through rest of the map
	for(int i=0;i<Simulation_System.Two_Dim_System.Size_X;i+=Grid_Sampling)
	for(int j=0;j<Simulation_System.Two_Dim_System.Size_Y;j+=Grid_Sampling)
	{

		Simulation_System.compute_ISIs_XY(i,j);
		Receiver = Simulation_System.Two_Dim_System.Activations_Positions;

		Simulation_System.Two_Dim_System.CUSTOM_VALUE_2[i][j] =
			NumAlg.get_number_of_words_causality_alphabet_method(&SenderS,
			&Receiver,Word_Length);
	}

	// sum up the custom map
	double Sum=0;
	for(int i=0;i<Simulation_System.Two_Dim_System.Size_X;i++)
	for(int j=0;j<Simulation_System.Two_Dim_System.Size_Y;j++)
	Sum += Simulation_System.Two_Dim_System.CUSTOM_VALUE_2[i][j];

	// assign value
	Simulation_System.Two_Dim_System.CUSTOM_VALUE[ci][cj] = Sum;

	} // master loop

	Simulation_System.compute_min_max_custom_value();

	opengl_panel_paint();

}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::AblateaccordingtoCL1Click(TObject *Sender)
{
	Ask_For_Single_Item_Form1->Caption = "Top% threshold";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "10";
	Ask_For_Single_Item_Form1->ShowModal();
	double TopTh = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

	// 1. Compute CL map
	ComputeElectrogramParameters1Click(this);
	Simulation_System.compute_min_max_custom_value();

	// 2. set threshold for ablation
	double Threshold =
		Simulation_System.Two_Dim_System.Min_Custom_Value +
		TopTh*(Simulation_System.Two_Dim_System.Max_Custom_Value-
			   Simulation_System.Two_Dim_System.Min_Custom_Value)/100.;

	// 3. ablate
	for(int i=0;i<Simulation_System.Two_Dim_System.Size_X;i++)
	for(int j=0;j<Simulation_System.Two_Dim_System.Size_Y;j++)
	if( Simulation_System.Two_Dim_System.CUSTOM_VALUE[i][j] < Threshold )
	Simulation_System.ablate_system_xy_point(i,j,1);

}

//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::ExportVoltageHistorytextfile1Click(TObject *Sender)
{
	if( SaveDialog1->Execute() )
	{
		Simulation_System.export_voltage_history(SaveDialog1->FileName,"voltage variable");
		opengl_panel_paint();
		ShowMessage("Done");
	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Generatesimulationfile1Click(TObject *Sender)
{
	double Simulation_Time = 6000;
	double End_Time = Simulation_System.get_current_time() + Simulation_Time;
	double prev_time_s = Simulation_System.get_current_time();
	Simulation_System.Signal_Sampling_Timestep = 0.5;
	long Signal_Length=0;

	double Spatial_Sampling = 2;
	ofstream df("tmp_filename");

	while( Simulation_System.get_current_time() < End_Time )
	{
		Simulation_System.compute_N_steps(25);
		Simulation_System.refresh_VW_matrices();

		//-----------------------------------------------------
		// record potentials
		//-----------------------------------------------------
		if( Simulation_System.get_current_time() >= prev_time_s +
			Simulation_System.Signal_Sampling_Timestep )
		{
			Simulation_System.record_potentials(Signal_Type_To_Record_RadioGroup->ItemIndex,Grid_Spacing_Edit->Text.ToInt());
			Signal_Length++;

			for(int x=0;x<(double)TWO_DIM_SYSTEM_X_SIZE;x+=Spatial_Sampling)
			for(int y=0;y<(double)TWO_DIM_SYSTEM_Y_SIZE;y+=Spatial_Sampling)
			{
				df << (double)((int)(Simulation_System.Two_Dim_System.VOLTAGE_HISTORY[x][y].DVector[0]*100))/100 << ",";
				Simulation_System.Two_Dim_System.VOLTAGE_HISTORY[x][y].DVector.clear();
			}

			df << endl;

			prev_time_s = Simulation_System.get_current_time();

			Time_Label->Caption = "Time: "+IntToStr((int)Simulation_System.get_current_time());

			opengl_panel_paint(); // ????
			Application->ProcessMessages();
		}

	}


	// save study
/*
	AnsiString AS = "4Dstudy "+IntToStr(TWO_DIM_SYSTEM_X_SIZE)+"x"+IntToStr(TWO_DIM_SYSTEM_Y_SIZE)+" ";
		AS += IntToStr(InhVar_RadioGroup->ItemIndex)+ " ";
		AS += RL_Edit->Text + " ";
		AS += ppL_Edit->Text + " ";
		AS += IL_Edit->Text + " ";
		AS += mi_min_Edit->Text + " ";
		AS += mi_max_Edit->Text + " ";

		AS += FloatToStr(Simulation_System.Two_Dim_System.a) + " ";
		AS += FloatToStr(Simulation_System.Two_Dim_System.k_par) + " ";
		AS += FloatToStr(Simulation_System.Two_Dim_System.eps) + " ";
		AS += FloatToStr(Simulation_System.Two_Dim_System.mi1) + " ";
		AS += FloatToStr(Simulation_System.Two_Dim_System.mi2) + " ";
		AS += FloatToStr(Simulation_System.Two_Dim_System.b) + " ";

		Simulation_System.Saved_Study_Filename = AS;
		ofstream df(Simulation_System.Saved_Study_Filename.c_str() );
		Simulation_System.save_object(&df);
		df.close();
*/

// Simulation_System.export_voltage_history(SaveDialog1->FileName,"voltage variable");


	df.close();

	AnsiString FF = "Voltage_Values " + FloatToStr(TWO_DIM_SYSTEM_X_SIZE/Spatial_Sampling) + "x"+ FloatToStr(TWO_DIM_SYSTEM_Y_SIZE/Spatial_Sampling)+"x"+ FloatToStr(Signal_Length) + ".csv";

	rename("tmp_filename",FF.c_str());

	ShowMessage("Done");

}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::GeneratesimulationfilePLANARWAVES1Click(TObject *Sender)

{
	double Simulation_Time = 6000;
	double End_Time = Simulation_System.get_current_time() + Simulation_Time;
	double prev_time_s = Simulation_System.get_current_time();
	Simulation_System.Signal_Sampling_Timestep = 0.5;
	long Signal_Length=0;

	long PACING_PERIOD = 30, Last_Pacing_Timepoint=-PACING_PERIOD;

	double Spatial_Sampling = 2;
	ofstream df("tmp_filename");

	while( Simulation_System.get_current_time() < End_Time )
	{
		Simulation_System.compute_N_steps(25);
		Simulation_System.refresh_VW_matrices();

		if( Simulation_System.get_current_time() - Last_Pacing_Timepoint >= PACING_PERIOD )
		{
			Simulation_System.stimulate_edge(1);
			Simulation_System.stimulate_edge(2);
			Last_Pacing_Timepoint = Simulation_System.get_current_time();
		}

		//-----------------------------------------------------
		// record potentials
		//-----------------------------------------------------
		if( Simulation_System.get_current_time() >= prev_time_s +
			Simulation_System.Signal_Sampling_Timestep )
		{
			Simulation_System.record_potentials(Signal_Type_To_Record_RadioGroup->ItemIndex,Grid_Spacing_Edit->Text.ToInt());
			Signal_Length++;

			for(int x=0;x<(double)TWO_DIM_SYSTEM_X_SIZE;x+=Spatial_Sampling)
			for(int y=0;y<(double)TWO_DIM_SYSTEM_Y_SIZE;y+=Spatial_Sampling)
			{
				df << (double)((int)(Simulation_System.Two_Dim_System.VOLTAGE_HISTORY[x][y].DVector[0]*100))/100 << ",";
				Simulation_System.Two_Dim_System.VOLTAGE_HISTORY[x][y].DVector.clear();
			}

			df << endl;

			prev_time_s = Simulation_System.get_current_time();

			Time_Label->Caption = "Time: "+IntToStr((int)Simulation_System.get_current_time());

			opengl_panel_paint(); // ????
			Application->ProcessMessages();
		}

	}

	df.close();

	AnsiString FF = "Voltage_Values " + FloatToStr(TWO_DIM_SYSTEM_X_SIZE/Spatial_Sampling) + "x"+ FloatToStr(TWO_DIM_SYSTEM_Y_SIZE/Spatial_Sampling)+"x"+ FloatToStr(Signal_Length) + ".csv";

	rename("tmp_filename",FF.c_str());

	ShowMessage("Done");

}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Mimi2_Disk_ButtonClick(TObject *Sender)
{
	if( Simulation_System.Which_System_Dimension == 2 )
	Simulation_System.add_mi1_mi2_R_disk(
						mi_min_Edit->Text.ToDouble(),
						mi_max_Edit->Text.ToDouble(),
						RL_Edit->Text.ToDouble(),
						InhVar_RadioGroup->ItemIndex );

	opengl_panel_paint();

}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::History_ScrollBarChange(TObject *Sender)
{
	paint_slice();
	opengl_panel_paint();

	// paint_slice();
	//Application->ProcessMessages();
	//opengl_panel_paint();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Panel_1MouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	MouseButton= Button;
	MouseStatus = MB_DOWN1;

	MousePt.s.X = X;
	MousePt.s.Y = Y;

		if( Simulation_System.Dimension_X > Simulation_System.Dimension_Y )
		Ratio_X = double(X)/(double)(Panel_1->Width);
		else
		Ratio_X = double(X)/
			((double)Simulation_System.Dimension_X*(double)Panel_1->Width/
				Simulation_System.Dimension_Y );

		if( Simulation_System.Dimension_Y > Simulation_System.Dimension_X )
		Ratio_Y = double(Y)/(double)(Panel_1->Height);
		else
		Ratio_Y = double(Y)/
			((double)Simulation_System.Dimension_Y*(double)Panel_1->Height/
				Simulation_System.Dimension_X );

	//------------------------------------
	// --- LMB action
	//----------------------------------------
	if( MouseButton == mbLeft )
//	&& OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.OpenGL_Panel_Dragging_Flag == OFF)
	{
		if( LMB_Action_RadioGroup->ItemIndex == 0 )
		Simulation_System.stimulate_system(
						 Ratio_X,
						 Ratio_Y, // double(Y)/(double)(Panel_1->Height),
						 double(Current_Z_Slice)/double(Simulation_System.Dimension_Z));

		if( LMB_Action_RadioGroup->ItemIndex == 1 )
		Simulation_System.ablate_system(
						 Ratio_X,
						 Ratio_Y,
						 double(Current_Z_Slice)/double(Simulation_System.Dimension_Z),+1 );

		if( LMB_Action_RadioGroup->ItemIndex == 2 )
		{
			get_corrected_pointed_location(Ratio_X,Ratio_Y,&Pointed_Spot_i,&Pointed_Spot_j);
			display_signals_from_history_array();
		}

		if( LMB_Action_RadioGroup->ItemIndex == 3 )
		{
			mark_line_point(Ratio_X,Ratio_Y);
			display_signals_from_history_array();
		}
	}
	//------------------------------------
	// --- LMB action
	//----------------------------------------
	if( MouseButton == mbRight )
	{

		if( LMB_Action_RadioGroup->ItemIndex == 1 )
		Simulation_System.ablate_system(
						 Ratio_X,
						 Ratio_Y,
						 double(Current_Z_Slice)/double(Simulation_System.Dimension_Z),-1 );
	}

	//------------------------------------
	// --- ARC BALL ROTATION SECTION
	//----------------------------------------
	if(is_Map_Panel_PopupMenu_item_tag_checked(MAP_POPUP_ITEM_ROTATE) == ON)
	if( MouseButton == mbLeft && OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.OpenGL_Panel_Dragging_Flag == OFF)
	{
		LastRot = ThisRot;
		ArcBall.click(&MousePt);
	}

	opengl_panel_paint();
}

//---------------------------------------------------------------------------

void TMain_Screen_Form::get_corrected_pointed_location(double Ratio_X,double Ratio_Y,int *i_coord,int *j_coord)
{
	// correct pointed element so that it contains non zero voltage history vectors (not all grid elements store history)
	int Spacing = Grid_Spacing_Edit->Text.ToInt();
	int Size,S2 = Spacing/2.;
	bool Found = false;

	double DX = Simulation_System.Dimension_X;
	double DY = Simulation_System.Dimension_Y;

	i_coord[0] = Ratio_X*DX;
	j_coord[0] = Ratio_Y*DY;

	int Counter=0;
	for(int s1=-S2;s1<=S2;s1++)
	for(int s2=-S2;s2<=S2;s2++)
	{
		int Size = Simulation_System.get_voltage_history_vector_size(i_coord[0]+s1,j_coord[0]+s2);

		if( !Found && Size > 1 )
		{
			i_coord[0] += s1;
			j_coord[0] += s2;
			Found = true;
		}
	}
}

//---------------------------------------------------------------------------

void TMain_Screen_Form::mark_line_point(double Ratio_X,double Ratio_Y)
{
	Simulation_System.mark_line_point(Ratio_X*Simulation_System.Dimension_X,Ratio_Y*Simulation_System.Dimension_Y);
	opengl_panel_paint();
}

//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::FormCreate(TObject *Sender)
{
	OpenGL_Panel_1.OpenGL_Panel = Panel_1;
	OpenGL_Panel_1.initialize();
	OpenGL_Panel_1.set_NavX_AP_view();
	OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Read_Palette();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::FormDestroy(TObject *Sender)
{
	OpenGL_Panel_1.delete_contexts();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Panel_1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	MouseButton = Button;
	MouseStatus = MB_UP1;
	OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.OpenGL_Panel_Dragging_Flag = OFF;
	repaint_all_controls();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Panel_1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
	Quat4fT     ThisQuat;

	//------------------------------------
	// --- ARC BALL ROTATION SECTION
	//----------------------------------------
	if(is_Map_Panel_PopupMenu_item_tag_checked(MAP_POPUP_ITEM_ROTATE) == ON)
	if (MouseButton == mbLeft && MouseStatus == MB_DOWN )
	{

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.OpenGL_Panel_Dragging_Flag == OFF)
	{
	OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.OpenGL_Panel_Dragging_Flag = ON;

		LastRot = ThisRot;			// Set Last Static Rotation To Last Dynamic One
		ArcBall.click(&MousePt);	// Update Start Vector And Prepare For Dragging
	} // dragging == OFF


	if(OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.OpenGL_Panel_Dragging_Flag == ON)
	{
		MousePt.s.X = X;
		MousePt.s.Y = Y;

		// Core of rotation
		ArcBall.drag(&MousePt, &ThisQuat);
		Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);
		Matrix3fMulMatrix3f(&ThisRot, &LastRot);
		Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);

		opengl_panel_paint();
	} // dragging == ON

	} // arc ball rotation

	//------------------------------------
	// --- ZOOM
	//----------------------------------------
	if(is_Map_Panel_PopupMenu_item_tag_checked(MAP_POPUP_ITEM_ZOOM) == ON)
	if (MouseButton == mbLeft && MouseStatus == MB_DOWN )
	{
		if (X > MousePt.s.X )
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ZoomFactor += (GLdouble)(X-MousePt.s.X) * (GLdouble) 1.0;
		if (X < MousePt.s.X )
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ZoomFactor -= (GLdouble)(MousePt.s.X-X) * (GLdouble) 1.0;
		if (X > MousePt.s.Y )
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ZoomFactor += (GLdouble)(Y-MousePt.s.Y) * (GLdouble) 1.0;
		if (X < MousePt.s.Y )
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ZoomFactor -= (GLdouble)(MousePt.s.Y-Y) * (GLdouble) 1.0;

		MousePt.s.X = X;
		MousePt.s.Y = Y;

		opengl_panel_paint();
	} // zoom

	//------------------------------------
	// PAN
	//------------------------------------
	if(is_Map_Panel_PopupMenu_item_tag_checked(MAP_POPUP_ITEM_PAN) == ON)
	if (MouseButton == mbLeft && MouseStatus == MB_DOWN )
	{
		if (X > MousePt.s.X ) OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Translation_X += (X-MousePt.s.X)/8.0;
		if (X < MousePt.s.X ) OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Translation_X -= (MousePt.s.X-X)/8.0;
		if (Y > MousePt.s.Y ) OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Translation_Y -= (Y-MousePt.s.Y)/8.0;
		if (Y < MousePt.s.Y ) OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Translation_Y += (MousePt.s.Y-Y)/8.0;

		MousePt.s.X = X;
		MousePt.s.Y = Y;

		opengl_panel_paint();
	} // pan

	//--------------------------------------
	// "Encircling area" selected
	//--------------------------------------
	if( is_Map_Panel_PopupMenu_item_tag_checked(MAP_POPUP_ITEM_ENCIRCLING) == ON)
	if (MouseButton == mbLeft && MouseStatus == MB_DOWN )
	{
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.OpenGL_Panel_Dragging_Flag = ON;
		opengl_panel_paint();
	}
}
//---------------------------------------------------------------------------

void TMain_Screen_Form::paint_slice()
{
	double x,y,z,ptr;
	double DX,DY;
	AnsiString Label, Unit;
	double Max = History_ScrollBar->Max;
	double Pos = History_ScrollBar->Position;
	long Time_Ptr = Pos/Max * Simulation_System.get_history_vector_size();

	Time_Label->Caption = "Time: "+IntToStr((int)(Time_Ptr)) + "/" + FloatToStr(Simulation_System.get_history_vector_size());
	Timer_Slider_Label->Caption = "Time: "+IntToStr((int)(Time_Ptr)) + "/" + FloatToStr(Simulation_System.get_history_vector_size());

	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, (double)Panel_1->Width, 0,
				  (double)Panel_1->Height);

	int Source = Display_Value_RadioGroup->ItemIndex;

	// gogh
	// Turn off lighting (to avoid effects from the light source position)
	glDisable(GL_LIGHTING);

	Simulation_System.compute_min_max_custom_value();

	DX = Simulation_System.Dimension_X;
	DY = Simulation_System.Dimension_Y;

	// point grid around pointed element

	//======================================================================
	// conduction arrows xxx
	//======================================================================

	double Grid_Size = HD_GRID_SIZE; // this is how many electrodes form grid (e.g. 4x4)(set in Program_Constants.h)
	double Grid_Spacing = Grid_Spacing_Edit->Text.ToInt(); // how many simulation mesh elements between grid electrodes
	int RR1=10;
	double length,x1,x2,y1,y2,cx,cy,arrow_scale=Arrow_Size_Edit->Text.ToDouble();
	VelocityVector_2D arrow;
	std::vector<long> Domains_Ids;
	RGBColor domain_color;
	int Arrow_Mesh_Spacing = Arrow_Mesh_Spacing_Edit->Text.ToDouble();

	// assign different color to each domain
	long domain_ptr,max_domain_id = Simulation_System.get_max_activation_time_id();

	glLineWidth(2.0f);

	if( CV_Vectors_Method_RadioGroup->ItemIndex == 0 ) // based on local activations
	{

	// when there is no arrow displayed at beginning or end it's because get_conduction_arrow tries to place Time_Ptr between two activations
	for(int i=0;i<DX;i+=Arrow_Mesh_Spacing)
	for(int j=0;j<DY;j+=Arrow_Mesh_Spacing)
	{
		x = (double)i/DX*(double)Panel_1->Width;
		y = (double)j/DY*(double)Panel_1->Height;

		arrow = Simulation_System.get_conduction_arrow(i,j,Current_Z_Slice,Time_Ptr);

		domain_ptr = Simulation_System.get_domain_ptr(i,j,Current_Z_Slice,Time_Ptr);

		if( arrow.amplitude > 0 ) // && domain_ptr >= 0 && domain_ptr < Simulation_System.Domains.size() )
		{
			// glColor4f( 1,1,1,0);
			glColor4f( 0,0,0,0);

			if( domain_ptr >= 0 && domain_ptr < Simulation_System.Domains.size() )
			{
				Domains_Ids.push_back(Simulation_System.Domains[domain_ptr].Domain_Id);
				domain_color = Simulation_System.Domains[domain_ptr].RGB_Color;

				if( Simulation_System.Domains[domain_ptr].Domain_Id > 0 )
				glColor4f( domain_color.r/255.,domain_color.g/255.,domain_color.b/255.,0);
			}

			arrow.amplitude = 1;

			length = arrow_scale*arrow.amplitude;

			x2 = x - length*cos(arrow.direction);
			y2 = y - length*sin(arrow.direction);
			x1 = x + length*cos(arrow.direction);
			y1 = y + length*sin(arrow.direction);
			glBegin(GL_LINES);

			// shaft
			glVertex3f(x1,Panel_1->Height-y1,0);
			glVertex3f(x2,Panel_1->Height-y2,0);

			// tip
			glVertex3f(x2,Panel_1->Height-y2,0);
			glVertex3f(x2 + arrow_scale*arrow.amplitude*cos(arrow.direction + 0.2),
						Panel_1->Height-(y2 + arrow_scale*arrow.amplitude*sin(arrow.direction + 0.2)),0);

			glVertex3f(x2,Panel_1->Height-y2,0);
			glVertex3f(x2 + arrow_scale*arrow.amplitude*cos(arrow.direction - 0.2),
						Panel_1->Height-(y2 + arrow_scale*arrow.amplitude*sin(arrow.direction - 0.2)),0);

			glEnd();
		}
		else
        if( i>10 && j>10)
        int df=34;
	}
	} // vectors at local activation times

	if( CV_Vectors_Method_RadioGroup->ItemIndex == 1 ) // based just on phase
	{

	// when there is no arrow displayed at beginning or end it's because get_conduction_arrow tries to place Time_Ptr between two activations
	for(int i=0;i<DX;i+=Arrow_Mesh_Spacing)
	for(int j=0;j<DY;j+=Arrow_Mesh_Spacing)
	{
		x = (double)i/DX*(double)Panel_1->Width;
		y = (double)j/DY*(double)Panel_1->Height;

		arrow.direction = Simulation_System.get_phase(i,j,Time_Ptr);
		arrow.amplitude = 1;

		domain_ptr = Simulation_System.get_domain_ptr(i,j,Current_Z_Slice,Time_Ptr);

		// glColor4f( 1,1,1,0);
		glColor4f( 0,0,0,0);

		if( domain_ptr >= 0 && domain_ptr < Simulation_System.Domains.size() )
		{
			Domains_Ids.push_back(Simulation_System.Domains[domain_ptr].Domain_Id);
			domain_color = Simulation_System.Domains[domain_ptr].RGB_Color;

			if( Simulation_System.Domains[domain_ptr].Domain_Id > 0 )
			glColor4f( domain_color.r/255.,domain_color.g/255.,domain_color.b/255.,0);
		}

		length = arrow_scale*arrow.amplitude;

		x2 = x - length*cos(arrow.direction);
		y2 = y - length*sin(arrow.direction);
		x1 = x + length*cos(arrow.direction);
		y1 = y + length*sin(arrow.direction);
		glBegin(GL_LINES);

		// shaft
		glVertex3f(x1,Panel_1->Height-y1,0);
		glVertex3f(x2,Panel_1->Height-y2,0);

		// tip
		glVertex3f(x2,Panel_1->Height-y2,0);
		glVertex3f(x2 + arrow_scale*arrow.amplitude*cos(arrow.direction + 0.2),
					Panel_1->Height-(y2 + arrow_scale*arrow.amplitude*sin(arrow.direction + 0.2)),0);

		glVertex3f(x2,Panel_1->Height-y2,0);
		glVertex3f(x2 + arrow_scale*arrow.amplitude*cos(arrow.direction - 0.2),
					Panel_1->Height-(y2 + arrow_scale*arrow.amplitude*sin(arrow.direction - 0.2)),0);

		glEnd();
	}
	} // vectors at local activation times


/*
	// display list of present domains
	std::set<double> uniqueNumbers(Domains_Ids.begin(), Domains_Ids.end());
	std::vector<double> Unique_Domains_Ids(uniqueNumbers.begin(), uniqueNumbers.end());

	Main_Control_Page_RichEdit->Lines[0].Clear();
	Main_Control_Page_RichEdit->Lines[0].Add("Domains present at current time point:");
	for(int q=0;q<Unique_Domains_Ids.size();q++)
	Main_Control_Page_RichEdit->Lines[0].Add(FloatToStr(Unique_Domains_Ids[q]));
*/
	//--------------------------------------------
	// grid at selected point
	//--------------------------------------------
	glBegin(GL_QUADS);
	if( Pointed_Spot_i > 0 && Pointed_Spot_j > 0 )
	for(int s1=0;s1<Grid_Size;s1++)
	for(int s2=0;s2<Grid_Size;s2++)
	{
		cx = Pointed_Spot_i + s1*Grid_Spacing;
		cy = Pointed_Spot_j + s2*Grid_Spacing;

		x = cx/DX*(double)Panel_1->Width;
		y = cy/DY*(double)Panel_1->Height;

		glColor4f( 0.2,0.2,0.2,0);
		glVertex3f( x-RR1,   Panel_1->Height-y-RR1, 0 );
		glVertex3f( x+RR1,   Panel_1->Height-y-RR1, 0 );
		glVertex3f( x+RR1,   Panel_1->Height-y+RR1, 0 );
		glVertex3f( x-RR1,   Panel_1->Height-y+RR1, 0 );
	}
	glEnd();

/*
	// pointed element
	glColor4f( 0.1,0.1,0,0);
	glBegin(GL_QUADS);
	RR1=10;
	glBegin(GL_QUADS);

	x = Ratio_X*(double)Panel_1->Width;
	y = Ratio_Y*(double)Panel_1->Height;

	glVertex3f( x-RR1,   Panel_1->Height-y-RR1, 0 );
	glVertex3f( x+RR1,   Panel_1->Height-y-RR1, 0 );
	glVertex3f( x+RR1,   Panel_1->Height-y+RR1, 0 );
	glVertex3f( x-RR1,   Panel_1->Height-y+RR1, 0 );

	glEnd();     Grid_Spacing_Edit->Text.ToInt()
*/

	//--------------------------------------------
	// paint simulation system
	//--------------------------------------------
	glBegin(GL_QUADS);
	for(int i=0;i<DX;i++)
	for(int j=0;j<DY;j++)
	{
		x = (double)i/DX*(double)Panel_1->Width;
		y = (double)j/DY*(double)Panel_1->Height;
		ptr= Simulation_System.get_color_code(Source,i,j,Current_Z_Slice,Current_T_Slice,Time_Ptr,Grid_Spacing_Edit->Text.ToInt(),Basic_CL_Edit->Text.ToDouble());
		if( ptr == -1 ) glColor4f(0,0,0,0); else
		glColor4f( 1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][2]/255.0,0);
		glVertex3f( x, Panel_1->Height-y, 0 );

		x = (double)(i+1)/DX*(double)Panel_1->Width;
		y = (double)j    /DY*(double)Panel_1->Height;
		ptr= Simulation_System.get_color_code(Source,i,j,Current_Z_Slice,Current_T_Slice,Time_Ptr,Grid_Spacing_Edit->Text.ToInt(),Basic_CL_Edit->Text.ToDouble());
		if( ptr == -1 ) glColor4f(0,0,0,0); else
		glColor4f( 1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][2]/255.0,0);
		glVertex3f( x, Panel_1->Height-y, 0 );

		x = (double)(i+1)/DX*(double)Panel_1->Width;
		y = (double)(j+1)/DY*(double)Panel_1->Height;
		ptr= Simulation_System.get_color_code(Source,i,j,Current_Z_Slice,Current_T_Slice,Time_Ptr,Grid_Spacing_Edit->Text.ToInt(),Basic_CL_Edit->Text.ToDouble());
		if( ptr == -1 ) glColor4f(0,0,0,0); else
		glColor4f( 1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][2]/255.0,0);
		glVertex3f( x, Panel_1->Height-y, 0 );

		x = (double)(i)  /DX*(double)Panel_1->Width;
		y = (double)(j+1)/DY*(double)Panel_1->Height;
		ptr= Simulation_System.get_color_code(Source,i,j,Current_Z_Slice,Current_T_Slice,Time_Ptr,Grid_Spacing_Edit->Text.ToInt(),Basic_CL_Edit->Text.ToDouble());
		if( ptr == -1 ) glColor4f(0,0,0,0); else
		glColor4f( 1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][2]/255.0,0);
		glVertex3f( x, Panel_1->Height-y, 0 );
	}
	glEnd();

	glEnable(GL_LIGHTING);

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);		// Select the modelview matrix
	glLoadIdentity();
}

//----------------------------------------------------------------------------------

void TMain_Screen_Form::paint_slab()
{
	double P_Width = 20;   // width of palette in pixels
	double P_Height = (double)Panel_1->Height/3.; // height of palette in pixels
	double P_Dens = 80;   // how many rectangles are building palette image
	double Left = 10; // distance from left edge
	double Top = P_Height;
	double x,y,z,ptr;
	long i,j,k;
	AnsiString Label, Unit;
	int Source = Display_Value_RadioGroup->ItemIndex;

	double Max = History_ScrollBar->Max;
	double Pos = History_ScrollBar->Position;
	long Time_Ptr = Pos/Max * Simulation_System.get_history_vector_size();

	// gogh3d

	glBegin(GL_QUADS);

	// XY side
	k = 0;
	for(i=0;i<Simulation_System.Dimension_X;i++)
	for(j=0;j<Simulation_System.Dimension_Y;j++)
	{
		ptr= Simulation_System.get_color_code(Source,i,j,k,Current_T_Slice,Time_Ptr,Grid_Spacing_Edit->Text.ToInt(),Basic_CL_Edit->Text.ToDouble());
		if( ptr == -1 || i==0 || i==Simulation_System.Dimension_X-1
					  || j==0 || j==Simulation_System.Dimension_Y-1 ) glColor4f(0,0,0,0); else
		glColor4f( 1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][2]/255.0,0);
		glVertex3f( i,j,k );

		ptr= Simulation_System.get_color_code(Source,i,j,k,Current_T_Slice,Time_Ptr,Grid_Spacing_Edit->Text.ToInt(),Basic_CL_Edit->Text.ToDouble());
		if( ptr == -1 || i==0 || i==Simulation_System.Dimension_X-1
					  || j==0 || j==Simulation_System.Dimension_Y-1 ) glColor4f(0,0,0,0); else
		glColor4f( 1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][2]/255.0,0);
		glVertex3f( i+1,j,k );

		ptr= Simulation_System.get_color_code(Source,i,j,k,Current_T_Slice,Time_Ptr,Grid_Spacing_Edit->Text.ToInt(),Basic_CL_Edit->Text.ToDouble());
		if( ptr == -1 || i==0 || i==Simulation_System.Dimension_X-1
					  || j==0 || j==Simulation_System.Dimension_Y-1 ) glColor4f(0,0,0,0); else
		glColor4f( 1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][2]/255.0,0);
		glVertex3f( i+1,j+1,k );

		ptr= Simulation_System.get_color_code(Source,i,j,k,Current_T_Slice,Time_Ptr,Grid_Spacing_Edit->Text.ToInt(),Basic_CL_Edit->Text.ToDouble());
		if( ptr == -1 || i==0 || i==Simulation_System.Dimension_X-1
					  || j==0 || j==Simulation_System.Dimension_Y-1 ) glColor4f(0,0,0,0); else
		glColor4f( 1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][2]/255.0,0);
		glVertex3f( i,j+1,k );
	}

	// XZ side
	j = 0;
	for(i=0;i<Simulation_System.Dimension_X;i++)
	for(k=0;k<Simulation_System.Dimension_Z;k++)
	{
		ptr= Simulation_System.get_color_code(Source,i,j,k,Current_T_Slice,Time_Ptr,Grid_Spacing_Edit->Text.ToInt(),Basic_CL_Edit->Text.ToDouble());
		if( ptr == -1 || i==0 || i==Simulation_System.Dimension_X-1
					  || k==0 || k==Simulation_System.Dimension_Z-1 ) glColor4f(0,0,0,0); else
		glColor4f( 1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][2]/255.0,0);
		glVertex3f( i,j,k );

		ptr= Simulation_System.get_color_code(Source,i,j,k,Current_T_Slice,Time_Ptr,Grid_Spacing_Edit->Text.ToInt(),Basic_CL_Edit->Text.ToDouble());
		if( ptr == -1 || i==0 || i==Simulation_System.Dimension_X-1
					  || k==0 || k==Simulation_System.Dimension_Z-1 ) glColor4f(0,0,0,0); else
		glColor4f( 1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][2]/255.0,0);
		glVertex3f( i+1,j,k );

		ptr= Simulation_System.get_color_code(Source,i,j,k,Current_T_Slice,Time_Ptr,Grid_Spacing_Edit->Text.ToInt(),Basic_CL_Edit->Text.ToDouble());
		if( ptr == -1 || i==0 || i==Simulation_System.Dimension_X-1
					  || k==0 || k==Simulation_System.Dimension_Z-1 ) glColor4f(0,0,0,0); else
		glColor4f( 1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][2]/255.0,0);
		glVertex3f( i+1,j,k+1 );

		ptr= Simulation_System.get_color_code(Source,i,j,k,Current_T_Slice,Time_Ptr,Grid_Spacing_Edit->Text.ToInt(),Basic_CL_Edit->Text.ToDouble());
		if( ptr == -1 || i==0 || i==Simulation_System.Dimension_X-1
					  || k==0 || k==Simulation_System.Dimension_Z-1 ) glColor4f(0,0,0,0); else
		glColor4f( 1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][2]/255.0,0);
		glVertex3f( i,j,k+1 );
	}

	// YZ side
	i = 0;
	for(j=0;j<Simulation_System.Dimension_Y;j++)
	for(k=0;k<Simulation_System.Dimension_Z;k++)
	{
		ptr= Simulation_System.get_color_code(Source,i,j,k,Current_T_Slice,Time_Ptr,Grid_Spacing_Edit->Text.ToInt(),Basic_CL_Edit->Text.ToDouble());
		if( ptr == -1
					  || j==0 || j==Simulation_System.Dimension_Y-1
					  || k==0 || k==Simulation_System.Dimension_Z-1 ) glColor4f(0,0,0,0); else
		glColor4f( 1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][2]/255.0,0);
		glVertex3f( i,j,k );

		ptr= Simulation_System.get_color_code(Source,i,j,k,Current_T_Slice,Time_Ptr,Grid_Spacing_Edit->Text.ToInt(),Basic_CL_Edit->Text.ToDouble());
		if( ptr == -1
					  || j==0 || j==Simulation_System.Dimension_Y-1
					  || k==0 || k==Simulation_System.Dimension_Z-1 ) glColor4f(0,0,0,0); else
		glColor4f( 1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][2]/255.0,0);
		glVertex3f( i,j+1,k );

		ptr= Simulation_System.get_color_code(Source,i,j,k,Current_T_Slice,Time_Ptr,Grid_Spacing_Edit->Text.ToInt(),Basic_CL_Edit->Text.ToDouble());
		if( ptr == -1
					  || j==0 || j==Simulation_System.Dimension_Y-1
					  || k==0 || k==Simulation_System.Dimension_Z-1 ) glColor4f(0,0,0,0); else
		glColor4f( 1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][2]/255.0,0);
		glVertex3f( i,j+1,k+1 );

		ptr= Simulation_System.get_color_code(Source,i,j,k,Current_T_Slice,Time_Ptr,Grid_Spacing_Edit->Text.ToInt(),Basic_CL_Edit->Text.ToDouble());
		if( ptr == -1
					  || j==0 || j==Simulation_System.Dimension_Y-1
					  || k==0 || k==Simulation_System.Dimension_Z-1 ) glColor4f(0,0,0,0); else
		glColor4f( 1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ThermalPal[(int)ptr][2]/255.0,0);
		glVertex3f( i,j,k+1 );
	}

	glEnd();
}

//----------------------------------------------------------------------------------

void TMain_Screen_Form::map_display_panels_paint(int Type)
{
	// clear panel
	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Panel_3D_Background_Color == 0 )
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Panel_3D_Background_Color == 1 )
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Rotate and translate
	OpenGL_Panel_1.opengl_panel_apply_transformations();

	// call paint function for current panel
	paint_3D_contents(Type);

	glFlush();

	SwapBuffers(OpenGL_Panel_1.hdc);
}
//---------------------------------------------------------------------------

	//-----------------------------------------------------------
	// Function controlling repainting of the OpenGL panel. Called by
	// paint event of OpenGL panel
	// Calls paint_map() function which actually paints something.
	//-----------------------------------------------------------

void TMain_Screen_Form::paint_3D_contents(int Type)
{
	if( Type == 0 )
	paint_slice();

	if( Type == 1 )
	paint_slab();
}

//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Recalculate_CL_ButtonClick(TObject *Sender)
{
	long History_Size = Simulation_System.get_history_vector_size();
	double ts = Simulation_System.Signal_Sampling_Timestep;
	if(ts==0) ts = 0.25;

	if( ts > 0 )
	{

	double Stop_Ptr,val,MPC,CL;
	std::vector<double> MPCs,Distances;

	int Corr_L_W_Size_ptr = CL_W_Size_Edit->Text.ToDouble()/ts;
	int Corr_L_step_ptr = CL_W_Step_Edit->Text.ToDouble()/ts;
	int BCL_Ptr = Basic_CL_Edit->Text.ToDouble()/ts;

	CL_Chart->Series[0]->Clear();
	MPC_Dist_Chart->Series[0]->Clear();
	MPC_Dist_Chart->Series[1]->Clear();

	if( Calculation_Target_RadioGroup->ItemIndex == 0 )
	CL_Chart->Title->Text->Text = "Mean Phase Coherence";
	if( Calculation_Target_RadioGroup->ItemIndex == 1 )
	CL_Chart->Title->Text->Text = "Correlation length";

	// plot MPC vs. dist for whole duration
	Simulation_System.get_correlation_length(0,History_Size-1,BCL_Ptr,&MPC,&CL,&MPCs,&Distances);
	for(long t=0;t<Distances.size();t++)
	MPC_Dist_Chart->Series[0]->AddXY(Distances[t],MPCs[t]);

	//-------------------------------------------------------------------
	for(long Start_Ptr=0;
			 Start_Ptr + Corr_L_W_Size_ptr < History_Size;
			 Start_Ptr += Corr_L_step_ptr)
	//-------------------------------------------------------------------
	{

	Stop_Ptr = Start_Ptr + Corr_L_W_Size_ptr;

	Simulation_System.get_correlation_length(
		Start_Ptr,Stop_Ptr,BCL_Ptr,&MPC,&CL,&MPCs,&Distances);

	if( Calculation_Target_RadioGroup->ItemIndex == 0 )
	CL_Chart->Series[0]->AddXY( ts*0.5*(Start_Ptr+Stop_Ptr),CL );

	if( Calculation_Target_RadioGroup->ItemIndex == 1 )
	CL_Chart->Series[0]->AddXY( ts*0.5*(Start_Ptr+Stop_Ptr),MPC );

	}
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::SC_RadioGroupClick(TObject *Sender)
{
	display_signals_from_history_array();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Calculation_Target_RadioGroupClick(TObject *Sender)
{
	Recalculate_CL_ButtonClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Calculate_CV_ButtonClick(TObject *Sender)
{
	Simulation_System.calculate_velocity_field(Basic_CL_Edit->Text.ToDouble(),CV_Mesh_Spacing_Edit->Text.ToInt());
//    Laminar_Domains_Calculate_ButtonClick(this);
	opengl_panel_paint();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Debug11Click(TObject *Sender)
{
	VelocityVector_2D CV;
	Point_2D grid[3][3];
	double timings[3][3];
	double Grid_Density = 1, s1=5,s2=4;

		grid[0][0] = Point_2D(s1-Grid_Density,s2-Grid_Density);
		grid[0][1] = Point_2D(s1-Grid_Density,s2);
		grid[0][2] = Point_2D(s1-Grid_Density,s2+Grid_Density);

		grid[1][0] = Point_2D(s1,s2-Grid_Density);
		grid[1][1] = Point_2D(s1,s2);
		grid[1][2] = Point_2D(s1,s2+Grid_Density);

		grid[2][0] = Point_2D(s1+Grid_Density,s2-Grid_Density);
		grid[2][1] = Point_2D(s1+Grid_Density,s2);
		grid[2][2] = Point_2D(s1+Grid_Density,s2+Grid_Density);

		timings[0][0] = 0;
		timings[0][1] = 1;
		timings[0][2] = 2;

		timings[1][0] = 0;
		timings[1][1] = 1;
		timings[1][2] = 2;

		timings[2][0] = 0;
		timings[2][1] = 1;
		timings[2][2] = 2;

		CV = NumAlg.calculateVelocityVector_2D_Grid(grid,timings);

		ShowMessage( "direction: " + FloatToStr(CV.direction) + "    amplitude = " + FloatToStr(CV.amplitude) );
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Laminar_Domains_Calculate_ButtonClick(TObject *Sender)
{
	Simulation_System.calculate_laminar_events(Direction_Tolerance_Edit->Text.ToDouble(),Grid_Spacing_Edit->Text.ToInt());

	// show full results
	ShowMessage(FloatToStr(Simulation_System.get_max_activation_time_id())+" laminar domains detected");

	std::vector<double> Volumes;
	for(int d=0;d<Simulation_System.Domains.size();d++)
	Volumes.push_back(Simulation_System.Domains[d].xyt_Volume);

	sort(Volumes.begin(), Volumes.end());

	RichEdit_Form->Caption = "xyt Volume of domains";
	RichEdit_Form->RichEdit->Lines[0].Clear();
	RichEdit_Form->RichEdit->Lines[0].Add("xyt Volume of domains");
	for(int d=0;d<Volumes.size();d++)
	RichEdit_Form->RichEdit->Lines[0].Add(FloatToStr(Volumes[Volumes.size()-1-d]));

	RichEdit_Form->ShowModal();

	opengl_panel_paint();
}

//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Angle_vs_distance_ButtonClick(TObject *Sender)
{
	MPC_Dist_Chart->Series[0]->Clear();
	MPC_Dist_Chart->Series[1]->Clear();

	double Max = History_ScrollBar->Max;
	double Pos = History_ScrollBar->Position;
	long Time_Ptr = Pos/Max * Simulation_System.get_history_vector_size();
	long BCL_Ptr = 0; // not used now....
	std::vector<double> Correlations, Distances;

	Simulation_System.phase_vs_distance_data(Time_Ptr,BCL_Ptr,&Correlations,
		&Distances,Phase_vs_Distance_Source_RadioGroup->ItemIndex);

	// paint scattered data
	if( Corr_Dist_Scatter_Type_RadioGroup->ItemIndex == 0 ) // normal values
	{
		for(long t=0;t<Distances.size();t++)
		MPC_Dist_Chart->Series[0]->AddXY(Distances[t],Correlations[t]);
	}
	if( Corr_Dist_Scatter_Type_RadioGroup->ItemIndex == 1 ) // loglog
	{
		for(long t=0;t<Distances.size();t++)
		if( Distances[t] > 0 && Correlations[t] > 0 )
		MPC_Dist_Chart->Series[0]->AddXY(log(Distances[t]),log(Correlations[t]));
	}

	if( Correlations.size() > 3 )
	{
	// paint means line
	auto min_dist = std::min_element(Distances.begin(), Distances.end());
	auto max_dist = std::max_element(Distances.begin(), Distances.end());
	double Intervals_Number = 100.0;
	double Interval = (*max_dist - *min_dist)/Intervals_Number;

	std::vector<double> means = NumAlg.calculate_moving_average_distances_data_pair(
		Distances, Correlations, Interval);

	if( Corr_Dist_Scatter_Type_RadioGroup->ItemIndex == 0 ) // normal values
	{
		for(long t=0;t<means.size();t++)
		MPC_Dist_Chart->Series[1]->AddXY(t*Interval,means[t]);
	}
	if( Corr_Dist_Scatter_Type_RadioGroup->ItemIndex == 1 ) // loglog
	{
		for(long t=0;t<means.size();t++)
		if( t*Interval > 0 && means[t] > 0 )
		MPC_Dist_Chart->Series[1]->AddXY(log(t*Interval),log(means[t]));
	}

	ShowMessage("Mean correlation (all possible pairs of locations)= " +
		FloatToStr( NumAlg.get_average_from_vector(&Correlations) ));
	}
	else
	ShowMessage("No valid data points found.");
}

//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Recalculate_Phase_ButtonClick(TObject *Sender)
{
	Simulation_System.recalculate_phase(Basic_CL_Edit->Text.ToDouble(),Sampling_Edit->Text.ToDouble());
	opengl_panel_paint();

}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Play_ButtonClick(TObject *Sender)
{
	Playback_flag = true;
	int speed = Playback_Speed_Edit->Text.ToDouble();
	while(Playback_flag)
	{
		History_ScrollBar->Position += speed;
        if( History_ScrollBar->Position == History_ScrollBar->Max ) History_ScrollBar->Position = 1;
		paint_slice();
		opengl_panel_paint();
		Application->ProcessMessages();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Screen_Form::Stop_Playback_ButtonClick(TObject *Sender)
{
	Playback_flag = false;
}
//---------------------------------------------------------------------------



