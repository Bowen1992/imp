/**
 *  \file DynamicListCLASSNAMEContainer.cpp
 *  \brief A list of PLURALVARIABLETYPE.
 *
 *  This file is generated by a script (internal/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2013 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/internal/InternalDynamicListCLASSNAMEContainer.h"
#include "IMP/CLASSNAMEModifier.h"
#include <IMP/base/check_macros.h>
#include <IMP/compatibility/set.h>
#include <algorithm>


IMP_BEGIN_INTERNAL_NAMESPACE

InternalDynamicListCLASSNAMEContainer
::InternalDynamicListCLASSNAMEContainer(Container *m,
                                        std::string name):
    P(m->get_model(), name), scope_(m) {
}


InternalDynamicListCLASSNAMEContainer
::InternalDynamicListCLASSNAMEContainer(Container *m,
                                        const char *name):
    P(m->get_model(), name), scope_(m) {
}


void InternalDynamicListCLASSNAMEContainer::do_show(std::ostream &out) const {
  IMP_CHECK_OBJECT(this);
  out << get_access()
      << " CLASSNAMEs." << std::endl;
}
void InternalDynamicListCLASSNAMEContainer::add(PASSINDEXTYPE vt) {
  PLURALINDEXTYPE cur;
  swap(cur);
  cur.push_back(vt);
  swap(cur);
}
void InternalDynamicListCLASSNAMEContainer
::add(const PLURALINDEXTYPE &c) {
  if (c.empty()) return;
  PLURALINDEXTYPE cur;
  swap(cur);
  cur+=c;
  swap(cur);
}

void InternalDynamicListCLASSNAMEContainer::set(PLURALINDEXTYPE cp) {
  swap(cp);
}
void InternalDynamicListCLASSNAMEContainer::clear() {
  PLURALINDEXTYPE t;
  swap(t);
}
bool InternalDynamicListCLASSNAMEContainer::
check_list(const ParticleIndexes& cp) const {
  ParticleIndexes app
    = scope_->get_all_possible_indexes();

  compatibility::set<ParticleIndex> all(app.begin(),
                                    app.end());
  for (unsigned int i=0; i< cp.size(); ++i) {
    IMP_USAGE_CHECK(all.find(cp[i]) != all.end(),
                    "Particle " << cp[i]
                    << " is not in the list of all possible particles");
  }
  return true;
}

ParticleIndexes
InternalDynamicListCLASSNAMEContainer::get_all_possible_indexes() const {
  return scope_->get_all_possible_indexes();
}

void InternalDynamicListCLASSNAMEContainer::do_before_evaluate() {
}


ParticlesTemp
InternalDynamicListCLASSNAMEContainer::get_input_particles() const {
  return ParticlesTemp();
}

ContainersTemp
InternalDynamicListCLASSNAMEContainer::get_input_containers() const {
  return ContainersTemp();
}


PLURALINDEXTYPE
InternalDynamicListCLASSNAMEContainer::get_range_indexes() const {
  return get_indexes();
}


IMP_END_INTERNAL_NAMESPACE
