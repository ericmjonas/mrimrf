#!/usr/bin/python
import numpy as np
from matplotlib import pyplot
import sys
sys.path.append("../")

from synth import synth
from synth import util
import core


N = 100
MAXPHASE = 5
#pb = synth.plane_box(N, 10, 1.0, 20)
#bk = np.random.normal(0.0, 1.0, size = (N, N))
#pb = synth.sphere(N, 35, 2.0, MAXPHASE*np.pi, bk)
pb = synth.spirals(N=3, MAXPHASE=MAXPHASE*np.pi)
pb_wrapped = util.wrap_phase(pb).astype(np.float32)

pyplot.ion()

pb_wrapped.shape = (1, pb_wrapped.shape[0], pb_wrapped.shape[1])

temps = np.linspace(10, 100, 10)
mrfs = core.pt.PT(pb_wrapped, MAXPHASE, temps)

imdata = []
for ti in range(len(temps)):
    pyplot.subplot(len(temps), 1, ti + 1)
    imresult = mrfs.chains[ti].latentPhase
    plotim = pyplot.imshow(imresult[0], cmap=pyplot.cm.gray,
                           interpolation='nearest',
                           vmin=-MAXPHASE*np.pi,
                           vmax=MAXPHASE*np.pi)
    imdata.append(plotim)
    
for iter in xrange(10000):
    for i in range(10):
        mrfs.run()
        mrfs.attemptswap()
    for i in range(len(temps)):
        imdata[i].set_data(mrfs.chains[i].latentPhase[0])
    pyplot.draw()



pyplot.show()
