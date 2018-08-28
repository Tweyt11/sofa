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

#ifndef SOFAGEOMETRY_CGAL_POINT2_H
#define SOFAGEOMETRY_CGAL_POINT2_H

#include <SofaGeometry/CGAL/Kernel.h>
#include <SofaGeometry/CGAL/Vector2.h>
#include <CGAL/Point_2.h>



namespace SofaGeometryCGAL
{

    typedef CGAL::Point_2< SofaGeometryCGAL::Kernel> Point2;

    bool geq(const Point2& p1, const Point2& p2){return CGAL::operator>=(p1, p2);}
    bool seq(const Point2& p1, const Point2& p2){return CGAL::operator<=(p1, p2);}
    bool greater(const Point2& p1, const Point2& p2){return CGAL::operator>(p1, p2);}
    bool smaller(const Point2& p1, const Point2& p2){return CGAL::operator<(p1, p2);}

    SofaGeometryCGAL::Vector2 diff (const Point2& p, const Point2& q){return CGAL::operator-(p, q);}
    Point2 add(const Point2& p, const SofaGeometryCGAL::Vector2& v){return CGAL::operator+(p, v);}
}
#endif // SOFAGEOMETRY_CGAL_POINT2_H
