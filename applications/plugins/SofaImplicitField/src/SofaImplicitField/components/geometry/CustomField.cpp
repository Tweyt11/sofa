/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2017 INRIA, USTL, UJF, CNRS, MGH                    *
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

#include <boost/algorithm/string.hpp>
#include <sofa/core/ObjectFactory.h>
#include <SofaPython/PythonEnvironment.h>
#include <SofaPython/PythonFactory.h>

#include <sofa/core/ObjectFactory.h>
using sofa::core::RegisterObject ;

#include <sofa/helper/system/FileMonitor.h>
using sofa::helper::system::FileMonitor ;

#include <sofa/core/objectmodel/IdleEvent.h>
using sofa::core::objectmodel::IdleEvent ;

#include <sofa/simulation/Node.h>
using sofa::simulation::Node ;

#include <cmath>
using std::max;
using sofa::core::objectmodel::ComponentState ;

#include "ScalarField.h"
using sofa::core::objectmodel::CStatus ;

#include <SofaBaseLinearSolver/FullMatrix.h>


#include "CustomField.h"
namespace sofa
{

namespace component
{

namespace geometry
{

namespace _customfield_
{

using sofa::simulation::PythonEnvironment ;


class MyFileListener : public sofa::helper::system::FileEventListener
{
        CustomField* m_cf ;
public:
        MyFileListener(CustomField* cf)
        {
            m_cf = cf ;
        }

