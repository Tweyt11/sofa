/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2017 INRIA, USTL, UJF, CNRS, MGH                    *
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
#ifndef SOFA_CORE_BEHAVIOR_BASEMASS_FWD_H
#define SOFA_CORE_BEHAVIOR_BASEMASS_FWD_H

#include <sofa/core/objectmodel/BaseData_fwd.h>

namespace sofa
{

namespace core
{

namespace behavior
{

class BaseMass ;

} /// namespace behavior

inline sofa::core::objectmodel::Base* AsBase(sofa::core::behavior::BaseMass* o) { return reinterpret_cast<sofa::core::objectmodel::Base*>(o) ;  }
inline sofa::core::objectmodel::BaseData* AsData(sofa::core::behavior::BaseMass*) { return nullptr; }
inline const sofa::core::objectmodel::BaseClass* GetClass(sofa::core::behavior::BaseMass* n) ;


} /// namespace core
} /// namespace sofa

#endif
