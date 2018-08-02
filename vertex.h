// This is vertex class

#ifndef _VERTEX_H
#define _VERTEX_H

#include "utility.h"
#include "vector3d.h"
#include "thermostat.h"
#include "edge.h"
#include "face.h"
#include "interface.h"

class EDGE;
class FACE;
class INTERFACE;

class VERTEX 
{
  public:

  // members
  unsigned int index;			// index
  double m;				// mass
  double q;                             	// charge
  VECTOR3D posvec;			// position vector
  VECTOR3D velvec;			// velocity vector
  vector<EDGE*> itsE;			// its edges
  vector<FACE*> itsF;			// its faces
  
  VECTOR3D itsnormal;
  double itsarea;
  double itsrefarea;
  
  VECTOR3D aforce;
  VECTOR3D vforce;
  VECTOR3D lforce;
  VECTOR3D forvec;
  VECTOR3D bforce;
  VECTOR3D sforce;
  VECTOR3D TForce;

  VECTOR3D neg_GradVi;      // NB modified.  Precise name for what was previously "dummy_vforce".
  VECTOR3D neg_GradAi;      // NB added.
  VECTOR3D neg_GradConi;    // NB added.  Neg constraint gradient on vertex, differs from above if quadratic.
  
  double Realke;
  double r, theta, phi;			//  Returns the polar coordinates of the vertex.
  
  void computenormal();
  void computearea();
  void compute_neg_VGrads(INTERFACE*/*, char*/); // 2017.09.16 NB added:  Compute given vertex net area & volume gradient.
  void stretching_forces(INTERFACE*);
  void tension_forces(INTERFACE*);           // 2017.09.16 NB added:  Compute the tension forces on a given vertex.

  //void constraint_forces(INTERFACE*);
  //void volume_constraint_gradient(INTERFACE*);
  
  // member functions
  
  // make a vertex
  VERTEX(VECTOR3D position = VECTOR3D(0,0,0)) : posvec(position)
  {
  }

  // get the polar coordinates for the vertex
  void get_polar()
  {
    r = posvec.GetMagnitude();
    theta = acos(posvec.z / r);
    phi = posvec.y > 0 ? acos(posvec.x / sqrt(r*r-posvec.z*posvec.z)) : 2*pi - acos(posvec.x / sqrt(r*r-posvec.z*posvec.z));
    return;
  }

  // update velocity of the vertex
  void update_real_velocity(double dt, THERMOSTAT main_bath, long double expfac)
  {
    velvec = ( ( velvec ^ (expfac)  ) + ( forvec ^ (0.5 * dt * sqrt(expfac)) ) );
    return;
  }

  // compute REAL kinetic energy
  void real_kinetic_energy()
  {
    Realke = 0.5 * m * (velvec*velvec);
    return;
  }

  // update position of the vertex
  void update_real_position(double dt)
  {
    posvec = ( posvec + (velvec ^ dt) );
    return;
  }

};

#endif