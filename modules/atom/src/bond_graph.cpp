/**
 *  \file bond_graph.cpp     \brief Decorators for a bond
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 *
 */

#include <IMP/atom/bond_graph.h>
#include <boost/graph/graph_concepts.hpp>
#include <IMP/SingletonContainer.h>

#ifdef _MSC_VER
#pragma warning( disable : 4675)
#endif

#include <boost/graph/isomorphism.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_utility.hpp>
#if BOOST_VERSION >= 104000
#include <boost/property_map/property_map.hpp>
#else
#include <boost/property_map.hpp>
#endif

IMPATOM_BEGIN_NAMESPACE
BondGraph::BondGraph(Hierarchy bd):
  sc_(new container::ListSingletonContainer(get_leaves(bd))) {
   for (container::ListSingletonContainer::ParticleIterator
         it= sc_->particles_begin();
       it != sc_->particles_end(); ++it) {
    if (!Bonded::particle_is_instance(*it)) {
      Bonded::setup_particle(*it);
    }
  }
}

BondGraph::VertexIntPropertyMap BondGraph::get_vertex_index_map() const {
  if (index_key_== IntKey()) {
    std::ostringstream oss;
    oss << this << " bond graph index";
    index_key_= IntKey(oss.str().c_str());
    int last=0;
    container::ListSingletonContainer *sc
      = const_cast<container::ListSingletonContainer*>(sc_.get());
    for (container::ListSingletonContainer::ParticleIterator
           it= sc->particles_begin();
         it != sc->particles_end(); ++it) {
      (*it)->add_attribute(index_key_, last);
      ++last;
    }
  }
  return VertexIntPropertyMap(index_key_);
}


BondGraph::~BondGraph() {
  if (sc_ && index_key_ != IntKey()) {
    for (container::ListSingletonContainer::ParticleIterator
           it= sc_->particles_begin();
         it != sc_->particles_end(); ++it) {
      (*it)->remove_attribute(index_key_);
    }
  }
}


void bgl_concept_checks() {

  boost::function_requires<boost::VertexListGraphConcept<BondGraph> >();
  boost::function_requires<boost::AdjacencyGraphConcept<BondGraph> >();
  boost::function_requires<boost::EdgeListGraphConcept<BondGraph> >();
  boost::function_requires<boost::IncidenceGraphConcept<BondGraph> >();
  boost::function_requires<boost::BidirectionalGraphConcept<BondGraph> >();

  BondGraph a,b;
  IntKey index("crazy temp index");
  ParticleKey pk("isomaping");
  boost::isomorphism(a,b,
         boost::isomorphism_map(BondGraph::VertexVertexPropertyMap(pk))
                     .vertex_index1_map(a.get_vertex_index_map())
                     .vertex_index2_map(b.get_vertex_index_map()));

  boost::dijkstra_shortest_paths(a, Bonded(),
        boost::predecessor_map(BondGraph::VertexVertexPropertyMap(pk))
     .weight_map(BondGraph::EdgeFloatPropertyMap(FloatKey("bond length")))
     .distance_map(BondGraph::VertexFloatPropertyMap(FloatKey("hi")))
     .vertex_index_map(a.get_vertex_index_map()));
}

IMPATOM_END_NAMESPACE
/*,
    boost::vertex_index1_map=make_attribute_property_map(index)),
    boost::vertex_index2_map=make_attribute_property_map(index))*/
