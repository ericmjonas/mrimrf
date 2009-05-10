#!/usr/bin/python
import numpy as np
from matplotlib import pyplot
import sys
sys.path.append("../")

from synth import synth
from synth import util
import core

pyplot.ion()

N = 100
MAXPHASE = 6
#pb = synth.plane_box(N, 10, 1.0, 20)
pb = synth.sphere(N, 30, 1.0, MAXPHASE*np.pi)
pb += np.random.rand(N, N) * 0
pb_wrapped = util.wrap_phase(pb).astype(np.float32)
print pb_wrapped.shape
pb_wrapped.shape = (1, pb_wrapped.shape[0], pb_wrapped.shape[1])
print pb_wrapped.shape
print pb_wrapped.dtype
mrf = core.pymrimrf.MRIMRF(MAXPHASE, pb_wrapped)
mrf.setSeed(5)

imresult = mrf.latentPhase
plotim = pyplot.imshow(imresult[0], cmap=pyplot.cm.gray,
                       interpolation='nearest',
                       vmin=-MAXPHASE*2*np.pi,
                       vmax=MAXPHASE*2*np.pi)

temps = np.linspace(100, 1, 100, -1)
print "trying", len(temps), "temps"
for t in temps:
    print "t = ", t, "score = ", mrf.score, mrf.score * t
    mrf.temp = t
    for i in range(100):
        mrf.sequential_gibbs_scan()
    plotim.set_array(mrf.latentPhase[0])
    pyplot.draw()



pyplot.show()
