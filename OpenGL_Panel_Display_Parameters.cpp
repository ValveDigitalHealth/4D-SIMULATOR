//---------------------------------------------------------------------------
#pragma hdrstop

#include "OpenGL_Panel_Display_Parameters.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
OpenGL_Panel_Display_Parameters_class::OpenGL_Panel_Display_Parameters_class()
{
	  // 3D world orientation initial parameters
	  ZoomFactor   = -180.0;
	  zNear        = 0.1F;
	  zFar         = 12200.0F;
	  Translation_X = 0.0;
	  Translation_Y = 0.0;

	Seed_Cross_Size_3D = 7;

	Transparency_Level = 1.0;

	OpenGL_Panel_Dragging_Flag = OFF;

	// Initial coordinates for patient doll parts
	Head_z_Coordinate = 60;
	Arms_x_Coordinate = 40;
	Legs_z_Coordinate = -50;

	Font_Type = 7;

	Data_Point_Size = 1;
	Electrode_Point_Size = 0.5;
	Scan_Surface_Point_Size = 0.1;

	Plaque_Display_Background_Color = 0;
}

//---------------------------------------------------------------------------

int OpenGL_Panel_Display_Parameters_class::Read_Palette()
{
	ifstream dfile("CARTO_Palette.inc",ios_base::in);

	if( dfile == NULL )
		{
			ShowMessage("\n Error : can't open 'CARTO_Palette.inc' file." );
			return FILE_NOT_FOUND;
		}

	  for( int i = 0; i < 256; i++ )
	  {
			dfile >> ThermalPal[i][0];
			dfile >> ThermalPal[i][1];
			dfile >> ThermalPal[i][2];
	  }

	  // Original_ThermalPal can be used as to restore original palette after
	  // palette transformations
	  for( int i = 0; i < 256; i++ )
	  {
			Original_ThermalPal[i][0] = ThermalPal[i][0];
			Original_ThermalPal[i][1] = ThermalPal[i][1];
			Original_ThermalPal[i][2] = ThermalPal[i][2];
	  }

	  return OK_RESULT;
}

//---------------------------------------------------------------------------

