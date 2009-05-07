#!/usr/bin/python
import numpy as np
from matplotlib import pyplot
import sys
sys.path.append("../")

from synth import synth
from synth import util
import core


pb = synth.plane_box(32, 10, 1.0, 6.0)
pb_wrapped = util.wrap_phase(pb).astype(np.float32)
print pb_wrapped.shape
pb_wrapped.shape = (1, pb_wrapped.shape[0], pb_wrapped.shape[1])
print pb_wrapped.shape

mrf = core.pymrimrf.MRIMRF(3, pb_wrapped)
for i in range(1000):
    mrf.sequential_gibbs_scan()

pyplot.subplot(1, 4, 1)
pyplot.imshow(pb, cmap=pyplot.cm.gray)

pyplot.subplot(1, 4, 2)
pyplot.imshow(pb_wrapped[0], cmap=pyplot.cm.gray, vmin=-np.pi, vmax=np.pi,
              interpolation='nearest')

pyplot.subplot(1, 4, 3)
imresult = mrf.latentVars
pyplot.imshow(imresult[0], cmap=pyplot.cm.gray, interpolation='nearest')

pyplot.subplot(1, 4, 4)
mrf.latentVars = pb_wrapped
pyplot.imshow(mrf.latentVars[0, :, :],
              cmap=pyplot.cm.gray, interpolation='nearest')


pyplot.show()
