\brief Support for small angle X-ray scattering (SAXS) data.

Small angle X-ray scattering (SAXS) is an experimental technique that uses
scattering of X-rays at low angles to determine structural information about
proteins in solution. This has the advantage that a crystal is not required;
however, the information is rotationally averaged.

This module provides a number of classes to handle SAXS profiles. For example,
Restraint includes the fit to a SAXS profile as an IMP restraint.

See also IMP::foxs for a command line tool to generate a SAXS profile for a
protein and fit it against an experimental profile, or IMP::saxs_merge for a
tool to combine multiple experimental profiles.

We provide a number of command line tools to handle SAXS profiles:

# cluster_profiles {#cluster_profiles_bin}
Cluster SAXS profiles.

# compute_chi {#compute_chi_bin}
Calculate chi value for SAXS profiles.

# rg {#rg_bin}
Compute radius of gyration from SAXS profiles.

# validate_profile {#validate_profile_bin}
Validate SAXS profiles.

# Info

_Author(s)_: Dina Schneidman, Seung Joong Kim, Yannick Spill

_Maintainer_: `duhovka`

_License_: [LGPL](http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html)
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

_Publications_:
 - See [main IMP papers list](@ref publications).
 - D. Schneidman-Duhovny, M. Hammel, A. Sali, ["FoXS: A Web server for Rapid Computation and Fitting of SAXS Profiles", Nucleic Acids Res 38, 2010](http://www.ncbi.nlm.nih.gov/pubmed/20507903).
 - D. Schneidman-Duhovny, M. Hammel, A. Sali, ["Macromolecular docking restrained by a small angle X-ray scattering profile", J Struct Biol 173(3), 2011](http://www.ncbi.nlm.nih.gov/pubmed/20920583).
 - D. Schneidman-Duhovny, M. Hammel, J. A. Tainer, A. Sali, ["Accurate SAXS profile computation and its assessment by contrast variation experiments", Biophys J 105(4), 2013](http://www.ncbi.nlm.nih.gov/pubmed/23972848).
