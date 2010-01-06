/**
 *  \file CloseBipartitePairsScoreState.h
 *  \brief Maintain a list of close pairs.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 */

#ifndef IMPCORE_CLOSE_BIPARTITE_PAIRS_SCORE_STATE_H
#define IMPCORE_CLOSE_BIPARTITE_PAIRS_SCORE_STATE_H

#include "config.h"
#include "ClosePairsFinder.h"
#include "ListPairContainer.h"
#include "CloseBipartitePairContainer.h"
#include <IMP/ScoreState.h>
#include <IMP/PairFilter.h>

#include <IMP/SingletonContainer.h>

IMPCORE_BEGIN_NAMESPACE

// for SWIG
class ListPairContainer;
class ClosePairsFinder;

/** \deprecated{Use CloseBipartitePairContainer instead}
 */
class IMPCOREEXPORT CloseBipartitePairsScoreState : public ScoreState
{
  IMP::internal::OwnerPointer<ClosePairsFinder> f_;
  IMP::internal::OwnerPointer<SingletonContainer> a_, b_;
  IMP::internal::OwnerPointer<CloseBipartitePairContainer> out_;
  Float distance_, slack_;
 protected:
  void set_model(Model *m);
 public:
  CloseBipartitePairsScoreState(SingletonContainer *a,
                                SingletonContainer *b);

  //! Set the distance threshold
  void set_distance(Float d);

  //! Set the amount of slack to add to the threshold
  /** Increasing this means the list is rebuilt more often but
      also the list is smaller.
   */
  void set_slack(Float s);

  //! Get the container where the list of close pairs is put
  PairContainer* get_close_pairs_container() const {
    return out_;
  }

  //! Set the algorithm used.
  void set_close_pairs_finder(ClosePairsFinder *f);

  //! Get the distance threshold.
  Float get_distance() const {
    return distance_;
  }

  IMP_SCORE_STATE(CloseBipartitePairsScoreState, get_module_version_info())
};

IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_CLOSE_BIPARTITE_PAIRS_SCORE_STATE_H */
