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
#ifndef SOFAIMPLICITFIELD_COMPONENT_SCALARFIELD_H
#define SOFAIMPLICITFIELD_COMPONENT_SCALARFIELD_H
#include <SofaImplicitField/config.h>

#include <sofa/core/objectmodel/BaseObject.h>

namespace sofa::core::objectmodel
{
class BaseObject ;
}

namespace sofa
{

namespace component
{

namespace geometry
{

using sofa::defaulttype::Vec3d ;
typedef std::function<double(double, double, double)> ScalarFieldFunctionR3R;
typedef std::function<Vec3d(double, double, double)>  GradientFieldFunctionR3R;

class ScalarFieldR3
{
public:
    ScalarFieldFunctionR3R   function {[](double,double,double)->double{return 0.0;}};
    GradientFieldFunctionR3R gradient {[](double,double,double)->Vec3d{return Vec3d();}};
};

namespace _scalarfield_
{

using sofa::core::objectmodel::BaseObject ;

////////////////// ///////////////
class SOFA_SOFAIMPLICITFIELD_API ScalarField : public BaseObject
{
public:
    SOFA_CLASS(ScalarField, BaseObject);

public:
    ScalarField();
    virtual ~ScalarField() override {}

    /// Compute the gradient using a first order finite-difference scheme.
    /// This is of lower precision compared to analytical gradient computed by derivating
    /// the equations.
    Vec3d getGradientByFinitDifference(const Vec3d& pos, int &domain) ;

    virtual int getDomain(Vec3d& pos, int domain) {
        SOFA_UNUSED(pos);
        SOFA_UNUSED(domain);
        return -1;
    }

    virtual double getValue(const Vec3d& pos, int& domain) = 0;
    inline double getValue(const Vec3d& pos) { int domain=-1; return getValue(pos,domain); }

    /// By default compute the gradient using a first order finite difference approache
    /// If you have analytical derivative don't hesitate to override this function.
    virtual Vec3d getGradient(const Vec3d& pos, int &domain);
    inline Vec3d getGradient(const Vec3d& pos) {int domain=-1; return getGradient(pos,domain); }

    /// Returns the value and the gradiant by evaluating one after an other.
    /// For some computation it is possible to implement more efficiently the computation
    /// By factoring the computing of the two...if you can do this please override this function.
    virtual void getValueAndGradient(const Vec3d& pos, double &value, Vec3d& grad, int &domain) ;
    inline void getValueAndGradient(const Vec3d& pos, double &value, Vec3d& grad)
    {
        int domain=-1;
        return getValueAndGradient(pos,value,grad,domain);
    }

    virtual bool computeSegIntersection(Vec3d& posInside, Vec3d& posOutside, Vec3d& intersecPos, int domain=-1);
    bool computeSegIntersection(Vec3d& posInside, double valInside, Vec3d& gradInside,
                                Vec3d& posOutside, double valOutside, Vec3d& gradOutside,
                                Vec3d& intersecPos, int domain=-1)
    {
        (void)valInside;
        (void)gradInside;
        (void)valOutside;
        (void)gradOutside;
        return computeSegIntersection(posInside, posOutside, intersecPos, domain);
    }

    virtual void projectPointonSurface(Vec3d& point, int i=-1);
    void projectPointonSurface(Vec3d& point, double value, Vec3d& grad, int domain=-1)
    {
        (void)value;
        (void)grad;
        projectPointonSurface(point, domain);
    }

    // TODO mettre les paramètres step=0.1 & countMax=30 en paramètre
    virtual bool projectPointonSurface2(Vec3d& point, int i, Vec3d& dir);
    bool projectPointonSurface2(Vec3d& point, int domain=-1)
    {
        Vec3d dir = Vec3d(0,0,0);
        return projectPointonSurface2(point, domain, dir);
    }

    virtual bool projectPointOutOfSurface(Vec3d& point, int i, Vec3d& dir, double &dist_out);
    bool projectPointOutOfSurface(Vec3d& point, int domain=-1)
    {
        Vec3d dir;
        double dist_out = 0.0;
        return projectPointOutOfSurface(point, domain, dir, dist_out);
    }

    /// C++ version of the field.
    Data<ScalarFieldR3> d_fieldFunction;

private:
    ScalarField(const ScalarField& n) ;
    ScalarField& operator=(const ScalarField& n) ;
};


} /// namespace _scalarfield_

using _scalarfield_::ScalarField ;

} /// namespace geometry

} /// namespace component

} /// namespace sofa

////////////////////////////////////////////////////////////////////////////////////////////////////
namespace sofa::core::objectmodel
{
/// Specialization for reading strings
template<> bool TData<sofa::component::geometry::ScalarFieldR3>::read( const std::string& str );
template<> void TData<sofa::component::geometry::ScalarFieldR3>::printValue( std::ostream& out) const;
template<> std::string TData<sofa::component::geometry::ScalarFieldR3>::getValueString() const ;
}


#endif

