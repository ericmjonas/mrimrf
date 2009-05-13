"""

Code to load in the datasets

"""

import nifti
from matplotlib import pyplot
import numpy as np

def default(subsample=1):
    nim = nifti.NiftiImage("../../data/test.out.nii")

    d = nim.data[0, ::subsample, ::subsample].astype(float)
    d = (d / 2**12 - 0.5) * 2*np.pi
    return d.astype(np.float32)

if __name__ == "__main__":
    d = default()
    pyplot.figure()
    pyplot.plot(d[250, :])
    pyplot.figure()
    pyplot.imshow(d)
    pyplot.show()
