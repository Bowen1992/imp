/**
 *  \file PairContainer.h    \brief A container for particle_pairs.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 */

#ifndef IMPCORE_PAIR_CONTAINER_H
#define IMPCORE_PAIR_CONTAINER_H

#include "config.h"
#include "internal/IndexingIterator.h"
#include "Particle.h"
#include "Container.h"
#include "utility.h"
#include "VersionInfo.h"
#include "base_types.h"
#include "Pointer.h"
#include "VectorOfRefCounted.h"
#include "VersionInfo.h"
#include "DerivativeAccumulator.h"
#include "internal/OwnerPointer.h"
#include "macros.h"

IMP_BEGIN_NAMESPACE
class PairModifier;
class PairScore;


//! A shared container for particle_pairs
/** Stores a searchable shared collection of particle_pairs.
    \ingroup restraints

    Implementors should see IMP_PAIR_CONTAINER().
 */
class IMPEXPORT PairContainer : public Container
{
  internal::OwnerPointer<Container> added_, removed_;
  struct Accessor: public NullDefault {
    typedef Accessor This;
    typedef ParticlePair result_type;
    typedef unsigned int argument_type;
    result_type operator()(argument_type i) const {
      return o_->get_particle_pair(i);
    }
    Accessor(PairContainer *pc): o_(pc){}
    Accessor(): o_(NULL){}
    IMP_COMPARISONS_1(o_);
  private:
    // This should be ref counted, but swig memory management is broken
    PairContainer* o_;
  };
 protected:
  /** Containers must have containers that keep track of the particles
      which have been added or since the last step. These containers
      must be registered with the parent PairContainer.

      Containers which are themselves returned by the get_added/removed
      functions do not have to register such containers.
  */
  void set_added_and_removed_containers(PairContainer* added,
                                        PairContainer* removed) {
    added_=added;
    removed_=removed;
  }

public:
#ifndef IMP_DOXYGEN
  bool get_is_added_or_removed_container() {
    return !added_;
  }
#endif

  PairContainer(std::string name="PairContainer %1%");

  /** \note This function may be linear. Be aware of the complexity
      bounds of your particular container.
   */
  virtual bool get_contains_particle_pair(const ParticlePair& v) const =0;
  //! return the number of particle_pairs in the container
  /** \note this isn't always constant time
   */
  virtual unsigned int get_number_of_particle_pairs() const =0;

  virtual ParticlePair get_particle_pair(unsigned int i) const=0;

#ifdef IMP_DOXYGEN
  //! An iterator through the contents of the container
  class ParticlePairIterator;
#else
  typedef internal::IndexingIterator<Accessor> ParticlePairIterator;
#endif
  //! begin iterating through the particle_pairs
  ParticlePairIterator particle_pairs_begin() const {
    // Since I can't make the count mutable in Object
    return
      ParticlePairIterator(Accessor(const_cast<PairContainer*>(this)),
                        0);
  }
  //! iterate through the particle_pairs
  ParticlePairIterator particle_pairs_end() const {
    return
      ParticlePairIterator(Accessor(const_cast<PairContainer*>(this)),
                        get_number_of_particle_pairs());
    }

  //! Apply a SingletonModifier to the contents
  virtual void apply(const PairModifier *sm)=0;

  //! Apply a SingletonModifier to the contents
  virtual void apply(const PairModifier *sm, DerivativeAccumulator &da)=0;

  //! Avaluate a score on the contents
  virtual double evaluate(const PairScore *s,
                          DerivativeAccumulator *da) const=0;

  /** \name Incremental Scoring
      When incremental scoring is used, the container keeps track of
      changes to it since the last Model::evaluate() call.
      \unstable{ParticlePairContainer::get_removed_pairs_container()}
      The address of the objects returned should not change over the lifetime
      of this container (but, of course, their contents will).
      @{
  */
  PairContainer* get_removed_pairs_container() const {
    IMP_USAGE_CHECK(added_, "The containers returned by "
                    << " get_added_pairs_container() do not "
                    << " track their own added and removed contents.",
                    ParticlePairException);
    PairContainer *ret= dynamic_cast<PairContainer*>(removed_.get());
    IMP_INTERNAL_CHECK(ret, "Cannot cast object " << removed_->get_name()
                       << " to a PairContainer.");
    return ret;
  }
  PairContainer* get_added_pairs_container() const {
    IMP_USAGE_CHECK(added_, "The containers returned by "
                    << " get_added_pairs_container() do not "
                    << " track their own added and removed contents.",
                    ParticlePairException);
    PairContainer *ret= dynamic_cast<PairContainer*>(added_.get());
    IMP_INTERNAL_CHECK(ret, "Cannot cast object " << added_->get_name()
                       << " to a PairContainer.");
    return ret;
  }
  /** Return the change in score (and derivatives) since the last
      evaluate of the current contents of the container.
  */
  virtual double evaluate_change(const PairScore *o,
                                DerivativeAccumulator *da) const = 0;


  /** Return the score of the last evaluate for the current contents of the
      container.
  */
  virtual double evaluate_prechange(const PairScore *o,
                                    DerivativeAccumulator *da) const = 0;
  /** @} */

  //! Get all the ParticlePairs from the container
  virtual ParticlePairsTemp get_particle_pairs() const=0;

#ifndef IMP_DOXYGEN
  ParticlePair get(unsigned int i) const {return get_particle_pair(i);}
  bool get_contains(const ParticlePair& v) const {
    return get_contains_particle_pair(v);
  }
  unsigned int get_number() const {return get_number_of_particle_pairs();}
#endif

  IMP_REF_COUNTED_NONTRIVIAL_DESTRUCTOR(PairContainer);
};

IMP_OUTPUT_OPERATOR(PairContainer);

//! A collection of containers
typedef VectorOfRefCounted<PairContainer*> PairContainers;

IMP_END_NAMESPACE

#endif  /* IMPCORE_PAIR_CONTAINER_H */
