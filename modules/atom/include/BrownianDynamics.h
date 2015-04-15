/**
 *  \file IMP/atom/BrownianDynamics.h
 *  \brief Simple molecular dynamics optimizer.
 *
 *  Copyright 2007-2015 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPATOM_BROWNIAN_DYNAMICS_H
#define IMPATOM_BROWNIAN_DYNAMICS_H

#include <IMP/atom/atom_config.h>
#include "Diffusion.h"
#include "Simulator.h"
#include "atom_macros.h"
#include <IMP/Particle.h>
#include <IMP/Optimizer.h>
#include <IMP/internal/units.h>
#include <IMP/algebra/Vector3D.h>

IMPATOM_BEGIN_NAMESPACE

// for swig
class SimulationParameters;

//! Simple Brownian dynamics simulator.
/** This is an implementation of a Brownian Dynamics simulator.

    _Input particles and score_

    Each optimized particle must have x,y,z attributes
    that are optimizable. In addition, each optimized particle must be
    decorated with the Diffusion decorator. Optionally, the
    RigidBodyDiffusion decorator can be used to specify a rotational
    diffusion coefficient for core::RigidBody particles.  The
    optimizer assumes the scoring function to be energy in kcal/mol, and the xyz
    coordinates to be in angstroms and the diffusion coefficient of
    each particle be in \f$A^2/fs\f$ (or \f$Radian^2/fs\f$ for rotational
    diffusion coefficient).  Particles without optimized x,y,z
    and nonoptimized D are skipped.

    The optimizer can either automatically determine which particles
    to use from the model or be passed a SingletonContainer for the
    particles. If such a container is passed, particles added to it
    during optimization state updates are handled properly.

    _Simulation_

    At each simulation time step, each particle is translated in the
    direction of the sum of a random diffusion vector and the gradient
    of the scoring function (force field) at the particle
    coordinates. The translation is proportional to the particle
    diffusion coefficient, the time step size, and the inverse of kT.
    Note that particles masses are not considered, only their
    diffusion coefficients.

    Similarly, rigid bodies are rotated by the sum of a random torque and a
    force field torque, proportionally to the rotational diffusion
    coefficient, the time step size, and inversely proportional kT.

    If the skt (stochastic Runge Kutta) flag is true, the simulation is
    altered slightly to apply the SKT scheme.

    \see Diffusion
    \see RigidBodyDiffusion
  */
class IMPATOMEXPORT BrownianDynamics : public Simulator {
 public:
  //! Create the optimizer
  /** If sc is not null, that container will be used to find particles
      to move, otherwise the model will be searched.
      @param m model associated with bd
      @param name name of bd object
      @param wave_factor for wave step function, see Simulator object,
                         if >1.001 or so, creates a wave of time steps
                         that are larger by up to wave_factor from
                         formal maximal time step

     @note wave_factor is an advanced feature - if you're not sure, just use
                       its default, see also Simulator::simulate_wave()
  */
  BrownianDynamics(Model *m, std::string name = "BrownianDynamics%1%",
                   double wave_factor = 1.0);
  void set_maximum_move(double ms) { max_step_ = ms; }
  void set_use_stochastic_runge_kutta(bool tf) { srk_ = tf; }

  IMP_OBJECT_METHODS(BrownianDynamics);

 private:
  virtual void setup(const ParticleIndexes &ps) IMP_OVERRIDE;
  virtual double do_step(const ParticleIndexes &sc,
                         double dt) IMP_OVERRIDE;
  virtual bool get_is_simulation_particle(ParticleIndex p) const
      IMP_OVERRIDE;

 private:
  void advance_chunk(double dtfs, double ikt, const ParticleIndexes &ps,
                     unsigned int begin, unsigned int end);
  void advance_coordinates_1(ParticleIndex pi, unsigned int i,
                             double dtfs, double ikT);
  void advance_coordinates_0(ParticleIndex pi, unsigned int i,
                             double dtfs, double ikT);
  void advance_orientation_0(ParticleIndex pi, double dtfs, double ikT);

  double max_step_;
  bool srk_;
  base::Vector<algebra::Vector3D> forces_;
};

/** Repeatedly run the current model with Brownian dynamics at different time
    steps to try to find the maximum time step that can be used without
    the model exploding.
*/
IMPATOMEXPORT double get_maximum_time_step_estimate(BrownianDynamics *bd);

#ifndef IMP_DOXYGEN
IMPATOMEXPORT double get_harmonic_sigma(double D, double f);
#endif
IMPATOM_END_NAMESPACE

#endif /* IMPATOM_BROWNIAN_DYNAMICS_H */
