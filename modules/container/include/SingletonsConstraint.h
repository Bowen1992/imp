/**
 *  \file SingletonsConstraint.h
 *  \brief Use a SingletonModifier applied to a Particles to
 *  maintain an invariant
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 */

#ifndef IMPCONTAINER_SINGLETONS_CONSTRAINT_H
#define IMPCONTAINER_SINGLETONS_CONSTRAINT_H

#include "container_config.h"
#include <IMP/SingletonContainer.h>
#include <IMP/SingletonModifier.h>
#include <IMP/Constraint.h>

IMP_BEGIN_NAMESPACE
// for swig
class SingletonContainer;
class SingletonModifier;
IMP_END_NAMESPACE

IMPCONTAINER_BEGIN_NAMESPACE
//! Apply a SingletonFunction to a SingletonContainer to maintain an invariant
/** The score state is passed up to two SingletonModifiers, one to
    apply before evaluation and the other after. The one after
    should take a DerivativeAccumulator as its last argument for
    SingletonModifier::apply() and will only be called if
    the score was computed with derivatives.

    An example showing a how to use such a score state to maintain a cover
    of the atoms of a protein by a sphere per residue.
    \verbinclude cover_particles.py

    \see SingletonConstraint
 */
class IMPCONTAINEREXPORT SingletonsConstraint : public Constraint
{
  IMP::internal::OwnerPointer<SingletonModifier> f_;
  IMP::internal::OwnerPointer<SingletonModifier> af_;
  IMP::internal::OwnerPointer<SingletonContainer> c_;
public:
  /** \param[in] c The Container to hold the elements to process
      \param[in] before The SingletonModifier to apply to all elements
      before evaluate.
      \param[in] after The SingletonModifier to apply to all elements
      after evaluate.
      \param[in] name The object name
   */
  SingletonsConstraint(SingletonContainer *c, SingletonModifier *before,
                       SingletonModifier *after,
                       std::string name="SingletonConstraint %1%");

  //! Apply this modifier to all the elements after an evaluate
  void set_after_evaluate_modifier(SingletonModifier* f) {
    af_=f;
  }

  //! Apply this modifier to all the elements before an evaluate
  void set_before_evaluate_modifier(SingletonModifier* f) {
    f_=f;
  }

  IMP_CONSTRAINT(SingletonsConstraint);
};


IMPCONTAINER_END_NAMESPACE

#endif  /* IMPCONTAINER_SINGLETONS_CONSTRAINT_H */
