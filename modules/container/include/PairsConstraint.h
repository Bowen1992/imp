/**
 *  \file PairsConstraint.h
 *  \brief Use a PairModifier applied to a ParticlePairs to
 *  maintain an invariant
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 */

#ifndef IMPCONTAINER_PAIRS_CONSTRAINT_H
#define IMPCONTAINER_PAIRS_CONSTRAINT_H

#include "container_config.h"
#include <IMP/PairContainer.h>
#include <IMP/PairModifier.h>
#include <IMP/Constraint.h>

IMP_BEGIN_NAMESPACE
// for swig
class PairContainer;
class PairModifier;
IMP_END_NAMESPACE

IMPCONTAINER_BEGIN_NAMESPACE
//! Apply a PairFunction to a PairContainer to maintain an invariant
/** The score state is passed up to two PairModifiers, one to
    apply before evaluation and the other after. The one after
    should take a DerivativeAccumulator as its last argument for
    PairModifier::apply() and will only be called if
    the score was computed with derivatives.

    An example showing a how to use such a score state to maintain a cover
    of the atoms of a protein by a sphere per residue.
    \verbinclude cover_particles.py

    \see PairConstraint
 */
class IMPCONTAINEREXPORT PairsConstraint : public Constraint
{
  IMP::internal::OwnerPointer<PairModifier> f_;
  IMP::internal::OwnerPointer<PairModifier> af_;
  IMP::internal::OwnerPointer<PairContainer> c_;
public:
  /** \param[in] c The Container to hold the elements to process
      \param[in] before The PairModifier to apply to all elements
      before evaluate.
      \param[in] after The PairModifier to apply to all elements
      after evaluate.
      \param[in] name The object name
   */
  PairsConstraint(PairContainer *c, PairModifier *before,
                       PairModifier *after,
                       std::string name="PairConstraint %1%");

  //! Apply this modifier to all the elements after an evaluate
  void set_after_evaluate_modifier(PairModifier* f) {
    af_=f;
  }

  //! Apply this modifier to all the elements before an evaluate
  void set_before_evaluate_modifier(PairModifier* f) {
    f_=f;
  }

  IMP_CONSTRAINT(PairsConstraint);
};


IMPCONTAINER_END_NAMESPACE

#endif  /* IMPCONTAINER_PAIRS_CONSTRAINT_H */
