#ifndef MAP_HPP
#define MAP_HPP


class Map
{
public:
    Map(int x, int y);
    ~Map();
    int add_object(int posx, int posy, int sizex, int sizey, int type);
    int delete_object(int posx, int posy, int sizex, int sizey);
    int** get_map();
    double** get_map_signal();
private:
    int **map;
    double **map_signal;
    int size_x, size_y;
};

#endif // MAP_HPP
