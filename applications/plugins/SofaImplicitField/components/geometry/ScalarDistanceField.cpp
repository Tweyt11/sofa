/******************************************************************************
 *       SOFA, Simulation Open-Framework Architecture, development version     *
 *                (c) 2006-2019 INRIA, USTL, UJF, CNRS, MGH                    *
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

#include "ScalarDistanceField.h"
#include <sofa/core/ObjectFactory.h>
namespace sofa::component::geometry::_scalardistancefield_
{

double ScalarDistanceField::getDistance (defaulttype::Vec3d &pos)
{
    int domain = -1;
    return doGetDistance (pos, domain);
}

double ScalarDistanceField::getDistance (defaulttype::Vec3d &pos, int &domain)
{
    return doGetDistance (pos, domain);
}

double ScalarDistanceField::doGetDistance (defaulttype::Vec3d &pos, int &domain)
{
    defaulttype::Vec3d grad;
    double             value;
    getValueAndGradient (pos, value, grad, domain);
    return getDistance (pos, value, grad.norm ());
}

double ScalarDistanceField::getDistance (defaulttype::Vec3d &pos, double value, double grad_norm)
{
    int domain = -1;
    return getDistance (pos, value, grad_norm, domain);
}

double ScalarDistanceField::getDistance (defaulttype::Vec3d &pos, double value, double grad_norm, int &domain)
{
    return doGetDistance (pos, value, grad_norm, domain);
}

double ScalarDistanceField::doGetDistance (defaulttype::Vec3d &pos, double value, double grad_norm, int &domain)
{
    SOFA_UNUSED (domain);
    SOFA_UNUSED (pos);
    /// use Taubin's distance by default
    if (grad_norm < 1e-10)
        return value < 0 ? double(std::numeric_limits<long>::min ()) :
                           double(std::numeric_limits<long>::max ());
    return value / grad_norm;
}

} // namespace sofa::component::geometry::_scalardistancefield_