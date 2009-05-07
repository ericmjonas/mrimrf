#!/usr/bin/python
import numpy as np
from matplotlib import pyplot
import sys
sys.path.append("../")

from synth import synth
from synth import util
import core


pb = synth.plane_box(8, 2, 1.0, 1.0)
pb_wrapped = util.wrap_phase(pb).astype(np.float32)
print pb_wrapped.shape
pb_wrapped.shape = (1, pb_wrapped.shape[0], pb_wrapped.shape[1])

mrf = core.pymrimrf.MRIMRF(3, pb_wrapped)
mrf.latentVars = pb_wrapped.astype(np.float32)

