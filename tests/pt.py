#!/usr/bin/python
import numpy as np
from matplotlib import pyplot
import sys
sys.path.append("../")
import tables

from synth import synth
from synth import util
import core
import experiments

outputTable = tables.openFile("output.h5", 'w')

N = 128
MAXPHASE = 10
#pb = synth.plane_box(N, 10, 1.0, 20)
bk = np.random.normal(0.0, 1.0, size = (N, N))
pb = synth.sphere(N, 48, 2.0, MAXPHASE*np.pi, bk)
t = outputTable.createArray("/", "truth", pb)
t.attrs.maxphase = MAXPHASE

#pb = synth.spirals(N=5, MAXPHASE=MAXPHASE*np.pi)
pb_wrapped = util.wrap_phase(pb).astype(np.float32)
#pb_wrapped = experiments.data.default(4)
pyplot.ion()

pb_wrapped.shape = (1, pb_wrapped.shape[0], pb_wrapped.shape[1])
t = outputTable.createArray("/", "data", pb_wrapped)


temps = [40., 30.,  25., 20.,  15, 10., 5.,  1.]
mrfs = core.pt.PT(pb_wrapped, MAXPHASE, temps)

rootchaing = outputTable.createGroup("/", "chains")
chaingroups = {}
for i, t in enumerate(temps):
    g = outputTable.createGroup(rootchaing, "chain_%d" % i)
    g._v_attrs.temp = t
    chaingroups[t] = g

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
    
for iter in xrange(1000):
    mrfs.run()
    for i in range(len(temps)):
        c = mrfs.chains[i]
        t = c.temp
        
        a = outputTable.createArray(chaingroups[t], "sample%05d" % iter, c.latentPhase)
        a.attrs.score = c.score
        a.attrs.iteration = iter
        
    if iter % 10 == 9:
        mrfs.attemptswap()
    for i in range(len(temps)):
        imdata[i].set_data(mrfs.chains[i].latentPhase[0])
    pyplot.draw()
    outputTable.flush()
pyplot.show()
