import pymrimrf
import numpy as np
from nose.tools import *

def test_hello():
    pymrimrf.hello("World")

def test_testarray():
    x = np.zeros((3, 4, 5), dtype=np.float)
    pymrimrf.testarray(x)
    


def test_pymrf_create():
    
    
    x = np.ones((3, 4, 5), dtype=np.float32)
    x[1, 2, 4] = 10
    x[2, 1, 2] = 20
    x[2, 1, 3] = 30
    x[2, 1, 4] = 40
    mrf = pymrimrf.MRIMRF(10, x)

    mrf.latentVars = x
    y = mrf.latentVars
    assert_equal(x.shape, y.shape)
    assert_equal(x[1, 2, 4], y[1, 2, 4])
    assert_equal(x[2, 1, 2], y[2, 1, 2])
    assert_equal(x[2, 1, 3], y[2, 1, 3])
    assert_equal(x[2, 1, 4], y[2, 1, 4])
    
def test_pymrf_run():
    x = np.ones((3, 4, 5), dtype=np.float32)
    mrf = pymrimrf.MRIMRF(10, x)

    mrf.latentVars = x
    for i in range(1000):
        mrf.sequential_gibbs_scan() # don't crash!
    
