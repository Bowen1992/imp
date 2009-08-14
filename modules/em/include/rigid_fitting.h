/**
 *  \file  rigid_fitting.h
 *  \brief preforms rigid fitting between a set of particles and a density map
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 */
#ifndef IMPEM_RIGID_FITTING_H
#define IMPEM_RIGID_FITTING_H

#include "internal/version_info.h"
#include <IMP/core/MonteCarlo.h>
#include <IMP/core/ConjugateGradients.h>
#include <IMP/core/RestraintSet.h>
#include <IMP/algebra/Transformation3D.h>
#include <IMP/VersionInfo.h>
#include <IMP/Particle.h>
#include <IMP/Model.h>
#include "DensityMap.h"
#include "FitRestraint.h"
#include "config.h"
#include "internal/version_info.h"
#include <IMP/display/LogOptimizerState.h>
#include <IMP/OptimizerState.h>
#include <IMP/helper/rigid_bodies.h>
#include <IMP/ScoreState.h>
#include <algorithm>
IMPEM_BEGIN_NAMESPACE


class IMPEMEXPORT FittingSolutions {
typedef std::pair<algebra::Transformation3D,Float> FittingSolution;
struct sort_by_cc
{
  bool operator()(const FittingSolution &s1, const FittingSolution & s2) const
  {
    return s1.second < s2.second;
  }
};
public:
  int get_number_of_solutions() const {return fs.size();}
  algebra::Transformation3D get_transformation(int i) const {
    return fs[i].first;}
  Float get_score(int i) const {return fs[i].second;}
  void add_solution(const algebra::Transformation3D &t,Float score) {
    fs.push_back(FittingSolution(t,score));}
  //sort by scores
  void sort() {
    std::sort(fs.begin(),fs.end(),sort_by_cc());
  }
protected:
  std::vector<FittingSolution> fs;
};

//! Local rigid fitting of a rigid body
/**
\brief Fit a set of particles to a density map around an anchor point.
       The fitting is assessed using the cross-correaltion score.
       The optimization is a standard MC/CG procedure.
       The function returns a list of solutions sortedo the cross-correlation
       score.
\note The transformations are the rigid-body tranformation
       (with respect to an internal coordinate system). To get the actual
       delta transformation from the original placement of the rigid body,
       use the operator/ with a reference trasnforamtion outside of this
       function.
\note The returned cross-correlation score is 1-cc, as we usually want to
      minimize a scroing function. Thus a score of 1 means no-correlation
      and a score of 0. is perfect correlation.
\param[in] rb          The rigid body to fit
\param[in] rb_state    The rigid body score state
\param[in] radius_key  The raidus key of the particles in the rigid body
\param[in] weight_key  The weight key of the particles in the rigid body
\param[in] dmap        The density map to fit to
\param[in] centroid    The point to fit the particles around
\param[in] fr          To contain the fitting solutions in the end of
                       the procedure
\param[in] display_log If provided, then intermediate states
                       in during the optimization procedure are printed
\param[in] number_of_optimization_runs  number of Monte Carlo optimizations
\param[in] number_of_mc_steps  number of steps in a Monte Carlo optimization
\param[in] number_of_cg_steps  number of Conjugate Gradients steps in
                               a Monte Carlo step
\param[in] max_translation maximum translation step in a MC optimization step
\param[in] max_rotation maximum rotation step in a single MC optimization step
 */
IMPEMEXPORT void  local_rigid_fitting_around_point(
   core::RigidBody &rb, ScoreState *rb_state, const FloatKey &rad_key,
   const FloatKey &wei_key,
   DensityMap *dmap, const algebra::Vector3D &anchor_centroid,
   FittingSolutions &fr,
   OptimizerState *display_log,
   Int number_of_optimization_runs = 5, Int number_of_mc_steps = 10,
   Int number_of_cg_steps=100,
   Float max_translation=2., Float max_rotation=5.);

