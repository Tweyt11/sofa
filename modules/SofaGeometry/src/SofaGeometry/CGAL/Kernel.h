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

#ifndef SOFAGEOMETRY_CGAL_KERNEL_H
#define SOFAGEOMETRY_CGAL_KERNEL_H


#include <CGAL/Cartesian.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/number_utils.h>


namespace SofaGeometryCGAL
{
    typedef CGAL::Exact_predicates_inexact_constructions_kernel  Kernel;
//    typedef CGAL::Simple_cartesian<double> Kernel;
    typedef typename Kernel::FT FT;

//    double to_double (const FT& x){return CGAL::to_double(x);}

}
#endif // SOFAGEOMETRY_CGAL_KERNEL_H
