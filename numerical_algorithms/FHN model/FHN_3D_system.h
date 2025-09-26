//---------------------------------------------------------------------------
#include "Numerical_Library.h"

#ifndef FHN_3D_systemH
#define FHN_3D_systemH
//---------------------------------------------------------------------------

class FHN_3D_system_class
{
    public:

    FHN_3D_system_class();
    ~FHN_3D_system_class();

    int save_object(ofstream* dfile);
    int load_object(ifstream* dfile);

    // data structures
    int Size_X;
    int Size_Y;
    int Size_Z;

    double*** V1;
    double*** V2;

    double*** W1;
    double*** W2;

    vector <vector_of_doubles> Middle_Row_Voltage_History;
    void record_potentials();
    void compute_ISIs(long Node_Position);
    vector<long>ISIs;
    long ISIs_Node_Ptr_X;

    double Min_Voltage,Max_Voltage;

    int Boundary_Conditions;

    void clear_system();

    void create_spiral(int Slice_Plane); // 0-x,1-y,2-z,3-t

    double* FHN_Activation_Variable_APD;
    double* FHN_Recovery_Variable_APD;
    int FHN_APD_Length;

    // parameters
    double ni,gamma,beta;
    double dx,dt;
    double DX,DY,DZ;

    double Global_Time;

    void calculate_V1_from_V2_fhn();
    void calculate_V2_from_V1_fhn();

    int allocate_tables();
    int free_tables();

    void compute_N_steps(int N);

    int get_color_code(int X,int Y,int Z);

    void stimulate_node(int X,int Y,int Z);
};

#endif
