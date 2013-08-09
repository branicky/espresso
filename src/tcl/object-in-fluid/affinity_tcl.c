/*
  Copyright (C) 2010,2011,2012,2013 The ESPResSo project
  Copyright (C) 2002,2003,2004,2005,2006,2007,2008,2009,2010 
    Max-Planck-Institute for Polymer Research, Theory Group
  
  This file is part of ESPResSo.
  
  ESPResSo is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  ESPResSo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/
/** \file affinity_tcl.c
 *
 *  Implementation of \ref affinity_tcl.h
 */
#include "affinity_tcl.h"
#include "../../object-in-fluid/affinity.h"

#ifdef AFFINITY

int tclprint_to_result_affinityIA(Tcl_Interp *interp, int i, int j)
{
  char buffer[TCL_DOUBLE_SPACE];
  IA_parameters *data = get_ia_param(i, j);

  Tcl_PrintDouble(interp, data->affinity_a, buffer);
  Tcl_AppendResult(interp, "affinity ", buffer, " ", (char *) NULL);
  Tcl_PrintDouble(interp, data->affinity_cut, buffer);
  Tcl_AppendResult(interp, buffer, " ", (char *) NULL);
  return TCL_OK;
}

int tclcommand_inter_parse_affinity(Tcl_Interp * interp,
				int part_type_a, int part_type_b,
				int argc, char ** argv)
{
  /* parameters needed for affinity */
  double a, cut;
  int change;

  /* get affinity interaction type */
  if (argc < 3) {
    Tcl_AppendResult(interp, "affinity potential needs 2 parameters: "
		     "<affinity_a> <affinity_cut>",
		     (char *) NULL);
    return 0;
  }

  /* copy affinity parameters */
  if ((! ARG_IS_D(1, a))     ||
      (! ARG_IS_D(2, cut)   )) {
    Tcl_AppendResult(interp, "affinity potential needs 2 parameters: "
		     "<affinity_a> <affinity_cut>",
		     (char *) NULL);
    return 0;
  }
  change = 3;
	
  
  Tcl_ResetResult(interp);
  if (affinity_set_params(part_type_a, part_type_b,
                             a, cut) == ES_ERROR) {
    Tcl_AppendResult(interp, "particle types must be non-negative", (char *) NULL);
    return 0;
  }
  return change;
}

#endif

