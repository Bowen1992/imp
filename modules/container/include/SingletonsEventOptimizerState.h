/**
 *  \file core/SingletonsEventOptimizerState.h    \brief Define some predicates.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 */

#ifndef IMPCONTAINER_SINGLETONS_EVENT_OPTIMIZER_STATE_H
#define IMPCONTAINER_SINGLETONS_EVENT_OPTIMIZER_STATE_H

#include "container_config.h"
#include <IMP/SingletonPredicate.h>
#include <IMP/OptimizerState.h>
#include <IMP/SingletonContainer.h>

IMPCONTAINER_BEGIN_NAMESPACE

/** Raise an IMP::base::EventException when a certain condition is met.
    Currently the supported logic is when the number of items in the
    container for which the predicate returns a certain value is in the
    range [min_count, max_count).
 */
class IMPCONTAINEREXPORT SingletonsEventOptimizerState:
    public OptimizerState {
  IMP::OwnerPointer<SingletonPredicate> pred_;
  IMP::OwnerPointer<SingletonContainer> container_;
  int v_;
  int min_, max_;
public:
  SingletonsEventOptimizerState(SingletonPredicate *pred,
                                SingletonContainer *container,
                                int value, int min_count, int max_count,
                                std::string name="ConstSingletonPredicate%1%");
  IMP_OPTIMIZER_STATE(SingletonsEventOptimizerState);
};

IMPCONTAINER_END_NAMESPACE

#endif  /* IMPCONTAINER_SINGLETONS_EVENT_OPTIMIZER_STATE_H */
