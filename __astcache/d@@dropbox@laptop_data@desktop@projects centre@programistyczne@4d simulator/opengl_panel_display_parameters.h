//---------------------------------------------------------------------------

#ifndef OpenGL_Panel_Display_ParametersH
#define OpenGL_Panel_Display_ParametersH

#include "AAMS_constants.h"

#include<fstream>
#include <ios>

#include<vcl.h>

using namespace std;

//---------------------------------------------------------------------------

class OpenGL_Panel_Display_Parameters_class
{
	public:

	OpenGL_Panel_Display_Parameters_class();


	//-----------------------------------------------------------
	// Variables used to set perspective on OpenGL panel
	// (gluPerspective() function called each time panel is
	// resized)
	//-----------------------------------------------------------
	double zNear, zFar;

	//-----------------------------------------------------------
	// Translation in X and Y axes.Used to implement
	// Pan transformation.
	//-----------------------------------------------------------
	int Translation_X,Translation_Y;

	//-----------------------------------------------------------
	// Flags set on when dragging over openGL panel occurs
	// Values: ON, OFF. Used in rotation, pan and zoom
	//-----------------------------------------------------------
	int OpenGL_Panel_Dragging_Flag;

	//-----------------------------------------------------------
	// Zoom factor - distance from (0,0,0) point
	// in 3D world
	//-----------------------------------------------------------
	double ZoomFactor;

	int Font_Type;

	//-------------------------------------------------------------------------
	// Transparency level of the objects painted in 3D world.
	// Set to 1 at all times.
	//-------------------------------------------------------------------------
	double Transparency_Level;

	int Seed_Cross_Size_3D;

	double Panel_3D_Background_Color; // 0 - black, 1 - white

	//-----------------------------------------------------------
	// Head, arms and legs position of patient doll for both panels
	// Position of the begining of the head,leg,arm.
	//-----------------------------------------------------------
	float Head_z_Coordinate;
	float Arms_x_Coordinate;
	float Legs_z_Coordinate;


	double Data_Point_Size;
	double 	Electrode_Point_Size;
    double Scan_Surface_Point_Size;

	double Plaque_Display_Background_Color;


    //-----------------------------------------------------------
    // Function: Read_Palette
    // Description: Reads palette used for color display of 3D maps
    //              Copied from CARTO system. 255 colors, RGB channels for each color
    //              Can be easily modified in case of (c) of CARTO palette
    // Input parameters: void
    // Returns: int (OK_RESULT if success, FILE_NOT_FOUND if no palette file found)
    //-----------------------------------------------------------
	int Read_Palette();

	//-----------------------------------------------------------
	// Palette used to colorcode values in OpenGL panel.
	// 256 colors, 3 channels for each color (R,G,B)
	//-----------------------------------------------------------
	double ThermalPal[256][3];    // CARTO palette, readed from file
	//-----------------------------------------------------------
	// Original palette. Used as a restore point when current palette
	// is modified (e.g. shifted)
	//-----------------------------------------------------------
	double Original_ThermalPal[256][3];

};

#endif

