import os
import decimal

import cv2 as cv
import numpy as np

#Change path to folder containing original heightmaps
path = r"..\SurfaceReconstruction\heightMaps"

def drange(x, y, jump):
  while x < y:
    yield float(x)
    x += decimal.Decimal(jump)

def decimate(img, fraction = 0.1):
    x = np.random.choice(img.shape[1], int(fraction*img.shape[0]*img.shape[1]))
    y = np.random.choice(img.shape[0], int(fraction*img.shape[0]*img.shape[1]))
    
    
    img[y, x] = 0
    return img

for file in os.listdir(path):
    if ".png" in file:
        img = cv.imread(os.path.join(path, file))
        
        if not os.path.exists(os.path.join(path, file[:-4])):
            os.mkdir(os.path.join(path, file[:-4]))

        cv.imwrite(os.path.join(path, file[:-4], file[:-4]+"_0.png"), img)
        print(img.shape)

        for f in list(drange(decimal.Decimal('0.1'), 1, decimal.Decimal('0.1'))):
            decimate(img, f)
            cv.imwrite(os.path.join(path, file[:-4], file[:-4] + "_{}".format(f*100) + ".png"), img)
        