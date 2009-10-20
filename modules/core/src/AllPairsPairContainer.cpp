/**
 *  \file AllPairsPairContainer.cpp   \brief A list of ParticlePairs.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 *
 */

#include "IMP/core/AllPairsPairContainer.h"
#include <IMP/PairModifier.h>
#include <algorithm>


IMPCORE_BEGIN_NAMESPACE

AllPairsPairContainer::AllPairsPairContainer( SingletonContainer *c): c_(c) {
  a_=1;
  b_=0;
  i_=0;
}

unsigned int AllPairsPairContainer::get_number_of_particle_pairs() const {
  unsigned int n= c_->get_number_of_particles();
  return n*(n-1)/2;
}

//!
ParticlePair AllPairsPairContainer::get_particle_pair(unsigned int i) const {
  IMP_INTERNAL_CHECK(i <  get_number_of_particle_pairs(),
                     "Invalid pair requested");
  IMP_LOG(VERBOSE, "All pairs asked for pair " << i << " current state is "
    << i_ << "= " << a_ << " " << b_ << std::endl);
  // dumb method, just increase the current pair until we get to the desired one
  if (static_cast<int>(i) < i_) {
    a_=1;
    b_=0;
    i_=0;
  }
  while (i_ < static_cast<int>(i)) {
    ++b_;
    if (b_==a_) {
      ++a_;
      b_=0;
    }
    ++i_;
  }
  IMP_LOG(VERBOSE, "Returned "
    << i_ << "= " << a_ << " " << b_ << std::endl);
  return ParticlePair(c_->get_particle(a_), c_->get_particle(b_));
}

bool
AllPairsPairContainer::get_contains_particle_pair(ParticlePair vt) const {
  return c_->get_contains_particle(vt.first)
      && c_->get_contains_particle(vt.second);
}

void AllPairsPairContainer::show(std::ostream &out) const {
  IMP_CHECK_OBJECT(this);
  out << "AllPairsPairContainer on ";
  c_->show(out);
}

void AllPairsPairContainer::apply(const PairModifier *sm) {
  for (unsigned int i=0;
       i< AllPairsPairContainer::get_number_of_particle_pairs();
       ++i) {
    ParticlePair pp= AllPairsPairContainer::get_particle_pair(i);
    sm->apply(pp[0], pp[1]);
  }
}

void AllPairsPairContainer::apply(const PairModifier *sm,
                                  DerivativeAccumulator &da) {
  for (unsigned int i=0;
       i< AllPairsPairContainer::get_number_of_particle_pairs();
       ++i) {
    ParticlePair pp= AllPairsPairContainer::get_particle_pair(i);
    sm->apply(pp[0], pp[1], da);
  }
}

double AllPairsPairContainer::evaluate(const PairScore *s,
                                       DerivativeAccumulator *da) const {
  double score=0;
  for (unsigned int i=0;
       i< AllPairsPairContainer::get_number_of_particle_pairs();
       ++i) {
    ParticlePair pp= AllPairsPairContainer::get_particle_pair(i);
    score+=s->evaluate(pp[0], pp[1], da);
  }
  return score;
}

ParticlePairsTemp AllPairsPairContainer::get_particle_pairs() const {
  ParticlePairsTemp ret(AllPairsPairContainer::get_number_of_particle_pairs());
  for (unsigned int i=0;
       i< AllPairsPairContainer::get_number_of_particle_pairs();
       ++i) {
    ret[i]= AllPairsPairContainer::get_particle_pair(i);
  }
  return ret;
}


ObjectsTemp AllPairsPairContainer::get_input_objects() const {
  return ObjectsTemp(1,c_);
}

IMPCORE_END_NAMESPACE
