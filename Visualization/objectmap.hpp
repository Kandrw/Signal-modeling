#ifndef OBJECTMAP_HPP
#define OBJECTMAP_HPP

#include <QGraphicsScene>

enum class type_material : int{
    NO_MATERIAL = 0,
    GLASS,
    IRR_GLASS,
    CONCRETE,
    WOOD,
    DRYWALL,
    COUNT_MATERIAL
};

extern QColor color_material[(int) type_material::COUNT_MATERIAL];
extern double (*attenuation_material[(int)type_material::COUNT_MATERIAL])(double);

#endif // OBJECTMAP_HPP
