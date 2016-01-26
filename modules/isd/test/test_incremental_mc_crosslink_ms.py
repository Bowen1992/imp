from __future__ import print_function
import IMP.core
import IMP.algebra
import IMP.isd
import IMP.test

class Tests(IMP.test.TestCase):
    def test_incremental(self):
        """Test ISD restraints with incremental MC scoring"""
        num_balls = 2
        num_mc_steps = 10
        m = IMP.Model()
        bb = IMP.algebra.BoundingBox3D(IMP.algebra.Vector3D(0, 0, 0),
                                       IMP.algebra.Vector3D(30, 30, 30))

        p1=IMP.Particle(m)
        p1i=p1.get_index()
        d = IMP.core.XYZR.setup_particle(p1)
        d.set_radius(10)
        d.set_coordinates(IMP.algebra.get_random_vector_in(bb))
        d.set_coordinates_are_optimized(True)

        p2=IMP.Particle(m)
        p2i=p2.get_index()
        d = IMP.core.XYZR.setup_particle(p2)
        d.set_radius(10)
        d.set_coordinates(IMP.algebra.get_random_vector_in(bb))
        d.set_coordinates_are_optimized(True)

        psigma=IMP.Particle(m)
        s1i=psigma.get_index()
        s2i=psigma.get_index()
        sigma = IMP.isd.Nuisance.setup_particle(psigma)
        sigma.set_lower(0.)
        sigma.set_nuisance(10)
        sigma.set_nuisance_is_optimized(True)

        ppsi=IMP.Particle(m)
        psii=ppsi.get_index()
        psi = IMP.isd.Nuisance.setup_particle(ppsi)
        psi.set_lower(0.)
        psi.set_nuisance(0.05)
        psi.set_nuisance_is_optimized(True)

        dr = IMP.isd.CrossLinkMSRestraint(
                                    m,
                                    21.0,
                                    0.01)

        dr.add_contribution((p1i, p2i), (s1i, s2i), psii)

        mc = IMP.core.MonteCarlo(m)
        isf = IMP.core.IncrementalScoringFunction([p1,p2,psigma,ppsi], [dr])
        mc.set_incremental_scoring_function(isf)

        mvs = [IMP.core.BallMover([p], 5) for p in [p1,p2]]
        sm = IMP.core.SerialMover(mvs)
        mc.add_mover(sm)

        initial=isf.evaluate(False)
        after = mc.optimize(num_mc_steps)


if __name__ == '__main__':
    IMP.test.main()
