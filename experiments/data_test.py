import nifti
from matplotlib import pyplot
import numpy as np

nim = nifti.NiftiImage("../../data/test.out.nii")

print nim.data.shape
d = nim.data[0].astype(float)
d = (d / 2**12 - 0.5) * np.pi
print np.max(d)

pyplot.figure()
pyplot.plot(d[250, :])
pyplot.figure()
pyplot.imshow(d)
pyplot.show()
