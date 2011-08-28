/**
 *  \file SingletonsRestraint.cpp
 *  \brief Implementation
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/core/internal/CoreSingletonsRestraint.h"
#include "IMP/core/SingletonRestraint.h"
#include <IMP/internal/container_helpers.h>

#include <IMP/SingletonScore.h>
#include <IMP/log.h>
#include <sstream>


IMPCORE_BEGIN_INTERNAL_NAMESPACE

CoreSingletonsRestraint
::CoreSingletonsRestraint(SingletonScore *ss,
                      SingletonContainer *pc,
                      std::string name):
  SingletonsScoreRestraint(name),
  ss_(ss), pc_(pc) {

}

double CoreSingletonsRestraint
::unprotected_evaluate(DerivativeAccumulator *accum) const
{
  IMP_OBJECT_LOG;
  IMP_CHECK_OBJECT(ss_);
  IMP_CHECK_OBJECT(pc_);

  return pc_->evaluate(ss_, accum);
}

double CoreSingletonsRestraint
::unprotected_evaluate_if_good(DerivativeAccumulator *da, double max) const {
  IMP_OBJECT_LOG;
  IMP_CHECK_OBJECT(ss_);
  IMP_CHECK_OBJECT(pc_);

  return pc_->evaluate_if_good(ss_, da, max);
}



ParticlesTemp CoreSingletonsRestraint::get_input_particles() const
{
  IMP_OBJECT_LOG;
  ParticlesTemp ret= IMP::internal::get_input_particles(ss_.get(),
                                      pc_->get_contained_particles());
  return ret;
}

ContainersTemp CoreSingletonsRestraint::get_input_containers() const
{
  ContainersTemp ret= IMP::internal::get_input_containers(ss_.get(),
                                          pc_->get_contained_particles());
  ret.push_back(pc_);
  return ret;
}


Restraints CoreSingletonsRestraint::create_decomposition() const {
  IMP_OBJECT_LOG;
  IMP_USAGE_CHECK(get_is_part_of_model(),
                  "Restraint must be part of model before being decomposed.");
  Restraints ret= pc_->create_decomposition(ss_);
  for (unsigned int i=0; i < ret.size(); ++i) {
    ret[i]->set_maximum_score(get_maximum_score());
  }
  for (unsigned int i=0; i< ret.size(); ++i) {
    std::ostringstream oss;
    oss << get_name() << " " << i;
    ret[i]->set_name(oss.str());
    ret[i]->set_model(get_model());
  }
  return ret;
}

Restraints CoreSingletonsRestraint::create_current_decomposition() const {
  IMP_OBJECT_LOG;
  IMP_USAGE_CHECK(get_is_part_of_model(),
                  "Restraint must be part of model before being decomposed.");
  Restraints ret;
  for (unsigned int i=0; i< pc_->get_number(); ++i) {
    Restraints cur=ss_->get_current_decomposition(pc_->get(i));
    for (unsigned int i=0; i < cur.size(); ++i) {
      cur[i]->set_maximum_score(get_maximum_score());
    }
    ret.insert(ret.end(), cur.begin(), cur.end());
  }
  for (unsigned int i=0; i< ret.size(); ++i) {
    std::ostringstream oss;
    oss << get_name() << " " << i;
    ret[i]->set_name(oss.str());
    ret[i]->set_model(get_model());
  }
  return ret;
}

void CoreSingletonsRestraint::do_show(std::ostream& out) const
{
  out << "score " << ss_->get_name() << std::endl;
  out << "container " << pc_->get_name() << std::endl;
}

IMPCORE_END_INTERNAL_NAMESPACE
