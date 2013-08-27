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
			   double kappa, double r0, double Kon, double Koff, double cut);

/** Calculate soft-sphere potential force between particle p1 and p2 */
MDINLINE void add_affinity_pair_force(Particle *p1, Particle *p2, IA_parameters *ia_params,
				double d[3], double dist, double force[3])
{
/************************
*  
* Here I can implement the affinity force. 
* I have the position of the particle - p1, and under p1->p.bond_site I have the coordinate of the bond_site. 
* Also, under d[3] I have the vector towards the constraint meaning that force on p1 should be in the direction of d[3].
*
* Algorithm: 
* 1. First check is whether I am in the cut-off radius: ?dist < affinity_cut?.
* 2. Then I check whether there exist a bond from the current particle: ?bond_site != -1? 
* 3. If yes, then I maintaind the bond. I put the forces and afterwards I decide whether the bond will brake or not.
* 4. If no, I maintain the creation of a bond. First I check whether I am in the area of possible bond creation: ?dist < affinity_r0?
* 5. If yes, I run the decision algorithm for bond creation and I either create or does not create the bond.
* 6. If I am not in the area of possible bond creation I do nothing
*  
*********************/
  int j;
  double fac=0.0;
  if(CUTOFF_CHECK(dist < ia_params->affinity_cut)) { // Checking whether I am inside the interaction cut-off radius.
    if(dist > 0.0) {
		//printf("bond_site: %f %f %f\n",p1->p.bond_site[0],p1->p.bond_site[1],p1->p.bond_site[2]);
		if ((p1->p.bond_site[0] >= 0) && (p1->p.bond_site[1] >= 0) && (p1->p.bond_site[2] >= 0)) // Checking whether any bond exists
		{ // Bond exists
			double folded_pos[3], vec[3], len2, len;
			int img[3];

			/* fold the coordinates of the particle */
			memcpy(folded_pos, p1->r.p, 3*sizeof(double));
			memcpy(img, p1->l.i, 3*sizeof(int));
			fold_position(folded_pos, img);
			//printf("folded positions: %f %f %f\n",folded_pos[0],folded_pos[1],folded_pos[2]);
			for(j=0;j<3;j++)
				vec[j] = p1->p.bond_site[j] - folded_pos[j]; // Shouldn't be the vec vector normalized? Probably yes, so:
			len2 = sqrlen(vec);
			len = sqrt(len2);
			fac = ia_params->affinity_kappa;
			for(j=0;j<3;j++)
				force[j] += fac * vec[j]/len;
			// Decision whether I should break the bond:
			// The random probability mechanism should be implemented, for now I just decide that bond breaks whenever it is longer than 2*r0.
			if (dist > 2*ia_params->affinity_r0) for(j=0;j<3;j++) p1->p.bond_site[j] = -1;
		}
		else if (dist < ia_params->affinity_r0)
		{ // Bond does not exist, we are inside of possible bond creation area, lets talk about creating a bond
			// The random probability mechanism should be implmeented, for now I just creat the bond
			double folded_pos[3];
			int img[3];
			/* fold the coordinates of the particle */
			memcpy(folded_pos, p1->r.p, 3*sizeof(double));
			memcpy(img, p1->l.i, 3*sizeof(int));
			fold_position(folded_pos, img);
			//printf("folded positions: %f %f %f\n",folded_pos[0],folded_pos[1],folded_pos[2]);
			//printf("d: %f %f %f\n",d[0],d[1],d[2]);
			for(j=0;j<3;j++)
				p1->p.bond_site[j] = folded_pos[j] - d[j];
		}
	}
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
