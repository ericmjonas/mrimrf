import numpy as np
from matplotlib import pyplot
import sys
sys.path.append("../")
import tables

h5file = tables.openFile("output_real_data_4.h5")

chainscores = {}
for c in h5file.root.chains._v_children:
    cg = h5file.root.chains._v_children[c]
    temp = cg._v_attrs.temp

    scores = np.zeros(len(cg._v_children))
    iters = np.zeros(len(cg._v_children), dtype=np.uint32)

    for i, s in enumerate(cg._v_children):
        iter = int(s[len('sample'):])
        scores[i] = cg._v_children[s].attrs.score
        iters[i] =  cg._v_children[s].attrs.iteration

    s = scores[np.argsort(iters)]
    chainscores[temp] = s * temp

for t, s in chainscores.iteritems():
    if t < 25:
        pyplot.plot(s, label="temp = %2f" % t)
pyplot.legend(loc=4)
pyplot.xlabel('sample')
pyplot.ylabel('score (log)')
pyplot.title("output_real_data.h5")

pyplot.figure()
ax = pyplot.subplot(1, 2, 2)
pyplot.imshow(h5file.root.chains.chain_9.sample00999.read()[0], cmap=pyplot.cm.gray,
              vmin=0, vmax=2 *  np.pi)
ax.set_xticks([])
ax.set_yticks([])


ax =pyplot.subplot(1, 2, 1)
pyplot.imshow(h5file.root.data.read()[0],  cmap=pyplot.cm.gray,
              vmin=0, vmax= np.pi)
ax.set_xticks([])
ax.set_yticks([])

pyplot.figure()
iters = [2, 50, 100, 200, 300, 400, 800, 999]

tempsets = {}

for iter in iters:
    for c in h5file.root.chains._v_children:
        cg = h5file.root.chains._v_children[c]
        t = cg._v_attrs.temp
        if t not in tempsets:
            tempsets[t] = []

        # get sample
        array = cg._v_children['sample%05d' %  iter]
        tempsets[t].append(array.read()[0])
        print len(tempsets[t])
        
# now the plotting:
temps = tempsets.keys()
temps.sort()
temps = temps[::-1]

TOPTEMPS = 10
pos = 1
for t in temps[:TOPTEMPS]:
    for i, iter in enumerate(iters):
        ax = pyplot.subplot(TOPTEMPS, len(iters), pos)
        ts = tempsets[t]
        pyplot.imshow(ts[i], cmap = pyplot.cm.gray, vmin=-10*np.pi,  vmax=10*np.pi)
        ax.set_xticks([])
        ax.set_yticks([])
        pos += 1

pyplot.show()

