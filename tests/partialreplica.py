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
pb = synth.spirals(N=5, MAXPHASE=MAXPHASE*np.pi)
pb_wrapped = util.wrap_phase(pb).astype(np.float32)

pyplot.ion()

pb_wrapped.shape = (1, pb_wrapped.shape[0], pb_wrapped.shape[1])

temps = [100., 80., 60., 40., 30., 20., 10.,  1.]
mrfs = core.pt.PT(pb_wrapped, MAXPHASE, temps)

imdata = []
for ti in range(len(temps)):
    ax = pyplot.subplot(np.ceil(len(temps)/4.0), 4, ti + 1)
    imresult = mrfs.chains[ti].latentPhase
    plotim = ax.imshow(imresult[0], cmap=pyplot.cm.gray,
                           interpolation='nearest',
                           vmin=-MAXPHASE*np.pi,
                           vmax=MAXPHASE*np.pi)
    ax.set_xticks([])
    ax.set_yticks([])
    
    imdata.append(plotim)
    
for iter in xrange(10000):
    mrfs.run()
    #mrfs.attemptswap()
    mrfs.partial_swap()
    for i in range(len(temps)):
        imdata[i].set_data(mrfs.chains[i].latentPhase[0])
    pyplot.draw()



pyplot.show()
