/**
 *  \file PbcBoxedRigidBodyMover.cpp
 *  \brief A mover that transforms a rigid body
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 *
 */
#include <IMP/membrane/PbcBoxedRigidBodyMover.h>
#include <IMP/core/XYZ.h>
#include <IMP/algebra/vector_generators.h>

IMPMEMBRANE_BEGIN_NAMESPACE

PbcBoxedRigidBodyMover::PbcBoxedRigidBodyMover(core::RigidBody d,
                               Particles ps,
                               Float max_translation, Float max_angle,
                               algebra::Vector3Ds centers,
                              algebra::Transformation3Ds transformations):
  Mover(d->get_model(),
        "PbcBoxedRigidBodyMover%1%")
{
  IMP_LOG(VERBOSE,"start PbcBoxedRigidBodyMover constructor");
  max_translation_=max_translation;
  max_angle_ =max_angle;
  d_= d;
  ps_ = ps;
  centers_ = centers;
  transformations_ = transformations;
  IMP_LOG(VERBOSE,"finish mover construction" << std::endl);
}

ParticlesTemp PbcBoxedRigidBodyMover::propose_move(Float f) {
  IMP_LOG(VERBOSE,"PbcBoxedRigidBodyMover::f is  : " << f <<std::endl);
  {
    ::boost::uniform_real<> rand(0,1);
    double fc =rand(base::random_number_generator);
    if (fc > f) return ParticlesTemp();
  }
  last_transformation_= d_.get_reference_frame().get_transformation_to();
  algebra::VectorD<3> translation
    = algebra::get_random_vector_in(algebra::Sphere3D(d_.get_coordinates(),
                                                      max_translation_));

  // find cell
  double mindist=1.0e+24;
  unsigned icell=0;
  for(unsigned int i=0;i<centers_.size();++i){
   double dist=algebra::get_l2_norm(translation-centers_[i]);
   if(dist<mindist){
    mindist=dist;
    icell=i;
   }
  }

  ParticlesTemp ret=ParticlesTemp(1, d_);
  if(icell!=0){ret.insert(ret.end(), ps_.begin(), ps_.end());}

  algebra::Transformation3D trans=transformations_[icell].get_inverse();
  translation=trans.get_transformed(translation);

  algebra::VectorD<3> axis =
    algebra::get_random_vector_on(algebra::Sphere3D(algebra::VectorD<3>(0.0,
                                                                        0.0,
                                                                        0.0),
                                                    1.));
  ::boost::uniform_real<> rand(-max_angle_,max_angle_);
  Float angle =rand(base::random_number_generator);
  algebra::Rotation3D r
    = algebra::get_rotation_about_axis(axis, angle);
  algebra::Rotation3D ri=trans.get_rotation()*
     d_.get_reference_frame().get_transformation_to().get_rotation();
  algebra::Rotation3D rc = r*ri;
  algebra::Transformation3D t(rc, translation);
  IMP_LOG(VERBOSE,"PbcBoxedRigidBodyMover:: propose move : " << t << std::endl);
  d_.set_reference_frame(algebra::ReferenceFrame3D(t));

// move also the other particles
  oldcoords_.clear();
  for(unsigned int i=0;i<ps_.size();++i){
   oldcoords_.push_back(core::XYZ(ps_[i]).get_coordinates());
   algebra::Vector3D newcoord=trans.get_transformed(oldcoords_[i]);
   core::XYZ(ps_[i]).set_coordinates(newcoord);
  }

  return ret;
}

void PbcBoxedRigidBodyMover::reset_move() {
 d_.set_reference_frame(algebra::ReferenceFrame3D(last_transformation_));
 last_transformation_= algebra::Transformation3D();
 for(unsigned int i=0;i<ps_.size();++i){
   core::XYZ(ps_[i]).set_coordinates(oldcoords_[i]);
 }
}

ParticlesTemp PbcBoxedRigidBodyMover::get_output_particles() const {
 ParticlesTemp ret=ParticlesTemp(1, d_);
 ret.insert(ret.end(), ps_.begin(), ps_.end());
 return ret;
}

void PbcBoxedRigidBodyMover::do_show(std::ostream &out) const {
  out << "max translation: " << max_translation_ << "\n";
  out << "max angle: " << max_angle_ << "\n";
}
IMPMEMBRANE_END_NAMESPACE
