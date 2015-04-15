/**
 *  \file DecayPairContainerOptimizerState.cpp
 *  \brief A pair container with a decaying list of pairs.
 *
 *  Copyright 2007-2015 IMP Inventors. All rights reserved.
 */

#include <IMP/misc/DecayPairContainerOptimizerState.h>
#include <IMP/pair_macros.h>
#include <IMP/container/ListPairContainer.h>

IMPMISC_BEGIN_NAMESPACE

DecayPairContainerOptimizerState::DecayPairContainerOptimizerState(
    PairPredicate *pred, const ParticlePairsTemp &initial_list,
    std::string name)
    : OptimizerState(initial_list[0][0]->get_model(), name),
      pred_(pred),
      input_(new container::ListPairContainer(initial_list, "decay input")) {
  output_ = new container::DynamicListPairContainer(input_, name + " output");
  output_->set(IMP::get_indexes(input_->get_particle_pairs()));
}

void DecayPairContainerOptimizerState::do_update(unsigned int) {
  IMP_OBJECT_LOG;
  ParticleIndexPairs to_remove;
  IMP_FOREACH(ParticleIndexPair pip, output_->get_contents()) {
    if (pred_->get_value_index(input_->get_model(), pip) == 0) {
      to_remove.push_back(pip);
    }
  }
  if (!to_remove.empty()) {
    IMP_LOG_TERSE("Removing " << to_remove << std::endl);
    ParticleIndexPairs old = output_->get_indexes();
    std::sort(old.begin(), old.end());
    std::sort(to_remove.begin(), to_remove.end());
    ParticleIndexPairs out;
    std::set_difference(old.begin(), old.end(), to_remove.begin(),
                        to_remove.end(), std::back_inserter(out));
    output_->set(out);
    IMP_LOG_VERBOSE("Remaining " << output_->get_particle_pairs() << " ");
  }
}

IMPMISC_END_NAMESPACE
