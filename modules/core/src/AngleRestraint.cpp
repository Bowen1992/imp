/**
 *  \file AngleRestraint.cpp \brief Angle restraint between three particles.
 *
 *  Copyright 2007-2015 IMP Inventors. All rights reserved.
 *
 */

#include <IMP/core/AngleRestraint.h>
#include <IMP/core/AngleTripletScore.h>

IMPCORE_BEGIN_NAMESPACE

AngleRestraint::AngleRestraint(UnaryFunction* score_func, kernel::Particle* p1,
                               kernel::Particle* p2, kernel::Particle* p3)
    : TripletRestraint(new AngleTripletScore(score_func),
                       kernel::ParticleTriplet(p1, p2, p3)) {}

AngleRestraint::AngleRestraint(UnaryFunction* score_func, XYZ p0, XYZ p1,
                               XYZ p2)
    : TripletRestraint(new AngleTripletScore(score_func),
                       kernel::ParticleTriplet(p0, p1, p2)) {}

IMPCORE_END_NAMESPACE
