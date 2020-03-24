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

#include <string>
#include <typeinfo>
namespace sofa::helper
{

template<class T>
class IsTemplateNameOverriden
{
    typedef char YesType[1];
    typedef char NoType[2];

    template<typename C> static YesType& test( decltype (&C::CustomTemplateName) );
    template<typename C> static NoType& test(...);

public:
    enum { value = sizeof(test<T>(0)) == sizeof(YesType) };
};

template<class T>
class IsClassNameOverriden
{
    typedef char YesType[1];
    typedef char NoType[2];

    template<typename C> static YesType& test( decltype (&C::CustomClassName) );
    template<typename C> static NoType& test(...);

public:
    enum { value = sizeof(test<T>(0)) == sizeof(YesType) };
};

class NameDecoder
{
private:
    template<class T, typename std::enable_if<IsClassNameOverriden<T>::value, int>::type = 0>
    static const std::string getOverridableName(){ return T::CustomClassName(); }

    template<class T, typename std::enable_if<not IsClassNameOverriden<T>::value, int>::type = 0>
    static const std::string getOverridableName(){ return decodeClassName(typeid(T)); }

    template<class T, typename std::enable_if<IsTemplateNameOverriden<T>::value, int>::type = 0>
    static const std::string getOverridableTemplateName(){ return T::CustomTemplateName(); }

    template<class T, typename std::enable_if<not IsTemplateNameOverriden<T>::value, int>::type = 0>
    static const std::string getOverridableTemplateName(){ return decodeTemplateName(typeid(T)); }

public:
    /// Helper method to get the type name
    template<class T>
    static std::string getTypeName()
    {
        return decodeTypeName(typeid(T));
    }


    /// Helper method to get the class name
    template<class T>
    static std::string getClassName()
    {
        return getOverridableName<T>();
    }

    /// Helper method to get the namespace name
    template<class T>
    static std::string getNamespaceName()
    {
        return decodeNamespaceName(typeid(T));
    }

    /// Helper method to get the template name
    template<class T>
    static std::string getTemplateName()
    {
        return getOverridableTemplateName<T>();
    }

    /// Helper method to get the template name
    template<class T>
    static std::string getShortName()
    {
        return shortName(getClassName<T>());
    }

    static std::string shortName( const std::string& src );

    /// Helper method to decode the type name
    static std::string decodeFullName(const std::type_info& t);

    /// Helper method to decode the type name to a more readable form if possible
    static std::string decodeTypeName(const std::type_info& t);

    /// Helper method to extract the class name (removing namespaces and templates)
    static std::string decodeClassName(const std::type_info& t);

    /// Helper method to extract the namespace (removing class name and templates)
    static std::string decodeNamespaceName(const std::type_info& t);

    /// Helper method to extract the template name (removing namespaces and class name)
    static std::string decodeTemplateName(const std::type_info& t);
};

}
