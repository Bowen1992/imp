/**
 *  \file SingletonScore.h    \brief Define SingletonScore.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 */

#ifndef IMP_SINGLETON_SCORE_H
#define IMP_SINGLETON_SCORE_H

#include "kernel_config.h"
#include "base_types.h"
#include "Particle.h"
#include "ParticleTuple.h"
#include "DerivativeAccumulator.h"
#include "internal/container_helpers.h"

IMP_BEGIN_NAMESPACE

//! Abstract score function
/** SingletonScores will evaluate the score and derivatives
    for the passed particles. Use in conjunction with various
    restraints such as IMP::core::SingletonsRestraint or
    IMP::core::SingletonRestraint.

    Implementers should check out IMP_SINGLETON_SCORE().
*/
class IMPEXPORT SingletonScore : public Object
{
public:
  SingletonScore(std::string name="SingletonScore %1%");
  //! Compute the score and the derivative if needed.
  virtual double evaluate(Particle* vt,
                          DerivativeAccumulator *da) const = 0;

#if !defined(IMP_DOXYGEN) && 1 != 1
  // backwards compatibility
  virtual double evaluate(Particle *a,
                          DerivativeAccumulator *da) const {
    return evaluate(Particle(a), da);
  }
#endif

  /** Implementations
      for these are provided by the IMP_SINGLETON_SCORE()
      macro.
  */
  virtual double evaluate(const ParticlesTemp &o,
                          DerivativeAccumulator *da) const = 0;

  /** \name Incremental evaluation
      Compute how much the score has changed since the last evaluate
      (and writing derivatives if they have changed). Implementations
      for these are provided by the IMP_SINGLETON_SCORE() macro.
      @{
  */
  virtual double evaluate_change(Particle* vt,
                                 DerivativeAccumulator *da) const = 0;

  virtual double evaluate_change(const ParticlesTemp &o,
                                 DerivativeAccumulator *da) const = 0;
  virtual double evaluate_prechange(Particle* vt,
                                    DerivativeAccumulator *da) const = 0;
  virtual double evaluate_prechange(const ParticlesTemp &o,
                                    DerivativeAccumulator *da) const = 0;
  //! Return true if the score for vt might have changed.
  virtual bool get_is_changed(Particle* vt) const =0;
  /** @} */

  /** Get the set of interaction induced by applying to the
      argument. */
  virtual ParticlesList
    get_interacting_particles(Particle* vt) const =0;

  /** Get the set of particles read when applied to the arguments. */
  virtual ParticlesTemp
    get_input_particles(Particle* vt) const =0;

  /** Get the set of input containers when this modifier is applied to
      the arguments. */
  virtual ContainersTemp
    get_input_containers(Particle* vt) const =0;

  IMP_REF_COUNTED_DESTRUCTOR(SingletonScore);
};

IMP_OBJECTS(SingletonScore,SingletonScores);

IMP_END_NAMESPACE

#endif  /* IMP_SINGLETON_SCORE_H */
