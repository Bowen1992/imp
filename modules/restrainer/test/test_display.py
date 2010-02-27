import sys
import os
import unittest
import IMP
import IMP.test
import IMP.restrainer
import IMP.atom
import IMP.display

class RestraintTest(IMP.test.TestCase):
    def setUp(self):
        IMP.test.TestCase.setUp(self)

        RepParser = IMP.restrainer.XMLRepresentation(self.get_input_file_name('pdb_representation.xml'))
        RestraintParser = IMP.restrainer.XMLRestraint(self.get_input_file_name('pdb_connectivity_restraint.xml'))
        DisplayParser = IMP.restrainer.XMLDisplay(self.get_input_file_name('pdb_display.xml'))

        self.representation = RepParser.run()
        self.restraint = RestraintParser.run()
        self.display = DisplayParser.run()

        self.Model = self.representation.to_model()
        self.restraint.add_to_representation(self.representation)

        self.log = self.display.create_log(self.representation, 'pdb_log_%03d.py')
        self.log.write('initial.py')

    def test_show(self):
        self.Model.show()
        self.Model.evaluate(False)

if __name__ == '__main__':
    unittest.main()