        void fileHasChanged(const std::string& sourcefile) override
        {
            SOFA_UNUSED(sourcefile) ;
            m_cf->reinit() ;
        }
};

void CustomField::handleEvent(sofa::core::objectmodel::Event *event)
{
    if (dynamic_cast<sofa::core::objectmodel::IdleEvent *>(event))
        FileMonitor::updates() ;
    else
        BaseObject::handleEvent(event);
}

CustomField::CustomField() :
    d_function (initData(&d_function, (std::string)"", "function", "Use a python function to implement the implicit field.")),
    d_gradient (initData(&d_gradient, (std::string)"", "gradient", "Use a python function to implement the gradient field. If not provided returns gradient using finite difference.")),
    d_glslFunction (initData(&d_glslFunction, (std::string)"", "glslFunction", "Use a python function to return glsl implicit field description.")),
    d_state (initData(&d_state, 0, "state", "This is a number indicating change in this component."))
{
    m_sourcefile = new MyFileListener(this) ;
}

CustomField::~CustomField()
{
    msg_warning() << "Delete customfield object" ;
    if(m_sourcefile)
    {
        FileMonitor::removeListener(m_sourcefile);
        delete m_sourcefile;
        m_sourcefile = nullptr;
    }
}


void CustomField::getCythonHook(PyObject*& module)
{
    if(module==nullptr)
    {
        msg_error() << "Invalid module" ;
        return ;
    }

    PyObject* pDict = PyModule_GetDict(module);
    if(pDict==nullptr)
    {
        msg_error() << "Missing dict" ;
        return ;
    }

    PyObject* fct = PyDict_GetItemString(pDict, "getCythonFunction");
    if (fct==nullptr)
    {
        msg_warning() << "No getCythonFunction() found. Use slow path instead." ;
        return ;
    }

    if (!PyCallable_Check(fct))
    {
        msg_error() << "The object does not have a callable 'getCythonFunction'" ;
        return ;
    }
    PyObject* res = PyObject_CallFunction(fct, "");

    if( res != nullptr )
    {
        PyObject* f=PyTuple_GetItem(res, 0) ;
        PyObject* s=PyTuple_GetItem(res, 1) ;

        if( PyCapsule_CheckExact(f) ){
            m_rawFunction = (FieldFunction)PyCapsule_GetPointer(f, "evalFunction") ;
            m_rawShape = s ;
       }
    }
}

PyObject* CustomField::getPythonFunction(const std::string& attribname, const std::string& attribvalue, PyObject*& module, bool doReload) const
{
    /// Parsing.
    if(attribvalue.empty())
    {
       return nullptr ;
    }

    std::vector<std::string> fields ;
    boost::split( fields, attribvalue, boost::is_any_of( "." ) );

    if(fields.size() != 2)
    {
        msg_error() << "Cannot parse '"<< attribname <<"="<< attribvalue <<" '. Eg: 'modulename.functionname'" ;
        return nullptr ;
    }

    std::string modulename = fields[0] ;
    std::string functionname = fields[1] ;

    if(modulename.empty() || functionname.empty())
    {
        msg_error() << "Module's name and function's name cannot be empty." ;
        return nullptr ;
    }

    if(module==nullptr){
        module = PyImport_ImportModule((char*)modulename.c_str()) ;
    }else if(doReload)
        module = PyImport_ReloadModule(module) ;

    if(module==nullptr)
    {
        PyErr_Print();
        return nullptr;
    }

    PyObject* pDict = PyModule_GetDict(module);
    if(pDict==nullptr)
    {
        PyErr_Print();
        return nullptr;
    }

    PyObject* fct = PyDict_GetItemString(pDict, functionname.c_str());
    if (fct==nullptr)
    {
        PyErr_Print();
        return nullptr;
    }

    if (!PyCallable_Check(fct))
    {
        msg_error() << "The object '"<< functionname <<"' is not callable '"<< modulename <<"'" ;
        return nullptr;
    }

    modulename += ".py" ;
    if( FileMonitor::addFile(modulename, m_sourcefile) < 0 )
        msg_error() << "Unable to register file: " << modulename ;
    return fct ;
}

void CustomField::updateGLSLCodeCacheFromPython()
{
    PythonEnvironment::gil lock {__func__} ;

    msg_info() << "Search for glsl rendering map" ;
    PyObject* glslFunction = getPythonFunction("function", d_glslFunction.getValue(), m_glslFunctionModule, false) ;
    if(glslFunction==nullptr)
    {
        msg_error() << "Unable to find a required callable object from attribute 'function=\""<< d_glslFunction.getValue() <<"\"'" ;
        m_glslcodes.clear() ;
        return ;
    }

    PyObject* res = PyObject_CallFunction(glslFunction, "");
    if(!res)
    {
        PyErr_Print() ;
    }
    PyObject* items  = PyTuple_GetItem(res, 0) ;
    PyObject* glslcode = PyTuple_GetItem(res, 1) ;

    /// Convert the list of instruction into a list of string to pass for GLSL.
    m_glslcodes.clear();
    std::vector<GLSLCodeFragment> evalList;
    for(unsigned int i=0;i<PyList_Size(glslcode);i++)
    {
        GLSLCodeFragment frag;
        frag.m_value = PyString_AsString( PyObject_Str( PyList_GetItem(glslcode, i) ));
        evalList.push_back(frag);
    }
    m_glslcodes["eval"] = evalList;

    /// Convert the list of variable into a GLSL string.
    std::vector<GLSLCodeFragment> variableList;
    for(unsigned int i=0;i<PyList_Size(items);i++)
    {
        PyObject* item = PyList_GetItem(items, i) ;
        PyObject* glslname = PyTuple_GetItem(item, 0);
        PyObject* glsltype = PyTuple_GetItem(item, 1) ;
        PyObject* glslcode = PyTuple_GetItem(item, 2);
        PyObject* glslvalue = PyTuple_GetItem(item, 3) ;

        GLSLCodeFragment frag;
        frag.m_dataname = PyString_AsString(glslname);
        frag.m_name = PyString_AsString(glslname);
        frag.m_type = PyString_AsString(glsltype);
        frag.m_value = PyString_AsString(glslvalue);
        variableList.push_back(frag);
    }

    m_glslcodes["variable"] = variableList;
}

const std::map<std::string, std::vector<GLSLCodeFragment> > &CustomField::getGLSLCode()
{
    updateGLSLCodeCacheFromPython();
    return m_glslcodes ;
}

void CustomField::init()
{
    msg_warning() << "INITING...BECAUSE OF FILE CHANGE" ;
    setStatus(CStatus::Busy) ;
    PythonEnvironment::gil lock(__func__) ;

    m_evalFunction = getPythonFunction("function", d_function.getValue(), m_functionModule, true) ;
    if(m_evalFunction==nullptr)
    {
        msg_error() << "Unable to find a required callable object from attribute 'function=\""<< d_function.getValue() <<"\"'" ;
        m_componentstate = ComponentState::Invalid ;
        setStatus(CStatus::Invalid) ;
        return ;
    }

    m_evalShape = getPythonFunction("function", "customshape.evalShape", m_functionModule, false) ;
    if(m_evalShape==nullptr)
    {
        msg_info() << "Unable to find a required callable object from attribute 'function=\""<< d_function.getValue() <<"\"'" ;
    }


    m_gradFunction = getPythonFunction("gradient", d_gradient.getValue(), m_gradientModule, false) ;
    if(m_gradFunction==nullptr)
    {
        msg_info() << "No gradient function found from attribute 'gradient=\"" << d_gradient.getValue() <<"\"'. Falling back to finite difference implementation" ;
    }    

    //TODO(dmarchal) N'importe quoi ! Codé en dure pour tester et ça reste !!!!Au cachot.
    PyObject* loadShape = getPythonFunction("function", "customshape.evalShape", m_functionModule, false) ;
    if (PyCallable_Check(loadShape)){
        msg_info() << "got an evalShape Function..." ;
        /*
        Node* me = (Node*)getContext() ;
        Node* root = (Node*)me->getRoot() ;

        PyObject* res = PyObject_CallFunction(loadShape, "O", PythonFactory::toPython(me));
        if(!res)
        {
            PyErr_Print() ;
            m_componentstate = ComponentState::Invalid ;
            setStatus(CStatus::Invalid) ;
            return ;
        }*/
    }else{
        dmsg_warning() << "No loadShape function found. use evalFunction." ;
    }

    msg_info() << "Search for getCythonHook" ;
    getCythonHook(m_functionModule) ;

    m_componentstate = ComponentState::Valid ;
    d_state.setValue(d_state.getValue()+1);
    setStatus(CStatus::Valid) ;
}


void CustomField::reinit()
{
    init() ;
}


double CustomField::getValue(const defaulttype::Vec3d &pos, int &domain)
{
    SOFA_UNUSED(domain);
    assert(m_evalFunction!=nullptr) ;

    if(m_componentstate!=ComponentState::Valid)
        return std::nan("") ;

    if(m_rawFunction)
        return m_rawFunction(m_rawShape, pos.x(), pos.y(), pos.z()) ;

    PythonEnvironment::gil lock(__func__) ;

    PyObject* res {nullptr};

    if(m_evalShape){
        if( !m_rawShape ){
            //TODO(dmarchal) N'importe quoi ! Codé en dure pour tester et ça reste !!!!Au cachot.
            PyObject* loadShape = getPythonFunction("function", "customshape.buildShapeFromSofaTree", m_functionModule, false) ;
            if (PyCallable_Check(loadShape)){
                msg_info() << "got a ShapeFromSofaTree Function..." ;
                Node* me = (Node*)getContext() ;
                Node* root = (Node*)me->getRoot() ;
                PyObject* res = PyObject_CallFunction(loadShape, "O", PythonFactory::toPython(me));
                if(!res)
                {
                    PyErr_Print() ;
                    m_componentstate = ComponentState::Invalid ;
                    setStatus(CStatus::Invalid) ;
                     return std::nan("") ;
                }
                std::cout << "SETTING RIH" << PyString_AsString( PyObject_Str(res)) << std::endl ;
                m_rawShape=res;
            }
        }
    }

    if(m_rawShape){
        res = PyObject_CallFunction(m_evalShape, "Offf", m_rawShape, pos.x(), pos.y(), pos.z());
    }else{
        res = PyObject_CallFunction(m_evalFunction, "fff", pos.x(), pos.y(), pos.z());
    }
    if(!res)
    {
        PyErr_Print() ;
        return std::nan("") ;
    }

    if(!PyFloat_Check(res))
    {
        msg_error() << "The evalFunction does not returns a Floating point value.";
        return std::nan("") ;
    }
    double tmp = PyFloat_AsDouble(res) ;
    Py_DecRef(res) ;
    return tmp;
}


Vec3d CustomField::getGradient(const Vec3d& pos, int &domain)
{
    SOFA_UNUSED(domain);
    Vec3d tmp(std::nan(""),std::nan(""),std::nan("")) ;

    /// The component is not valid. We return nan.
    if(m_componentstate!=ComponentState::Valid)
        return tmp ;

    //// The component is valid but we have no gradient function. Thus we use the finite difference version.
    if(m_gradFunction==nullptr)
        return ScalarField::getGradient(pos, domain) ;

    PythonEnvironment::gil lock(__func__) ;

    PyObject* res = PyObject_CallFunction(m_gradFunction, "fff", pos.x(), pos.y(), pos.z());
    if(!res)
    {
        PyErr_Print() ;
        return tmp ;
    }

    if(!PyList_Check(res))
    {
        msg_error() << "The gradient function does not returns a list." ;
        return tmp ;
    }

    if(PyList_Size(res) != 3)
    {
        msg_error() << "The gradient function does not returns a list with three elements." ;
        return tmp ;
    }

    for(unsigned int i=0;i<3;i++){
        if(!PyFloat_Check( PyList_GetItem(res, i)))
        {
            msg_error() << "The gradient function returned a list with non floating point values at indice " << i ;
            return tmp ;
        }
    }

    tmp.set(PyFloat_AsDouble(PyList_GetItem(res,0)),
            PyFloat_AsDouble(PyList_GetItem(res,1)),
            PyFloat_AsDouble(PyList_GetItem(res,2))) ;

    Py_DecRef(res) ;

    return tmp ;
}

///factory register
int CustomFieldClass = RegisterObject("A custom scalar field. The scalar function is implemented with python.")
        .add< CustomField >() ;

SOFA_DECL_CLASS(CustomField)

} /// namespace _customfield_

} /// namespace geometry

} /// namespace component

} /// namespace sofa
