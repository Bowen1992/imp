/**
 *  \file SingletonScore.h    \brief Define SingletonScore.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 */

#ifndef IMP_SINGLETON_SCORE_H
#define IMP_SINGLETON_SCORE_H

#include "config.h"
#include "base_types.h"
#include "Interaction.h"
#include "Particle.h"
#include "DerivativeAccumulator.h"

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
  virtual Float evaluate(Particle *a,
                         DerivativeAccumulator *da) const = 0;

  virtual Float evaluate(const ParticlesTemp &o,
                         DerivativeAccumulator *da) const = 0;

  /** Get the set of interaction induced by applying to the
      argument. */
  virtual ParticlesList
    get_interacting_particles(Particle *a) const =0;

  /** Get the set of particles read when applied to the arguments. */
  virtual ParticlesTemp
    get_input_particles(Particle *a) const =0;

  IMP_REF_COUNTED_DESTRUCTOR(SingletonScore)
};

IMP_OUTPUT_OPERATOR(SingletonScore);

IMP_END_NAMESPACE

#endif  /* IMP_SINGLETON_SCORE_H */
