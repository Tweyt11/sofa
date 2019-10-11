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
#include <sofa/core/DataTracker.h>
using sofa::core::DataTrackerEngine;
#include <sofa/core/objectmodel/Link.h>
using sofa::core::objectmodel::SingleLink;

#include <sofa/core/objectmodel/BaseObject.h>

#include <sofa/helper/testing/BaseTest.h>
using sofa::helper::testing::BaseTest ;

#include <algorithm>

namespace std{
std::istream& operator>>(std::istream& i, std::vector<int>& c)
{
    return i;
}

std::ostream& operator<<(std::ostream& i, const std::vector<int>& c)
{
    return i;
}
}

namespace sofa {

enum class ComponentState
{
    Valid,
    Invalid
};

std::istream& operator>>(std::istream& i, ComponentState& c)
{
    c = ComponentState::Valid;
    return i;
}

std::ostream& operator<<(std::ostream& o, const ComponentState& c)
{
    o << "ComponentState";
    return o;
}


///  Test suite for data callbacks
struct DataComponentState_test: public BaseTest
{
    class NewBaseObject : public sofa::core::objectmodel::BaseObject
    {
    public:
        /// data attached to an object
        Data<ComponentState> d_componentState {ComponentState::Valid};
        DataTrackerEngine m_internalCB;

        void reinit()
        {
            msg_warning() << "BaseObject is updated";
            d_componentState = ComponentState::Valid;
        }

        NewBaseObject()
            : sofa::core::objectmodel::BaseObject()
            , d_componentState(initData(&d_componentState, ComponentState::Valid, "componentState", "componentState"))
        {
            m_internalCB.addOutput(&d_componentState);
            m_internalCB.setUpdateCallback((std::bind(&NewBaseObject::reinit, this)));
        }
    };

    class MeshLoader : public NewBaseObject
    {
    public:
        /// data attached to an object
        Data<std::string> d_filename;
        Data<int> d_numVertices {10};
        Data<std::vector<int>> d_vertices {};

        void reinit()
        {
            msg_warning() << "Loading a new mesh: " << d_filename.getValue() ;
            d_numVertices = d_numVertices.getValue()*2;
            std::vector<int> f;
            f.resize(d_numVertices.getValue());
            d_vertices = f;
            d_componentState = ComponentState::Valid;
        }

        MeshLoader() : NewBaseObject()
          , d_filename(initData(&d_filename, "undefined", "filename", "filename"))
          , d_numVertices(initData(&d_numVertices, 1, "numVertices", "numVertices"))
          , d_vertices(initData(&d_vertices, std::vector<int>(), "vertices", "vertices"))
        {
            m_internalCB.addInputs({&d_filename});
            m_internalCB.addOutputs({&d_numVertices, &d_vertices, &d_componentState});
            m_internalCB.setUpdateCallback((std::bind(&MeshLoader::reinit, this)));
        }
    };

    class MechanicalObject : public NewBaseObject
    {
    public:
        Data<std::vector<int>> d_positions {};

        void doInternalUpdate()
        {
            msg_warning() << "Update mechanical object" ;
        }

        MechanicalObject() : NewBaseObject()
        {
            m_internalCB.addInput(&d_positions);
            m_internalCB.addOutput(&d_componentState);
            //m_internalCB.addOutput(&d_componentState);
            m_internalCB.setUpdateCallback((std::bind(&MechanicalObject::reinit, this)));
        }

        void fakeAddForce()
        {
            if(d_componentState.getValue() != ComponentState::Valid)
                return ;

            std::vector<int> newPos;
            newPos.resize(d_positions.getValue().size());
            d_positions = newPos;
        }
    };

    class UniformMass : public NewBaseObject
    {
    public:
        SingleLink<UniformMass, MeshLoader, sofa::core::objectmodel::BaseLink::FLAG_STOREPATH> s_topology;
        Data<ComponentState> d_topologystate;

        Data<std::vector<int>> d_masses;

        void reinit()
        {
            msg_warning() << "UniformMass is updated" ;
            if(s_topology.get()==nullptr){
                /// To fetch from the context :)
                s_topology.set( getContext()->get<MeshLoader>() );
            }

            /// If this didn't worked then we quit
            if(s_topology.getValue()==nullptr){
                d_componentState = ComponentState::Invalid;
                msg_error("The component UniformMass is not yet ready !");
                return;
            }
            /// Beurk.
            d_topologystate.setParent(&(s_topology.get()->d_componentState));
            d_componentState = ComponentState::Valid;
        }

        UniformMass() : NewBaseObject()
        {
            m_internalCB.addInput(&d_topologystate);
            m_internalCB.addOutput(&d_componentState);
            m_internalCB.setUpdateCallback((std::bind(&UniformMass::reinit, this)));
        }

        void fakeAddMass()
        {
            if(d_componentState.getValue() != ComponentState::Valid)
                return ;
        }
    };
};


TEST_F(DataComponentState_test, testDataCallbackWithBind_1)
{

    /// mimicking the following scene
    /// <MeshLoader name="loader">>
    MeshLoader loader;

    /// <MechanicalObject position="@loader.position"/>
    MechanicalObject state;
    state.d_positions.setParent( loader.findData("positions") );

    /// <UniformMass topology="@loader"/>
    UniformMass mass;
    mass.s_topology.set( &loader );

    /// <FEMForceField/>
    state.fakeAddForce();
    mass.fakeAddMass();
}


}// namespace sofa
