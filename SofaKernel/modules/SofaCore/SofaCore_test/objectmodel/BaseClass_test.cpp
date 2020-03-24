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
/******************************************************************************
 * Contributors:                                                              *
 *    - apalliyali1@hamad.qa                                                  *
 *****************************************************************************/
#include <sofa/core/objectmodel/BaseObject.h>
using sofa::core::objectmodel::BaseObject ;



#include <sofa/core/ObjectFactory.h>

#include <sofa/helper/testing/BaseTest.h>
using sofa::helper::testing::BaseTest ;

namespace sofa{
namespace another_namespace{

class EmptyObject : public BaseObject
{
public:
    SOFA_CLASS(EmptyObject, BaseObject) ;
};

}
}

namespace sofa{
namespace numbered_namespace_123{

class NumberedClass123 : public BaseObject
{
public:
    SOFA_CLASS(NumberedClass123, BaseObject) ;
};

class NumberedClass456 : public another_namespace::EmptyObject
{
public:
    SOFA_CLASS(NumberedClass456, another_namespace::EmptyObject) ;
};

class CustomName123 : public BaseObject
{
public:
    SOFA_CLASS(CustomName123, BaseObject) ;

    static const std::string CustomClassName(){ return "ClassWithACustomName"; }

    template<class T>
    static const std::string className(){ return "TEST TEST"; }
};

}
}

class BaseClass_test: public BaseTest
{
public:
    sofa::another_namespace::EmptyObject m_ptr1;
    sofa::numbered_namespace_123::NumberedClass123 m_ptr2;
    sofa::numbered_namespace_123::NumberedClass456 m_ptr3;
    sofa::numbered_namespace_123::CustomName123 m_ptr4;

    sofa::core::objectmodel::Base* m_baseptr1 {&m_ptr1};
    sofa::core::objectmodel::Base* m_baseptr2 {&m_ptr2};
    sofa::core::objectmodel::Base* m_baseptr3 {&m_ptr3};
    sofa::core::objectmodel::Base* m_baseptr4 {&m_ptr4};
};

///
/// tests that all the BaseClass returned from GetClass function are refering to the same
/// BaseClass instance.
///
TEST_F(BaseClass_test, checkClassEquivalence  )
{
    EXPECT_EQ(sofa::another_namespace::EmptyObject::GetClass(), m_ptr1.getClass());
    EXPECT_EQ(sofa::another_namespace::EmptyObject::GetClass(), m_baseptr1->getClass());

    EXPECT_EQ(sofa::numbered_namespace_123::NumberedClass123::GetClass(), m_ptr2.getClass());
    EXPECT_EQ(sofa::numbered_namespace_123::NumberedClass123::GetClass(), m_baseptr2->getClass());

    EXPECT_EQ(sofa::numbered_namespace_123::NumberedClass456::GetClass(), m_ptr3.getClass());
    EXPECT_EQ(sofa::numbered_namespace_123::NumberedClass456::GetClass(), m_baseptr3->getClass());
}

TEST_F(BaseClass_test, checkStaticClassName  )
{
    ASSERT_EQ(BaseObject::className(&m_ptr1),"EmptyObject") ;
    ASSERT_EQ(BaseObject::className(&m_ptr2),"NumberedClass123") ;
    ASSERT_EQ(BaseObject::className(&m_ptr3),"NumberedClass456") ;

    ASSERT_EQ(BaseObject::className<sofa::another_namespace::EmptyObject>(),"EmptyObject") ;
    ASSERT_EQ(BaseObject::className<sofa::numbered_namespace_123::NumberedClass123>(),"NumberedClass123") ;
    ASSERT_EQ(BaseObject::className<sofa::numbered_namespace_123::NumberedClass456>(),"NumberedClass456") ;
}

TEST_F(BaseClass_test, checkStaticCustomClassName  )
{
    ASSERT_EQ(BaseObject::className(&m_ptr4),"ClassWithACustomName") ;
    ASSERT_EQ(BaseObject::className<sofa::numbered_namespace_123::CustomName123>(),"ClassWithACustomName") ;
}

TEST_F(BaseClass_test, checkDynamicClassName  )
{
    EXPECT_EQ(m_ptr1.getClassName(),"EmptyObject") ;
    EXPECT_EQ(m_ptr2.getClassName(),"NumberedClass123") ;
    EXPECT_EQ(m_ptr3.getClassName(),"NumberedClass456") ;
}

TEST_F(BaseClass_test, checkDynamicCustomName  )
{
    EXPECT_EQ(m_ptr4.getClassName(),"ClassWithACustomName") ;
}

TEST_F(BaseClass_test, checkDynamicClassNameOnBase  )
{
    ASSERT_EQ(m_baseptr1->getClassName(),"EmptyObject") ;
    ASSERT_EQ(m_baseptr2->getClassName(),"NumberedClass123") ;
    ASSERT_EQ(m_baseptr3->getClassName(),"NumberedClass456") ;
}

TEST_F(BaseClass_test, checkDynamicClassCustomNameOnBase  )
{
    ASSERT_EQ(m_baseptr4->getClassName(),"ClassWithACustomName") ;
}

TEST_F(BaseClass_test, checkNameSpace  )
{
    ASSERT_EQ(BaseObject::namespaceName(&m_ptr1),"sofa::another_namespace") ;
    ASSERT_EQ(BaseObject::namespaceName(&m_ptr2),"sofa::numbered_namespace_123") ;
    ASSERT_EQ(BaseObject::namespaceName(&m_ptr2),"sofa::numbered_namespace_123") ;
}
