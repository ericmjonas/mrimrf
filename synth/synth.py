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
