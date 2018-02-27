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



// File automatically generated by "generateTypedef"


#ifndef SOFA_TYPEDEF_TopologyObject_double_H
#define SOFA_TYPEDEF_TopologyObject_double_H

//Default files containing the declaration of the vector type
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/Mat.h>


#ifdef SOFA_GPU_CUDA
#include <sofa/gpu/cuda/CudaTypes.h>
#endif
#ifdef SOFA_GPU_OPENCL
#include <sofa/gpu/opencl/OpenCLTypes.h>
#endif

#include "ManifoldEdgeSetGeometryAlgorithms.h"
#include "ManifoldEdgeSetTopologyAlgorithms.h"
#include "ManifoldTriangleSetTopologyAlgorithms.h"




//---------------------------------------------------------------------------------------------
//Typedef for ManifoldEdgeSetGeometryAlgorithms
typedef sofa::component::topology::ManifoldEdgeSetGeometryAlgorithms<sofa::defaulttype::StdRigidTypes<3, double> > ManifoldEdgeSetGeometryAlgorithmsRigid3d;
typedef sofa::component::topology::ManifoldEdgeSetGeometryAlgorithms<sofa::defaulttype::StdRigidTypes<2, double> > ManifoldEdgeSetGeometryAlgorithmsRigid2d;
typedef sofa::component::topology::ManifoldEdgeSetGeometryAlgorithms<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, double>, sofa::defaulttype::Vec<1, double>, double> > ManifoldEdgeSetGeometryAlgorithms1d;
typedef sofa::component::topology::ManifoldEdgeSetGeometryAlgorithms<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, double>, sofa::defaulttype::Vec<2, double>, double> > ManifoldEdgeSetGeometryAlgorithms2d;
typedef sofa::component::topology::ManifoldEdgeSetGeometryAlgorithms<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > ManifoldEdgeSetGeometryAlgorithms3d;



//---------------------------------------------------------------------------------------------
//Typedef for ManifoldEdgeSetTopologyAlgorithms
typedef sofa::component::topology::ManifoldEdgeSetTopologyAlgorithms<sofa::defaulttype::StdRigidTypes<3, double> > ManifoldEdgeSetTopologyAlgorithmsRigid3d;
typedef sofa::component::topology::ManifoldEdgeSetTopologyAlgorithms<sofa::defaulttype::StdRigidTypes<2, double> > ManifoldEdgeSetTopologyAlgorithmsRigid2d;
typedef sofa::component::topology::ManifoldEdgeSetTopologyAlgorithms<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, double>, sofa::defaulttype::Vec<1, double>, double> > ManifoldEdgeSetTopologyAlgorithms1d;
typedef sofa::component::topology::ManifoldEdgeSetTopologyAlgorithms<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, double>, sofa::defaulttype::Vec<2, double>, double> > ManifoldEdgeSetTopologyAlgorithms2d;
typedef sofa::component::topology::ManifoldEdgeSetTopologyAlgorithms<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > ManifoldEdgeSetTopologyAlgorithms3d;



//---------------------------------------------------------------------------------------------
//Typedef for ManifoldTriangleSetTopologyAlgorithms
typedef sofa::component::topology::ManifoldTriangleSetTopologyAlgorithms<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, double>, sofa::defaulttype::Vec<1, double>, double> > ManifoldTriangleSetTopologyAlgorithms1d;
typedef sofa::component::topology::ManifoldTriangleSetTopologyAlgorithms<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, double>, sofa::defaulttype::Vec<2, double>, double> > ManifoldTriangleSetTopologyAlgorithms2d;
typedef sofa::component::topology::ManifoldTriangleSetTopologyAlgorithms<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > ManifoldTriangleSetTopologyAlgorithms3d;

#endif
