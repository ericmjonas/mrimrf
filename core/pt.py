import pymrimrf

import numpy as np
from matplotlib import pyplot

class PT(object):
    def __init__(self, data, maxphase, temps=(1.0,)):
        """
        Create the PT data
        """

        self.chains = [pymrimrf.MRIMRF(maxphase, data) for t in temps]
        for c, t in zip(self.chains, temps):
            c.temp = t
            
    def run(self):
        for c in self.chains:
            c.sequential_gibbs_scan()

    def attemptswap(self):
        if len(self.chains) > 1:
            i = np.random.randint(0, len(self.chains) -1)

            low_c = self.chains[i]
            high_c = self.chains[i+1]
            
            lowscore = low_c.score  # * low_c.temp
            highscore = high_c.score #* high_c.temp
            lowtemp = low_c.temp
            hightemp = high_c.temp

            score = (highscore / low_c.temp + lowscore / high_c.temp)
            score -= (lowscore / low_c.temp + highscore / high_c.temp)

            p = np.exp(score)

            print "considering a swap between chains ", i,  "and", i+1, score
            u = np.random.rand()

            if u < p:
                # accept
                print "accepted swap" 
                self.chains[i] = high_c
                high_c.temp = lowtemp
                
                self.chains[i+1] = low_c
                low_c.temp = hightemp
            else:
                # nothing
                pass

            
                
            
            
        
