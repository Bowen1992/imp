/**
 *  \file membrane/ExampleDecorator.h     \brief Add a name to a particle.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPMEMBRANE_HELIX_DECORATOR_H
#define IMPMEMBRANE_HELIX_DECORATOR_H

#include "membrane_config.h"

#include <IMP/core/core_macros.h>
#include <IMP/Particle.h>
#include <IMP/decorator_macros.h>
#include <IMP/Model.h>
#include <IMP/Decorator.h>
#include <IMP/exception.h>

IMPMEMBRANE_BEGIN_NAMESPACE

class IMPMEMBRANEEXPORT HelixDecorator: public Decorator
{


  static FloatKey get_helix_key(unsigned int i);

public:

  IMP_DECORATOR(HelixDecorator, Decorator);

  static HelixDecorator setup_particle(Particle *p, Float b, Float e) {
    p->add_attribute(get_helix_key(0),b);
    p->add_attribute(get_helix_key(1),e);
    HelixDecorator boundaries(p);
    return boundaries;
  }

//! Get the beginning
  Float get_begin() const {
    return get_particle()->get_value(get_helix_key(0));
  }

//! Get the end
  Float get_end() const {
    return get_particle()->get_value(get_helix_key(1));
  }

  static bool particle_is_instance(Particle *p) {
    IMP_USAGE_CHECK((p->has_attribute(get_helix_key(0))
               && p->has_attribute(get_helix_key(1)))
              || (!p->has_attribute(get_helix_key(0))
                  && !p->has_attribute(get_helix_key(1))),
              "Particle expected to either begin and end or none.");
    return p->has_attribute(get_helix_key(1));
  }

//! set begin
  void set_begin(Float v) {
    get_particle()->set_value(get_helix_key(0), v);
  }

//! set end
  void set_end(Float v) {
    get_particle()->set_value(get_helix_key(1), v);
  }

};

IMP_VALUES(HelixDecorator, HelixDecorators);

IMPMEMBRANE_END_NAMESPACE

#endif  /* IMPMEMBRANE_HELIX_DECORATOR_H */
