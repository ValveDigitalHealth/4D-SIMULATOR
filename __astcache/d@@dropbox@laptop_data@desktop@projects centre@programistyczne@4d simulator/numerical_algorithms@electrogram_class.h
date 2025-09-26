//---------------------------------------------------------------------------

#ifndef Electrogram_classH
#define Electrogram_classH
//---------------------------------------------------------------------------

#include<vector>

class Electrogram
{
	public:

	std::vector<double> Egm;
	std::vector<double> Phase;
    std::vector<long> Activation_Timings_Ptr;

	double x,y,z;
	double Timestep_ms;

};
#endif
