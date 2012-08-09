/**
 *  \file IMP/container/AllPairContainer.h
 *  \brief Return all pairs from a SingletonContainer
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 */

#ifndef IMPCONTAINER_ALL_PAIR_CONTAINER_H
#define IMPCONTAINER_ALL_PAIR_CONTAINER_H

#include "container_config.h"
#include <IMP/generic.h>
#include <IMP/PairContainer.h>
#include <IMP/SingletonContainer.h>
#include <IMP/container/ListPairContainer.h>
#include <IMP/pair_macros.h>
IMPCONTAINER_BEGIN_NAMESPACE

//! Return all unordered pairs of particles taken from the SingletonContainer
/** Here is an example using this container to restrain all particles in a set
    to be within a a certain distance of one another.
    \verbinclude restrain_diameter.py

    \note Sequential access is much more efficient than random access which is
    suicidally slow for now. Complain if you want fast(er) random access.
    We might listen.

    \usesconstraint
 */
class IMPCONTAINEREXPORT AllPairContainer : public PairContainer
{
  IMP::OwnerPointer<SingletonContainer> c_;
  friend class AllBipartitePairContainer;
#define IMP_AP_LOOP(body)                       \
  ParticleIndexes pis= c_->get_indexes();       \
  for (unsigned int i=0; i< pis.size(); ++i) {  \
    for (unsigned int j=0; j< i; ++j) {         \
      ParticleIndexPair item(pis[i], pis[j]);   \
      body;                                     \
    }                                           \
  }

  IMP_IMPLEMENT_PAIR_CONTAINER_OPERATIONS(AllPairContainer,
                                               IMP_AP_LOOP);

#undef IMP_AP_LOOP

public:
  //! Get the individual particles from the passed SingletonContainer
  AllPairContainer(SingletonContainerAdaptor c,
                   std::string name="AllPairContainer%1%");

  IMP_PAIR_CONTAINER(AllPairContainer);
};

IMP_OBJECTS(AllPairContainer,AllPairContainers);

IMPCONTAINER_END_NAMESPACE

#endif  /* IMPCONTAINER_ALL_PAIR_CONTAINER_H */
