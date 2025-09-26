//--------------------------------------------------
// Signal types
//--------------------------------------------------

#ifndef ROVING_SIGNAL
#define ROVING_SIGNAL 0
#endif

#ifndef REFERENCE_SIGNAL
#define REFERENCE_SIGNAL 1
#endif

#ifndef ECG_SIGNAL
#define ECG_SIGNAL 2
#endif

#ifndef NORMALIZED_SIGNAL
#define NORMALIZED_SIGNAL 3
#endif

//--------------------------------------------------
// Max and min values for double (check in case of different complier)
//--------------------------------------------------

#ifndef Max_Double
#define Max_Double (double)(1.7e+308)
#endif

#ifndef Min_Double
#define Min_Double (double)(-1.7e+308)
#endif

#ifndef FILE_NOT_FOUND
#define FILE_NOT_FOUND -1
#endif

#ifndef UNABLE_TO_OPEN_FILE
#define UNABLE_TO_OPEN_FILE -2
#endif

#ifndef MAX_NODE_NEIGHBORS
#define MAX_NODE_NEIGHBORS 64
#endif



#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef OK_RESULT
#define OK_RESULT 1
#endif

#ifndef FAILURE
#define FAILURE 0
#endif


#ifndef X_AXIS
#define X_AXIS 0
#endif

#ifndef Y_AXIS
#define Y_AXIS 1
#endif

#ifndef Z_AXIS
#define Z_AXIS 2
#endif



// Geometry flags

#ifndef GEO_FLAG_INTERIOR
#define GEO_FLAG_INTERIOR 0
#endif

#ifndef GEO_FLAG_BORDER
#define GEO_FLAG_BORDER 1
#endif


//-------------------------------------------------------------------------------
// These are OpenGL 3D panel popup menu item tags.
// ATTENTION: items are selected from popup menu and order must be in accordance
// with below numbers!
//-------------------------------------------------------------------------------

#ifndef MAP_POPUP_ITEM_SELECT_MODE
#define MAP_POPUP_ITEM_SELECT_MODE 1
#endif

#ifndef MAP_POPUP_ITEM_ROTATE
#define MAP_POPUP_ITEM_ROTATE 2
#endif

#ifndef MAP_POPUP_ITEM_ZOOM
#define MAP_POPUP_ITEM_ZOOM 3
#endif

#ifndef MAP_POPUP_ITEM_PAN
#define MAP_POPUP_ITEM_PAN 4
#endif

#ifndef MAP_POPUP_ITEM_ENCIRCLING
#define MAP_POPUP_ITEM_ENCIRCLING 5
#endif



// Number of different types of defined segments

#ifndef NOT_SEGMENTED
#define NOT_SEGMENTED 0
#endif

#ifndef BLOODPOOL_SEGMENT
#define BLOODPOOL_SEGMENT 1
#endif

#ifndef LA_SEGMENT
#define LA_SEGMENT 2
#endif

#ifndef RA_SEGMENT
#define RA_SEGMENT 3
#endif

#ifndef LV_SEGMENT
#define LV_SEGMENT 4
#endif

#ifndef RV_SEGMENT
#define RV_SEGMENT 5
#endif

#ifndef AORTA_SEGMENT
#define AORTA_SEGMENT 6
#endif

#ifndef CS_SEGMENT
#define CS_SEGMENT 7
#endif

#ifndef SPINE_SEGMENT
#define SPINE_SEGMENT 8
#endif

#ifndef CUSTOM1_SEGMENT
#define CUSTOM1_SEGMENT 9
#endif

#ifndef SEGMENTS_TYPES_NUMBER
#define SEGMENTS_TYPES_NUMBER 10 // = max id of segment +1
#endif

//----------------------------------------------------------------
// SCREEN LAYOUT CONSTANTS - update those when screen compontnest structure is changed
//----------------------------------------------------------------

#ifndef EP_MAPPING_TABINDEX
#define EP_MAPPING_TABINDEX 0
#endif

//-------------------------------------------------
// APPLICATION STATE
//-------------------------------------------------

#ifndef STUDY_NOT_OPENED
#define STUDY_NOT_OPENED 0
#endif

#ifndef STUDY_OPENED
#define STUDY_OPENED 1
#endif


