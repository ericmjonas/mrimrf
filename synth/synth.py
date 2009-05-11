#!/usr/bin/python
import numpy as np
from matplotlib import pyplot
import util

def plane_box(N, r, min, max):
    """
    generate square box in the center of size R
    with phase ranging along the X axis from min to max
    
    """

    d = np.zeros((N, N), dtype=np.float)

    
    x = N/2
    y = N/2
    
    for xi in range(x-r, x+r):
        d[y-r:y+r, xi] = min + (max - min)/(2*r) * (xi-r)

    return d

def sphere(N, R, phasemin, phasemax, d = None):
    """
    Generate a centered sphere with phase that varies from min
    to max
    """
    if d == None:
        d = np.zeros((N, N), dtype=np.float)

    
    x_center = N/2
    y_center = N/2
    for x in xrange(N):
        for y in xrange(N):
            r = np.sqrt((x - x_center)**2 + (y-y_center)**2)
            if r < R:
                d[x, y] = phasemax -  (phasemax-phasemin) * r/R
    return d


def spirals(N=1, MAXPHASE=np.pi):
    """
    Load the spirals dataset, downsampling by N
    """
    from PIL import Image
    im = Image.open("../synth/spirals.png")
    a = np.asarray(im)
    am = a.mean(axis=2)
    am = am[::N, ::N]
    
    normed = am / np.mean(am)
    
    return normed * MAXPHASE

def test_plane_box():
    pb = plane_box(256, 80, 1.0, 20.0)
    pyplot.subplot(1, 2, 1)
    pyplot.imshow(pb, cmap=pyplot.cm.gray)
    pyplot.subplot(1, 2, 2)
    pyplot.imshow(util.wrap_phase(pb), cmap=pyplot.cm.gray, vmin=-np.pi, vmax=np.pi)
    pyplot.show()


def test_plane_box_small():
    pb = plane_box(32, 10, 1.0, 3.0)
    pyplot.subplot(1, 2, 1)
    pyplot.imshow(pb, cmap=pyplot.cm.gray)
    pyplot.subplot(1, 2, 2)
    pyplot.imshow(util.wrap_phase(pb), cmap=pyplot.cm.gray, vmin=-np.pi, vmax=np.pi)
    pyplot.show()

def test_sphere():
    pb = sphere(256, 60, 1.0, 20.0)
    pyplot.subplot(1, 2, 1)
    pyplot.imshow(pb, cmap=pyplot.cm.gray)
    pyplot.subplot(1, 2, 2)
    pyplot.imshow(util.wrap_phase(pb), cmap=pyplot.cm.gray, vmin=-np.pi, vmax=np.pi)
    pyplot.show()


