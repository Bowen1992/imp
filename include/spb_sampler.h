/**
 *  \file spb_sampler.h
 *  \brief SPB Sampler
 *
 *  Copyright 2011 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPMEMBRANE_SPB_SAMPLER_H
#define IMPMEMBRANE_SPB_SAMPLER_H
#include "membrane_config.h"
#include <IMP.h>
#include <IMP/atom.h>
#include <IMP/core.h>


IMPMEMBRANE_BEGIN_NAMESPACE

IMPMEMBRANEEXPORT Pointer<core::MonteCarlo> setup_SPBMonteCarlo
(Model *m, const core::MoversTemp& mvs, double temp, SPBParameters myparam);

IMPMEMBRANEEXPORT void add_BallMover
 (Particles ps, double dx, core::MoversTemp& mvs);

IMPMEMBRANEEXPORT void add_PbcBoxedMover
(Particles ps, double dx, algebra::Vector3Ds centers,
 algebra::Transformation3Ds trs, core::MoversTemp& mvs);

IMPMEMBRANEEXPORT void add_PbcBoxedRigidBodyMover
(Particles ps,double dx,double dang,algebra::Vector3Ds centers,
 algebra::Transformation3Ds trs, core::MoversTemp& mvs);

IMPMEMBRANE_END_NAMESPACE

#endif  /* IMPMEMBRANE_SPB_SAMPLER_H */
