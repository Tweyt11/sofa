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

#include <sofa/helper/testing/BaseTest.h>

#include <sofa/defaulttype/VecTypes.h>
using sofa::defaulttype::ResizableExtVector;

#include <SofaBaseVisual/VisualModelImpl.h>
using sofa::component::visualmodel::VisualModelImpl;
using sofa::core::topology::BaseMeshTopology;
using sofa::core::objectmodel::ComponentState;
using sofa::core::objectmodel::DataFileName;
using sofa::core::objectmodel::BaseData;
using sofa::core::loader::Material;

#include <SofaSimulationGraph/SimpleApi.h>
namespace simpleapi = sofa::simpleapi;

namespace {

template <typename _DataTypes>
struct VisualModelImpl_test : public ::testing::Test
{
    typedef _DataTypes DataTypes;

    VisualModelImpl_test()
	{
		//Init
	}

};

struct StubVisualModelImpl : public VisualModelImpl {};

// Define the list of DataTypes to instanciate
using testing::Types;
typedef Types<
    sofa::defaulttype::Vec3Types
> DataTypes; // the types to instanciate.

// Test suite for all the instanciations
TYPED_TEST_CASE(VisualModelImpl_test, DataTypes);

template <class T>
bool ResizableVector_Comparison(ResizableExtVector< T > expected,
                                ResizableExtVector< T > actual)
{
    if (expected.size() != actual.size())
        return false;
    return true;
}

// Ctor test case
TEST( VisualModelImpl_test , testInput )
{
    EXPECT_MSG_NOEMIT(Error, Warning) ;

    auto simu = simpleapi::createSimulation();
    auto node = simpleapi::createRootNode(simu, "Root");
    auto visual = simpleapi::createObject(node, "VisualModel");

    visual->init();
    EXPECT_EQ( visual->getComponentState(), ComponentState::Valid );
}

// Ctor test case
TEST( VisualModelImpl_test , testInvalidFileName )
{
    EXPECT_MSG_NOEMIT(Warning);
    EXPECT_MSG_EMIT(Error);

    auto simu = simpleapi::createSimulation();
    auto node = simpleapi::createRootNode(simu, "Root");
    auto visual = simpleapi::createObject(node, "VisualModel", {{"filename","invalid.ogl"}} );

    visual->init();
    EXPECT_EQ( visual->getComponentState(), ComponentState::Valid );
}


// Ctor test case
TEST( VisualModelImpl_test , checkThatMembersAreCorrectlyConstructed )
{
    StubVisualModelImpl visualModel;

    // crazy way to remove warnings
    bool false_var = false;
    bool true_var  = true;

    ASSERT_EQ(false_var, visualModel.useTopology);
    ASSERT_EQ(-1,        visualModel.lastMeshRev);
    ASSERT_EQ(true_var,  visualModel.castShadow);
    ASSERT_EQ(false_var, visualModel.m_initRestPositions.getValue());
    ASSERT_EQ(true_var,  visualModel.getUseNormals());
    ASSERT_EQ(true_var,  visualModel.m_updateNormals.getValue());
    ASSERT_EQ(false_var, visualModel.m_computeTangents.getValue());
    ASSERT_EQ(true_var,  visualModel.m_updateTangents.getValue());
    ASSERT_EQ(true_var,  visualModel.m_handleDynamicTopology.getValue());
    ASSERT_EQ(true_var,  visualModel.m_fixMergedUVSeams.getValue());


    ASSERT_EQ(true_var, ResizableVector_Comparison(VisualModelImpl::VecCoord(), visualModel.m_vertices2.getValue()));
    ASSERT_EQ(true_var, ResizableVector_Comparison(VisualModelImpl::VecCoord(), visualModel.m_vtangents.getValue()));
    ASSERT_EQ(true_var, ResizableVector_Comparison(VisualModelImpl::VecCoord(), visualModel.m_vbitangents.getValue()));

    ASSERT_EQ(true_var, ResizableVector_Comparison(ResizableExtVector< BaseMeshTopology::Edge >(), visualModel.m_edges.getValue()));
    ASSERT_EQ(true_var, ResizableVector_Comparison(ResizableExtVector< BaseMeshTopology::Triangle >(), visualModel.m_triangles.getValue()));
    ASSERT_EQ(true_var, ResizableVector_Comparison(ResizableExtVector< BaseMeshTopology::Quad >(), visualModel.m_quads.getValue()));
    ASSERT_EQ(true_var, ResizableVector_Comparison(ResizableExtVector<int>(), visualModel.m_vertPosIdx.getValue()));
    ASSERT_EQ(true_var, ResizableVector_Comparison(ResizableExtVector<int>(), visualModel.m_vertNormIdx.getValue()));

    ASSERT_EQ(DataFileName().getValue(), visualModel.fileMesh.getValue());
    ASSERT_EQ(DataFileName().getValue(), visualModel.texturename.getValue());
    ASSERT_EQ(VisualModelImpl::Vec3Real(), visualModel.m_translation.getValue());
    ASSERT_EQ(VisualModelImpl::Vec3Real(), visualModel.m_rotation.getValue());
    ASSERT_EQ(VisualModelImpl::Vec3Real(1.0,1.0,1.0), visualModel.m_scale.getValue());
    ASSERT_EQ(VisualModelImpl::TexCoord(1.0,1.0), visualModel.m_scaleTex.getValue());
    ASSERT_EQ(VisualModelImpl::TexCoord(0.0,0.0), visualModel.m_translationTex.getValue());

    ASSERT_EQ(Material().name, visualModel.material.getValue().name);
    ASSERT_EQ(false_var, visualModel.putOnlyTexCoords.getValue());
    ASSERT_EQ(false_var, visualModel.srgbTexturing.getValue());
    ASSERT_EQ(false_var, visualModel.xformsModified);
    ASSERT_EQ(0, visualModel.m_topology);
    ASSERT_EQ(true_var, visualModel.getDataAliases().find("filename") != visualModel.getDataAliases().end());

    ASSERT_EQ("Vector", std::string(visualModel.m_vertices2.getGroup()));
    ASSERT_EQ("Vector", std::string(visualModel.m_vnormals.getGroup()));
    ASSERT_EQ("Vector", std::string(visualModel.m_vtexcoords.getGroup()));
    ASSERT_EQ("Vector", std::string(visualModel.m_vtangents.getGroup()));
    ASSERT_EQ("Vector", std::string(visualModel.m_vbitangents.getGroup()));
    ASSERT_EQ("Vector", std::string(visualModel.m_edges.getGroup()));
    ASSERT_EQ("Vector", std::string(visualModel.m_triangles.getGroup()));
    ASSERT_EQ("Vector", std::string(visualModel.m_quads.getGroup()));

    ASSERT_EQ("Transformation", std::string(visualModel.m_translation.getGroup()));
    ASSERT_EQ("Transformation", std::string(visualModel.m_rotation.getGroup()));
    ASSERT_EQ("Transformation", std::string(visualModel.m_scale.getGroup()));

    ASSERT_EQ(false_var, visualModel.m_edges.getFlag(BaseData::FLAG_AUTOLINK));

    ASSERT_EQ(1u, visualModel.xforms.size());
}

} //sofa
