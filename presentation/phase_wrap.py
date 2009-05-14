import numpy as np
from matplotlib import pyplot
import sys
sys.path.append("../")

from synth import synth
from synth import util
import core


N = 100
MAXPHASE = 10
#pb = synth.plane_box(N, 10, 1.0, 20)
bk = np.random.normal(0.0, 1.0, size = (N, N)) * 0.1
pb = synth.sphere2(N, 40, 2.0, MAXPHASE*np.pi, bk)
pb_wrapped = util.wrap_phase(pb).astype(np.float32)
print pb_wrapped.shape

ax = pyplot.subplot(2, 2, 1)
pyplot.imshow(pb, cmap=pyplot.cm.gray, interpolation='nearest',
              vmin=0, vmax=np.pi*MAXPHASE)
ax.set_xticks([])
ax.set_yticks([])

ax = pyplot.subplot(2, 2, 2)
pyplot.plot(pb[50, :])
ax.set_xticks([])
ax.set_yticks([])
    
ax = pyplot.subplot(2, 2, 3)
pyplot.imshow(pb_wrapped, cmap=pyplot.cm.gray, interpolation='nearest',
              vmin=-np.pi, vmax=np.pi)
ax.set_xticks([])
ax.set_yticks([])

ax = pyplot.subplot(2, 2, 4)
pyplot.plot(pb_wrapped[50, :])
ax.set_xticks([])
ax.set_yticks([])

pyplot.show()
