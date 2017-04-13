/**
 *  \file EzRestraint.h
 *  \brief FRET_R Restraint
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 */

#ifndef IMPSPB_EZ_RESTRAINT_H
#define IMPSPB_EZ_RESTRAINT_H

#include "IMP/Restraint.h"
#include "spb_config.h"
//#include "IMP/restraint_macros.h"
#include <IMP/Particle.h>
#include <IMP/UnaryFunction.h>
#include <IMP/base_types.h>
#include <string>

IMPSPB_BEGIN_NAMESPACE

//! Ez Potential Restraint
/** Ez Restraint description here

 */
class IMPSPBEXPORT EzRestraint : public Restraint {
  Particles ps_;
  UnaryFunctions ufs_;
  Floats get_parameters(std::string restype);

 public:
  EzRestraint(Particles ps);

  virtual double unprotected_evaluate(IMP::DerivativeAccumulator *accum) const
      IMP_OVERRIDE;
  IMP::ModelObjectsTemp do_get_inputs() const;

  // void show(std::ostream &) const;

  IMP_OBJECT_METHODS(EzRestraint);
  IMP_SHOWABLE(EzRestraint);
  //  IMP_RESTRAINT(EzRestraint);
};

IMPSPB_END_NAMESPACE

#endif /* IMPSPB_EZ_RESTRAINT_H */
