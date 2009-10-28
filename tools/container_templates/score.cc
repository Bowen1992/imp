/**
 *  \file GroupnameScore.cpp  \brief Define GroupnameScore
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 */

#include <IMP/GroupnameScore.h>
#include <IMP/internal/utility.h>

IMP_BEGIN_NAMESPACE

namespace {
  unsigned int next_index=0;
}

GroupnameScore::GroupnameScore(std::string name):
  Object(internal::make_object_name(name, next_index++))
{
  /* Implemented here rather than in the header so that PairScore
     symbols are present in the kernel DSO */
}

IMP_END_NAMESPACE
