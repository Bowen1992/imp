/**
 *  \file file.cpp
 *  \brief Get directories used by IMP.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 *
 */

#include <IMP/base/Showable.h>
IMPBASE_BEGIN_NAMESPACE
Showable::Showable(Object *o): str_(o->get_name()){}

IMPBASE_END_NAMESPACE
