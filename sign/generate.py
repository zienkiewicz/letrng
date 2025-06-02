#!/usr/bin/env python

from Crypto.PublicKey import RSA
from Crypto import Random
from io import BytesIO

with open('../postprocessed.bin', 'rb') as f:
    seed = f.read()

class DeterministicRNG:
    def __init__(self, seed_bytes):
        self.buffer = BytesIO(seed_bytes)

    def read(self, n):
        return self.buffer.read(n)

rng1 = DeterministicRNG(seed)
key1 = RSA.generate(2048, randfunc=rng1.read)
with open("first.pem", "wb") as f:
    f.write(key1.export_key())

rng2 = DeterministicRNG(seed[1000000:])
key2 = RSA.generate(2048, randfunc=rng2.read)
with open("second.pem", "wb") as f:
    f.write(key2.export_key())

