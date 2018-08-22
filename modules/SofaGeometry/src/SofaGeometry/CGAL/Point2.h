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
 *     - damien.marchal@univ-lille1.fr
 *     - thomas.morzadec@inria.fr
 ****************************************************************************/

#ifndef SOFAGEOMETRY_CGAL_POINT2_H
#define SOFAGEOMETRY_CGAL_POINT2_H

#include <CGAL/Point_2.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/number_utils.h>


namespace SofaGeometryCGAL
{
    typedef CGAL::Simple_cartesian<double> Kernel;

    typedef typename Kernel::FT FT;
    typedef CGAL::Point_2< Kernel > Point2;

    double to_double (const FT& x){std::cout<<"I get by"<<CGAL::to_double(x)<<std::endl;  return CGAL::to_double(x);}
    bool geq(const Point2& p1, const Point2& p2){return CGAL::operator>=(p1, p2);}
    bool seq(const Point2& p1, const Point2& p2){return CGAL::operator<=(p1, p2);}
    bool greater(const Point2& p1, const Point2& p2){return CGAL::operator>(p1, p2);}
    bool smaller(const Point2& p1, const Point2& p2){return CGAL::operator<(p1, p2);}
}
#endif // SOFAGEOMETRY_CGAL_POINT2_H
