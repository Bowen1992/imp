/**
 *  \file Model.cpp \brief Storage of a model, its restraints,
 *                         constraints and particles.
 *
 *  Copyright 2007-2015 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/Model.h"
#include "IMP/Particle.h"
#include "IMP/internal/scoring_functions.h"
#include "IMP/set.h"

IMPKERNEL_BEGIN_NAMESPACE

//! Constructor
Model::Model(std::string name) : Object(name) {
  cur_stage_ = internal::NOT_EVALUATING;
  set_was_used(true);
  first_call_ = true;
#if IMP_HAS_CHECKS >= IMP_INTERNAL
  internal::FloatAttributeTable::set_masks(
      &this->Masks::read_mask_, &this->Masks::write_mask_,
      &this->Masks::add_remove_mask_, &this->Masks::read_derivatives_mask_,
      &this->Masks::write_derivatives_mask_);
  internal::StringAttributeTable::set_masks(&this->Masks::read_mask_,
                                            &this->Masks::write_mask_,
                                            &this->Masks::add_remove_mask_);
  internal::IntAttributeTable::set_masks(&this->Masks::read_mask_,
                                         &this->Masks::write_mask_,
                                         &this->Masks::add_remove_mask_);
  internal::ObjectAttributeTable::set_masks(&this->Masks::read_mask_,
                                            &this->internal::Masks::write_mask_,
                                            &this->Masks::add_remove_mask_);
  internal::WeakObjectAttributeTable::set_masks(
      &this->Masks::read_mask_, &this->internal::Masks::write_mask_,
      &this->Masks::add_remove_mask_);
  internal::IntsAttributeTable::set_masks(&this->Masks::read_mask_,
                                          &this->Masks::write_mask_,
                                          &this->Masks::add_remove_mask_);
  internal::ObjectsAttributeTable::set_masks(&this->Masks::read_mask_,
                                             &this->Masks::write_mask_,
                                             &this->Masks::add_remove_mask_);
  internal::ParticleAttributeTable::set_masks(&this->Masks::read_mask_,
                                              &this->Masks::write_mask_,
                                              &this->Masks::add_remove_mask_);
  internal::ParticlesAttributeTable::set_masks(&this->Masks::read_mask_,
                                               &this->Masks::write_mask_,
                                               &this->Masks::add_remove_mask_);
#endif
  // must be last
  restraints_ = new RestraintSet(this, 1.0, "Model Restraints");
}

IMP_LIST_ACTION_IMPL(Model, ScoreState, ScoreStates, score_state,
                     score_states, ScoreState *, ScoreStates);

ParticleIndex Model::add_particle_internal(Particle *p) {
  IMP_CHECK_OBJECT(this);
  IMP_CHECK_OBJECT(p);
  p->set_was_used(true);
  ParticleIndex ret;
  if (free_particles_.empty()) {
    ret = ParticleIndex(particle_index_.size());
  } else {
    ret = free_particles_.back();
    free_particles_.pop_back();
  }
  p->id_ = ret;
  int maxp = std::max<unsigned int>(particle_index_.size(),
                                    get_as_unsigned_int(ret) + 1);
  particle_index_.resize(maxp);
  particle_index_[ret] = p;

#if IMP_HAS_CHECKS >= IMP_INTERNAL
  // xstd::cout << "Resizing to " << particle_index_.size() << std::endl;
  Masks::read_mask_.resize(particle_index_.size(), true);
  Masks::write_mask_.resize(particle_index_.size(), true);
  Masks::add_remove_mask_.resize(particle_index_.size(), true);
  Masks::read_derivatives_mask_.resize(particle_index_.size(), true);
  Masks::write_derivatives_mask_.resize(particle_index_.size(), true);
#endif
  return ret;
}

void Model::do_remove_particle(ParticleIndex pi) {
  if (undecorators_index_.size() > static_cast<size_t>(pi.get_index())) {
    for (unsigned int i = 0; i < undecorators_index_[pi].size(); ++i) {
      undecorators_index_[pi][i]->teardown(pi);
    }
    undecorators_index_[pi].clear();
  }
  internal::FloatAttributeTable::clear_attributes(pi);
  internal::StringAttributeTable::clear_attributes(pi);
  internal::IntAttributeTable::clear_attributes(pi);
  internal::ObjectAttributeTable::clear_attributes(pi);
  internal::WeakObjectAttributeTable::clear_attributes(pi);
  internal::IntsAttributeTable::clear_attributes(pi);
  internal::ObjectsAttributeTable::clear_attributes(pi);
  internal::ParticleAttributeTable::clear_attributes(pi);
  internal::ParticlesAttributeTable::clear_attributes(pi);
  free_particles_.push_back(pi);
  particle_index_[pi]->set_model(nullptr);
  particle_index_[pi] = nullptr;
}

void Model::remove_particle(ParticleIndex pi) { do_remove_particle(pi); }

ParticleIndexes Model::get_particle_indexes() {
  ParticleIndexes ret;
  for (unsigned int i = 0; i < particle_index_.size(); ++i) {
    if (particle_index_[ParticleIndex(i)]) {
      ret.push_back(ParticleIndex(i));
    }
  }
  return ret;
}

ModelObjectsTemp Model::get_model_objects() const {
  ModelObjectsTemp ret;
  ret.reserve(dependency_graph_.size());
  IMP_FOREACH(const DependencyGraph::value_type & vt, dependency_graph_) {
    ret.push_back(const_cast<ModelObject *>(vt.first));
  }
  return ret;
}

ParticleIndex Model::add_particle(std::string name) {
  IMP_NEW(Particle, p, (this, name));
  return p->get_index();
}

//! Get the name of a particle
std::string Model::get_particle_name(ParticleIndex pi) {
  return get_particle(pi)->get_name();
}


void Model::add_undecorator(ParticleIndex pi, Undecorator *d) {
  undecorators_index_.resize(
      std::max<size_t>(pi.get_index() + 1, undecorators_index_.size()));
  undecorators_index_[pi].push_back(d);
}

Particle *Model::get_particle(ParticleIndex p) const {
  IMP_USAGE_CHECK(get_has_particle(p), "Invalid particle requested");
  return particle_index_[p];
}

bool Model::get_has_particle(ParticleIndex p) const {
  if (particle_index_.size() <= get_as_unsigned_int(p)) return false;
  return particle_index_[p];
}

void Model::do_add_score_state(ScoreState *obj) {
  IMP_INTERNAL_CHECK(cur_stage_ == internal::NOT_EVALUATING,
                     "The set of score states cannot be changed during"
                         << "evaluation.");
  if (!obj->get_model()) obj->set_model(this);
  obj->set_was_used(true);
  IMP_LOG_VERBOSE("Added score state " << obj->get_name() << std::endl);
  IMP_IF_CHECK(USAGE) {
    boost::unordered_set<ScoreState *> in(score_states_begin(),
                                          score_states_end());
    IMP_USAGE_CHECK(in.size() == get_number_of_score_states(),
                    "Score state already in model " << obj->get_name());
  }
}

void Model::do_remove_score_state(ScoreState *obj) { obj->set_model(nullptr); }

void Model::clear_particle_caches(ParticleIndex pi) {
  internal::FloatAttributeTable::clear_caches(pi);
  internal::StringAttributeTable::clear_caches(pi);
  internal::IntAttributeTable::clear_caches(pi);
  internal::ObjectAttributeTable::clear_caches(pi);
  internal::WeakObjectAttributeTable::clear_caches(pi);
  internal::IntsAttributeTable::clear_caches(pi);
  internal::ObjectsAttributeTable::clear_caches(pi);
  internal::ParticleAttributeTable::clear_caches(pi);
  internal::ParticlesAttributeTable::clear_caches(pi);
}

void Model::add_data(ModelKey mk, Object *o) {
  model_data_.resize(std::max<int>(mk.get_index() + 1, model_data_.size()));
  model_data_[mk.get_index()] = o;
}

Object *Model::get_data(ModelKey mk) const {
  return model_data_[mk.get_index()].get();
}

void Model::remove_data(ModelKey mk) {
  model_data_[mk.get_index()] = nullptr;
}

bool Model::get_has_data(ModelKey mk) const {
  if (model_data_.size() > mk.get_index()) {
    return model_data_[mk.get_index()];
  } else {
    return false;
  }
}

void Model::do_destroy() {
  IMP_OBJECT_LOG;
  IMP_LOG_TERSE("Destroying model" << std::endl);
  // make sure we clear their data to free model objects they are keeping alive
  IMP_FOREACH(Particle * p, particle_index_) {
    if (p) {
      remove_particle(p->get_index());
    }
  }
  while (!dependency_graph_.empty()) {
    ModelObject *mo =
        const_cast<ModelObject *>(dependency_graph_.begin()->first);
    IMP_CHECK_OBJECT(mo);
    mo->set_model(nullptr);
  }
}

/////////////////////////////////////////// NOT YET DEPRECATED STUFF
RestraintsTemp Model::get_restraints() const {
  return restraints_->get_restraints();
}

/////////////////////////////////////////// DEPRECATED STUFF
ScoringFunction *Model::create_model_scoring_function() {
  IMPKERNEL_DEPRECATED_METHOD_DEF(2.1,
                                  "Use a ScoringFunction instead.");
  return restraints_->create_scoring_function();
}
void Model::add_restraint(Restraint *r) {
  IMPKERNEL_DEPRECATED_METHOD_DEF(2.1,
                                  "Add to a ScoringFunction instead.");
  if (!r->get_model()) r->set_model(this);
  restraints_->add_restraint(r);
}
void Model::remove_restraint(Restraint *r) {
  IMPKERNEL_DEPRECATED_METHOD_DEF(2.1,
                                  "Use a ScoringFunction instead.");
  restraints_->remove_restraint(r);
}
ScoringFunction *Model::create_scoring_function() {
  return create_model_scoring_function();
}

void Model::set_maximum_score(Restraint *r, double s) {
  IMPKERNEL_DEPRECATED_METHOD_DEF(2.1,
                                  "Set the maximum on the restraint itself.");
  r->set_maximum_score(s);
}
void Model::set_maximum_score(double s) {
  IMPKERNEL_DEPRECATED_METHOD_DEF(2.1, "Use a ScoringFunction.");
  return restraints_->set_maximum_score(s);
}

ParticlesTemp Model::get_particles() const {
  IMPKERNEL_DEPRECATED_METHOD_DEF(2.1, "Use get_particle_indexes().");
  return ParticlesTemp(particles_begin(), particles_end());
}

Model::ParticleIterator Model::particles_begin() const {
  IMPKERNEL_DEPRECATED_METHOD_DEF(2.1, "Use get_particle_indexes().");
  return ParticleIterator(NotNull(), particle_index_.begin(),
                          particle_index_.end());
}

Model::ParticleIterator Model::particles_end() const {
  IMPKERNEL_DEPRECATED_METHOD_DEF(2.1, "Use get_particle_indexes().");
  return ParticleIterator(NotNull(), particle_index_.end(),
                          particle_index_.end());
}

void Model::remove_particle(Particle *p) {
  IMPKERNEL_DEPRECATED_METHOD_DEF(2.1, "Use the ParticleIndex version.");
  remove_particle(p->get_index());
}

RestraintSet *Model::get_root_restraint_set() {
  IMPKERNEL_DEPRECATED_METHOD_DEF(
      2.1, "Use a scoring function instead of the Model.");
  return restraints_;
}
unsigned int Model::get_number_of_restraints() const {
  IMPKERNEL_DEPRECATED_METHOD_DEF(
      2.1, "Use a scoring function instead of the Model.");
  return restraints_->get_number_of_restraints();
}

Restraint *Model::get_restraint(unsigned int i) const {
  IMPKERNEL_DEPRECATED_METHOD_DEF(
      2.1, "Use a scoring function instead of the Model.");
  return restraints_->get_restraint(i);
}

unsigned int Model::get_number_of_particles() const {
  IMPKERNEL_DEPRECATED_METHOD_DEF(2.1, "Use get_particle_indexes() instead.");
  return get_particles().size();
}

IMPKERNEL_END_NAMESPACE
