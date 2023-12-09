@echo off


IF "%1%" == "clean" (
    del array.txt
    del map_heights.png
    del *.exe
    goto EXT1

)

rem Получения значения пикслелей с тепловой карты высот
python img_to_array.py
rem Сопоставления значений пикселей с градиентом, результат высота в данной точке
g++ array_img_to_heights_map.cpp -o array_img_to_heights_map
array_img_to_heights_map
rem Генерация карты высот
python heights_to_map_heights.py


:EXT1
@echo Exit parser