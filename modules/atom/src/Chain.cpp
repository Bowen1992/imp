/**
 *  \file Chain.cpp
 *  \brief XXXX.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 *
 */

#include "IMP/atom/Chain.h"


IMPATOM_BEGIN_NAMESPACE


IntKey Chain::get_id_key() {
  static IntKey k("chain");
  return k;
}

void Chain::show(std::ostream &out) const {
  out << "Chain: " << get_id() << std::endl;
}

IMPATOM_END_NAMESPACE
