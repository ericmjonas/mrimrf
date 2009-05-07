#!/usr/bin/python
import numpy as n
import pywrap

# random test
r = n.random.rand(10, 12)
rp = pywrap.identity2(r)
print r.shape
print rp.shape

assert (r == rp).all()


# random test
r = n.random.randint(1, 255, (200, 300)).astype(n.uint8); 

rp = pywrap.identity4(r)
print r.shape
print rp.shape
assert (r == rp).all()
