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

#include <SofaImplicitField/components/geometry/ScalarField.h>
#include <sofa/core/DataEngine.h>
#include <sofa/helper/vector.h>

/// Enable OpenCL API exception.
#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#include <CL/cl2.hpp>

namespace sofaopencl
{

using sofa::defaulttype::Vec3d;
using sofa::core::objectmodel::Data;
using sofa::component::geometry::ScalarFieldR3;

class OpenCLScalarField : public sofa::core::DataEngine
{
public:
    SOFA_CLASS(OpenCLScalarField, DataEngine);

    OpenCLScalarField();
    double getValue(const Vec3d& pos);

    void doUpdateGPU();
    void doUpdate() override;
    void initGPU();
    void compileShaderCode(const std::string& kernelcode);


    /// C++ version of the field.
    Data<std::string>   d_inputShaderCode;
    Data<ScalarFieldR3> d_inputFieldFunction;

    Data<sofa::helper::vector<Vec3d>> d_inputPoints;
    Data<sofa::helper::vector<SReal>> d_outputValues;
    Data<sofa::helper::vector<Vec3d>> d_outputGradients;

private:
    cl::Buffer buffer_A;
    cl::Buffer buffer_B;
    cl::Buffer buffer_C;

    cl::KernelFunctor<cl::Buffer, cl::Buffer>* m_kernelfunction;

    bool m_isInited {false};
    cl::Program m_oclprogram;
    cl::Context m_oclcontext;
    cl::Kernel m_oclkernel;
    cl::Device m_ocldevice;
    cl::CommandQueue m_oclqueue;
    size_t m_bufferSize {0};
};

} // namespace sofaopencl

