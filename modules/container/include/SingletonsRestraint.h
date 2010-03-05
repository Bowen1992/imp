/**
 *  \file SingletonsRestraint.h
 *  \brief Apply a SingletonScore to each Particle in a list.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPCONTAINER_SINGLETONS_RESTRAINT_H
#define IMPCONTAINER_SINGLETONS_RESTRAINT_H

#include "container_config.h"

#include <IMP/core/internal/CoreSingletonsRestraint.h>

#include <iostream>

IMPCONTAINER_BEGIN_NAMESPACE

//! Applies a SingletonScore to each Particle in a list.
/** This restraint stores the used particles in a Particles.
    The container used can be set so that the list can be shared
    with other containers (or a nonbonded list can be used). By default
    a ListSingletonContainer is used and the
    {add_, set_, clear_}particle{s} methodas can be used.

    Examples using various multiplicity containers:
    \htmlinclude restrain_in_sphere.py.html
    \htmlinclude nonbonded_interactions.py.html

    \see SingletonRestraint
 */
class IMPCONTAINEREXPORT SingletonsRestraint :
#if defined(SWIG) || defined(IMP_DOXYGEN)
public Restraint
#else
public core::internal::CoreSingletonsRestraint
#endif
{
  typedef core::internal::CoreSingletonsRestraint P;
  IMP::internal::OwnerPointer<SingletonScore> ss_;
  IMP::internal::OwnerPointer<SingletonContainer> pc_;
  mutable double score_;
public:

 //! Create the restraint with a shared container
  /** \param[in] ss The function to apply to each particle.
      \param[in] pc The container containing the stored particles. This
      container is not copied.
      \param[in] name The object name
   */
  SingletonsRestraint(SingletonScore *ss,
                      SingletonContainer *pc,
                      std::string name="SingletonsRestraint %1%");

#if defined(IMP_DOXYGEN) || defined(SWIG)
  IMP_INCREMENTAL_RESTRAINT(SingletonsRestraint);

  //! Get the container used to store Particles
  SingletonContainer* get_singleton_container() {
    return pc_;
  }

  SingletonScore* get_singleton_score() const {
    return ss_;
  }
#else
  IMP_OBJECT(SingletonsRestraint);
#endif
};

IMPCONTAINER_END_NAMESPACE

#endif  /* IMPCONTAINER_SINGLETONS_RESTRAINT_H */
