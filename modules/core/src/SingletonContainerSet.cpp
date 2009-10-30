/**
 *  \file SingletonContainerSet.cpp
 *  \brief A set of SingletonContainers.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 *
 */

#include "IMP/core/SingletonContainerSet.h"
#include <algorithm>


IMPCORE_BEGIN_NAMESPACE

namespace {
  SingletonContainerSet* get_set(SingletonContainer* c) {
    return dynamic_cast<SingletonContainerSet*>(c);
  }
}

SingletonContainerSet
::SingletonContainerSet(bool): SingletonContainer("added or removed for set") {
}

SingletonContainerSet
::SingletonContainerSet(std::string name):
  SingletonContainer(name) {
  set_added_and_removed_containers( create_untracked_container(),
                                    create_untracked_container());
}

SingletonContainerSet
::SingletonContainerSet(const SingletonContainers& in,
                        std::string name):
  SingletonContainer(name) {
  set_singleton_containers(in);
  set_added_and_removed_containers( create_untracked_container(),
                                    create_untracked_container());
}


bool
SingletonContainerSet
::get_contains_particle(Particle* vt) const {
  for (SingletonContainerConstIterator it= singleton_containers_begin();
       it != singleton_containers_end(); ++it) {
    if ((*it)->get_contains_particle(vt)) return true;
  }
  return false;
}

void SingletonContainerSet::show(std::ostream &out) const {
  IMP_CHECK_OBJECT(this);
  out << "SingletonContainerSet with "
      << get_number_of_particles()
      << " particles." << std::endl;
}

unsigned int
SingletonContainerSet::get_number_of_particles() const {
  unsigned int sum=0;
  for (SingletonContainerConstIterator it= singleton_containers_begin();
       it != singleton_containers_end(); ++it) {
    sum+= (*it)->get_number_of_particles();
  }
  return sum;
}

Particle*
SingletonContainerSet::get_particle(unsigned int i) const {
  for (SingletonContainerConstIterator it= singleton_containers_begin();
       it != singleton_containers_end(); ++it) {
    if ( i >= (*it)->get_number_of_particles()) {
      i-= (*it)->get_number_of_particles();
    } else {
      return (*it)->get_particle(i);
    }
  }
  throw IndexException("out of range");
}



IMP_LIST_IMPL(SingletonContainerSet,
              SingletonContainer,
              singleton_container,
              SingletonContainer*,
              SingletonContainers,
              {
                if (!get_is_added_or_removed_container()) {
                  get_set(get_added_singletons_container())
                    ->add_singleton_container(obj
                           ->get_added_singletons_container());
                }
                obj->set_was_owned(true);
              },,
              if (!get_is_added_or_removed_container()) {
                get_set(get_removed_singletons_container())
                  ->add_singleton_container(obj
                       ->get_removed_singletons_container());
              })


void SingletonContainerSet::apply(const SingletonModifier *sm) {
  for (unsigned int i=0; i< get_number_of_singleton_containers(); ++i) {
    get_singleton_container(i)->apply(sm);
  }
}

void SingletonContainerSet::apply(const SingletonModifier *sm,
                               DerivativeAccumulator &da) {
  for (unsigned int i=0; i< get_number_of_singleton_containers(); ++i) {
    get_singleton_container(i)->apply(sm, da);
  }
}

double SingletonContainerSet::evaluate(const SingletonScore *s,
                                       DerivativeAccumulator *da) const {
  double score=0;
  for (unsigned int i=0; i< get_number_of_singleton_containers(); ++i) {
    score+=get_singleton_container(i)->evaluate(s, da);
  }
  return score;
}


double SingletonContainerSet::evaluate_change(const SingletonScore *s,
                                              DerivativeAccumulator *da) const {
  double score=0;
  for (unsigned int i=0; i< get_number_of_singleton_containers(); ++i) {
    score+=get_singleton_container(i)->evaluate_change(s, da);
  }
  return score;
}

double SingletonContainerSet::evaluate_prechange(const SingletonScore *s,
                                             DerivativeAccumulator *da) const {
  double score=0;
  for (unsigned int i=0; i< get_number_of_singleton_containers(); ++i) {
    score+=get_singleton_container(i)->evaluate_prechange(s, da);
  }
  return score;
}



ParticlesTemp SingletonContainerSet::get_particles() const {
  ParticlesTemp ret;
  for (unsigned int i=0; i< get_number_of_singleton_containers(); ++i) {
    ParticlesTemp c= get_singleton_container(i)->get_particles();
    ret.insert(ret.end(), c.begin(), c.end());
  }
  return ret;
}


ObjectsTemp SingletonContainerSet::get_input_objects() const {
  return ObjectsTemp(singleton_containers_begin(),
                     singleton_containers_end());
}


IMPCORE_END_NAMESPACE
