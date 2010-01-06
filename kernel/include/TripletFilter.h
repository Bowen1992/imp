/**
 *  \file TripletFilter.h    \brief A filter for particle_triplets.
 *
 *  This file is generated by a script (core/tools/make-containers).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 */

#ifndef IMPCORE_TRIPLET_FILTER_H
#define IMPCORE_TRIPLET_FILTER_H

#include "config.h"
#include "Particle.h"
#include "utility.h"
#include "VersionInfo.h"
#include "base_types.h"
#include "VersionInfo.h"

IMP_BEGIN_NAMESPACE


//! A shared filter for particle_triplets
/** Stores a searchable shared collection of particle_triplets.
    \ingroup restraints

    Implementors should see IMP_TRIPLET_FILTER().
 */
class IMPEXPORT TripletFilter : public Object
{
public:
  TripletFilter(std::string name="TripletFilter %1%");

  /** \note This function may be linear. Be aware of the complexity
      bounds of your particular filter.
   */
  virtual bool get_contains_particle_triplet(const ParticleTriplet& p) const =0;

  /** \name Interactions
      Return the set of particles used when applied to the passed
      list.
      @{
  */
  virtual ParticlesTemp get_input_particles(const ParticleTriplet& p) const=0;
  virtual ObjectsTemp get_input_objects(const ParticleTriplet& p) const=0;
  /** @} */

  IMP_REF_COUNTED_DESTRUCTOR(TripletFilter)
};

IMP_OUTPUT_OPERATOR(TripletFilter);

//! A collection of filters
typedef VectorOfRefCounted<TripletFilter*> TripletFilters;

IMP_END_NAMESPACE

#endif  /* IMPCORE_TRIPLET_FILTER_H */
