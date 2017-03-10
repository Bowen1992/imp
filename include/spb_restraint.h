/**
 *  \file spb_restraint.h
 *  \brief SPB Restraints
 *
 *  Copyright 2011 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPMEMBRANE_SPB_RESTRAINT_H
#define IMPMEMBRANE_SPB_RESTRAINT_H
#include "membrane_config.h"
#include <IMP.h>
#include <IMP/atom.h>
#include <IMP/core.h>
#include <IMP/isd.h>
#include <IMP/em2d.h>

IMPMEMBRANE_BEGIN_NAMESPACE

IMPMEMBRANEEXPORT base::Pointer<core::DistancePairScore>
 get_pair_score(FloatRange dist, double kappa);

IMPMEMBRANEEXPORT base::Pointer<core::SphereDistancePairScore>
 get_sphere_pair_score(FloatRange dist, double kappa);

IMPMEMBRANEEXPORT base::Pointer<core::SphereDistancePairScore>
 get_sphere_pair_score(double dist, double kappa);

IMPMEMBRANEEXPORT void add_SPBexcluded_volume
 (Model *m,atom::Hierarchies& hs,bool GFP_exc_volume,double kappa);

IMPMEMBRANEEXPORT void add_internal_restraint(Model *m,std::string name,
atom::Molecule protein_a,atom::Molecule protein_b,double kappa,double dist);

IMPMEMBRANEEXPORT void add_my_connectivity
(Model *m,std::string name,atom::Molecule protein, double kappa);

IMPMEMBRANEEXPORT void add_restrain_protein_length(Model *m
,const atom::Hierarchy& hs,
std::string protein_a,Particle *dist,double sigma0_dist);

IMPMEMBRANEEXPORT void add_restrain_coiledcoil_to_cterm(Model *m
,const atom::Hierarchy& hs,
std::string protein_a,Particle *dist,double sigma0_dist);

IMPMEMBRANEEXPORT base::Pointer<container::MinimumPairRestraint>
do_bipartite_mindist
(Model *m,Particles p1,Particles p2,
 base::Pointer<core::SphereDistancePairScore> sps,bool filter=true);

IMPMEMBRANEEXPORT base::Pointer<container::MinimumPairRestraint>
do_bipartite_mindist
(Model *m,Particles p1,Particles p2,
 base::Pointer<core::DistancePairScore> sps,bool filter=true);

IMPMEMBRANEEXPORT void add_layer_restraint(Model *m,
container::ListSingletonContainer *lsc, FloatRange range, double kappa);

IMPMEMBRANEEXPORT void add_bayesian_layer_restraint
(Model *m, container::ListSingletonContainer *lsc, Particle *a, Particle *b);

IMPMEMBRANEEXPORT base::Pointer<isd::FretRestraint> fret_restraint
(Model *m, atom::Hierarchies& hs,
 std::string protein_a, std::string residues_a,
 std::string protein_b, std::string residues_b, double fexp,
 FretParameters Fret, std::string cell_type, bool use_GFP,
 Particle *Kda, Particle *Ida, Particle *R0, Particle *Sigma0, Particle *pBl);

IMPMEMBRANEEXPORT base::Pointer<isd::FretRestraint> fret_restraint
(Model *m, atom::Hierarchies& hs,
 std::string protein_a, std::string residues_a,
 std::string protein_b, int residues_b, double fexp,
 FretParameters Fret, std::string cell_type, bool use_GFP,
 Particle *Kda, Particle *Ida, Particle *R0, Particle *Sigma0, Particle *pBl);

IMPMEMBRANEEXPORT base::Pointer<isd::FretRestraint> fret_restraint
(Model *m, atom::Hierarchies& hs,
 std::string protein_a, int residues_a,
 std::string protein_b, std::string residues_b, double fexp,
 FretParameters Fret, std::string cell_type, bool use_GFP,
 Particle *Kda, Particle *Ida, Particle *R0, Particle *Sigma0, Particle *pBl);

IMPMEMBRANEEXPORT base::Pointer<container::MinimumPairRestraint> y2h_restraint
(Model *m,
 const atom::Hierarchy& ha,std::string protein_a,IntRange residues_a,
 atom::Hierarchies& hb,std::string protein_b,IntRange residues_b,
 double kappa);

IMPMEMBRANEEXPORT base::Pointer<container::MinimumPairRestraint> y2h_restraint
(Model *m,
 const atom::Hierarchy& ha,std::string protein_a,std::string residues_a,
 atom::Hierarchies& hb, std::string protein_b, std::string residues_b,
 double kappa);

IMPMEMBRANEEXPORT base::Pointer<container::MinimumPairRestraint> y2h_restraint
(Model *m,
 const atom::Hierarchy& ha,std::string protein_a,IntRange residues_a,
 atom::Hierarchies& hb,std::string protein_b,std::string residues_b,
 double kappa);

IMPMEMBRANEEXPORT base::Pointer<container::MinimumPairRestraint> y2h_restraint
(Model *m,
 const atom::Hierarchy& ha,std::string protein_a,std::string residues_a,
 atom::Hierarchies& hb,std::string protein_b,IntRange residues_b,
 double kappa);

IMPMEMBRANEEXPORT void add_symmetry_restraint
 (Model *m,atom::Hierarchies& hs,algebra::Transformation3Ds transformations,
  Particle *SideXY, Particle *SideZ);

IMPMEMBRANEEXPORT void add_link(Model *m,
 const atom::Hierarchy& h, std::string protein_a, std::string residues_a,
 atom::Hierarchies& hs, std::string protein_b, IntRange residues_b,
 double kappa);

IMPMEMBRANEEXPORT void add_link(Model *m,
 const atom::Hierarchy& h, std::string protein_a, std::string residues_a,
 atom::Hierarchies& hs, std::string protein_b, std::string residues_b,
 double kappa);

IMPMEMBRANEEXPORT std::vector<core::RigidBody> get_rigid_bodies(Particles ps);

IMPMEMBRANEEXPORT void add_tilt_restraint
(Model *m,Particle *p,FloatRange trange,double kappa);

IMPMEMBRANEEXPORT void add_tilt(Model *m, const atom::Hierarchy& h,
 std::string name, IntRange range, double tilt, double kappa);

IMPMEMBRANEEXPORT void add_GFP_restraint
 (Model *m, const atom::Hierarchy& h, double kappa);

IMPMEMBRANEEXPORT void add_stay_close_restraint(Model *m,
 const atom::Hierarchy& h, std::string protein, double kappa);

IMPMEMBRANEEXPORT void add_stay_close_restraint(Model *m,
 const atom::Hierarchy& h, std::string protein, int residue, double kappa);

IMPMEMBRANEEXPORT void add_stay_on_plane_restraint(Model *m,
 const atom::Hierarchy& h, std::string protein, int residue, double kappa);

IMPMEMBRANEEXPORT void add_diameter_rgyr_restraint(Model *m,
 const atom::Hierarchy& h, std::string protein,
 double diameter, double rgyr, double kappa);

IMPMEMBRANEEXPORT base::Pointer<membrane::EM2DRestraint> em2d_restraint
(Model *m, atom::Hierarchies& hs,
 std::string protein, EM2DParameters EM2D, Particle *Sigma);

IMPMEMBRANEEXPORT base::Pointer<membrane::EM2DRestraint> em2d_restraint
(Model *m, atom::Hierarchies& hs, std::string protein,
 EM2DParameters EM2D, Floats sigma_grid, Floats fmod_grid);

IMPMEMBRANE_END_NAMESPACE

#endif  /* IMPMEMBRANE_SPB_RESTRAINT_H */
