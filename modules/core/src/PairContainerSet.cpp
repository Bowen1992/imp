/**
 *  \file PairContainerSet.cpp
 *  \brief Simple xyzr decorator.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 *
 */

#include "IMP/core/PairContainerSet.h"
#include <algorithm>


IMPCORE_BEGIN_NAMESPACE

PairContainerSet
::PairContainerSet(){
}

PairContainerSet::~PairContainerSet(){}

bool
PairContainerSet
::get_contains_particle_pair(ParticlePair vt) const {
  for (PairContainerConstIterator it= pair_containers_begin();
       it != pair_containers_end(); ++it) {
    if ((*it)->get_contains_particle_pair(vt)) return true;
  }
  return false;
}

void PairContainerSet::show(std::ostream &out) const {
  IMP_CHECK_OBJECT(this);
  out << "PairContainerSet with "
      << get_number_of_particle_pairs()
      << " particle_pairs." << std::endl;
}

unsigned int
PairContainerSet::get_number_of_particle_pairs() const {
  unsigned int sum=0;
  for (PairContainerConstIterator it= pair_containers_begin();
       it != pair_containers_end(); ++it) {
    sum+= (*it)->get_number_of_particle_pairs();
  }
  return sum;
}

ParticlePair
PairContainerSet::get_particle_pair(unsigned int i) const {
  for (PairContainerConstIterator it= pair_containers_begin();
       it != pair_containers_end(); ++it) {
    if ( i >= (*it)->get_number_of_particle_pairs()) {
      i-= (*it)->get_number_of_particle_pairs();
    } else {
      return (*it)->get_particle_pair(i);
    }
  }
  throw IndexException("out of range");
}



IMP_CONTAINER_IMPL(PairContainerSet,
                   PairContainer,
                   pair_container,
                   PairContainerIndex,,,)

IMPCORE_END_NAMESPACE
