## \example core/pair_restraint.py
# Restrain the distance between a pair of particles.
#

import IMP.example
import sys

IMP.setup_from_argv(sys.argv, "pair restraint")

(m, c) = IMP.example.create_model_and_particles()

uf = IMP.core.Harmonic(0, 1)
df = IMP.core.DistancePairScore(uf)
r = IMP.core.PairRestraint(df, (c.get_particle(0), c.get_particle(1)))
print(r.evaluate(False))
