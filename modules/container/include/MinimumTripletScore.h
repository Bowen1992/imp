/**
 *  \file IMP/container/MinimumTripletScore.h
 *  \brief Define TripletScore.
 *
 *  This file is generated by a script (tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 */

#ifndef IMPCONTAINER_MINIMUM_TRIPLET_SCORE_H
#define IMPCONTAINER_MINIMUM_TRIPLET_SCORE_H

#include "container_config.h"
#include <IMP/TripletScore.h>
#include <IMP/triplet_macros.h>

IMPCONTAINER_BEGIN_NAMESPACE

//! Evaluate the min or max n particle_triplet scores of the passed set
/** Each of the set of TripletScores is evaluated and the sum of the
    minimum n is returned.
*/
class IMPCONTAINEREXPORT MinimumTripletScore : public TripletScore
{
  TripletScores scores_;
  unsigned int n_;
public:
  MinimumTripletScore(const TripletScoresTemp &scores,
                       unsigned int n=1,
                       std::string name="TripletScore %1%");
  IMP_TRIPLET_SCORE(MinimumTripletScore);

  Restraints create_current_decomposition(const ParticleTriplet& vt) const;
};

IMP_OBJECTS(MinimumTripletScore,MinimumTripletScores);

IMPCONTAINER_END_NAMESPACE

#endif  /* IMPCONTAINER_MINIMUM_TRIPLET_SCORE_H */
