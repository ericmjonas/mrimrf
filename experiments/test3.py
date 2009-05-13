#!/usr/bin/python
import numpy as np
from matplotlib import pyplot
import sys
sys.path.append("../")

from synth import synth
from synth import util
import core


N = 384
MAXPHASE = 5
#pb = synth.plane_box(N, 10, 1.0, 20)
bk = np.random.normal(0.0, 1.0, size = (N, N)) * 0.1
pb = synth.sphere(N, 100, 2.0, MAXPHASE*np.pi, bk)
#pb = synth.spirals(N=3, MAXPHASE=MAXPHASE*np.pi)
## pyplot.imshow(pb)
## pyplot.show()
pb_wrapped = util.wrap_phase(pb).astype(np.float32)
## pyplot.imshow(pb_wrapped)
## pyplot.show()
pyplot.ion()

print pb_wrapped.shape
pb_wrapped.shape = (1, pb_wrapped.shape[0], pb_wrapped.shape[1])
print pb_wrapped.shape
print pb_wrapped.dtype
mrf = core.pymrimrf.MRIMRF(MAXPHASE, pb_wrapped)
mrf.setSeed(5)

pyplot.subplot(2, 2, 1)

imresult = mrf.latentPhase
pyplot.imshow(pb_wrapped[0], cmap=pyplot.cm.gray, interpolation='nearest',
              vmin=-np.pi, vmax=np.pi)


pyplot.subplot(2, 2, 2)
plotim = pyplot.imshow(imresult[0], cmap=pyplot.cm.gray,
                       interpolation='nearest',
                       vmin=-MAXPHASE*np.pi,
                       vmax=MAXPHASE*np.pi)

## coloring = mrf.getColoring()
## fcoloring = (coloring % 256).astype(float)
## plotcoloring = pyplot.imshow(fcoloring[0], interpolation='nearest',
##                              vmin=0, vmax=256, cmap=pyplot.cm.prism)
pyplot.subplot(2, 2, 3)
plotWraps = pyplot.imshow(mrf.latentPhaseWraps[0].astype(np.float),
                          interpolation='nearest',
                          vmin = -MAXPHASE, vmax=MAXPHASE)

## plotDeltas = pyplot.imshow(mrf.latentPhaseWraps[0] == mrf.latentPhaseWraps[0],
##                           interpolation='nearest',
##                            vmin=0, vmax=1)

pyplot.subplot(2, 2, 4)
plotPartitions = pyplot.imshow(mrf.currentPartitioning[0],
                               interpolation = 'nearest',
                               vmin = 0, vmax=10)


pyplot.draw()

#temps = np.linspace(100, 1, 100, -1)
temps = np.concatenate((np.logspace(2, 0, 10, -1), np.ones(1000)))
print "trying", len(temps), "temps"
for t in temps:
    print "t = ", t, "score = ", mrf.score, mrf.score * t
    mrf.temp = t
    for i in range(10):
        mrf.random_gibbs_scan()
    mrf.ddmcmc_flip_gibbs(0.0)

    plotim.set_array(mrf.latentPhase[0])
    plotWraps.set_array(mrf.latentPhaseWraps[0].astype(np.float))
    plotPartitions.set_array(mrf.currentPartitioning[0])
    pyplot.draw()



pyplot.show()
