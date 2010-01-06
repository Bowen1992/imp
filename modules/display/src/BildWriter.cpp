/**
 *  \file BildWriter.cpp
 *  \brief XXXX.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 *
 */

#include "IMP/display/BildWriter.h"


IMPDISPLAY_BEGIN_NAMESPACE

BildWriter::BildWriter(std::string name): Writer(name){
}

void BildWriter::show(std::ostream &out) const {
  out << "BildWriter" << std::endl;
}

void BildWriter::on_open() {
}

void BildWriter::on_close() {
}


IMP_REGISTER_WRITER(BildWriter, ".bild")

IMPDISPLAY_END_NAMESPACE
