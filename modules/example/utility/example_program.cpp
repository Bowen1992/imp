/*
 * Copyright 2007-2015 IMP Inventors. All rights reserved.
 */

#include <IMP/Model.h>
#include <IMP/base/flags.h>
#include <IMP/base/Pointer.h>
#include <IMP/core/RestraintsScoringFunction.h>
#include <IMP/example/ExampleRestraint.h>
#include <IMP/container/ListPairContainer.h>
#include <IMP/core/XYZ.h>

using namespace IMP;
int main(int argc, char **argv) {
  base::setup_from_argv(argc, argv, "An example program");
  base::Pointer<Model> m(new Model());
  ParticleIndex p0 = m->add_particle("P0");
  core::XYZ d0 = core::XYZ::setup_particle(m, p0);
  ParticleIndex p1 = m->add_particle("P1");
  core::XYZ d1 = core::XYZ::setup_particle(m, p1);
  base::Pointer<example::ExampleRestraint> r0 =
      new example::ExampleRestraint(m, p0, 1);
  base::Pointer<example::ExampleRestraint> r1 =
      new example::ExampleRestraint(m, p1, 1);
  RestraintsTemp rs;
  rs.push_back(r0);
  rs.push_back(r1);
  d0.set_coordinates(algebra::Vector3D(0, 0, 2));
  d1.set_coordinates(algebra::Vector3D(0, 0, 3));

  base::Pointer<core::RestraintsScoringFunction> scoring_function =
      new core::RestraintsScoringFunction(rs);
  return 0;
}
