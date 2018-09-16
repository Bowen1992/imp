\brief A method for merging SAXS profiles using Gaussian processes.

saxs_merge {#saxs_merge_bin}
==========

A method for merging SAXS profiles using Gaussian processes.
Small-Angle X-ray Scattering (SAXS) is an experimental technique that allows
gathering structural information on biomolecules in solution. High-quality
SAXS profiles are usually obtained by manual merging of scattering profiles
from different concentrations and exposure times.

SAXS Merge is a fully automated statistical method for merging SAXS profiles
using Gaussian processes. This method requires only the buffer-subtracted
profile files in a specific order. At the heart of its formulation is
nonlinear interpolation using Gaussian Processes, which provide a statement
of the problem that accounts for correlation in the data.
The method is [benchmarked](https://salilab.org/saxsmerge/benchmark).

A [webserver](https://salilab.org/saxsmerge) is also available.

_Examples_:
 - [Merging of SAXS profiles](@ref saxs_merge)

# Info

_Author(s)_: Yannick Spill, Seong Joong Kim, Dina Schneidman-Duhovny

_Maintainer_: `yannickspill`

_License_: [LGPL](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html)
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

_Publications_:
 - SAXS Merge: an Automated Webserver to Statistically Merge SAXS Profiles. Submitted.
 - SAXS Merge: an Automated Statistical Method to Merge SAXS Profiles using Gaussian Processes. Accepted in the Journal of Synchrotron Radiation.
