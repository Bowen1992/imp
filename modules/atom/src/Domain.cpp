/**
 *  \file Domain.cpp
 *  \brief XXXX.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/atom/Domain.h"


IMPATOM_BEGIN_NAMESPACE

Domain::~Domain(){}

const Domain::Data& Domain::get_data() {
  static Data data;
  return data;
}

void Domain::show(std::ostream &out) const {
  out << "Domain: " << get_begin_index()
      << " to " << get_end_index() << std::endl;
}

IMPATOM_END_NAMESPACE
