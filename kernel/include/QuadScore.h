/**
 *  \file QuadScore.h    \brief Define QuadScore.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 */

#ifndef IMP_QUAD_SCORE_H
#define IMP_QUAD_SCORE_H

#include "config.h"
#include "base_types.h"
#include "Particle.h"
#include "DerivativeAccumulator.h"
#include "internal/container_helpers.h"

IMP_BEGIN_NAMESPACE

//! Abstract score function
/** QuadScores will evaluate the score and derivatives
    for the passed particles. Use in conjunction with various
    restraints such as IMP::core::QuadsRestraint or
    IMP::core::QuadRestraint.

    Implementers should check out IMP_QUAD_SCORE().
*/
class IMPEXPORT QuadScore : public Object
{
public:
  QuadScore(std::string name="QuadScore %1%");
  //! Compute the score and the derivative if needed.
  virtual double evaluate(const ParticleQuad& vt,
                          DerivativeAccumulator *da) const = 0;

#if !defined(IMP_DOXYGEN) && 4 != 1
  // backwards compatibility
  virtual double evaluate(Particle *a, Particle *b, Particle *c, Particle *d,
                          DerivativeAccumulator *da) const {
    return evaluate(ParticleQuad(a,b,c,d), da);
  }
#endif

  /** An implementations
      for this is provided by the IMP_SINGLETON_SCORE,
      IMP_PAIR_SCORE macros.
  */
  virtual double evaluate(const ParticleQuadsTemp &o,
                          DerivativeAccumulator *da) const = 0;

  /** \name Incremental evaluation
      Compute how much the score has changed since the last evaluate
      (and writing derivatives if they have changed). Implementations
      for these are provided by the IMP_SINGLETON_SCORE,
      IMP_PAIR_SCORE macros.
      @{
  */
  virtual double evaluate_change(const ParticleQuad& vt,
                                 DerivativeAccumulator *da) const = 0;

  virtual double evaluate_change(const ParticleQuadsTemp &o,
                                 DerivativeAccumulator *da) const = 0;
  virtual double evaluate_prechange(const ParticleQuad& vt,
                                    DerivativeAccumulator *da) const = 0;
  virtual double evaluate_prechange(const ParticleQuadsTemp &o,
                                    DerivativeAccumulator *da) const = 0;

  /** @} */

  /** Get the set of interaction induced by applying to the
      argument. */
  virtual ParticlesList
    get_interacting_particles(const ParticleQuad& vt) const =0;

  /** Get the set of particles read when applied to the arguments. */
  virtual ParticlesTemp
    get_input_particles(const ParticleQuad& vt) const =0;

  /** Get the set of input containers when this modifier is applied to
      the arguments. */
  virtual ContainersTemp
    get_input_containers(const ParticleQuad& vt) const =0;

  IMP_REF_COUNTED_DESTRUCTOR(QuadScore)
};

IMP_OUTPUT_OPERATOR(QuadScore);

IMP_END_NAMESPACE

#endif  /* IMP_QUAD_SCORE_H */
