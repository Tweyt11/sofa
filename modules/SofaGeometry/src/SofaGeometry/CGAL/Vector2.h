/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2018 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
/*****************************************************************************
 * Contributors:
 *     - thomas.morzadec@inria.fr
 ****************************************************************************/

#ifndef SOFAGEOMETRY_CGAL_VECTOR2_H
#define SOFAGEOMETRY_CGAL_VECTOR2_H

#include <CGAL/Vector_2.h>
#include <SofaGeometry/CGAL/Kernel.h>
#include <CGAL/Point_2.h>


namespace SofaGeometryCGAL
{

    typedef CGAL::Vector_2<SofaGeometryCGAL::Kernel> Vector2;

    Vector2 mul_right(const Vector2& v, const SofaGeometryCGAL::FT& s){return CGAL::operator* (v, s);}


    Vector2 mul_left(const SofaGeometryCGAL::FT& s, const Vector2& v){return CGAL::operator* (s, v);}



}
#endif // SOFAGEOMETRY_CGAL_VECTOR2_H
