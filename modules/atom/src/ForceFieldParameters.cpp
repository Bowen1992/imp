/**
 * \file ForceFieldParameters \brief
 *
 * Copyright 2007-8 Sali Lab. All rights reserved.
 *
 */

#include <IMP/atom/ForceFieldParameters.h>
#include <IMP/core/XYZR.h>
#include <IMP/Particle.h>
#include <IMP/atom/CharmmParameters.h>

IMPATOM_BEGIN_NAMESPACE

Float ForceFieldParameters::get_radius(const AtomType& atom_type,
                                       const ResidueType& residue_type) const
{
  String force_field_atom_type =
    get_force_field_atom_type(atom_type, residue_type);
  return get_radius(force_field_atom_type);
}

Float ForceFieldParameters::get_epsilon(const AtomType& atom_type,
                          const ResidueType& residue_type) const
{
  String force_field_atom_type =
    get_force_field_atom_type(atom_type, residue_type);
  return get_epsilon(force_field_atom_type);
}

void ForceFieldParameters::add_radii(Hierarchy mhd, FloatKey radius_key) const
{
  Particles ps = get_by_type(mhd, Hierarchy::ATOM);
  for(unsigned int i=0; i<ps.size(); i++) {
    Float radius = get_radius(Atom(ps[i]).get_atom_type(),
                              get_residue_type(Atom(ps[i])));
    core::XYZR::create(ps[i], radius, radius_key);
  }
  // TODO: handle N-term and C-term
}

Float ForceFieldParameters::get_radius(
                           const String& force_field_atom_type) const
{
  if(force_field_atom_type.length() > 0 &&
     force_field_2_vdW_.find(force_field_atom_type) !=
     force_field_2_vdW_.end()) {
    return force_field_2_vdW_.find(force_field_atom_type)->second.second;
  }
  std::cerr << "Warning! Radius not found "
            << force_field_atom_type << std::endl;
  return 1.7; // SOME DEFAULT VALUE!!
}

Float ForceFieldParameters::get_epsilon(
                           const String& force_field_atom_type) const
{
  if(force_field_atom_type.length() > 0 &&
     force_field_2_vdW_.find(force_field_atom_type) !=
     force_field_2_vdW_.end()) {
    return force_field_2_vdW_.find(force_field_atom_type)->second.first;
  }
  std::cerr << "Warning! Epsilon not found "
            << force_field_atom_type << std::endl;
  return -0.1; // SOME DEFAULT VALUE!!
}


void ForceFieldParameters::add_bonds(Hierarchy mhd) const {
  add_bonds(mhd, Hierarchy::RESIDUE);
  add_bonds(mhd, Hierarchy::NUCLEICACID);
  add_bonds(mhd, Hierarchy::FRAGMENT);
}

void ForceFieldParameters::add_bonds(Hierarchy mhd,
                         Hierarchy::Type type) const {
  // get all residues
  Particles ps = get_by_type(mhd, type);
  Residue prev_rd;
  for(unsigned int i=0; i<ps.size(); i++) {
    Residue rd = Residue::cast(ps[i]);
    // add bonds to the current residue
    add_bonds(rd);
    // add bond between the residues (if same chain)
    if(i>0 && get_chain(prev_rd) == get_chain(rd) &&
       prev_rd.get_index() <= rd.get_index())
      add_bonds(prev_rd, rd);
    prev_rd = rd;
  }
}

void ForceFieldParameters::add_bonds(Residue rd1, Residue rd2) const {
  Atom ad1, ad2;
  // connect two residues by C-N bond
  if(rd1.get_type() == Hierarchy::RESIDUE &&
     rd2.get_type() == Hierarchy::RESIDUE) {
    ad1 = get_atom(rd1, atom::AT_C);
    ad2 = get_atom(rd2, atom::AT_N);
   }
  // connect two nucleic acids by O3'-P bond
  if(rd1.get_type() == Hierarchy::NUCLEICACID &&
     rd2.get_type() == Hierarchy::NUCLEICACID) {
    ad1 = get_atom(rd1, atom::AT_O3p);
    ad2 = get_atom(rd2, atom::AT_P);
  }
  if(!ad1 || !ad2) return;
  Particle* p1 = ad1.get_particle();
  Particle* p2 = ad2.get_particle();

  Bonded b1,b2;
  if(Bonded::is_instance_of(p1)) b1 = Bonded::cast(p1);
  else b1 = Bonded::create(p1);

  if(Bonded::is_instance_of(p2)) b2 = Bonded::cast(p2);
  else b2 = Bonded::create(p2);

  IMP::atom::Bond bd = bond(b1, b2, IMP::atom::Bond::COVALENT);
}

void ForceFieldParameters::add_bonds(Residue rd) const {
  ResidueType type = rd.get_residue_type();
  if(residue_bonds_.find(type) == residue_bonds_.end()) return;

  const std::vector<Bond>& bonds = residue_bonds_.find(type)->second;
  for(unsigned int i=0; i<bonds.size(); i++) {
    Atom ad1 = get_atom(rd, bonds[i].type1_);
    Atom ad2 = get_atom(rd, bonds[i].type2_);
    if(!ad1 || !ad2) continue;

    Particle* p1 = ad1.get_particle();
    Particle* p2 = ad2.get_particle();

    Bonded b1,b2;
    if(Bonded::is_instance_of(p1)) b1 = Bonded::cast(p1);
    else b1 = Bonded::create(p1);

    if(Bonded::is_instance_of(p2)) b2 = Bonded::cast(p2);
    else b2 = Bonded::create(p2);

    IMP::atom::Bond bd = bond(b1, b2, bonds[i].bond_type_);
  }
}

String ForceFieldParameters::get_force_field_atom_type(
                                      const AtomType& atom_type,
                                      const ResidueType& residue_type) const {
  static String empty_atom_type;
  if(atom_res_type_2_force_field_atom_type_.find(residue_type) ==
     atom_res_type_2_force_field_atom_type_.end()) {
    std::cerr << "Warning! Residue not found " << residue_type << std::endl;
    return empty_atom_type;
  }

  //  string fixedAtomName = fixAtomName(atomName, residueName, NORM);
  const AtomTypeMap& atom_map =
    atom_res_type_2_force_field_atom_type_.find(residue_type)->second;
  if(atom_map.find(atom_type) == atom_map.end()) {
    std::cerr << "Warning! Atom not found " << atom_type
              << " residue " << residue_type << std::endl;
    return empty_atom_type;
  }

  return atom_map.find(atom_type)->second.first;
}


ForceFieldParameters *default_force_field_parameters() {
  static Pointer<CharmmParameters> cfp
    (new CharmmParameters(IMP::get_data_directory() +"/atom/top.lib",
                          IMP::get_data_directory() +"/atom/par.lib"));
  return cfp;
}

IMPATOM_END_NAMESPACE
