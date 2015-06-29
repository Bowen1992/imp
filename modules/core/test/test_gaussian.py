#!/usr/bin/env python

import IMP
import IMP.core
import IMP.atom
import IMP.algebra
import IMP.test
import numpy as np
import numpy.random
from math import cos, sin, pi, sqrt, exp, log


def create_test_points(mu, radii):
    testers = [[mu[0], mu[1], mu[2]]]
    for i in range(3):
        t = mu[:]
        t[i] += radii[i] + 1
        testers.append(t)
        t = mu[:]
        t[i] -= radii[i]
        testers.append(t)
    return testers


class TestGaussian(IMP.test.TestCase):

    def test_gauss_rigid_body(self):
        """Test decorating an existing rigid body as a Gaussian"""
        m = IMP.Model()
        p = IMP.Particle(m)
        t0 = IMP.algebra.Transformation3D(
                           IMP.algebra.get_identity_rotation_3d(),
                           IMP.algebra.Vector3D(0,0,1))
        r0 = IMP.algebra.ReferenceFrame3D(t0)
        t1 = IMP.algebra.Transformation3D(
                           IMP.algebra.get_identity_rotation_3d(),
                           IMP.algebra.Vector3D(1,0,0))
        r1 = IMP.algebra.ReferenceFrame3D(t1)
        r = IMP.core.RigidBody.setup_particle(p, r0)
        g = IMP.core.Gaussian.setup_particle(p,
                           IMP.algebra.Gaussian3D(r1, [1,20,30]))
        t = g.get_reference_frame().get_transformation_to().get_translation()
        self.assertEqual([x for x in t], [1,0,0])

    def gauss_setup(self):

        # setup problem
        self.m = IMP.Model()
        self.mu = np.random.random_sample(3,) * 20
        self.std = [5, 10, 20]
        self.var = [s ** 2 for s in self.std]
        az = np.random.random() * pi
        ay = np.random.random() * pi
        ax = np.random.random() * pi
        Rz = np.array([[cos(az), -sin(az), 0],
                       [sin(az), cos(az), 0],
                       [0, 0, 1]])
        Ry = np.array([[cos(ay), 0, -sin(ay)],
                       [0, 1, 0],
                       [sin(ay), 0, cos(ay)]])
        Rx = np.array([[1, 0, 0],
                       [0, cos(ax), -sin(ax)],
                       [0, sin(ax), cos(ax)]])
        self.rotation = np.dot(Rz, np.dot(Rx, Ry))
        covar = np.dot(
            self.rotation,
            np.dot(np.diag(self.var),
                   np.transpose(self.rotation)))

        self.p0 = IMP.Particle(self.m)
        shape0 = IMP.algebra.get_gaussian_from_covariance(covar.tolist(),
                                                          self.mu.tolist())
        self.g0 = IMP.core.Gaussian.setup_particle(self.p0, shape0)

    def test_gaussian_setup(self):
        '''check gaussian setup correctly defines the rotation'''

        # create test points along axes
        self.gauss_setup()
        init_test_pts = create_test_points([0, 0, 0], self.std)

        # use transform on the points
        init_rot = IMP.algebra.get_rotation_from_matrix(
            *np.ndarray.flatten(self.rotation))
        init_trans = IMP.algebra.Transformation3D(init_rot, self.mu)

        # now decorate a particle with the gaussian and see if it has the right
        # parameters
        final_test_pts = create_test_points([0, 0, 0],
                     [sqrt(x) for x in self.g0.get_gaussian().get_variances()])
        final_trans = self.g0.get_gaussian(
        ).get_reference_frame(
        ).get_transformation_to(
        )

        # finally do comparison
        max_min_dist = 0.0
        not_chosen = set(range(len(final_test_pts)))
        vi = [init_trans.get_transformed(IMP.algebra.Vector3D(p1))
              for p1 in init_test_pts]
        vf = [final_trans.get_transformed(IMP.algebra.Vector3D(p2))
              for p2 in final_test_pts]
        for ni, i in enumerate(vi):
            min_dist = 1000.0
            min_nf = 0
            for nf in not_chosen:
                dist = IMP.algebra.get_distance(IMP.algebra.Vector3D(i),
                                                IMP.algebra.Vector3D(vf[nf]))
                if dist < min_dist:
                    min_dist = dist
                    min_nf = nf
            if min_dist > max_min_dist:
                max_min_dist = min_dist
            not_chosen -= set([min_nf])
        self.assertLessEqual(max_min_dist, 3.0)

    def test_gaussian_consistency(self):
        """ test if gaussian stays a gaussian!"""
        m = IMP.Model()
        rbd = IMP.core.RigidBody.setup_particle(IMP.Particle(m, "rb"),
                                                IMP.algebra.ReferenceFrame3D())
        p = IMP.Particle(m)
        IMP.core.Gaussian.setup_particle(p,IMP.algebra.Gaussian3D(
            IMP.algebra.ReferenceFrame3D(),[1,1,1]))
        self.assertEqual(IMP.core.Gaussian.get_is_setup(p),True)
        rbd.add_non_rigid_member(p)
        self.assertEqual(IMP.core.Gaussian.get_is_setup(p),True)

if __name__ == '__main__':
    IMP.test.main()
