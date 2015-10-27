\brief Fitting atomic structures into a cryo-electron microscopy density map.

MultiFit is a computational method for simultaneously fitting atomic
structures of components into their assembly density map at
resolutions as low as 25Å. The component positions and orientations
are optimized with respect to a scoring function that includes the
quality-of-fit of components in the map, the protrusion of components
from the map envelope, as well as the shape complementarity between
pairs of components. The scoring function is optimized by an exact
inference optimizer DOMINO that efficiently finds the global minimum
in a discrete sampling space.

For more information please see the
[MultiFit](http://www.salilab.org/multifit/) website.

See the IMP::cnmultifit module for a similar protocol for handling symmetric
complexes.

multifit: command line tool {#multifit_bin}
===========================

Generally, this module is not used directly; instead, the
`multifit` command line tool is used. For an example, see
[Modeling of 3sfd](@ref multifit_3sfd).

Web server
==========

A [webserver](http://salilab.org/multifit/) that uses both this module
and the IMP::cnmultifit module is also available.

complex_to_anchor_graph {#complex_anchor_bin}
=======================

Build an anchor graph from a PDB file.

# Info

_Author(s)_: Keren Lasker

_Maintainer_: `benmwebb`

_License_: [GPL](http://www.gnu.org/licenses/gpl.html)
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

_Publications_:
 - See [main IMP papers list](@ref publications).
 - K. Lasker, M. Topf, A. Sali, H. J. Wolfson, ["Inferential optimization for simultaneous fitting of multiple components into a cryoEM map of their assembly", J Mol Biol 388, 2009](http://www.ncbi.nlm.nih.gov/pubmed/19233204)
