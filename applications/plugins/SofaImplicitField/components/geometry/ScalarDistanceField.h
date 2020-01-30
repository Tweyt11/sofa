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
#pragma once
#include <SofaImplicitField/components/geometry/ScalarField.h>
#include <SofaImplicitField/config.h>
#include <sofa/core/objectmodel/BaseObject.h>

namespace sofa::component::geometry
{

namespace _scalardistancefield_
{

using sofa::core::objectmodel::BaseObject;
using sofa::defaulttype::Vec3d;

////////////////// ///////////////
class SOFA_SOFAIMPLICITFIELD_API ScalarDistanceField : public ScalarField
{
    public:
    SOFA_CLASS (ScalarDistanceField, ScalarField);

    public:
    double getDistance (Vec3d &pos);
    double getDistance (Vec3d &pos, int &domain);
    double getDistance (defaulttype::Vec3d &pos, double value, double grad_norm);
    double getDistance (defaulttype::Vec3d & /*pos*/, double value, double grad_norm, int &domain);

    protected:
    ScalarDistanceField () {}
    ~ScalarDistanceField () override {}
    double doGetDistance (Vec3d &pos, int &domain);
    double doGetDistance (defaulttype::Vec3d & /*pos*/, double value, double grad_norm, int &domain);

    private:
    ScalarDistanceField (const ScalarDistanceField &n);
    ScalarDistanceField &operator= (const ScalarDistanceField &n);
};


} // namespace _scalardistancefield_

using _scalardistancefield_::ScalarDistanceField;

} // namespace sofa::component::geometry
