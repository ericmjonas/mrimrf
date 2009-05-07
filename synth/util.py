import numpy as np
from matplotlib import pyplot


def wrap_phase(x):

    y = (x + np.pi) % (np.pi*2) - np.pi
    return y

def test_wrap_phase():
    phi = np.linspace(-16, 16)
    phi_wrapped = wrap_phase(phi)
    pyplot.plot(phi, phi)
    pyplot.plot(phi, phi_wrapped)
    pyplot.show()
    
