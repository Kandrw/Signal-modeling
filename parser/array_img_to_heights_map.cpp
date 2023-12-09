#include <iostream>





int h_by_color[][4] = {
        {255, 122, 122, 207},
        {255, 69, 69, 201},
        {255, 6, 6, 194},
        {255, 45, 0, 188},
        {255, 99, 0, 182},
        {255, 153, 0, 176},
        {255, 208, 0, 170},
        {247, 255, 0, 164},
        {192, 255, 0, 158},
        {136, 255, 0, 152},
        {90, 255, 0, 147},
        {33, 255, 0, 141},
        {0, 255, 13, 136},
        {0, 255, 61, 131},
        {0, 255, 109, 126},
        {0, 255, 195, 121},
        {0, 255, 199, 117},
        {0, 255, 240, 113},
        {0, 227, 255, 109},
        {0, 192, 255, 106}
    };

int color_in_heights(int r, int g, int b){
    int r2, g2, b2, h;

    //for(int i = 20 - 1; i >= 0; --i){
    for(int i = 0; i < 20; ++i){
    
        r2 = h_by_color[i][0];
        g2 = h_by_color[i][1];
        b2 = h_by_color[i][2];
        h = h_by_color[i][3];
        
        if(r > 230 && r2 > 230){
            if(b > 0 && b2 > 0){
            //if(g > 5 && g2 > ){
                // if(g > 122 && g2 > 122){
                //     return 207;
                // }
                
                // if(g > 6 && g2 > 6 && b > 6 && b2 > 6){
                //     return 201;
                // }
                // if(g < 45 && b < 6)
                //     return 194;

                if(g >= g2 && b >= b2){
                    


                    if(g > 122 && g2 > 122){
                        return 207;
                    }
                    
                    if(g > 6 && g2 > 6 && b > 6 && b2 > 6){
                        return 201;
                    }
                    if(g < 45 && b < 6)
                        return 194;


                    //return h;
                }
            }
            else{
                if(g <= g2){
                    
                    return h;
                }
            }
        }
        else if(g > 230 && b <= 20){
            if(r >= r2){
                
                return h;
            }
        }
        else if(r <= 32 && r2 <= 32 && g > 230 && g2 > 230){
            if(b <= b2){
                return h;
            }
            
        }
        else if(r <= 10 && b > 240 && r2 <= 20){
            if(r <= 20 && g >= g2 ){
                return h;
            }
        }
    }
    printf("\t%d + %d + %d\n", r, g, b);
    return 0;
}



int main(){
    char filename_array[] = "array.txt";
    char filename_heights[] = "heights.txt";
    FILE *file = fopen(filename_array, "r");
    if(!file){
        printf("File %s not found\n", filename_array);
        return -1;
    }
    FILE *file_out = fopen(filename_heights, "w");

    int y, x, z;
    fscanf(file, "%d%d%d", &y, &x, &z);

    printf("%d %d %d \n", y, x, z);

    int r, g, b, h;
    //y = 1;
    //x = 20;
    for(int i = 0; i < y; i++){
        for(int i2 = 0; i2 < x; i2++){
            fscanf(file, "%d%d%d", &r, &g, &b);
            h = color_in_heights(r, g, b);
            //fprintf(file_out, "%d %d %d %d\n",r, g, b, h);
            fprintf(file_out, " %d\n",h);
            if(h == 0){
                printf("[%d ]Error\n", i * y + i2);
                return -1;
            }
            
        }
    }
    fclose(file);
    fclose(file_out);
    printf("End read and write\n");

    return 0;
}






