/**
 *  \file SingletonFilter.h    \brief A filter for particles.
 *
 *  This file is generated by a script (core/tools/make-containers).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 */

#ifndef IMP_SINGLETON_FILTER_H
#define IMP_SINGLETON_FILTER_H

#include "kernel_config.h"
#include "Particle.h"
#include "utility.h"
#include "VersionInfo.h"
#include "base_types.h"
#include "VersionInfo.h"
#include "ParticleTuple.h"

IMP_BEGIN_NAMESPACE


//! A shared filter for particles
/** Stores a searchable shared collection of particles.
    \ingroup restraints

    Implementors should see IMP_SINGLETON_FILTER().
 */
class IMPEXPORT SingletonFilter : public Object
{
public:
  SingletonFilter(std::string name="SingletonFilter %1%");

  /** \note This function may be linear. Be aware of the complexity
      bounds of your particular filter.
   */
  virtual bool get_contains_particle(Particle* p) const =0;

  /** \name Interactions
      Return the set of particles used when applied to the passed
      list.
      @{
  */
  virtual ParticlesTemp get_input_particles(Particle* p) const=0;
  virtual ObjectsTemp get_input_objects(Particle* p) const=0;
  /** @} */

  IMP_REF_COUNTED_DESTRUCTOR(SingletonFilter);
};

IMP_OUTPUT_OPERATOR(SingletonFilter);

IMP_OBJECTS(SingletonFilter,SingletonFilters);

IMP_END_NAMESPACE

#endif  /* IMP_SINGLETON_FILTER_H */
