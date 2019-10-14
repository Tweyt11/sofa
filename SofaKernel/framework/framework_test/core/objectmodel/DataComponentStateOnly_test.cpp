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
std::istream& operator>>(std::istream& i, std::vector<int>& c);
std::ostream& operator<<(std::ostream& i, const std::vector<int>& c);
}

namespace sofa {

enum class ComponentState
{
    Valid,
    Invalid
};

std::istream& operator>>(std::istream& i, ComponentState& c);
std::ostream& operator<<(std::ostream& o, const ComponentState& c);

template<class B>
class NewLink : public Data<ComponentState>
{
public:
    NewLink(){}

    void set(B* l)
    {
        if(l==nullptr)
        {
            setParent(nullptr);
            return;
        }
        m_target = l;
        setParent( &(m_target->d_componentState) );
    }

    B* get()
    {
        updateIfDirty();
        return m_target;
    }
    B* m_target {nullptr};
};


///  Test suite for data callbacks
struct DataComponentStateOnly_test: public BaseTest
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


    template<class B>
    class NewData : public Data<B>
    {
    public:
        NewData(){}

        void setParent(sofa::core::objectmodel::BaseData* o)
        {
            Data<B>::setParent(o);
            Data<B>::setDirtyOutputs();
        }

        const B& getValue()
        {
            if(Data<B>::getParent()!=nullptr && Data<B>::getParent()->getOwner() != nullptr )
            {
                if(Data<B>::getParent()->getOwner()->findData("componentState")->isDirty())
                {
                    if(Data<B>::getOwner()!=nullptr)
                    {
                        /// We check the component State...
                        std::cout << "GET OWNER VALUE FOR " << Data<B>::getOwner()->getName()  << ":" << Data<B>::getName() << std::endl;
                        Data<B>::getOwner()->findData("componentState")->getValueString();
                    }

                    /// We check the component State...
                    std::cout << "   - GET PARENT VALUE FOR " << Data<B>::getParent()->getOwner()->getName()  << ":" << Data<B>::getName() << std::endl;
                    Data<B>::getParent()->getOwner()->findData("componentState")->getValueString();
                }
            }
            return Data<B>::getValue();
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
            msg_warning() << "BEGIN MeshLoader";

            d_numVertices = d_numVertices.getValue()*2;
            std::vector<int> f;
            f.resize(d_numVertices.getValue());
            d_vertices = f;

            msg_warning() << "Loading a new mesh from: " << d_filename.getValue()
                          << "     number of vertices: " << d_numVertices.getValue();

            d_componentState = ComponentState::Valid;
            msg_warning() << "END MeshLoader";

        }

        MeshLoader() : NewBaseObject()
          , d_filename(initData(&d_filename, "undefined", "filename", "filename"))
          , d_numVertices(initData(&d_numVertices, 1, "numVertices", "numVertices"))
          , d_vertices(initData(&d_vertices, std::vector<int>(), "vertices", "vertices"))
        {
            m_internalCB.addInputs({&d_filename});
            m_internalCB.addOutputs({&d_componentState});
            m_internalCB.addOutputs({&d_vertices});
            m_internalCB.setUpdateCallback((std::bind(&MeshLoader::reinit, this)));
        }
    };

    class MechanicalObject : public NewBaseObject
    {
    public:
        NewData<std::vector<int>> d_positions {};

        void reinit()
        {
            msg_warning() << "BEGIN MechanicaObjectl object. ";
            msg_warning() << "        number of positions: " << d_positions.getValue().size();
            d_componentState = ComponentState::Valid;
            msg_warning() << "END MechanicaObjectl";
        }

        MechanicalObject() : NewBaseObject()

        {
            d_positions.setName("positions");
            d_positions.setOwner(this);
            m_internalCB.addInput(&d_positions);
            m_internalCB.addOutput(&d_componentState);
            m_internalCB.setUpdateCallback((std::bind(&MechanicalObject::reinit, this)));
        }

        void fakeAddForce()
        {
            if(d_componentState.getValue() != ComponentState::Valid)
                return ;

            std::vector<int> newPos;
            newPos.resize(d_positions.getValue().size());
            d_positions.setValue(newPos);
            std::cout << "fakeAddForce with number of positions: " << d_positions.getValue().size() << std::endl;
        }
    };

    class UniformMass : public NewBaseObject
    {
    public:
        NewLink<MechanicalObject> l_mstate;
        Data<std::vector<int>> d_masses;

        void reinit()
        {
            msg_warning() << "BEGIN Uniform Mass";
            /// If this didn't worked then we quit
            if(l_mstate.get()==nullptr){
                d_componentState = ComponentState::Invalid;
                msg_error() << "The component UniformMass is not yet ready because there is a missing link to the 'mstate'";
                return;
            }
            //if( d_masses.getValue().size() != l_mstate.get()->d_positions.getValue().size())
            //{

            d_masses = l_mstate.get()->d_positions.getValue();

            msg_warning() << "UniformMass is updated from: " << l_mstate.get()->getName()
                          << "                  mass size: " << d_masses.getValue().size() ;
            //}
            d_componentState = ComponentState::Valid;
            msg_warning() << "END Uniform Mass";
        }

        UniformMass() : NewBaseObject()
        {
            d_masses.setName("masses");
            d_masses.setOwner(this);
            d_componentState.setName("componentstate");
            m_internalCB.addInput(&l_mstate);
            m_internalCB.addOutput(&d_componentState);
            //m_internalCB.addOutput(&d_masses);
            m_internalCB.setUpdateCallback((std::bind(&UniformMass::reinit, this)));
        }

        void fakeAddMass()
        {
            if(d_componentState.getValue() != ComponentState::Valid)
                return ;
            std::cout << "number of masse: " << d_masses.getValue().size() << std::endl;
        }
    };
};


TEST_F(DataComponentStateOnly_test, testUpdatePolicy)
{
    /// <MechanicalObject position="@loader.position"/>
    MeshLoader loader;
    loader.setName("loader1");
    loader.d_filename.setValue("file1.obj");

    MechanicalObject state;
    state.setName("state");

    state.d_positions.setParent( &loader.d_vertices ); /// Parenting is tricky as we need to connect set the parent.

    UniformMass mass;
    mass.setName("mass");
    mass.l_mstate.set( &state );  /// Connect the mass to the mechanical object

    std::cout << "STEP 1:" << std::endl;
    ASSERT_EQ(mass.d_componentState.getValue(), ComponentState::Valid);

    std::cout << "STEP 2:" << std::endl;
    mass.d_masses.getValue();

    std::cout << "STEP 3:" << std::endl;
    loader.d_filename.setValue("file2.obj");
    ASSERT_EQ(mass.d_componentState.getValue(), ComponentState::Valid);


    std::cout << "STEP 4:" << std::endl;
    state.fakeAddForce();
    mass.fakeAddMass();

    std::cout << "STEP 5:" << std::endl;
    state.fakeAddForce();
    mass.fakeAddMass();

    std::cout << "STEP 6:" << std::endl;
    state.fakeAddForce();
    mass.fakeAddMass();

}



}// namespace sofa
