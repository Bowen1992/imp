/**
 *  \file IMP/isd/CysteineCrossLinkRestraint.h
 *  \brief A sigmoid shaped restraint between
 *  residues with discrete classifier
 *  and ambiguous assignment. To be used with
 *  cross-linking mass-spectrometry data.
 *
 *  Copyright 2007-2015 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPISD_CYSTEINE_CROSS_LINK_RESTRAINT_H
#define IMPISD_CYSTEINE_CROSS_LINK_RESTRAINT_H
#include "isd_config.h"
#include <IMP/Restraint.h>
#include <IMP/isd/CrossLinkData.h>
#include <IMP/isd/CysteineCrossLinkData.h>

IMPISD_BEGIN_NAMESPACE
//! A restraint for cysteine cross-linking data.
/** It models the frequency fexp derived from gel separation of cross-linked
    complexes. The experimental frequency fexp is derived as the
    fraction of dimeric versus monomeric complex, upon cysteine
    cross-linking.  The forward model is derived from free energy
    calculations.  The likelyhood is modeled as a normal distribution
    function truncated over the interval [0,1]. To contruct the
    restraint, the class CysteineCrossLinkData and CrossLinkData have
    to be initialized (see for instance
    test_CysteineCrossLinkRestraint.py).  Input parameters and
    constructor. There are two different constructors p1 and p2 are
    the two cross-linked sites.  \f$ \beta \f$ is the beta parameter
    in the distribution function.  The \f$ \alpha \f$ parameter is
    constrained by the condition that the variance is gaussian by the
    formula \f$ \omega \f$ is a scale factor for the beta parameter,
    to allow for outlier detection.  Usage: Construct the ambiguous
    set using CysteineCrossLinkRestraint(\f$ \beta \f$, \f$ \omega
    \f$, xlyield, \f$ f_{exp} \f$) method.  The model frequencies of
    each individual copy are summed, weighted and multiplied by the
    unknown crosslink yield (epsilon).  Pass individual cross-linked
    pairs to add_contribution(p0, p1, w) command, where "w" is a state
    weight parameter.
 */

class IMPISDEXPORT CysteineCrossLinkRestraint : public Restraint {
  Particles ps1_;
  Particles ps2_;
  std::vector<Particles> pslist1_;
  std::vector<Particles> pslist2_;
  Pointer<Particle> beta_;
  Pointer<Particle> sigma_;
  Pointer<Particle> epsilon_;  // k * t at the exponential
  Pointer<Particle> weight_;
  PointerMember<CrossLinkData> data_;
  PointerMember<CysteineCrossLinkData> ccldata_;
  int constr_type_;
  double fexp_;
  bool use_CA_;

 public:
  //! Create the restraint.
  /** Restraints should store the particles they are to act on,
      preferably in a Singleton or PairContainer as appropriate.
   */
  CysteineCrossLinkRestraint(Particle *beta, Particle *sigma,
                             Particle *epsilon,
                             Particle *weight, CrossLinkData *data,
                             double fexp);

  CysteineCrossLinkRestraint(Particle *beta, Particle *sigma,
                             Particle *epsilon,
                             Particle *weight, CrossLinkData *data,
                             CysteineCrossLinkData *ccldata);

  /* call for probability */
  double get_probability() const;

  double get_standard_error() const;

  Floats get_frequencies() const;

  double get_model_frequency() const;

  Floats get_distances() const;

  double get_truncated_normalization(double mean, double sigma) const;

  double get_normal_pdf(double mean, double sigma, double x) const;

  void add_contribution(Particle *p1, Particle *p2);
  void add_contribution(Particles p1, Particles p2);

  algebra::Vector3D get_CB_coordinates(const Particles &ps) const;

  unsigned get_number_of_contributions() const;

  /** This macro declares the basic needed methods: evaluate and show
   */
  virtual double unprotected_evaluate(IMP::DerivativeAccumulator *accum)
      const IMP_OVERRIDE;
  virtual IMP::ModelObjectsTemp do_get_inputs() const IMP_OVERRIDE;
  IMP_OBJECT_METHODS(CysteineCrossLinkRestraint);
};

IMPISD_END_NAMESPACE

#endif /* IMPISD_CYSTEINE_CROSS_LINK_RESTRAINT_H */
