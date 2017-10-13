/**
 *  \file Chain.cpp
 *  \brief Store info for a protein chain.
 *
 *  Copyright 2007-2017 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/atom/Chain.h"

IMPATOM_BEGIN_NAMESPACE

StringKey Chain::get_id_key() {
  static StringKey k("chain");
  return k;
}

StringKey Chain::get_sequence_key() {
  static StringKey k("sequence");
  return k;
}


void Chain::show(std::ostream &out) const { out << "Chain " << get_id(); }

Chain get_chain(Hierarchy h) {
  do {
    if (h == Hierarchy()) {
      return Chain();
    }

    if (Chain::get_is_setup(h)) {
      return Chain(h);
    }
  } while ((h = h.get_parent()));
  return Chain();
}

IMPATOM_END_NAMESPACE
