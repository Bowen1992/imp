from __future__ import print_function
import IMP
import IMP.test
import IMP.container
import math


class Tests(IMP.test.TestCase):

    """Tests for all pairs pair container"""

    def test_allp(self):
        """Checking decomposition of plural restraints"""
        m = IMP.Model()
        for i in range(0, 10):
            p = IMP.Particle(m)
            IMP.core.XYZR.setup_particle(p)
        lp = IMP.container.ListSingletonContainer(m.get_particles())
        apc = IMP.container.AllPairContainer(lp)
        dps = IMP.core.SoftSpherePairScore(1)
        r = IMP.container.PairsRestraint(dps, apc)
        dr = r.create_decomposition()
        print(dr.evaluate(False))
        cdr = IMP.RestraintSet.get_from(dr).get_restraints()
        # IMP.base.set_log_level(IMP.base.VERBOSE)
        print("deling")
        del dr
        print("eval single")
        for rc in cdr:
            print(rc.evaluate(False))
if __name__ == '__main__':
    IMP.test.main()