//! Local rigid fitting of a rigid body
/**
\brief Fit a set of particles to a density map around their centroid.
       The fitting is assessed using the cross-correaltion score.
       The optimization is a standard MC/CG procedure.
       The function returns a list of solutions sortedo the cross-correlation
       score.
\note The transformations are the rigid-body tranformation
       (with respect to an internal coordinate system). To get the actual
       delta transformation from the original placement of the rigid body,
       use the operator/ with a reference trasnforamtion outside of this
       function.
\note The returned cross-correlation score is 1-cc, as we usually want to
      minimize a scroing function. Thus a score of 1 means no-correlation
      and a score of 0. is perfect correlation.
\param[in] rb          The rigid body to fit
\param[in] rb_state    The rigid body score state
\param[in] radius_key  The raidus key of the particles in the rigid body
\param[in] weight_key  The weight key of the particles in the rigid body
\param[in] dmap        The density map to fit to
\param[in] fr          To contain the fitting solutions in the end of
                       the procedure
\param[in] display_log If provided, then intermediate states
                       in during the optimization procedure are printed
\param[in] number_of_optimization_runs  number of Monte Carlo optimizations
\param[in] number_of_mc_steps  number of steps in a Monte Carlo optimization
\param[in] number_of_cg_steps  number of Conjugate Gradients steps in
                               a Monte Carlo step
\param[in] max_translation maximum translation step in a MC optimization step
\param[in] max_rotation maximum rotation step in a single MC optimization step
 */

inline void local_rigid_fitting(
   core::RigidBody &rb, ScoreState *rb_state, const FloatKey &rad_key,
   const FloatKey &wei_key,
   DensityMap *dmap,
   FittingSolutions &fr,
   OptimizerState *display_log,
   Int number_of_optimization_runs = 5, Int number_of_mc_steps = 10,
   Int number_of_cg_steps=100,
   Float max_translation=2., Float max_rotation=5.) {
   algebra::Vector3D rb_cen=
     IMP::core::centroid(core::XYZs(rb.get_members()));
   local_rigid_fitting_around_point(
     rb, rb_state, rad_key, wei_key,dmap, rb_cen,fr,display_log,
     number_of_optimization_runs, number_of_mc_steps,
     number_of_cg_steps, max_translation, max_rotation);
}

IMPEMEXPORT void local_rigid_fitting_around_points(
   core::RigidBody &rb, ScoreState *rb_state,
   const FloatKey &rad_key, const FloatKey &wei_key,
   DensityMap *dmap, const algebra::Vector3Ds &anchor_centroid,
   FittingSolutions &fr, OptimizerState *display_log,
   Int number_of_optimization_runs = 5, Int number_of_mc_steps = 10,
   Int number_of_cg_steps=100,
   Float max_translation=2., Float max_rotation=5.);



//! Local grid search rigid fitting
/**
\brief Fit a set of particles to a density map around their centroid.
       The fitting is assessed using the cross-correaltion score.
       The optimization is a grid search
\note The transformations are not clustered.
\param[in] ps       The particles to be fitted (treated rigid)
\param[in] model    The model
\param[in] rad_key  The raidus key of the particles in the rigid body
\param[in] wei_key  The weight key of the particles in the rigid body
\param[in] dmap        The density map to fit to
\param[in] fr          To contain the fitting solutions in the end of
                       the procedure
\param[in] number_of_optimization_runs  number of Monte Carlo optimizations
\param[in] number_of_mc_steps  number of steps in a Monte Carlo optimization
\param[in] number_of_cg_steps  number of Conjugate Gradients steps in
                               a Monte Carlo step
\param[in] max_voxel_translation Sample translations within
                                 -max_voxel_translation to max_voxel_translation
\param[in] translation_step      The translation sampling step
\param[in] number_of_rotations   The number of rotations to sample
 */

IMPEMEXPORT void local_rigid_fitting_grid_search(
   Particles &ps, Model *model, const FloatKey &rad_key,
   const FloatKey &wei_key,
   DensityMap *dmap,
   FittingSolutions &fr,
   Int max_voxels_translation=2,
   Int translation_step=1, Int number_of_rotations = 200);


//! Compute fitting scores for a given set of rigid transformations
/**
\brief Score how well a set of particles fit to the map in various
       rigid transformations.
\param[in] ps       The particles to be fitted (treated rigid)
\param[in] em_map        The density map to fit to
\param[in] rad_key  The raidus key of the particles in the rigid body
\param[in] wei_key  The weight key of the particles in the rigid body
\param[in] tranformations   A set of rigid transformations
\param[in] fr   The solutions will be stored in a FittingSolutions data stucture
 */
IMPEMEXPORT void compute_fitting_scores(const Particles &ps,
   DensityMap *em_map,
   const FloatKey &rad_key, const FloatKey &wei_key,
   const std::vector<IMP::algebra::Transformation3D>& transformations,
   FittingSolutions &fr);
IMPEM_END_NAMESPACE
#endif  /* IMPEM_RIGID_FITTING_H */
