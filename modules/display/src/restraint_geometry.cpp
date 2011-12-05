/**
 *  \file restraint_geometry.cpp
 *  \brief XXXX.
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/display/restraint_geometry.h"
#include <IMP/container/PairsRestraint.h>
#include <IMP/container/ListPairContainer.h>
#include <IMP/container/SingletonsRestraint.h>
#include <IMP/algebra/Segment3D.h>

IMPDISPLAY_BEGIN_NAMESPACE

RestraintGeometry::RestraintGeometry(Restraint*r): Geometry(r->get_name()),
                                                   r_(r), m_(r_->get_model()){}
RestraintGeometry::RestraintGeometry(Restraint*r, Model *m):
  Geometry(r->get_name()),
  r_(r), m_(m){}

namespace {
  bool get_has_coordinates(Particle *p) {
    return p->has_attribute(FloatKey(0));
  }
  algebra::Vector3D get_coordinates(Particle *p) {
    return p->get_model()->get_sphere(p->get_index()).get_center();
  }
}


IMP::display::Geometries RestraintGeometry::get_components() const {
  IMP_CHECK_OBJECT(r_);
  m_->update();
  Pointer<Restraint> rd= r_->create_current_decomposition();
  RestraintSet *rs= dynamic_cast<RestraintSet*>(rd.get());
  IMP::display::Geometries ret;
  if (!rs) {
    ParticlesTemp ps= r_->get_input_particles();
    for (unsigned int i=0; i < ps.size(); ++i) {
      if (!get_has_coordinates(ps[i])) continue;
      for (unsigned int j=0; j< i; ++j) {
        if (!get_has_coordinates(ps[j])) continue;
        ret.push_back(new SegmentGeometry
                      (algebra::Segment3D(get_coordinates(ps[i]),
                                          get_coordinates(ps[j]))));
      }
    }
  } else {
    for (unsigned int i=0; i< rs->get_number_of_restraints(); ++i) {
      Restraint *rc=rs->get_restraint(i);
      rc->set_was_used(true);
      ret.push_back(new RestraintGeometry(rc, m_));
    }
  }
  return ret;
}

void RestraintGeometry::do_show(std::ostream &out) const {
  out << "  restraint: " << r_->get_name() << std::endl;
}




IMPDISPLAY_END_NAMESPACE
