/**
 *  \file Atom.cpp   \brief Simple atoms decorator.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 *
 */

#include <IMP/atom/Residue.h>
#include <IMP/atom/Atom.h>
#include <IMP/atom/Chain.h>

#include <IMP/base_types.h>
#include <IMP/log.h>

#include <sstream>
#include <vector>

IMPATOM_BEGIN_NAMESPACE

#define NAME_DEF(STR) const ResidueType STR(ResidueType::add_key(#STR))
#define NAME_DEF2(NAME, STR) const ResidueType NAME(ResidueType::add_key(STR))
#define NAME_ALIAS(OLD_NAME, NAME, STRING) const ResidueType NAME       \
  (ResidueType::add_alias(OLD_NAME, STRING))

NAME_DEF(UNK);
NAME_DEF(GLY);
NAME_DEF(ALA);
NAME_DEF(VAL);
NAME_DEF(LEU);
NAME_DEF(ILE);
NAME_DEF(SER);
NAME_DEF(THR);
NAME_DEF(CYS);
NAME_DEF(MET);
NAME_DEF(PRO);
NAME_DEF(ASP);
NAME_DEF(ASN);
NAME_DEF(GLU);
NAME_DEF(GLN);
NAME_DEF(LYS);
NAME_DEF(ARG);
NAME_DEF(HIS);
NAME_DEF(PHE);
NAME_DEF(TYR);
NAME_DEF(TRP);
NAME_DEF(ACE);
NAME_DEF(NH2);
NAME_DEF(HOH);

// RNA/DNA
NAME_DEF2(ADE, "A");
NAME_DEF2(URA, "U");
NAME_DEF2(CYT, "C");
NAME_DEF2(GUA, "G");
NAME_DEF2(THY, "T");
NAME_DEF2(DADE, "DA");
NAME_DEF2(DURA, "DU");
NAME_DEF2(DCYT, "DC");
NAME_DEF2(DGUA, "DG");
NAME_DEF2(DTHY, "DT");
// old format support
// the second parameter is a dummy name for macro only
NAME_ALIAS(ADE, ADE_A, "ADE");
NAME_ALIAS(URA, ADE_U, "URA");
NAME_ALIAS(CYT, ADE_C, "CYT");
NAME_ALIAS(GUA, ADE_G, "GUA");
NAME_ALIAS(THY, ADE_T, "THY");


void Residue::show(std::ostream &out) const
{
  out << "residue #" << get_index() << " of name "
      << get_residue_type();
}


void Residue::set_residue_type(ResidueType t)
{
  get_particle()->set_value(get_residue_type_key(), t.get_index());
}

IntKey Residue::get_index_key() {
  static IntKey k("residue_index");
  return k;
}

IntKey Residue::get_residue_type_key() {
  static IntKey k("residue_type");
  return k;
}

IntKey Residue::get_insertion_code_key() {
  static IntKey k("residue_icode");
  return k;
}

Chain get_chain(Residue rd, bool nothrow) {
  Hierarchy mhd(rd.get_particle());
  do {
    mhd= mhd.get_parent();
    if (mhd == Hierarchy()) {
      if (nothrow) return Chain();
      else {
        IMP_THROW("Residue is not the child of a chain",
                  ValueException);
      }
    }
    if (Chain::particle_is_instance(mhd)) {
      return Chain(mhd);
    }
  } while (true);
  return Chain();
}

Hierarchy get_next_residue(Residue rd) {
  // only handle simple case so far
  Hierarchy p= rd.get_parent();
  /*if (!p.get_as_chain()) {
    IMP_NOT_IMPLEMENTED("get_next_residue() only handles the simple case"
                        << " so far. Complain about it.");
                        }*/
  Chain c= p.get_as_chain();
  Hierarchy r=get_residue(c, rd.get_index()+1);
  return r;
}

namespace {
  struct RP: public std::pair<char, ResidueType> {
    RP(ResidueType rt, char c): std::pair<char, ResidueType>(c, rt) {}
  };
}

ResidueType get_residue_type(char c) {
  static const RP names[]={RP(ALA, 'A'),
                           RP(ARG, 'R'),
                           RP(ASP, 'D'),
                           RP(ASN, 'N'),
                           RP(CYS, 'C'),
                           RP(GLN, 'Q'),
                           RP(GLU, 'E'),
                           RP(GLY, 'G'),
                           RP(HIS, 'H'),
                           RP(ILE, 'I'),
                           RP(LEU, 'L'),
                           RP(LYS, 'K'),
                           RP(MET, 'M'),
                           RP(PHE, 'F'),
                           RP(PRO, 'P'),
                           RP(SER, 'S'),
                           RP(THR, 'T'),
                           RP(TYR, 'Y'),
                           RP(TRP, 'W'),
                           RP(VAL, 'V'),
                           RP(UNK, 'X')};
  static const IMP::internal::Map<char, ResidueType> map(names,
                                               names+sizeof(names)/sizeof(RP));
  if (map.find(c) == map.end()) {
    IMP_THROW("Residue name not found " << c, ValueException);
  } else {
    return map.find(c)->second;
  }
}


IMPATOM_END_NAMESPACE
