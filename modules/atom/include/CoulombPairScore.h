/**
 *  \file CoulombPairScore.h
 *  \brief Coulomb (electrostatic) score between a pair of particles.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 */

#ifndef IMPATOM_COULOMB_PAIR_SCORE_H
#define IMPATOM_COULOMB_PAIR_SCORE_H

#include "atom_config.h"
#include <IMP/PairScore.h>
#include <IMP/Pointer.h>
#include <IMP/atom/smoothing_functions.h>

IMPATOM_BEGIN_NAMESPACE

//! Coulomb (electrostatic) score between a pair of particles.
/** The two particles in the pair must be Charged particles.
    The form of the potential is \f[
       \frac{1}{4\pi\epsilon_0\epsilon_r} \frac{q_i q_j}{|r_{ij}|}
    \f] where \f$\epsilon_0\f$ is the permittivity of vacuum,
    \f$\epsilon_r\f$ the relative dielectric (adjustable; 1.0 by default),
    \f$q_i\f$ and \f$q_j\f$ the charges on the two particles,
    and \f$|r_{ij}|\f$ the distance between them.
 */
class IMPATOMEXPORT CoulombPairScore : public PairScore
{
  IMP::internal::OwnerPointer<SmoothingFunction> smoothing_function_;
  double relative_dielectric_;
  double multiplication_factor_;

  void calculate_multiplication_factor();

public:
  CoulombPairScore(SmoothingFunction *f) : smoothing_function_(f) {
    set_relative_dielectric(1.0);
  }

  void set_relative_dielectric(double relative_dielectric) {
    relative_dielectric_ = relative_dielectric;
    calculate_multiplication_factor();
  }

  double get_relative_dielectric() const { return relative_dielectric_; }

  IMP_SIMPLE_PAIR_SCORE(CoulombPairScore);
};

IMPATOM_END_NAMESPACE

#endif  /* IMPATOM_COULOMB_PAIR_SCORE_H */
