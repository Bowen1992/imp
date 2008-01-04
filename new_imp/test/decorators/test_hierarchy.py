import unittest
import IMP
import IMP.test

class DecoratorTests(IMP.test.TestCase):
    def test_hierarchy(self):
        """Check that the hierarchy works fine"""
        m = IMP.Model()
        pp=IMP.Particle()
        m.add_particle(pp)
        pp.thisown=0
        ppd= IMP.HierarchyDecorator.cast(pp)
        ppnd= IMP.NameDecorator.create(pp)
        ppnd.set_name("root");
        for i in range(0,10):
            pc=IMP.Particle()
            pcd= IMP.HierarchyDecorator.create(pc)
            m.add_particle(pc)
            ppd.add_child(pcd)
            pcnd= IMP.NameDecorator.create(pc)
            pcnd.set_name(str("child "+str(i)));

        self.assertEqual(ppd.get_number_of_children(), 10,
                         "Number of children is not as expected")
        ppd.validate()


if __name__ == '__main__':
    unittest.main()
