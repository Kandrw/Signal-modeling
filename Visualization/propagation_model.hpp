#ifndef PROPAGATIONMODEL_HPP
#define PROPAGATIONMODEL_HPP


enum PathlossModel{
    RMaLOS,
    RMaNLOS,
    UMaLOS,
    UMaNLOS,
    UMiLOS,
    UMiNLOS,
    COUNT_MODEL
};


class PropagationModel
{
public:
    PropagationModel(PathlossModel type_model, double h_UT, double h_BS);
    double PL_func(double d, double f);
private:
    PathlossModel typemodel;
    double h_UT;
    double h_BS;
    double d_BP;

};

#endif // PROPAGATIONMODEL_HPP
