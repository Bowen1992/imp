/**
 *  \file PbcBoxedRigidBodyMover.h
 *  \brief A mover that transform a rigid body
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPMEMBRANE_PBC_BOXED_RIGID_BODY_MOVER_H
#define IMPMEMBRANE_PBC_BOXED_RIGID_BODY_MOVER_H

#include "membrane_config.h"
#include <IMP/core/MonteCarlo.h>
#include <IMP/algebra/Vector3D.h>
#include <IMP/algebra/Transformation3D.h>
#include <IMP/SingletonContainer.h>
//#include <IMP/core/rigid_bodies.h>
//#include <IMP/core/mover_macros.h>

IMPMEMBRANE_BEGIN_NAMESPACE

//! Modify the transformation of a rigid body
/** The transformation of a rigid body is moved in a ball of given
    size. The probability distribution is uniform over the ball.
    \see MonteCarlo
 */
class IMPMEMBRANEEXPORT PbcBoxedRigidBodyMover:public core::MonteCarloMover
{
public:
  /** The rigid body is rotated and translated to move
      \param[in] d   the rigid body decorator
      \param[in] max_translation maximum translation during a step
      \param[in] max_rotation maximum rotation angle in radians
   */
  PbcBoxedRigidBodyMover(core::RigidBody d, Particles ps,
                 Float max_translation,
                 Float max_rotation, algebra::Vector3Ds centers,
                 algebra::Transformation3Ds transformations,
                 Particle *px, Particle *py, Particle *pz);

protected:
  virtual IMP::ModelObjectsTemp do_get_inputs() const IMP_OVERRIDE;
  virtual core::MonteCarloMoverResult do_propose() IMP_OVERRIDE;
  virtual void do_reject() IMP_OVERRIDE;


  IMP_OBJECT_METHODS(PbcBoxedRigidBodyMover);
  IMP_SHOWABLE(PbcBoxedRigidBodyMover);

private:

  algebra::Transformation3D last_transformation_;
    algebra::Vector3Ds oldcoords_;
    algebra::Transformation3Ds oldtrs_;
    Float max_translation_;
    Float max_angle_;
    algebra::Vector3Ds centers_;
    algebra::Transformation3Ds transformations_;
    core::RigidBody d_;
    IMP::Particles ps_;
    IMP::Particles ps_norb_;
    std::vector<core::RigidBody> rbs_;

  // particles for scaling
  IMP::PointerMember<IMP::Particle> px_;
  IMP::PointerMember<IMP::Particle> py_;
  IMP::PointerMember<IMP::Particle> pz_;

  algebra::Vector3D get_vector(algebra::Vector3D center);
  algebra::Transformation3D get_transformation(algebra::Transformation3D trans);
  IMP::Particles         get_particles(IMP::Particles ps);
  std::vector<core::RigidBody> get_rigid_bodies(IMP::Particles ps);



};

IMPMEMBRANE_END_NAMESPACE

#endif  /* IMPMEMBRANE_PBC_BOXED_RIGID_BODY_MOVER_H */
