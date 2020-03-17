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
 * Contributors:
 *     - damien.marchal@univ-lille.fr
 *****************************************************************************/

#include <sofa/core/objectmodel/Base.h>
using sofa::core::objectmodel::Base ;
using sofa::core::objectmodel::ComponentState;

#include <SofaSimulationGraph/testing/BaseSimulationTest.h>
using sofa::helper::testing::BaseSimulationTest ;
using sofa::simulation::Node ;

#include <sofa/core/objectmodel/BaseObject.h>
using sofa::core::objectmodel::BaseObject;

using sofa::defaulttype::Rigid3Types;
using sofa::defaulttype::Vec3Types;

namespace customns
{
class CustomBaseObject : public BaseObject
{
public:
    SOFA_CLASS(CustomBaseObject, BaseObject);
};

template<class D>
class CustomBaseObjectT : public BaseObject
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(CustomBaseObjectT, D), BaseObject);

    std::string getClassName() const override
    {
        return "MyFakeClassName";
    }


};
}

using customns::CustomBaseObject;
using customns::CustomBaseObjectT;


class Base_test: public BaseSimulationTest
{
public:
    virtual ~Base_test(){}
    void testComponentState()
    {
        EXPECT_MSG_NOEMIT(Error, Warning) ;
        importPlugin("SofaComponentAll") ;
        std::stringstream scene ;
        scene << "<?xml version='1.0'?>"
                 "<Node name='Root' gravity='0 -9.81 0' time='0' animate='0' >               \n"
                 "   <Node name='child1'>                                                    \n"
                 "      <MechanicalObject />                                                 \n"
                 "      <Node name='child2'>                                                 \n"
                 "      </Node>                                                              \n"
                 "   </Node>                                                                 \n"
                 "</Node>                                                                    \n" ;

        SceneInstance c("xml", scene.str()) ;
        c.initScene() ;

        Node* root = c.root.get() ;
        ASSERT_NE(root, nullptr) ;

        ASSERT_NE(root->findData("componentState"), nullptr);
        root->m_componentstate = ComponentState::Valid;
        ASSERT_EQ(root->m_componentstate, ComponentState::Valid);
        root->m_componentstate = ComponentState::Loading;
        ASSERT_EQ(root->m_componentstate, ComponentState::Loading);
    }
};

TEST_F(Base_test , testComponentState )
{
    this->testComponentState();
}

TEST_F(Base_test , testBaseClass)
{
    EXPECT_EQ(CustomBaseObject::GetClass()->className, "CustomBaseObject");
    EXPECT_EQ(CustomBaseObject::GetClass()->templateName, "");
    EXPECT_EQ(CustomBaseObject::GetClass()->shortName, "customBaseObject");

    EXPECT_EQ(CustomBaseObjectT<Rigid3Types>::GetClass()->className, "CustomBaseObjectT");
    EXPECT_EQ(CustomBaseObjectT<Rigid3Types>::GetClass()->templateName, "StdRigidTypes<3, double> >");
    EXPECT_EQ(CustomBaseObjectT<Rigid3Types>::GetClass()->shortName, "customBaseObjectT");

    EXPECT_EQ(CustomBaseObjectT<Vec3Types>::GetClass()->className, "CustomBaseObjectT");
    EXPECT_EQ(CustomBaseObjectT<Vec3Types>::GetClass()->templateName, "StdVectorTypes<Vec<3, double>, Vec<3, double>, double> >");
    EXPECT_EQ(CustomBaseObjectT<Vec3Types>::GetClass()->shortName, "customBaseObjectT");
}

static bool beforeRefactoring = true;

TEST_F(Base_test , testGetClassName)
{
    CustomBaseObject o;
    EXPECT_EQ(o.getClassName(), "CustomBaseObject");
    EXPECT_EQ(o.getTemplateName(), "");
    EXPECT_EQ(o.getTypeName(), "CustomBaseObject");

    CustomBaseObjectT<Rigid3Types> ot;
    EXPECT_EQ(ot.getClassName(), "CustomBaseObjectT");
    EXPECT_EQ(ot.getTypeName(), "CustomBaseObjectT<StdRigidTypes<3, double> >>");
    EXPECT_EQ(ot.getTemplateName(), "StdRigidTypes<3, double> >");

    EXPECT_EQ(Base::className<CustomBaseObjectT<Rigid3Types>>(), "CustomBaseObjectT");
    EXPECT_EQ(Base::typeName<CustomBaseObjectT<Rigid3Types>>(), "CustomBaseObjectTStdRigidTypes<3, double> >");
    //EXPECT_EQ(Base::typeName<CustomBaseObjectT<Rigid3Types>>(), "CustomBaseObjectT<StdRigidTypes<3, double> >");
    EXPECT_EQ(Base::namespaceName<CustomBaseObjectT<Rigid3Types>>(), "customns");
}

TEST_F(Base_test , testClassNameOverrideBeforeRefactoring)
{
    CustomBaseObjectT<Vec3Types> o;
    EXPECT_EQ(o.getClassName(), "MyFakeClassName");
    EXPECT_EQ(Base::className<decltype(o)>(), "MyFakeName");
}

