import sys
import os
import numpy as np
import cv2 as cv

from scipy.interpolate import UnivariateSpline

#Run as python reconstruct.py <source image path> <destination image path>
source = sys.argv[1]
dst = sys.argv[2]

img = cv.imread(source)

def reconstruct(img):
    res_row = np.zeros((img.shape[0], img.shape[1]))
    res_col = np.zeros((img.shape[0], img.shape[1]))

    for i in range(128):
        imgrow = img[i, :, 0]
        input = np.arange(128)

        zero_list = list(np.where(imgrow==0)[0])

        non_zero_imgrow = np.delete(imgrow, zero_list)
        non_zero_input = np.delete(input, zero_list)
        
        if len(non_zero_input)<4:
            res_row[i, :] = imgrow
        else:
            spline = UnivariateSpline(non_zero_input, non_zero_imgrow)
            res_row[i,:] = spline(input).astype(int)

    for i in range(128):
        imgcol = img[:, i, 0]
        input = np.arange(128)

        zero_list = list(np.where(imgcol==0)[0])

        non_zero_imgcol = np.delete(imgcol, zero_list)
        non_zero_input = np.delete(input, zero_list)
        
        if len(non_zero_input)<4:
            res_col[:, i] = imgcol
        else:
            spline = UnivariateSpline(non_zero_input, non_zero_imgcol)
            res_col[:,i] = spline(input).astype(int)

    res = (res_row + res_col)/2
    res = np.stack([res, res, res], axis=2).astype(int)
    
    return res

def repeat(img, n):
    #Gives an odd warning when executed this way.
    res = img
    for i in range(n):
        res = reconstruct(res)

    return res

res = reconstruct(img)
#res = repeat(img, 5)

cv.imwrite(dst, res)
