/**
 *  \file spb_reload_minimize.cpp
 *  \brief SPB Minimizer
 *
 *  Copyright 2011 IMP Inventors. All rights reserved.
 *
 */
#include <IMP/core.h>
#include <IMP/atom.h>
#include <IMP/membrane.h>
#include <IMP/rmf.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <boost/algorithm/string.hpp>

using namespace IMP;
using namespace IMP::membrane;

int main(int argc, char* argv[])
{

std::string inputfile="config.ini";
int i=1;
while(i<argc){
 if(strcmp(argv[i],"-in")==0){++i; inputfile=argv[i];}
 ++i;
}

// parsing input
std::cout << "Parsing input file" << std::endl;
SPBParameters mydata=get_SPBParameters(inputfile,"0");

// create a new model
IMP_NEW(Model,m,());

// List of particles for layer restraint
IMP_NEW(container::ListSingletonContainer,bCP_ps,(m));
IMP_NEW(container::ListSingletonContainer,CP_ps,(m));
IMP_NEW(container::ListSingletonContainer,IL2_ps,(m));
// List of Movers for MC, not used here
core::Movers mvs;
//
// PROTEIN REPRESENTATION
//
std::cout << "Creating representation" << std::endl;
atom::Hierarchies all_mol=
 create_representation(m,mydata,bCP_ps,CP_ps,IL2_ps,mvs);
atom::Hierarchies hhs;
for(unsigned int i=0;i<all_mol.size();++i){
 atom::Hierarchies hs=all_mol[i].get_children();
 for(unsigned int j=0;j<hs.size();++j) {hhs.push_back(hs[j]);}
}
//
// CREATING RESTRAINTS
//
std::cout << "Creating restraints" << std::endl;
std::map< std::string, Pointer<RestraintSet> > rst_map=
 spb_assemble_restraints(m,mydata,all_mol,bCP_ps,CP_ps,IL2_ps);
//
// PREPARE OUTPUT
//
RMF::FileHandle rh_out = RMF::create_rmf_file("traj_reload_0.rmf");
for(unsigned int i=0;i<hhs.size();++i){rmf::add_hierarchy(rh_out, hhs[i]);}
// add restraints
//IMP::rmf::add_restraints(rh_out, m->get_restraints());
// adding key for score
RMF::Category my_kc= rh_out.add_category("my data");
RMF::FloatKey my_key_out0=rh_out.add_float_key(my_kc,"my score",true);
RMF::IntKey   my_key_out1=rh_out.add_int_key(my_kc,"my index",true);
RMF::FloatKey my_key_out2=rh_out.add_float_key(my_kc,"my bias",true);
//
FILE *logfile, *fretfile;
logfile  = fopen("log.reload","w");
fretfile = fopen("log.fret","w");
//
// OPTIMIZER
//
IMP_NEW(core::ConjugateGradients,cg,(m));

std::cout << "Extracting good configurations" << std::endl;
unsigned int nminimized=0;
unsigned int totframes=0;
unsigned int currentframe=0;
unsigned int iout_name=0;

// cycle on all iterations
for(int iter=0;iter<mydata.niter;++iter){
 std::vector<RMF::FileHandle> rhs;
 std::vector<RMF::Category> my_kcs;
 std::vector<RMF::FloatKey> my_keys0, my_keys2;
 std::vector<RMF::IntKey>   my_keys1;
 std::stringstream iter_str;
 iter_str << iter;
 for(int irep=0;irep<mydata.nrep;++irep){
  std::stringstream irep_str;
  irep_str << irep;
  rhs.push_back(RMF::open_rmf_file(mydata.trajfile+irep_str.str()+
                "_"+iter_str.str()+".rmf"));
  rmf::link_hierarchies(rhs[irep], hhs);
  my_kcs.push_back(rhs[irep].get_category("my data"));
  my_keys0.push_back(rhs[irep].get_float_key(my_kcs[irep],"my score",true));
  my_keys1.push_back(rhs[irep].get_int_key(my_kcs[irep],"my index",true));
  my_keys2.push_back(rhs[irep].get_float_key(my_kcs[irep],"my bias",true));
 }
// number of frames
 unsigned int nframes=rhs[0].get_number_of_frames();

//
 for(unsigned int imc=0;imc<nframes;++imc){
  for(int irep=0;irep<mydata.nrep;++irep){
// increment frame counter
   ++totframes;
 // retrieve scores and index
   double myscore = (rhs[irep].get_root_node()).get_value(my_keys0[irep],imc);
   int    myindex = (rhs[irep].get_root_node()).get_value(my_keys1[irep],imc);
   double mybias  = (rhs[irep].get_root_node()).get_value(my_keys2[irep],imc);
// replica index filtering
   bool   filter=true;
   if(mydata.replica_index>-1 && mydata.replica_index!=myindex){filter=false;}
// if good enough...
   if(myscore<mydata.cutoff && filter){
// load configuration from file
    rmf::load_frame(rhs[irep],imc);
    double myscore_min = myscore;
// do coniugate gradient
    if(mydata.cg_steps>0){
     cg->do_optimize(mydata.cg_steps);
     myscore_min = m->evaluate(false);
    }
    //myscore_min = m->evaluate(false);
    double fretr_score=0.0;
    double y2h_score=0.0;
    if(mydata.add_fret){fretr_score=rst_map["FRET_R"]->evaluate(false);}
    if(mydata.add_y2h) {y2h_score=rst_map["Y2H"]->evaluate(false);}

// get individual contribution to fret score
    Floats fretr_scores;
    bool fretisgood=true;
    for(unsigned i=0;i<rst_map["FRET_R"]->get_number_of_restraints();++i){
      double score=rst_map["FRET_R"]->get_restraint(i)->evaluate(false);
      score = sqrt(2.0*score) / mydata.Fret.sigmas[i];
      if(score>mydata.Fret.sigmamult){fretisgood=false;}
      fretr_scores.push_back(score);
    }
// filter on individual score?
    if((mydata.Fret.sigmafilter && fretisgood) || !mydata.Fret.sigmafilter){
// print main log file
     fprintf(logfile,"%10d  %12.6f %12.6f  %12.6f %12.6f  %5d  %3d %12.6f\n",
      nminimized,myscore,myscore_min,fretr_score,y2h_score,
      iout_name,myindex,mybias);
// print fret log file
     fprintf(fretfile,"%10d  ",nminimized);
     for(unsigned i=0;i<fretr_scores.size();++i){
      fprintf(fretfile,"%12.6lf ",fretr_scores[i]);
     }
     fprintf(fretfile,"\n");
// write to RMF file
     (rh_out.get_root_node()).set_value(my_key_out0,myscore_min,currentframe);
     (rh_out.get_root_node()).set_value(my_key_out1,myindex,currentframe);
     (rh_out.get_root_node()).set_value(my_key_out2,mybias,currentframe);
     rmf::save_frame(rh_out,currentframe);
     ++nminimized;
     ++currentframe;
    }
   }
// time to create a new file for output?
   if(totframes%mydata.chunk==0 && totframes<nframes*mydata.nrep){
    currentframe=0;
    std::stringstream iout;
    iout_name = totframes/mydata.chunk;
    iout << iout_name;
    rh_out = RMF::create_rmf_file("traj_reload_"+iout.str()+".rmf");
    for(unsigned int i=0;i<hhs.size();++i){rmf::add_hierarchy(rh_out, hhs[i]);}
    //IMP::rmf::add_restraints(rh_out, m->get_restraints());
    my_kc= rh_out.add_category("my data");
    my_key_out0=rh_out.add_float_key(my_kc,"my score",true);
    my_key_out1=rh_out.add_int_key(my_kc,"my index",true);
    my_key_out2=rh_out.add_float_key(my_kc,"my bias",true);
   }
  }
 }
}

std::cout << "Number of good configurations " << nminimized << std::endl;
fclose(logfile);
fclose(fretfile);
return 0;
}
