#!/usr/bin/python
import numpy as np
from matplotlib import pyplot
import sys
sys.path.append("../")

from synth import synth
from synth import util
import core

N = 40
pb = synth.plane_box(N, 10, 1.0, 6)
pb_wrapped = util.wrap_phase(pb).astype(np.float32)
print pb_wrapped.shape
pb_wrapped.shape = (1, pb_wrapped.shape[0], pb_wrapped.shape[1])
print pb_wrapped.shape

mrf = core.pymrimrf.MRIMRF(2, pb_wrapped)
temps = np.linspace(100, 1, 100, -1)
print "trying", len(temps), "temps"
for t in temps:
    print "t = ", t
    mrf.temp = t
    for i in range(200):
        mrf.random_gibbs_scan()

pyplot.subplot(2, 3, 1)
pyplot.imshow(pb, cmap=pyplot.cm.gray)
pyplot.subplot(2, 3, 4)
pyplot.plot(pb[N/2, :])


pyplot.subplot(2, 3, 2)
pyplot.imshow(pb_wrapped[0], cmap=pyplot.cm.gray, vmin=-np.pi, vmax=np.pi,
              interpolation='nearest')
pyplot.subplot(2, 3, 5)
pyplot.plot(pb_wrapped[0, N/2, :])

pyplot.subplot(2, 3, 3)
imresult = mrf.latentVars
pyplot.imshow(imresult[0], cmap=pyplot.cm.gray, interpolation='nearest')
pyplot.subplot(2, 3, 6)
pyplot.plot(imresult[0, N/2, :])


pyplot.show()
