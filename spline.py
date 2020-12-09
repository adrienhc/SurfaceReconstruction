import numpy as np
from PIL import Image
from numpy import asarray
from scipy.interpolate import RectBivariateSpline
from scipy.interpolate import interp2d 
import matplotlib.pyplot as plt
from numpy import *
import matplotlib
import sys

image = Image.open(sys.argv[1]) 
rawim = asarray(image) 

x = np.arange(0, 128, 1)
y = np.arange(0, 128, 1)
X, Y = np.meshgrid(x, y)
Z = np.array(rawim[:,:,0])
interp_spline = RectBivariateSpline(y, x, Z)
#interp_spline = interp2d(y, x, Z, 'linear')

x2 = np.arange(0, 128, 0.25)
y2 = np.arange(0, 128, 0.25)
X2, Y2 = np.meshgrid(x2,y2)
Z2 = interp_spline(y2, x2)

matplotlib.image.imsave(sys.argv[2], Z2)

'''
fig, ax = plt.subplots(nrows=1, ncols=2, subplot_kw={'projection': '3d'})
ax[0].plot_surface(X, Y, Z)
ax[1].plot_surface(X2, Y2, Z2)
fig.tight_layout()
plt.show()
'''