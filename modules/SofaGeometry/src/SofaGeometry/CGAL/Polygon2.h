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

#ifndef SOFAGEOMETRY_CGAL_POLYGON2_H
#define SOFAGEOMETRY_CGAL_POLYGON2_H

#include <SofaGeometry/CGAL/Kernel.h>
#include <SofaGeometry/CGAL/Point2.h>
#include <CGAL/Polygon_2.h>


namespace SofaGeometryCGAL

{
    typedef CGAL::Polygon_2<SofaGeometryCGAL::Kernel> Polygon2;

    bool eq(const Polygon2& p1, const Polygon2& p2){return CGAL::operator==(p1, p2);}
    bool neq(const Polygon2& p1, const Polygon2& p2){return CGAL::operator!=(p1, p2);}


}
#endif // SOFAGEOMETRY_CGAL_POLYGON2_H
