# -*- coding: utf-8 -*-
"""
Created on Sun Nov 26 13:24:19 2023

@author: Andrey
"""


from PIL import Image
import numpy as np

import sys



file_heights = "heights.txt"


file = open(file_heights, "r")

iter1 = 0
N = 1000
d2 = []
data = []
while(True):
    line = file.readline()
    if(line == ""):
        break
    
    rt = int(line)
    d2.append(rt)
    if(len(d2) == N):
        data.append(d2)
        d2 = []
    

    
    iter1 += 1
file.close()

img_arr = np.array(data)
img = Image.fromarray(img_arr.astype('uint8'))
img.save("map_heights.png")
print("Save image heights map")









