/**
 *  \file ListSingletonContainer.cpp   \brief A list of Particles.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 *
 */

#include "IMP/core/ListSingletonContainer.h"
#include "IMP/SingletonModifier.h"
#include "IMP/SingletonScore.h"
#include <IMP/core/internal/singleton_helpers.h>
#include <algorithm>


IMPCORE_BEGIN_NAMESPACE

namespace {
  ListSingletonContainer* get_list(SingletonContainer *g) {
    return dynamic_cast<ListSingletonContainer*>(g);
  }
}

IMP_ACTIVE_CONTAINER_DEF(ListSingletonContainer);

ListSingletonContainer
::ListSingletonContainer(bool):
  SingletonContainer("added or removed list"){}

ListSingletonContainer
::ListSingletonContainer(const Particles &ps,
                         std::string name):
  SingletonContainer(name)
{
  if (ps.empty()) return;
  for (unsigned int i=0; i< ps.size(); ++i) {
    IMP_USAGE_CHECK(IMP::internal::get_model(ps[i])
                    == IMP::internal::get_model(ps[0]),
                    "All particles in container must have the same model. "
                    << "Particle " << IMP::internal::get_name(ps[i])
                    << " does not.",
                    Particle*Exception);
  }
  set_particles(ps);
  set_added_and_removed_containers( create_untracked_container(),
                                    create_untracked_container());
}

ListSingletonContainer
::ListSingletonContainer(std::string name):
  SingletonContainer(name){
  set_added_and_removed_containers( create_untracked_container(),
                                    create_untracked_container());
}

ListSingletonContainer
::ListSingletonContainer(const char *name):
  SingletonContainer(name){
  set_added_and_removed_containers( create_untracked_container(),
                                    create_untracked_container());
}

IMP_LISTLIKE_SINGLETON_CONTAINER_DEF(ListSingletonContainer)

void ListSingletonContainer::show(std::ostream &out) const {
  IMP_CHECK_OBJECT(this);
  out << "ListSingletonContainer with " << get_number_of_particles()
      << " particles." << std::endl;
}



void ListSingletonContainer::set_particles(ParticlesTemp sc) {
  if (!get_has_model() && !get_is_added_or_removed_container()
      && !sc.empty()) {
    set_model(IMP::internal::get_model(sc[0]));
  }
  internal::update_list(data_, sc, this);
}


void ListSingletonContainer::clear_particles() {
  if (!get_is_added_or_removed_container()) {
    get_list(get_removed_singletons_container())->add_particles(data_);
  }
  data_.clear();
}


void ListSingletonContainer::add_particle(Particle* vt) {
  if (!get_has_model() && !get_is_added_or_removed_container()) {
    set_model(IMP::internal::get_model(vt));
  }
  data_.insert(std::lower_bound(data_.begin(),
                                data_.end(), vt), vt);
  if (!get_is_added_or_removed_container()) {
    get_list(get_added_singletons_container())->add_particle(vt);
  }
  IMP_USAGE_CHECK(get_is_added_or_removed_container()
                  || !get_removed_singletons_container()
                  ->get_contains_particle(vt),
                  "You cannot remove and add the same item in one time step.",
                  Particle*Exception);
}
void ListSingletonContainer::add_particles(const ParticlesTemp &c) {
  if (c.empty()) return;
  if (!get_has_model() && !get_is_added_or_removed_container()) {
    set_model(IMP::internal::get_model(c[0]));
  }
  data_.insert(data_.end(), c.begin(), c.end());
  std::sort(data_.begin(), data_.end());
  if (!get_is_added_or_removed_container()) {
    get_list(get_added_singletons_container())->add_particles(c);
  }
  IMP_IF_CHECK(USAGE) {
    for (unsigned int i=0; i< c.size(); ++i) {
      IMP_USAGE_CHECK(get_is_added_or_removed_container()
                      || !get_removed_singletons_container()
                      ->get_contains_particle(c[i]),
            "You cannot remove and add the same item in one time step.",
                      Particle*Exception);
    }
  }
}


ObjectsTemp ListSingletonContainer::get_input_objects() const {
  return ObjectsTemp();
}


void ListSingletonContainer::do_before_evaluate() {
  std::remove_if(data_.begin(), data_.end(),
                 IMP::internal::ContainerTraits<Particle>::IsInactive());
}

void ListSingletonContainer::do_after_evaluate() {
  get_list(get_added_singletons_container())->clear_particles();
  get_list(get_removed_singletons_container())->clear_particles();
}

ParticlesTemp ListSingletonContainer::get_state_input_particles() const {
  return ParticlesTemp();
}

IMPCORE_END_NAMESPACE
