//---------------------------------------------------------------------------

#ifndef Two_D_system_CR_region_classH
#define Two_D_system_CR_region_classH

#include "Program_Constants.h"

#include "Numerical_Library.h"


//---------------------------------------------------------------------------
class Two_D_system_CR_region_class
{
	public:
		vector <int> X,Y; // coordinates
		vector <int> R; // radius

		void clear();
};

#endif