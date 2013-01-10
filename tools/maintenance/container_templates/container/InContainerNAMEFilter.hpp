/**
 *  \file IMP/container/InContainerCLASSNAMEFilter.h
 *  \brief A filter for CLASSNAMEs.
 *
 *  This file is generated by a script (core/tools/make-containers).
 *  Do not edit directly.
 *
 *  Copyright 2007-2013 IMP Inventors. All rights reserved.
 */

#ifndef IMPCONTAINER_IN_CONTAINER_HEADERNAME_FILTER_H
#define IMPCONTAINER_IN_CONTAINER_HEADERNAME_FILTER_H

#include <IMP/container/container_config.h>
#include <IMP/CLASSNAMEPredicate.h>
#include <IMP/CLASSNAMEContainer.h>
#include <IMP/internal/container_helpers.h>
#include "internal/CLASSNAMEContainerIndex.h"
#include <IMP/LCCLASSNAME_macros.h>
#include <IMP/base/warning_macros.h>

IMPCONTAINER_BEGIN_NAMESPACE


//! A filter which returns true if a container containers the CLASSNAME
/** This predicate returns 1 if the passed tuple is in the container.
    \note Use the handle_permutations parameter to the constructor to
    determine whether only exact matchers, or matches under permutation
    are considered matching. By default they are are.
 */
class IMPCONTAINEREXPORT InContainerCLASSNAMEFilter :
    public CLASSNAMEPredicate
{
  IMP::base::OwnerPointer<internal::CLASSNAMEContainerIndex> c_;
public:
  InContainerCLASSNAMEFilter(CLASSNAMEContainer *c,
                             std::string name="CLASSNAMEFilter %1%");
  InContainerCLASSNAMEFilter(CLASSNAMEContainer *c,
                             bool handle_permutations,
                             std::string name="CLASSNAMEFilter %1%");

  IMP_INDEX_HEADERNAME_PREDICATE(InContainerCLASSNAMEFilter,{
      IMP_UNUSED(m);
      return c_->get_contains(pi);
    },{
      ModelObjectsTemp ret;
      ret+= IMP::get_particles(m, pi);
      ret.push_back(c_);
      return ret;
    });
};


IMP_OBJECTS(InContainerCLASSNAMEFilter, InContainerCLASSNAMEFilters);

IMPCONTAINER_END_NAMESPACE

#endif  /* IMPCONTAINER_IN_CONTAINER_HEADERNAME_FILTER_H */
