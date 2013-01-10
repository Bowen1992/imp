/**
 *  \file IMP/container/DistributePairsScoreState.h
 *  \brief Apply a PairScore to each Pair in a list.
 *
 *  WARNING This file was generated from DistributeNAMEsScoreState.hpp
 *  in tools/maintenance/container_templates/container
 *  by tools/maintenance/make-container.
 *
 *  Copyright 2007-2013 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPCONTAINER_DISTRIBUTE_PAIRS_SCORE_STATE_H
#define IMPCONTAINER_DISTRIBUTE_PAIRS_SCORE_STATE_H

#include <IMP/container/container_config.h>

#include "DynamicListPairContainer.h"
#include <IMP/PairPredicate.h>
#include <IMP/PairContainer.h>
#include <IMP/base/Vector.h>
#include <IMP/pair_macros.h>
#include <IMP/score_state_macros.h>
#include <boost/tuple/tuple.hpp>

#include <iostream>

IMPCONTAINER_BEGIN_NAMESPACE

//! Distribute contents of one container into several based on predicates
/** This ScoreState takes a list of predicates and values. For
    each tuple in the input container, it is placed in a given output container
    if the predicate, when applied, has the passed value.

    \note The output containers contents are not necessarily disjoint.
*/
class IMPCONTAINEREXPORT DistributePairsScoreState :
public ScoreState
{
  base::OwnerPointer<PairContainer> input_;
  typedef boost::tuple<base::Pointer<DynamicListPairContainer>,
                base::OwnerPointer<PairPredicate>, int> Data;
  base::Vector<Data> data_;
  mutable bool updated_;
  void update_lists_if_necessary() const;
public:
  DistributePairsScoreState(PairContainerAdaptor input,
                      std::string name="DistributePairsScoreState %1%");

  /** A given tuple will go into the returned container if \c predicate
      returns \c value when applied to it.*/
  DynamicListPairContainer *add_predicate(PairPredicate *predicate,
                                        int value) {
    IMP_NEW(DynamicListPairContainer, c, (input_,
                                        predicate->get_name()+ " output"));
    data_.push_back(Data(c, predicate, value));
    return c;
  }
  virtual void do_before_evaluate() IMP_OVERRIDE;
  virtual void do_after_evaluate(DerivativeAccumulator *da) IMP_OVERRIDE;
  virtual ModelObjectsTemp do_get_inputs() const IMP_OVERRIDE;
  virtual ModelObjectsTemp do_get_outputs() const IMP_OVERRIDE;
  IMP_OBJECT(DistributePairsScoreState);
};

IMPCONTAINER_END_NAMESPACE

#endif  /* IMPCONTAINER_DISTRIBUTE_PAIRS_SCORE_STATE_H */
