/**
 *  \file PairModifier.cpp   \brief A function on Particles.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 *
 */

#include "IMP/PairModifier.h"
#include "IMP/internal/utility.h"

IMP_BEGIN_NAMESPACE

namespace {
  unsigned int next_index=0;
}

PairModifier::PairModifier(std::string name):
  Object(internal::make_object_name(name, next_index++)){
}

IMP_END_NAMESPACE
