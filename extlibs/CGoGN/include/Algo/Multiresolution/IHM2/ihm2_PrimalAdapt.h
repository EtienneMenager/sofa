/*******************************************************************************
* CGoGN: Combinatorial and Geometric modeling with Generic N-dimensional Maps  *
* version 0.1                                                                  *
* Copyright (C) 2009-2012, IGG Team, LSIIT, University of Strasbourg           *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Web site: http://cgogn.unistra.fr/                                           *
* Contact information: cgogn@unistra.fr                                        *
*                                                                              *
*******************************************************************************/

#ifndef __MAP2MR_PRIMAL_ADAPT__
#define __MAP2MR_PRIMAL_ADAPT__

#include "Topology/ihmap/ihm2.h"
#include "Topology/generic/traversor/traversorCell.h"
#include "Topology/generic/traversor/traversor2.h"

#include <cmath>

namespace CGoGN
{

namespace Algo
{

namespace MR
{

namespace Primal
{

namespace Adaptive
{

template <typename PFP>
class IHM2
{

public:
	typedef typename PFP::MAP MAP ;

protected:
	MAP& m_map;
	bool shareVertexEmbeddings ;

	FunctorType* vertexVertexFunctor ;
	FunctorType* edgeVertexFunctor ;
	FunctorType* faceVertexFunctor ;

public:
	IHM2(MAP& map) ;
    ~IHM2();
    /***************************************************
     *               CELLS INFORMATION                 *
     ***************************************************/

    /**
     * Return the level of the edge of d in the current level map
     */
    inline unsigned int edgeLevel(Dart d) const;

    /**
     * Return the level of the face of d in the current level map
     */
    unsigned int faceLevel(Dart d) ;

    /**
     * Given the face of d in the current level map,
     * return a level 0 dart of its origin face
     */
    Dart faceOrigin(Dart d) ;

    /**
     * Return the oldest dart of the face of d in the current level map
     */
    Dart faceOldestDart(Dart d) ;

    /**
     * Return true if the edge of d in the current level map
     * has already been subdivided to the next level
     */
    bool edgeIsSubdivided(Dart d) ;

    /**
     * Return true if the edge of d in the current level map
     * is subdivided to the next level,
     * none of its resulting edges is in turn subdivided to the next level
     * and the middle vertex is of degree 2
     */
    bool edgeCanBeCoarsened(Dart d) ;

    /**
     * Return true if the face of d in the current level map
     * has already been subdivided to the next level
     */
    bool faceIsSubdivided(Dart d) ;

    /**
     * Return true if the face of d in the current level map
     * is subdivided to the next level
     * and none of its resulting faces is in turn subdivided to the next level
     */
    bool faceIsSubdividedOnce(Dart d) ;

protected:
	/***************************************************
	 *               SUBDIVISION                       *
	 ***************************************************/



	/**
	 * coarsen the edge of d from the next level
	 */
	void coarsenEdge(Dart d) ;


public:

    /**
     * subdivide the edge of d to the next level
     */
    void subdivideEdge(Dart d) ;
    /**
     * checkForSurrounded check if the cell wont be surrounded by subdivided cells (before subdividing phi2(d)) to avoid the case of a non subdivided cell surrounded by subdivided ones.
     */
    bool checkForSurrounded (Dart d);

    inline bool applyVertexVertexFunctor(Dart d)
    {
        return this->vertexVertexFunctor->operator ()(d);
    }

    inline bool applyEdgeVertexFunctor(Dart d)
    {
        return this->edgeVertexFunctor->operator ()(d);
    }

    inline bool applyFaceVertexFunctor(Dart d)
    {
        return this->faceVertexFunctor->operator ()(d);
    }

	/**
	 * subdivide the face of d to the next level
	 */
	unsigned int subdivideFace(Dart d, bool triQuad = true, bool OneLevelDifference = true);

	/**
	 * coarsen the face of d from the next level
	 */
	void coarsenFace(Dart d) ;

	/**
	 * vertices attributes management
	 */
    void setVertexVertexFunctor(FunctorType* f)
    {
        delete vertexVertexFunctor;
        vertexVertexFunctor = f ;
    }

    void setEdgeVertexFunctor(FunctorType* f)
    {
        delete edgeVertexFunctor;
        edgeVertexFunctor = f ;
    }

    void setFaceVertexFunctor(FunctorType* f) {
        delete faceVertexFunctor;
        faceVertexFunctor = f ;
    }
} ;


} // namespace Adaptive

} // namespace Primal

} // namespace MR

} // namespace Algo

} // namespace CGoGN

#include "Algo/Multiresolution/IHM2/ihm2_PrimalAdapt.hpp"

#endif