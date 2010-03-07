/**
 *  \file GroupnamesRestraint.h
 *  \brief Apply a GroupnameScore to each Classname in a list.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPCONTAINER_GROUPNAMES_RESTRAINT_H
#define IMPCONTAINER_GROUPNAMES_RESTRAINT_H

#include "container_config.h"

#include <IMP/core/internal/CoreGroupnamesRestraint.h>

#include <iostream>

IMPCONTAINER_BEGIN_NAMESPACE

//! Applies a GroupnameScore to each Classname in a list.
/** This restraint stores the used particles in a Classnames.
    The container used can be set so that the list can be shared
    with other containers (or a nonbonded list can be used). By default
    a ListGroupnameContainer is used and the
    {add_, set_, clear_}classname{s} methodas can be used.

    Examples using various multiplicity containers:
    \htmlinclude restrain_in_sphere.py.html
    \htmlinclude nonbonded_interactions.py.html

    \see GroupnameRestraint
 */
class IMPCONTAINEREXPORT GroupnamesRestraint :
#if defined(SWIG) || defined(IMP_DOXYGEN)
public Restraint
#else
public core::internal::CoreGroupnamesRestraint
#endif
{
  typedef core::internal::CoreGroupnamesRestraint P;
  IMP::internal::OwnerPointer<GroupnameScore> ss_;
  IMP::internal::OwnerPointer<GroupnameContainer> pc_;
  mutable double score_;
public:

 //! Create the restraint with a shared container
  /** \param[in] ss The function to apply to each particle.
      \param[in] pc The container containing the stored particles. This
      container is not copied.
      \param[in] name The object name
   */
  GroupnamesRestraint(GroupnameScore *ss,
                      GroupnameContainer *pc,
                      std::string name="GroupnamesRestraint %1%");

#if defined(IMP_DOXYGEN) || defined(SWIG)
  IMP_INCREMENTAL_RESTRAINT(GroupnamesRestraint);

  //! Get the container used to store Particles
  GroupnameContainer* get_groupname_container() {
    return pc_;
  }

  GroupnameScore* get_groupname_score() const {
    return ss_;
  }
#else
  IMP_OBJECT(GroupnamesRestraint);
#endif
};

IMP_OBJECTS(GroupnamesRestraint);

IMPCONTAINER_END_NAMESPACE

#endif  /* IMPCONTAINER_GROUPNAMES_RESTRAINT_H */
