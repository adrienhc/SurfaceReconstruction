import cv2 as cv
import numpy as np

import os

#Change path to folder containing original heightmaps
path = r"..\SurfaceReconstruction\heightMaps"

for file in os.listdir(path):
    if ".png" in file:
        img = cv.imread(os.path.join(path, file))

        if img.shape[0]==513 and img.shape[1]==513:
            img = img[0:512, 0:512]

        img = cv.resize(img,(128, 128), interpolation = cv.INTER_CUBIC)

        cv.imwrite(os.path.join(path, file), img)