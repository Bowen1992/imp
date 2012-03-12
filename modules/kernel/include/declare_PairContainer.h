/**
 *  \file declare_PairContainer.h    \brief A container for Pairs.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 */

#ifndef IMPKERNEL_DECLARE_PAIR_CONTAINER_H
#define IMPKERNEL_DECLARE_PAIR_CONTAINER_H

#include "kernel_config.h"
#include "internal/IndexingIterator.h"
#include "declare_Particle.h"
#include "container_base.h"
#include "internal/container_helpers.h"
#include "DerivativeAccumulator.h"
#include "internal/OwnerPointer.h"
#include "ParticleTuple.h"
#include "macros.h"

IMP_BEGIN_NAMESPACE
class PairModifier;
class PairDerivativeModifier;
class PairScore;

//! A shared container for Pairs
/** Stores a searchable shared collection of Pairs.
    \ingroup restraints

    \implementationwithoutexample{PairContainer, IMP_PAIR_CONTAINER}
 */
class IMPEXPORT PairContainer : public Container
{
 protected:
  PairContainer(){}
  PairContainer(Model *m,
                     std::string name="PairContainer %1%");
public:
  typedef ParticlePair ContainedType;
  typedef ParticlePairsTemp ContainedTypes;
  /** \note This function may be linear. Be aware of the complexity
      bounds of your particular container.
   */
  virtual bool get_contains_particle_pair(const ParticlePair& v) const =0;

  ParticlePairsTemp get_particle_pairs() const {
    return IMP::internal::get_particle(get_model(),
                                       get_indexes());
  }
#ifndef IMP_DOXGEN
  //! return the number of Pairs in the container
  /** \note this isn't always constant time
   */
  virtual unsigned int get_number_of_particle_pairs() const {
    return get_number();
  }

  virtual ParticlePair get_particle_pair(unsigned int i) const {
    return get(i);
  }

#endif

  //! Apply a SingletonModifier to the contents
  virtual void apply(const PairModifier *sm) const=0;
  //! Apply a SingletonModifier to the contents
  virtual void apply(const PairDerivativeModifier *sm,
                     DerivativeAccumulator &da) const=0;

  //! Evaluate a score on the contents
  virtual double evaluate(const PairScore *s,
                          DerivativeAccumulator *da) const=0;

  //! Evaluate a score on the contents
  virtual double evaluate_if_good(const PairScore *s,
                                  DerivativeAccumulator *da,
                                  double max) const=0;

  /** Return true if the contents of the container changed since the last
      evaluate.
  */
  virtual bool get_contents_changed() const=0;

#ifndef IMP_DOXYGEN
  typedef ParticlePair value_type;
  ParticlePair get(unsigned int i) const {
    return IMP::internal::get_particle(get_model(),
                                       get_indexes()[i]);
  }
  ParticlePairsTemp get() const {
    return IMP::internal::get_particle(get_model(), get_indexes());
  }
  bool get_contains(const ParticlePair& v) const {
    return get_contains_particle_pair(v);
  }
  unsigned int get_number() const {return get_indexes().size();}
  virtual ParticleIndexPairs get_indexes() const=0;
  virtual ParticleIndexPairs get_all_possible_indexes() const=0;
  virtual Restraints create_decomposition(PairScore *s) const=0;
#ifndef SWIG
  virtual bool get_provides_access() const {return false;}
  virtual const ParticleIndexPairs& get_access() const {
    IMP_THROW("Object not implemented properly.", IndexException);
  }
#endif
#endif

  IMP_REF_COUNTED_NONTRIVIAL_DESTRUCTOR(PairContainer);
};

IMP_END_NAMESPACE

#endif  /* IMPKERNEL_DECLARE_PAIR_CONTAINER_H */
