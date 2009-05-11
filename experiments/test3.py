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
MAXPHASE = 20
#pb = synth.plane_box(N, 10, 1.0, 20)
bk = np.random.normal(0.0, 1.0, size = (N, N))
pb = synth.sphere(N, 35, 2.0, MAXPHASE*np.pi, bk)

pb_wrapped = util.wrap_phase(pb).astype(np.float32)
print pb_wrapped.shape
pb_wrapped.shape = (1, pb_wrapped.shape[0], pb_wrapped.shape[1])
print pb_wrapped.shape
print pb_wrapped.dtype
mrf = core.pymrimrf.MRIMRF(MAXPHASE, pb_wrapped)
mrf.setSeed(5)

pyplot.subplot(1, 3, 1)

imresult = mrf.latentPhase
pyplot.imshow(pb_wrapped[0], cmap=pyplot.cm.gray, interpolation='nearest',
              vmin=-np.pi, vmax=np.pi)

pyplot.subplot(1, 3, 2)
plotim = pyplot.imshow(imresult[0], cmap=pyplot.cm.gray,
                       interpolation='nearest',
                       vmin=-MAXPHASE*np.pi,
                       vmax=MAXPHASE*np.pi)

pyplot.subplot(1, 3, 3)
## coloring = mrf.getColoring()
## fcoloring = (coloring % 256).astype(float)
## plotcoloring = pyplot.imshow(fcoloring[0], interpolation='nearest',
##                              vmin=0, vmax=256, cmap=pyplot.cm.prism)
plotWraps = pyplot.imshow(mrf.latentPhaseWraps[0].astype(np.float),
                          interpolation='nearest',
                          vmin = -MAXPHASE, vmax=MAXPHASE)

## pyplot.subplot(1, 3, 3)
## plotDeltas = pyplot.imshow(mrf.latentPhaseWraps[0] == mrf.latentPhaseWraps[0],
##                           interpolation='nearest',
##                            vmin=0, vmax=1)

temps = np.linspace(30, 1, 30, -1)
print "trying", len(temps), "temps"
for t in temps:
    print "t = ", t, "score = ", mrf.score, mrf.score * t
    mrf.temp = t
    for i in range(100):
        mrf.sequential_gibbs_scan()
    latent_pre_sw = mrf.latentPhase[0]
    #mrf.swendsenWangMove()
    latent_post_sw = mrf.latentPhase[0]
    print np.sum((latent_pre_sw != latent_post_sw)), "pixels changed"
    #plotDeltas.set_array(latent_pre_sw != latent_post_sw)
    plotim.set_array(mrf.latentPhase[0])
    plotWraps.set_array(mrf.latentPhaseWraps[0].astype(np.float))
    pyplot.draw()



pyplot.show()
