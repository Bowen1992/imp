/**
 *  \file spb_ISD_particles.cpp
 *  \brief SPB ISD particles
 *
 *  Copyright 2011 IMP Inventors. All rights reserved.
 *
 */
#include <IMP/core.h>
#include <IMP/isd2.h>
#include <IMP/membrane.h>
#include <string>
#include <map>

using namespace IMP;
using namespace IMP::membrane;

IMPMEMBRANE_BEGIN_NAMESPACE

std::map<std::string, Pointer<Particle> > add_ISD_particles
 (Model *m, SPBParameters mydata, core::Movers& mvs)
{

std::map<std::string, Pointer<Particle> > ISD_ps;
IMP_NEW(isd2::NuisanceRangeModifier,nrm,());

// kda particle
IMP_NEW(Particle,pKda,(m));
// initial value
Float Kda_0=(mydata.Fret.KdaMin+mydata.Fret.KdaMax)/2.;
isd2::Scale Kda=isd2::Scale::setup_particle(pKda,Kda_0);
Kda.set_lower(mydata.Fret.KdaMin);
Kda.set_upper(mydata.Fret.KdaMax);
IMP_NEW(core::SingletonConstraint,sc0,(nrm,NULL,Kda));
m->add_score_state(sc0);
Kda->set_is_optimized(Kda.get_nuisance_key(),true);
// add mover
add_NuisanceMover(Kda,mydata.MC.dKda,mvs);
// add particle to map
ISD_ps["Kda"]=pKda;

// Ida particle
IMP_NEW(Particle,pIda,(m));
isd2::Scale Ida=isd2::Scale::setup_particle(pIda,mydata.Fret.Ida);
Ida.set_lower(1.0);
Ida.set_upper(mydata.Fret.Ida+4.0*mydata.Fret.IdaErr);
IMP_NEW(core::SingletonConstraint,sc1,(nrm,NULL,Ida));
m->add_score_state(sc1);
Ida->set_is_optimized(Ida.get_nuisance_key(),true);
// add mover
add_NuisanceMover(Ida,mydata.MC.dIda,mvs);
// add Gaussian restraint on Ida
IMP_NEW(isd2::GaussianRestraint,gr,(Ida,mydata.Fret.Ida,mydata.Fret.IdaErr));
m->add_restraint(gr);
// add particle to map
ISD_ps["Ida"]=pIda;

// Sigma0 particle
IMP_NEW(Particle,pSigma0,(m));
// initial value
Float Sigma0_0=(mydata.Fret.Sigma0Min+mydata.Fret.Sigma0Max)/2.;
isd2::Scale Sigma0=isd2::Scale::setup_particle(pSigma0,Sigma0_0);
Sigma0.set_lower(mydata.Fret.Sigma0Min);
Sigma0.set_upper(mydata.Fret.Sigma0Max);
IMP_NEW(core::SingletonConstraint,sc2,(nrm,NULL,Sigma0));
m->add_score_state(sc2);
Sigma0->set_is_optimized(Sigma0.get_nuisance_key(),true);
// add mover
add_NuisanceMover(Sigma0,mydata.MC.dSigma0,mvs);
// add particle to map
ISD_ps["Sigma0"]=pSigma0;

// R0 particle
IMP_NEW(Particle,pR0,(m));
isd2::Scale R0=isd2::Scale::setup_particle(pR0,mydata.Fret.R0);
R0.set_lower(mydata.Fret.R0);
R0.set_upper(mydata.Fret.R0);
R0->set_is_optimized(R0.get_nuisance_key(),false);
// add particle to map
ISD_ps["R0"]=pR0;

// A particle
IMP_NEW(Particle,pA,(m));
// initial value
Float A_0=-mydata.CP_thicknessMin/2.;
isd2::Scale A=isd2::Scale::setup_particle(pA,A_0);
A.set_lower(-mydata.CP_thicknessMax+mydata.CP_thicknessMin/2.);
A.set_upper(-mydata.CP_thicknessMin/2.);
IMP_NEW(core::SingletonConstraint,sc3,(nrm,NULL,A));
m->add_score_state(sc3);
A->set_is_optimized(A.get_nuisance_key(),true);
// add mover
add_NuisanceMover(A,mydata.MC.dA,mvs);
// add particle to map
ISD_ps["A"]=pA;

// B particle
IMP_NEW(Particle,pB,(m));
// initial value
Float B_0=mydata.CP_thicknessMin/2.;
isd2::Scale B=isd2::Scale::setup_particle(pB,B_0);
B.set_lower(B_0);
B.set_upper(B_0);
B->set_is_optimized(B.get_nuisance_key(),false);
// add particle to map
ISD_ps["B"]=pB;

// SideXY particle
IMP_NEW(Particle,pSideXY,(m));
// initial value
isd2::Scale SideXY=isd2::Scale::setup_particle(pSideXY,1.0);
SideXY.set_lower(1.0);
SideXY.set_upper(mydata.sideMax/mydata.sideMin);
IMP_NEW(core::SingletonConstraint,sc4,(nrm,NULL,SideXY));
m->add_score_state(sc4);
SideXY->set_is_optimized(SideXY.get_nuisance_key(),true);
// add mover
add_NuisanceMover(SideXY,mydata.MC.dSide,mvs);
// add Jeffreys prior to SideXY
IMP_NEW(isd2::JeffreysRestraint,jr,(pSideXY));
m->add_restraint(jr);
// add particle to map
ISD_ps["SideXY"]=pSideXY;

// SideZ particle
IMP_NEW(Particle,pSideZ,(m));
// initial value
isd2::Scale SideZ=isd2::Scale::setup_particle(pSideZ,1.0);
SideZ.set_lower(1.0);
SideZ.set_upper(1.0);
SideZ->set_is_optimized(SideZ.get_nuisance_key(),false);
// add particle to map
ISD_ps["SideZ"]=pSideZ;

return ISD_ps;
}

IMPMEMBRANE_END_NAMESPACE
