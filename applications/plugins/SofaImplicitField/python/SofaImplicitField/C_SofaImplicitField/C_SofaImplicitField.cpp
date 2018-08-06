/// C_SofaImplicitField is implementing the python bindings of some of
/// the SofaImplicitField component.
///
/// Contributors:
///    - damien.marchal@univ-lille.fr
///
/// Copyright 2018 CNRS
///
#include <SofaPython/PythonMacros.h>
#include <SofaPython/PythonFactory.h>
#include <SofaPython/Binding_Data.h>
#include <SofaPython/PythonToSofa.inl>

#include <SofaImplicitField/components/geometry/ScalarField.h>

namespace sofaimplicitfield
{

using sofa::defaulttype::Vec3d;
using sofa::core::objectmodel::BaseObject;
using sofa::core::objectmodel::Base;

using sofa::component::geometry::ScalarField;

static PyObject * __SofaImplicitField_helloWorld(PyObject * /*self*/, PyObject * /*args*/)
{
    return nullptr;
}


SP_DECLARE_CLASS_TYPE(ScalarField)

static inline ScalarField* get_ScalarField(PyObject* obj) {
    return sofa::py::unwrap<ScalarField>(obj);
}


using sofa::defaulttype::Vector3 ;
static inline Vector3* get_vector3(PyObject* obj) {
    return sofa::py::unwrap<Vector3>(obj);
}


static PyObject * ScalarField_getValue(PyObject *self, PyObject *args)
{
    ScalarField* obj = get_ScalarField( self );
    if(obj == nullptr)
    {
        PyErr_SetString(PyExc_TypeError,
                        "ScalarType.getValue needs a 'ScalarField' self object");
        return nullptr;
    }

    PyObject* py_pos;
    PyArg_ParseTuple(args, "O", &py_pos) ;
    Vector3* pos = get_vector3( py_pos );
    if (!pos)
    {
        PyErr_BadArgument();
        return NULL;
    }

    return PyFloat_FromDouble(obj->getValue( *pos ));
}

SP_CLASS_METHODS_BEGIN(ScalarField)
SP_CLASS_METHOD(ScalarField, getValue)
SP_CLASS_METHODS_END

SP_CLASS_TYPE_SPTR(ScalarField,ScalarField,BaseObject)

static PyObject * ScalarField_new(PyTypeObject * cls, PyObject * args, PyObject* /*kwargs*/)
{
        PyObject* py_node = PyTuple_GetItem(args, 0) ;

        BaseObject* obj = sofa::py::unwrap<BaseObject>(py_node) ;
        if(obj==nullptr)
        {
            PyErr_SetString(PyExc_TypeError,
                            "ScalarType.__new__ needs a Sofa.BaseObject as first argument");
            return nullptr;
        }

        if( dynamic_cast<ScalarField*>(obj) == nullptr )
        {
            PyErr_SetString(PyExc_TypeError,
                            "ScalarType.__new__ needs a 'ScalarField' component");
            return nullptr;
        }

        PyObject* instance = BuildPySPtr<Base>(obj, cls);
        return instance;
}

/// Methods of the module
SP_MODULE_METHODS_BEGIN(__SofaImplicitField)
SP_MODULE_METHOD(__SofaImplicitField, helloWorld)
//SP_MODULE_METHOD(_Compliant,getImplicitAssembledSystem)
//SP_MODULE_METHOD(_Compliant,getLambda)
//SP_MODULE_METHOD(_Compliant,propagateLambdas)
SP_MODULE_METHODS_END

namespace {
static struct patch {
    patch() {
        SP_SOFAPYTYPEOBJECT(ScalarField).tp_new = ScalarField_new;
    }
} patcher;
}



void createAndInitSofaImplicitFieldPythonModule()
{
    sofa::simulation::PythonEnvironment::gil lock(__func__);
    static std::string docstring=R"(
                                 SofaImplicitField module.

                                 This module is part of the SofaImplicitField plugin and contains function and binding to the c++ sofa components.
                                 )";

    static PyObject *s__SofaImplicitFieldPythonModule = SP_INIT_MODULE(__SofaImplicitField, docstring.c_str())
    SP_ADD_CLASS( s__SofaImplicitFieldPythonModule, ScalarField );
}

} /// namespace sofaimplicitfield
