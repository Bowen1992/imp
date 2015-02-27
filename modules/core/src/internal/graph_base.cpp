/**
 *  \file graph_base.cpp   \brief classes for implementing a graph.
 *
 *  Copyright 2007-2015 IMP Inventors. All rights reserved.
 *
 */

#include <IMP/core/internal/graph_base.h>

#include <sstream>

IMPCORE_BEGIN_INTERNAL_NAMESPACE

void graph_initialize_node(kernel::Particle *p, const GraphData &d) {
  p->add_attribute(d.setup_key_, 1);
}

/** \internal */
bool graph_is_node(kernel::Particle *p, const GraphData &d) {
  return p->has_attribute(d.setup_key_);
}

kernel::Particle *graph_connect(kernel::Particle *a, kernel::Particle *b,
                        GraphData &d) {
  kernel::Model *m = a->get_model();
  kernel::Particle *p = new kernel::Particle(m);
  p->add_attribute(d.node_keys_[0], a);
  p->add_attribute(d.node_keys_[1], b);
  for (int i = 0; i < 2; ++i) {
    kernel::Particle *cp = ((i == 0) ? a : b);
    if (m->get_has_attribute(d.edges_key_, cp->get_index())) {
      kernel::ParticleIndexes c =
          m->get_attribute(d.edges_key_, cp->get_index());
      c.push_back(p->get_index());
      m->set_attribute(d.edges_key_, cp->get_index(), c);
    } else {
      m->add_attribute(d.edges_key_, cp->get_index(),
                       kernel::ParticleIndexes(1, p->get_index()));
    }
  }

  return p;
}

void graph_disconnect(kernel::Particle *e, const GraphData &d) {
  kernel::Particle *p[2];
  p[0] = graph_get_node(e, 0, d);
  p[1] = graph_get_node(e, 1, d);
  for (int i = 0; i < 2; ++i) {
    kernel::ParticleIndexes pis =
        e->get_model()->get_attribute(d.edges_key_, p[i]->get_index());
    pis.erase(std::find(pis.begin(), pis.end(), e->get_index()));
    if (!pis.empty()) {
      e->get_model()->set_attribute(d.edges_key_, p[i]->get_index(), pis);
    } else {
      e->get_model()->remove_attribute(d.edges_key_, p[i]->get_index());
    }
  }
  e->get_model()->remove_particle(e);
}

kernel::Particle *graph_get_edge(kernel::Particle *a, int i,
                                 const GraphData &d) {
  IMP_USAGE_CHECK(
      a->get_model()->get_has_attribute(d.edges_key_, a->get_index()),
      "Particle " << a->get_name() << " does not have "
                  << "enough edges");
  kernel::ParticleIndexes all =
      a->get_model()->get_attribute(d.edges_key_, a->get_index());
  IMP_USAGE_CHECK(all.size() > static_cast<unsigned int>(i),
                  "Particle " << a->get_name()
                              << " does not have enough edges");
  return a->get_model()->get_particle(all[i]);
}

kernel::ParticleIndexes graph_get_edges(kernel::Particle *a,
                                        const GraphData &d) {
  if (!a->get_model()->get_has_attribute(d.edges_key_, a->get_index())) {
    return kernel::ParticleIndexes();
  }
  kernel::ParticleIndexes all =
      a->get_model()->get_attribute(d.edges_key_, a->get_index());
  return all;
}

kernel::Particle *graph_get_neighbor(kernel::Particle *a, int i,
                                     const GraphData &d) {
  kernel::Particle *edge = graph_get_edge(a, i, d);
  if (graph_get_node(edge, 0, d) == a) {
    return graph_get_node(edge, 1, d);
  } else {
    IMP_INTERNAL_CHECK(graph_get_node(edge, 1, d) == a, "Broken graph");
    return graph_get_node(edge, 0, d);
  }
}

unsigned int graph_get_number_of_edges(kernel::Particle *a,
                                       const GraphData &d) {
  if (!a->get_model()->get_has_attribute(d.edges_key_, a->get_index()))
    return 0;
  else
    return a->get_model()->get_attribute(d.edges_key_, a->get_index()).size();
}

void graph_initialize_edge(kernel::Particle *a, const GraphData &d) {
  a->add_attribute(d.node_keys_[0], nullptr);
  a->add_attribute(d.node_keys_[1], nullptr);
}

IMPCORE_END_INTERNAL_NAMESPACE
