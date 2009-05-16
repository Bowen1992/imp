/**
 *  \file GroupnamesRestraint.cpp
 *  \brief Apply a GroupnameScore function to a container of Values .
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 *
 */

#include "IMP/core/GroupnamesRestraint.h"
#include <IMP/internal/container_helpers.h>
#include "IMP/core/ListGroupnameContainer.h"

#include <IMP/GroupnameScore.h>
#include <IMP/log.h>


IMPCORE_BEGIN_NAMESPACE

GroupnamesRestraint
::GroupnamesRestraint(GroupnameScore *ss,
                      const Classnames &pc):
  ss_(ss),
  pc_(new ListGroupnameContainer(pc))
{
}

GroupnamesRestraint
::GroupnamesRestraint(GroupnameScore *ss,
                      GroupnameContainer *pc): ss_(ss), pc_(pc) {

}

Float GroupnamesRestraint::evaluate(DerivativeAccumulator *accum)
{
  IMP_OBJECT_LOG;
  IMP_CHECK_OBJECT(ss_);
  IMP_CHECK_OBJECT(pc_);

  double score=0;


  for (GroupnameContainer::ClassnameIterator
         it= pc_->classnames_begin();
       it != pc_->classnames_end(); ++it) {
    double lscore= IMP::internal::ContainerTraits<Classname>
      ::evaluate(ss_, *it, accum);
    IMP_LOG(VERBOSE, IMP::internal::streamable(*it).get_name()
            << " has score " << lscore << std::endl);
    score+=lscore;
  }

  return score;
}


ParticlesList GroupnamesRestraint::get_interacting_particles() const
{
  IMP_OBJECT_LOG;
  if (!IMP::internal::ContainerTraits<Classname>::is_singleton) {
    ParticlesList ret;
    for (GroupnameContainer::ClassnameIterator it
           = pc_->classnames_begin();
         it != pc_->classnames_end(); ++it) {
      ret.push_back(IMP::internal
                    ::ContainerTraits<Classname>::create_set(*it));
    }
    return ret;
  } else {
    return ParticlesList();
  }
}

namespace {
  ListGroupnameContainer *
  check_methods(GroupnameContainer *pc, std::string str) {
    ListGroupnameContainer *ret
      = dynamic_cast<ListGroupnameContainer*>(pc);
    if (! ret) {
      std::ostringstream oss;
      oss << "Method GroupnamesRestraint::" << str
          << " can only be called if the GroupnameContainer "
          << " is a ListGroupnameContainer.";
      throw InvalidStateException(oss.str().c_str());
    }
    return ret;
  }
}

void GroupnamesRestraint::add_classnames(const Classnames &ps) {
  ListGroupnameContainer *pc= check_methods(pc_.get(), "add_classnames");
  pc->add_classnames(ps);
}

void GroupnamesRestraint::set_classnames(const Classnames &ps){
  ListGroupnameContainer *pc= check_methods(pc_.get(), "add_classnames");
  pc->set_classnames(ps);
}

void GroupnamesRestraint::add_classname(Value v){
  ListGroupnameContainer *pc= check_methods(pc_.get(), "add_classnames");
  pc->add_classname(v);
}

void GroupnamesRestraint::clear_classnames() {
  ListGroupnameContainer *pc= check_methods(pc_.get(), "add_classnames");
  pc->clear_classnames();
}


void GroupnamesRestraint::show(std::ostream& out) const
{
  out << "ContainerRestraint with score function ";
  ss_->show(out);
  out << " and container ";
  pc_->show(out);
  out << std::endl;
}

IMPCORE_END_NAMESPACE
