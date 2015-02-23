/**
 *  \file Restraint.cpp   \brief Abstract base class for all restraints.
 *
 *  Copyright 2007-2015 IMP Inventors. All rights reserved.
 *
 */

#include <cmath>

#include "IMP/kernel/Decorator.h"
#include "IMP/kernel/Particle.h"

IMPKERNEL_BEGIN_INTERNAL_NAMESPACE
extern base::Vector<std::pair<ParticleFunction, ParticleFunction> >
    particle_validators;

void add_particle_check(ParticleFunction instance, ParticleFunction check) {
  particle_validators.push_back(std::make_pair(instance, check));
}

IMPKERNEL_END_INTERNAL_NAMESPACE

IMPKERNEL_BEGIN_NAMESPACE

Decorator::Decorator(ParticleAdaptor p)
  : model_(p.get_model()), pi_(p.get_particle_index()), is_valid_(true) {}

void check_particle(Particle *p) {
  for (unsigned int i = 0; i < internal::particle_validators.size(); ++i) {
    if (internal::particle_validators[i]
            .first(p->get_model(), p->get_index())) {
      internal::particle_validators[i].second(p->get_model(), p->get_index());
    }
  }
}

Undecorator::Undecorator(kernel::Model *m, std::string name)
    : base::Object(name), m_(m) {}

IMPKERNEL_END_NAMESPACE
