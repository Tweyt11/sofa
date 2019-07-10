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
#include <InvertibleFVM/config.h>
#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace component
{

extern "C" {
    SOFA_InvertibleFVM_API void initExternalModule();
    SOFA_InvertibleFVM_API const char* getModuleName();
    SOFA_InvertibleFVM_API const char* getModuleVersion();
    SOFA_InvertibleFVM_API const char* getModuleLicense();
    SOFA_InvertibleFVM_API const char* getModuleDescription();
    SOFA_InvertibleFVM_API const char* getModuleComponentList();
}

void initExternalModule()
{
    static bool first = true;
    if (first)
    {
        first = false;
    }
}

const char* getModuleName()
{
    return "InvertibleFVM";
}

const char* getModuleVersion()
{
    return "0";
}

const char* getModuleLicense()
{
    return "LGPL";
}


const char* getModuleDescription()
{
    return "Implementation of article \"Invertible finite elements for robust simulation of large deformation\", Irving, G., Teran, J., and Fedkiw, R., SCA 2004";
}

const char* getModuleComponentList()
{
     /// string containing the names of the classes provided by the plugin
     static std::string classes = sofa::core::ObjectFactory::getInstance()->listClassesFromTarget(sofa_tostring(SOFA_TARGET));
     return classes.c_str();
}

}

}


