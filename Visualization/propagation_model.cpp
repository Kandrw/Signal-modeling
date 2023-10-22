#include "propagation_model.hpp"

#include <iostream>
#include <cmath>

/*d in meters, f in db*/
double model_UMaLOS(double d, double f, double d_BP, double h_BS, double h_UT){
    if(d <= 10.0)
        return 0;
    if(d >= 10.0 && d <= d_BP)
        return 28.0 + 22.0 * log10(d) + 20.0 * log10(f);
    return 28.0 + 40.0 * log10(d) + 20.0 * log10(f) - \
            9.0 * log10( (d_BP * d_BP) + pow( (h_BS - h_UT), 2) );
}
double model_UMaNLOS(double d, double f, double d_BP, double h_BS, double h_UT){
    return 0.0;
}


PropagationModel::PropagationModel(
    PathlossModel type_model, double h_UT, double h_BS) : \
    typemodel(type_model), h_UT(h_UT), h_BS(h_BS)
{

}
/*d in meters, f in db*/
double PropagationModel::PL_func(double d, double f){
    switch (typemodel) {
    case RMaLOS:

        return 0;
    case RMaNLOS:

        return 0;
    case UMaLOS:
        return model_UMaLOS(d, f, d_BP, h_BS, h_UT);

    default:
        std::cout<<"Not found Model\n";
        return 0;
    }
}
