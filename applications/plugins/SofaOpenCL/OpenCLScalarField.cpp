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
#include <sofa/core/ObjectFactory.h>
using sofa::core::RegisterObject;

#include <sofa/helper/AdvancedTimer.h>
using sofa::helper::ScopedAdvancedTimer;

#include <SofaOpenCL/OpenCLProgram.h>
using sofa::gpu::opencl::OpenCLProgram;

#include <SofaOpenCL/OpenCLKernel.h>
using sofa::gpu::opencl::OpenCLKernel;

#include "OpenCLScalarField.h"

namespace sofaopencl
{

using sofa::core::objectmodel::ComponentState;

OpenCLScalarField::OpenCLScalarField() :
    d_inputFieldFunction(initData(&d_inputFieldFunction, ScalarFieldR3{}, "evaluationFunction", "the function to evaluate"))
  ,d_inputShaderCode(initData(&d_inputShaderCode, "kernelCode", "The kernel code to use"))
  ,d_inputPoints(initData(&d_inputPoints, "points", "The location where the function field is evaluated"))
  ,d_outputValues(initData(&d_outputValues, "values", "The field values at given points"))
  ,d_outputGradients(initData(&d_outputGradients, "gradients", "The field gradient at given points"))
  ,m_kernelfunction{nullptr}
{
    //addInput(&d_inputPoints);
    //addOutput(&d_outputValues);
    //addOutput(&d_outputGradients);

    addUpdateCallback("", {&d_inputShaderCode},
                      [this](){
        std::cout << "PYUSHING NEW SHADER CODE FROM input " << d_inputShaderCode.getCounter() << std::endl;
        this->initGPU();
        this->compileShaderCode(d_inputShaderCode.getValue());
        doUpdate();
        return sofa::core::objectmodel::ComponentState::Valid;
    },
    {&d_outputValues, &d_outputGradients}
    );

    addUpdateCallback("", {&d_inputPoints, &d_inputFieldFunction},
                      [this](){
        doUpdate();
        return sofa::core::objectmodel::ComponentState::Valid;
    },
    {&d_outputValues, &d_outputGradients}
    );
}

double OpenCLScalarField::getValue(const Vec3d& pos)
{
    return 1.0;
}

template<class D>
sofa::helper::ReadAccessor<D> getReadAccessor(D& c)
{
    return sofa::helper::ReadAccessor<D>{ c };
}

template<class D>
sofa::helper::WriteOnlyAccessor<D> getWriteOnlyAccessor(D& c)
{
    return sofa::helper::WriteOnlyAccessor<D>{ c };
}

// auto generated from ./clext.py
#ifndef __CL_EXT__
#define __CL_EXT__


#include <stdio.h>


/*
 * Given a cl code and return a string represenation
 */
const char* clGetErrorString(int errorCode) {
    switch (errorCode) {
    case 0: return "CL_SUCCESS";
    case -1: return "CL_DEVICE_NOT_FOUND";
    case -2: return "CL_DEVICE_NOT_AVAILABLE";
    case -3: return "CL_COMPILER_NOT_AVAILABLE";
    case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5: return "CL_OUT_OF_RESOURCES";
    case -6: return "CL_OUT_OF_HOST_MEMORY";
    case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8: return "CL_MEM_COPY_OVERLAP";
    case -9: return "CL_IMAGE_FORMAT_MISMATCH";
    case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -12: return "CL_MAP_FAILURE";
    case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15: return "CL_COMPILE_PROGRAM_FAILURE";
    case -16: return "CL_LINKER_NOT_AVAILABLE";
    case -17: return "CL_LINK_PROGRAM_FAILURE";
    case -18: return "CL_DEVICE_PARTITION_FAILED";
    case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";
    case -30: return "CL_INVALID_VALUE";
    case -31: return "CL_INVALID_DEVICE_TYPE";
    case -32: return "CL_INVALID_PLATFORM";
    case -33: return "CL_INVALID_DEVICE";
    case -34: return "CL_INVALID_CONTEXT";
    case -35: return "CL_INVALID_QUEUE_PROPERTIES";
    case -36: return "CL_INVALID_COMMAND_QUEUE";
    case -37: return "CL_INVALID_HOST_PTR";
    case -38: return "CL_INVALID_MEM_OBJECT";
    case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40: return "CL_INVALID_IMAGE_SIZE";
    case -41: return "CL_INVALID_SAMPLER";
    case -42: return "CL_INVALID_BINARY";
    case -43: return "CL_INVALID_BUILD_OPTIONS";
    case -44: return "CL_INVALID_PROGRAM";
    case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46: return "CL_INVALID_KERNEL_NAME";
    case -47: return "CL_INVALID_KERNEL_DEFINITION";
    case -48: return "CL_INVALID_KERNEL";
    case -49: return "CL_INVALID_ARG_INDEX";
    case -50: return "CL_INVALID_ARG_VALUE";
    case -51: return "CL_INVALID_ARG_SIZE";
    case -52: return "CL_INVALID_KERNEL_ARGS";
    case -53: return "CL_INVALID_WORK_DIMENSION";
    case -54: return "CL_INVALID_WORK_GROUP_SIZE";
    case -55: return "CL_INVALID_WORK_ITEM_SIZE";
    case -56: return "CL_INVALID_GLOBAL_OFFSET";
    case -57: return "CL_INVALID_EVENT_WAIT_LIST";
    case -58: return "CL_INVALID_EVENT";
    case -59: return "CL_INVALID_OPERATION";
    case -60: return "CL_INVALID_GL_OBJECT";
    case -61: return "CL_INVALID_BUFFER_SIZE";
    case -62: return "CL_INVALID_MIP_LEVEL";
    case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64: return "CL_INVALID_PROPERTY";
    case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66: return "CL_INVALID_COMPILER_OPTIONS";
    case -67: return "CL_INVALID_LINKER_OPTIONS";
    case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";
    case -69: return "CL_INVALID_PIPE_SIZE";
    case -70: return "CL_INVALID_DEVICE_QUEUE";
    case -71: return "CL_INVALID_SPEC_ID";
    case -72: return "CL_MAX_SIZE_RESTRICTION_EXCEEDED";
    case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
    case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
    case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
    case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
    case -1006: return "CL_INVALID_D3D11_DEVICE_KHR";
    case -1007: return "CL_INVALID_D3D11_RESOURCE_KHR";
    case -1008: return "CL_D3D11_RESOURCE_ALREADY_ACQUIRED_KHR";
    case -1009: return "CL_D3D11_RESOURCE_NOT_ACQUIRED_KHR";
    case -1010: return "CL_INVALID_DX9_MEDIA_ADAPTER_KHR";
    case -1011: return "CL_INVALID_DX9_MEDIA_SURFACE_KHR";
    case -1012: return "CL_DX9_MEDIA_SURFACE_ALREADY_ACQUIRED_KHR";
    case -1013: return "CL_DX9_MEDIA_SURFACE_NOT_ACQUIRED_KHR";
    case -1093: return "CL_INVALID_EGL_OBJECT_KHR";
    case -1092: return "CL_EGL_RESOURCE_NOT_ACQUIRED_KHR";
    case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
    case -1057: return "CL_DEVICE_PARTITION_FAILED_EXT";
    case -1058: return "CL_INVALID_PARTITION_COUNT_EXT";
    case -1059: return "CL_INVALID_PARTITION_NAME_EXT";
    case -1094: return "CL_INVALID_ACCELERATOR_INTEL";
    case -1095: return "CL_INVALID_ACCELERATOR_TYPE_INTEL";
    case -1096: return "CL_INVALID_ACCELERATOR_DESCRIPTOR_INTEL";
    case -1097: return "CL_ACCELERATOR_TYPE_NOT_SUPPORTED_INTEL";
    case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
    case -1098: return "CL_INVALID_VA_API_MEDIA_ADAPTER_INTEL";
    case -1099: return "CL_INVALID_VA_API_MEDIA_SURFACE_INTEL";
    case -1100: return "CL_VA_API_MEDIA_SURFACE_ALREADY_ACQUIRED_INTEL";
    case -1101: return "CL_VA_API_MEDIA_SURFACE_NOT_ACQUIRED_INTEL";
    default: return "CL_UNKNOWN_ERROR";
    }
}

/*
 * check cl error, if not CL_SUCCESS, print to stderr
 */
int clCheckError(int errorCode) {
    if (errorCode != 0) {
        fprintf(stderr, "%s\n", clGetErrorString(errorCode));
    }
    return errorCode;
}

#endif /* __CL_EXT__*/

void OpenCLScalarField::initGPU()
{
    /// OpenCL program
    if(!m_isInited)
    {
        try
        {
            std::vector<cl::Platform> all_platforms;
            cl::Platform::get(&all_platforms);
            if(all_platforms.size()==0){
                std::cout<<" No platforms found. Check OpenCL installation!\n";
                exit(1);
            }
            cl::Platform default_platform=all_platforms[0];
            std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

            std::vector<cl::Device> all_devices;
            default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
            if(all_devices.size()==0){
                std::cout<<" No devices found. Check OpenCL installation!\n";
                exit(1);
            }
            m_ocldevice = all_devices[0];
            std::cout<< "Using device: "<<m_ocldevice.getInfo<CL_DEVICE_NAME>()<<"\n";
            m_oclcontext = cl::Context(m_ocldevice);
            m_oclqueue = cl::CommandQueue(m_oclcontext);
            m_isInited = true;
        }catch(cl::Error& e)
        {
            std::cout << "Exception in '" << e.what() << "', error message is '" << clGetErrorString(e.err()) << "'" << std::endl;
        }
    }
}

void OpenCLScalarField::compileShaderCode(const std::string& kernelcode)
{
    m_componentstate = ComponentState::Invalid;
    m_kernelfunction = nullptr;
    try{
        cl::Program::Sources sources;

        // kernel calculates for each element C=A+B
        sources.push_back({kernelcode.c_str(),kernelcode.length()});

        m_oclprogram = cl::Program(m_oclcontext,sources);
        m_oclprogram.build({m_ocldevice});
    }catch(cl::Error& e)
    {
        std::cout << "Exception in '" << e.what() << "', error message is '" << clGetErrorString(e.err()) << "'" << std::endl;
    }

    try{
        m_kernelfunction = new cl::KernelFunctor<cl::Buffer, cl::Buffer>(cl::Kernel(m_oclprogram,"main"));
        m_componentstate = ComponentState::Valid;
    }catch(cl::Error& e)
    {
        std::cout << "Exception in '" << e.what() << "', error message is '" << clGetErrorString(e.err()) << "'" << std::endl;
    }
}

void OpenCLScalarField::doUpdateGPU()
{
    auto points = getReadAccessor(d_inputPoints);
    auto values = getWriteOnlyAccessor(d_outputValues);

    ScopedAdvancedTimer time("OpenCLScalarField::doUpdate");
    try{
        if(m_bufferSize != points.size())
        {
            /// RebuildMake the data buffer.
            buffer_A = cl::Buffer(m_oclcontext, CL_MEM_READ_ONLY, sizeof(SReal)*4*points.size());
            buffer_C = cl::Buffer(m_oclcontext, CL_MEM_WRITE_ONLY, sizeof(SReal)*points.size());
            values.resize(points.size());
        }

        /// Copy the CPU data to the GPU
        cl::copy(m_oclqueue, points.begin(), points.end(), buffer_A);

        /// call the computation code.
        cl::EnqueueArgs eargs(m_oclqueue, cl::NDRange(points.size()));
        (*m_kernelfunction)(eargs, buffer_A, buffer_C);

        /// Copy back the results.
        cl::copy(m_oclqueue, buffer_C, values.begin(), values.end());
    }catch(cl::Error& e)
    {
        std::cout << "Exception in '" << e.what() << "', error message is '" << clGetErrorString(e.err()) << "'" << std::endl;
    }
}

void OpenCLScalarField::doUpdate()
{
    if(m_kernelfunction!=nullptr)
        doUpdateGPU();

    return;

    auto fieldFunctions = d_inputFieldFunction.getValue();
    auto points = getReadAccessor(d_inputPoints);
    auto values = getWriteOnlyAccessor(d_outputValues);
    auto gradients = getWriteOnlyAccessor(d_outputGradients);

    auto& function = fieldFunctions.function;
    auto& arrayFunction = fieldFunctions.arrayFunction;

    /// An array function is provided, we use it as it may have better performances
    if(arrayFunction)
    {
        return arrayFunction(points.ref(), values.wref());
    }
    values.resize(points.size());
    /// We apply the field function to the points array.
    for(unsigned int i=0;i<points.size();i++)
    {
        auto& pts = points[i];
        values[i] = function(pts.x(), pts.y(), pts.y());
    }
}

static int ScalarFieldClassID = RegisterObject("A ScalarField function evaluated using a opencl for computation")
        .add<OpenCLScalarField>();

} // namespace sofaopencl
