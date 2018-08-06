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
/*
 * MeshGenerationFromPolyhedron.h
 *
 *  Created on: 27 oct. 2009
 *      Author: froy
 */

#ifndef ASYNCUPDATE_H
#define ASYNCUPDATE_H

#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/objectmodel/Link.h>
#include <CGALPlugin/components/engine/MeshGenerationFromImplicit.h>
#include <SofaBaseTopology/TetrahedronSetTopologyModifier.h>
#include <SofaBaseTopology/TetrahedronSetTopologyContainer.h>
#include <CGALPlugin/config.h>

namespace cgalplugin
{

using sofa::core::objectmodel::BaseLink ;

class AsyncUpdate : public sofa::core::objectmodel::BaseObject
{
public:
    SOFA_CLASS(AsyncUpdate, sofa::core::objectmodel::BaseObject);

    AsyncUpdate();
    virtual ~AsyncUpdate(){}
    void init();
    void reinit();
    void update();
    void draw(const sofa::core::visual::VisualParams* vparams);

    sofa::core::objectmodel::SingleLink< AsyncUpdate,
                sofa::component::engine::MeshGenerationFromImplicitShape,
                BaseLink::FLAG_STOREPATH|BaseLink::FLAG_STRONGLINK> m_source;

    sofa::core::objectmodel::SingleLink< AsyncUpdate,
                sofa::component::topology::TetrahedronSetTopologyContainer,
                BaseLink::FLAG_STOREPATH|BaseLink::FLAG_STRONGLINK> m_dest;
};

} //cgal

#endif /* CGALPLUGIN_MESHGENERATIONFROMPOLYHEDRON_H */
