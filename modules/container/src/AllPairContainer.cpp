/**
 *  \file AllPairContainer.cpp   \brief A list of ParticlePairs.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/container/AllPairContainer.h"
#include <IMP/core/internal/DifferenceSingletonContainer.h>
#include <IMP/container/AllBipartitePairContainer.h>
#include <IMP/PairModifier.h>
#include <algorithm>

#define FOREACH(expr)                                                   \
  unsigned int szc=c_->get_number_of_particles();                       \
  for (unsigned int i=0; i< szc; ++i) {                                 \
    Particle *a= c_->get_particle(i);                                   \
    for (unsigned int j=0; j< i; ++j) {                                 \
      ParticlePair p(a, c_->get_particle(j));                           \
      expr;                                                             \
    }                                                                   \
  }


IMPCONTAINER_BEGIN_NAMESPACE

AllPairContainer::AllPairContainer(SingletonContainer *c,
                                             bool): c_(c) {
  a_=1;
  b_=0;
  i_=0;
}

AllPairContainer::AllPairContainer(SingletonContainer *c): c_(c) {
  a_=1;
  b_=0;
  i_=0;
  IMP_NEW(IMP::core::internal::DifferenceSingletonContainer, old,
          (c_, c_->get_removed_singletons_container()));

  IMP::Pointer<PairContainerSet> removed
    = PairContainerSet::create_untracked_container();
  {
    IMP_NEW(AllPairContainer, all,
            (c_->get_removed_singletons_container(),
             false));
    removed->add_pair_container(all);
    IMP_NEW(AllBipartitePairContainer, leftr,
            (c_->get_removed_singletons_container(),
             old,
             false));
    removed->add_pair_container(leftr);
  }
  IMP::Pointer<PairContainerSet> added
    = PairContainerSet::create_untracked_container();
  {
    IMP_NEW(AllPairContainer,all,
            (c_->get_added_singletons_container(),
             false));
    added->add_pair_container(all);
    IMP_NEW(AllBipartitePairContainer,leftr,
            (c_->get_added_singletons_container(),
             old,
             false));
    added->add_pair_container(leftr);
  }
  set_added_and_removed_containers(added, removed);
}

unsigned int AllPairContainer::get_number_of_particle_pairs() const {
  unsigned int n= c_->get_number_of_particles();
  return n*(n-1)/2;
}

//!
ParticlePair AllPairContainer::get_particle_pair(unsigned int i) const {
  IMP_INTERNAL_CHECK(i <  get_number_of_particle_pairs(),
                     "Invalid pair requested");
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
  return ParticlePair(c_->get_particle(a_), c_->get_particle(b_));
}

bool
AllPairContainer::get_contains_particle_pair(const ParticlePair &p) const {
  return c_->get_contains_particle(p[0])
      && c_->get_contains_particle(p[1]);
}

void AllPairContainer::do_show(std::ostream &out) const {
  IMP_CHECK_OBJECT(this);
  out << "container " << *c_ << std::endl;
}

ContainersTemp AllPairContainer::get_input_containers() const {
  return ContainersTemp(1,c_);
}

ParticlesTemp AllPairContainer::get_contained_particles() const {
  return c_->get_contained_particles();
}

bool AllPairContainer::get_contained_particles_changed() const {
  return c_->get_contained_particles_changed();
}

IMP_PAIR_CONTAINER_METHODS_FROM_FOREACH(AllPairContainer)


IMPCONTAINER_END_NAMESPACE
