/**
 *  \file MinimumTripletScoreRestraint.h
 *  \brief Score based on the minimum score over a set of Triplets
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 */

#ifndef IMPCORE_MINIMUM_TRIPLET_SCORE_RESTRAINT_H
#define IMPCORE_MINIMUM_TRIPLET_SCORE_RESTRAINT_H

#include "config.h"
#include <IMP/Restraint.h>
#include <IMP/TripletScore.h>
#include <IMP/TripletContainer.h>

IMPCORE_BEGIN_NAMESPACE

//! Score based on the minimum over a set of Triplets
/** The score is evaluated for each of the Triplets in the container
    and the value of the minumum n scores is used. That is,
    if n is 1, the value of the restraint is the value of the lowest
    score over the container.
    \see MaximumTripletScoreRestraint
 */
class IMPCOREEXPORT MinimumTripletScoreRestraint
: public Restraint
{
  IMP::internal::OwnerPointer<TripletScore> f_;
  IMP::internal::OwnerPointer<TripletContainer> c_;
  unsigned int n_;
public:
  /** n is the number of minimumal scores to use.
   */
  MinimumTripletScoreRestraint(TripletScore *f,
                                 TripletContainer *c,
                                 unsigned int n=1,
                                 std::string name
                                 ="MinimumTripletScoreRestraint %1%");

  IMP_RESTRAINT(MinimumTripletScoreRestraint, get_module_version_info());

  //! Set the number of lowest scores to use.
  void set_n(unsigned int n) { n_=n;}
};


IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_MINIMUM_TRIPLET_SCORE_RESTRAINT_H */
