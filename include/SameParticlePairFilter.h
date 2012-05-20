/**
 *  \file SameParticlePairFilter.h
 *  \brief A Score on the distance between a pair of particles.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 */

#ifndef IMPMEMBRANE_SAME_PARTICLE_PAIR_FILTER_H
#define IMPMEMBRANE_SAME_PARTICLE_PAIR_FILTER_H

#include "membrane_config.h"
#include <IMP/PairPredicate.h>
#include <IMP/pair_macros.h>

IMPMEMBRANE_BEGIN_NAMESPACE

//!
/** Filter particles belonging to the same rigid body
*/
class IMPMEMBRANEEXPORT SameParticlePairFilter : public PairPredicate
{
public:
  SameParticlePairFilter();
  IMP_PAIR_PREDICATE(SameParticlePairFilter);
};


IMPMEMBRANE_END_NAMESPACE

#endif  /* IMPMEMBRANE_SAME_PARTICLE_PAIR_FILTER_H */
