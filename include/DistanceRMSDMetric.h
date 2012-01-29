/**
 *  \file DistanceRMSDMetric.h
 *  \brief Distance RMSD Metric
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 */

#ifndef IMPMEMBRANE_DISTANCE_RMSD_METRIC_H
#define IMPMEMBRANE_DISTANCE_RMSD_METRIC_H

#include "membrane_config.h"
#include <IMP/statistics.h>
#include <IMP/algebra.h>

IMPMEMBRANE_BEGIN_NAMESPACE

/** Compute the RMSD between two sets of particles in two configurations.
 */
class IMPMEMBRANEEXPORT DistanceRMSDMetric: public statistics::Metric {

  Particles ps_;
  algebra::Transformation3Ds tr_;
  std::vector<Floats> matrices_;
  std::vector< std::vector<unsigned> > matrixmap_;

  void initialize(Ints align);
  Floats get_distance_matrix(algebra::Vector3Ds coords) const;
  Float  get_distance(algebra::Vector3D v0, algebra::Vector3D v1) const;
  double get_drmsd(Floats m0, Floats m1) const;

 public:
  DistanceRMSDMetric(Particles ps, Ints align, algebra::Transformation3Ds tr);
  void add_configuration();

  IMP_METRIC(DistanceRMSDMetric);
};

 IMPMEMBRANEEXPORT statistics::PartitionalClustering* create_gromos_clustering
 (statistics::Metric *d, double cutoff);


IMPMEMBRANE_END_NAMESPACE

#endif  /* IMPMEMBRANE_DISTANCE_RMSD_METRIC_H */
