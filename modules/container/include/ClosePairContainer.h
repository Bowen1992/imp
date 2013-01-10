/**
 *  \file IMP/container/ClosePairContainer.h
 *  \brief Return all pairs from a SingletonContainer
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2013 IMP Inventors. Close rights reserved.
 */

#ifndef IMPCONTAINER_CLOSE_PAIR_CONTAINER_H
#define IMPCONTAINER_CLOSE_PAIR_CONTAINER_H

#include <IMP/container/container_config.h>
#include <IMP/core/internal/CoreClosePairContainer.h>
#include <IMP/Optimizer.h>

IMPCONTAINER_BEGIN_NAMESPACE

/** \brief Return all close unordered pairs of particles taken from
    the SingletonContainer

    This maintains a list of particles whose inter-sphere distance is
    smaller than the distance parameter.

    In order to do this efficiently the class actually computes all
    pairs within distance+slack of one another. As long as the
    particles don't move more than the slack amount, the list is still
    valid and doesn't need to be recomputed. The container keeps track
    internally of how far the particles have moved using a score
    state, and is also updated via a score state.

    \usesconstraint

    Here is a simple example of using this for a nonbonded list
    \verbinclude nonbonded_interactions.py

    \note This class uses the IMP::core::BoxSweepClosePairsFinder by
    default if \ref cgal "CGAL" is available.

    \ingroup CGAL
    \see CloseBipartitePairContainer
    \see ClosePairsFinder

 */
class IMPCONTAINEREXPORT ClosePairContainer :
#if defined(IMP_DOXYGEN) || defined(SWIG)
public PairContainer
#else
public core::internal::CoreClosePairContainer
#endif
{
  typedef core::internal::CoreClosePairContainer P;
public:
  //! Get the individual particles from the passed SingletonContainer
  ClosePairContainer(SingletonContainerAdaptor c, double distance,
                     double slack=1);

  //! Get the individual particles from the passed SingletonContainer
  ClosePairContainer(SingletonContainerAdaptor c, double distance,
                     core::ClosePairsFinder *cpf,
                     double slack=1);

#if defined(SWIG) || defined(IMP_DOXYGEN)
  /** @name Methods to control the set of filters

     PairPredicate objects can be used as filters to prevent
     the addition of pairs to the container output list. Pairs
     for which the predicates evaluate to a non-zero value are
     excluded from the list.
  */
  /**@{*/
  IMP_LIST(public, PairPredicate, pair_filter,
           PairPredicate*, PairPredicates);
   /**@}*/
  void set_slack(double s);
  double get_slack() const;
  IMP_PAIR_CONTAINER(ClosePairContainer);
#else
  IMP_OBJECT(ClosePairContainer);
#endif
};

IMP_OBJECTS(ClosePairContainer,ClosePairContainers);


/** Estimate the proper slack based on
    - the time taken to evaluate the passed restraints for a given
    number of particles in the non-bonded list
    - the number of pairs in the list as a function of slack size
    - the amount the particles are moved by the optimizer
    - the time taken to compute the close pairs as a function
    of slack size

    For best results, make the particles start in a
    that is "typical" for the optimization.
*/
IMPCONTAINEREXPORT double
get_slack_estimate(const ParticlesTemp& ps,
                   double upper_bound,
                   double step,
                   const RestraintsTemp &restraints,
                   bool derivatives,
                   Optimizer *opt,
                   ClosePairContainer *cpc);


IMPCONTAINER_END_NAMESPACE

#endif  /* IMPCONTAINER_CLOSE_PAIR_CONTAINER_H */
