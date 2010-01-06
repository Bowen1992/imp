/**
 *  \file SingletonFilter.cpp   \brief Filter for particle.
 *
 *  This file is generated by a script (core/tools/make-filter).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 *
 */

#include "IMP/SingletonFilter.h"
#include "IMP/internal/utility.h"
#include "IMP/SingletonModifier.h"

IMP_BEGIN_NAMESPACE

namespace {
  unsigned int next_index=0;
}


SingletonFilter::SingletonFilter(std::string name) {
  set_name(internal::make_object_name(name, next_index++));
}

IMP_END_NAMESPACE
