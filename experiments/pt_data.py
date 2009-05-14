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

outputTable = tables.openFile("output_real_data_1.h5", 'w')

pb_wrapped = experiments.data.default(1)

#pb_wrapped = experiments.data.default(4)
pyplot.ion()

pb_wrapped.shape = (1, pb_wrapped.shape[0], pb_wrapped.shape[1])
t = outputTable.createArray("/", "data", pb_wrapped)
MAXPHASE = 4

temps = [80, 60, 40., 30.,  25., 20.,  15, 10., 5.,  1.]
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
    
for iter in xrange(5000):
    mrfs.run()
    for i in range(len(temps)):
        c = mrfs.chains[i]
        t = c.temp
        
        a = outputTable.createArray(chaingroups[t], "sample%05d" % iter, c.latentPhase)
        a.attrs.score = c.score
        a.attrs.iteration = iter
    if iter % 10 == 4:
        mrfs.partial_swap()

    if iter % 10 == 9:
        mrfs.attemptswap()
    for i in range(len(temps)):
        imdata[i].set_data(mrfs.chains[i].latentPhase[0])
    pyplot.draw()
    outputTable.flush()
pyplot.show()
