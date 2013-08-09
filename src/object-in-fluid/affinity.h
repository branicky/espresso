/*
  Copyright (C) 2010,2012,2013 The ESPResSo project
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
#ifndef _OBJECT_IN_FLUID_AFFINITY_H
#define _OBJECT_IN_FLUID_AFFINITY_H

/** \file affinity.h
 *  Routines to calculate the affinity  force 
 *  for a particle pair.
 *  \ref forces.c
*/

#include "../utils.h"
#include "../interaction_data.h"
#include "../particle_data.h"
#include "../mol_cut.h"

#ifdef AFFINITY

int affinity_set_params(int part_type_a, int part_type_b,
			   double a, double cut);

/** Calculate soft-sphere potential force between particle p1 and p2 */
MDINLINE void add_affinity_pair_force(Particle *p1, Particle *p2, IA_parameters *ia_params,
				double d[3], double dist, double force[3])
{
  int j;
  double fac=0.0;
  if(CUTOFF_CHECK(dist < ia_params->affinity_cut)) { 
    if(dist > 0.0) {
      fac = ia_params->affinity_a*dist;
      for(j=0;j<3;j++)
	force[j] += fac * d[j];

    }
    /*
     * 
     * Here I can implement the affinity force. I have the position of the particle - p1, and under p1->p.bond_site I have the coordinate of the bond_site.
     */
      
     p1->p.bond_site[0] 
    //ONEPART_TRACE(if(p1->p.identity==check_id) fprintf(stderr,"%d: OPT: affinity   f = (%.3e,%.3e,%.3e) with part id=%d at dist %f fac %.3e\n",this_node,p1->f.f[0],p1->f.f[1],p1->f.f[2],p2->p.identity,dist,fac));
    //ONEPART_TRACE(if(p2->p.identity==check_id) fprintf(stderr,"%d: OPT: affinity   f = (%.3e,%.3e,%.3e) with part id=%d at dist %f fac %.3e\n",this_node,p2->f.f[0],p2->f.f[1],p2->f.f[2],p1->p.identity,dist,fac));
  }
}

///** calculate soft-sphere energy between particle p1 and p2. */
//MDINLINE double soft_pair_energy(Particle *p1, Particle *p2, IA_parameters *ia_params,
				//double d[3], double dist)
//{
  //double r_off;

  //if(CUTOFF_CHECK(dist < ia_params->soft_cut+ia_params->soft_offset)) {
    //r_off = dist - ia_params->soft_offset;
    ///* normal case: resulting force/energy smaller than zero. */
   
    //return soft_energy_r(ia_params->soft_a, ia_params->soft_n, r_off);
    
  //}
  //return 0.0;
//}

#endif /* ifdef AFFINITY */
#endif
