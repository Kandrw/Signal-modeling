# -*- coding: utf-8 -*-
"""
Created on Sun Nov 26 13:16:31 2023

@author: Andrey
"""

from PIL import Image
import numpy as np

import sys

file_heat = "heat_map.png"
filename_array = "array.txt"
img = Image.open(file_heat)
img_array = np.array(img)
shp = img_array.shape
file = open(filename_array, "w")
shp = list(shp)
shp = ' '.join(map(str, shp))
file.write(shp + '\n')
for i in range(len(img_array)):
    for i2 in range(len(img_array[0])):
        dfg = img_array[i][i2][0:3]
        str2 = ' '.join(map(str, list(dfg)))
        file.write(str2 + '\n')
        pass
    print(i)

file.close()
sys.exit()



















