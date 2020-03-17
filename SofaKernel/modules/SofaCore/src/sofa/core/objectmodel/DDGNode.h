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
#ifndef SOFA_CORE_OBJECTMODEL_DDGNODE_H
#define SOFA_CORE_OBJECTMODEL_DDGNODE_H

#include <sofa/core/core.h>
#include <sofa/core/objectmodel/Link.h>
#include <sofa/core/objectmodel/BaseClass.h>
#include <sofa/helper/NameDecoder.h>
#include <list>

namespace sofa
{

namespace core
{

namespace objectmodel
{

using sofa::helper::NameDecoder;

class Base;
class BaseData;
class DDGNode;
class BaseObjectDescription;

template<>
class LinkTraitsPtrCasts<DDGNode>
{
public:
    static sofa::core::objectmodel::Base* getBase(sofa::core::objectmodel::DDGNode* n);
    static sofa::core::objectmodel::BaseData* getData(sofa::core::objectmodel::DDGNode* n);
};

/**
 *  \brief Abstract base to manage data dependencies. BaseData and DataEngine inherites from this class
 *
 */
class SOFA_CORE_API DDGNode
{
public:

    //typedef MultiLink<DDGNode, DDGNode, BaseLink::FLAG_DOUBLELINK|BaseLink::FLAG_DATALINK> DDGLink;
    //typedef DDGLink::Container DDGLinkContainer;
    typedef std::vector<DDGNode*> DDGLinkContainer;
    typedef DDGLinkContainer::const_iterator DDGLinkIterator;

    /// Constructor
    DDGNode();

    /// Destructor. Automatically remove remaining links
    virtual ~DDGNode();

    /// Add a new input to this node
    void addInput(DDGNode* n);

    /// Remove an input from this node
    void delInput(DDGNode* n);

    /// Add a new output to this node
    void addOutput(DDGNode* n);

    /// Remove an output from this node
    void delOutput(DDGNode* n);

    /// Get the list of inputs for this DDGNode
    const DDGLinkContainer& getInputs();

    /// Get the list of outputs for this DDGNode
    const DDGLinkContainer& getOutputs();

    /// Update this value
    virtual void update() = 0;

    /// Returns true if the DDGNode needs to be updated
    bool isDirty(const core::ExecParams*) const { return isDirty(); }
    bool isDirty() const { return dirtyFlags.dirtyValue; }

    /// Indicate the value needs to be updated
    [[deprecated("Aspects have been removed. If the feature was of interest for you, please contact sofa-framework")]]
    virtual void setDirtyValue(const core::ExecParams*) final { return setDirtyValue(); }
    virtual void setDirtyValue();

    /// Indicate the outputs needs to be updated. This method must be called after changing the value of this node.
    [[deprecated("Aspects have been removed. If the feature was of interest for you, please contact sofa-framework")]]
    virtual void setDirtyOutputs(const core::ExecParams*) final { setDirtyOutputs(); }
    virtual void setDirtyOutputs();

    /// Set dirty flag to false
    [[deprecated("Aspects have been removed. If the feature was of interest for you, please contact sofa-framework")]]
    void cleanDirty(const core::ExecParams*){ cleanDirty(); }
    void cleanDirty();

    /// Notify links that the DGNode has been modified
    [[deprecated("Aspects have been removed. If the feature was of interest for you, please contact sofa-framework")]]
    virtual void notifyEndEdit(const core::ExecParams*) final { notifyEndEdit(); }
    virtual void notifyEndEdit();

    /// Utility method to call update if necessary. This method should be called before reading of writing the value of this node.
    [[deprecated("Aspects have been removed. If the feature was of interest for you, please contact sofa-framework")]]
    void updateIfDirty(const core::ExecParams*) const { updateIfDirty(); }
    void updateIfDirty() const
    {
        if (isDirty())
        {
            const_cast <DDGNode*> (this)->update();
        }
    }

    virtual const std::string& getName() const = 0;

    virtual Base* getOwner() const = 0;

    virtual BaseData* getData() const = 0;

    virtual bool findDataLinkDest(DDGNode*& ptr, const std::string& path, const BaseLink* link);

    void addLink(BaseLink* l);

protected:

    DDGLinkContainer inputs;
    DDGLinkContainer outputs;

    virtual void doAddInput(DDGNode* n)
    {
        inputs.push_back(n);
    }

    virtual void doDelInput(DDGNode* n)
    {
        inputs.erase(std::remove(inputs.begin(), inputs.end(), n));
    }

    virtual void doAddOutput(DDGNode* n)
    {
        outputs.push_back(n);
    }

    virtual void doDelOutput(DDGNode* n)
    {
        outputs.erase(std::remove(inputs.begin(), inputs.end(), n));
    }

    /// the dirtyOutputs flags of all the inputs will be set to false
    void cleanDirtyOutputsOfInputs(const core::ExecParams*) { cleanDirtyOutputsOfInputs(); }
    void cleanDirtyOutputsOfInputs();

private:

    struct DirtyFlags
    {
        DirtyFlags() : dirtyValue(false), dirtyOutputs(false) {}

        bool dirtyValue;
        bool dirtyOutputs;
    };
    DirtyFlags dirtyFlags;
};

} // namespace objectmodel

} // namespace core

} // namespace sofa

#endif
