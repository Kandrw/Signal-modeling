#include "map.hpp"

#include "objectmap.hpp"

Map::Map(int x, int y) : size_x(x), size_y(y)
{
    map = new int*[size_x];
    map_signal = new double*[size_x];
    for(int i = 0; i < size_y; ++i){
        map[i] = new int[size_x]{};
        map_signal[i] = new double[size_x]{};
    }

}
Map::~Map(){

    for(int i = 0; i < size_y; ++i){
        delete[] map[i];
        delete[] map_signal[i];
    }
    delete[] map;
    delete[] map_signal;
}

int Map::add_object(int posx, int posy, int sizex, int sizey, int type){

    if(posx < 0 || posy < 0){
        return -1;
    }
    if(sizex + posx >= this->size_x || sizey + posy >= this->size_y){
        return -1;
    }
    int x, y;
    for(y = posy; y < sizey + posy; ++y){
        for(x = posx; x < sizex + posx; ++x){
            map[y][x] = type;
        }
    }
    return 0;
}
int Map::delete_object(int posx, int posy, int sizex, int sizey){

    if(posx < 0 || posy < 0){
        return -1;
    }
    if(sizex + posx >= this->size_x || sizey + posy >= this->size_y){
        return -1;
    }
    int x, y;
    for(y = posy; y < sizey + posy; ++y){
        for(x = posx; x < sizex + posx; ++x){
            map[y][x] = (int) type_material::NO_MATERIAL;
        }
    }
    return 0;
}
int** Map::get_map(){
    return map;
}
double** Map::get_map_signal(){
    return map_signal;
}
