#!/usr/bin/python
import numpy as np
from matplotlib import pyplot
import sys
sys.path.append("../")

from synth import synth
from synth import util
import core

pyplot.ion()

N = 40
#pb = synth.plane_box(N, 10, 1.0, 6)
pb = synth.sphere(N, 15, 1.0, 20)
pb += np.random.rand(N, N) * 3
pb_wrapped = util.wrap_phase(pb).astype(np.float32)
print pb_wrapped.shape
pb_wrapped.shape = (1, pb_wrapped.shape[0], pb_wrapped.shape[1])
print pb_wrapped.shape

mrf = core.pymrimrf.MRIMRF(2, pb_wrapped)
mrf.setSeed(3)

imresult = mrf.latentVars
plotim = pyplot.imshow(imresult[0], cmap=pyplot.cm.gray,
                       interpolation='nearest', vmin=-10, vmax=10)


temps = np.linspace(100, 1, 100, -1)
print "trying", len(temps), "temps"
for t in temps:
    print "t = ", t
    mrf.temp = t
    for i in range(200):
        mrf.sequential_gibbs_scan()
    plotim.set_array(mrf.latentVars[0])
    pyplot.draw()



pyplot.show()
