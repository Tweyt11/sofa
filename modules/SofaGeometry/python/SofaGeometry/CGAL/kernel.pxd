from  libcpp cimport bool
from libcpp.memory cimport shared_ptr
from cython.operator cimport typeid
from cython.operator cimport dereference as deref, preincrement as inc, address as address
from SofaGeometry.CGAL cimport cpp_kernel

cdef class Exact_double:
    cdef shared_ptr[cpp_kernel.Exact_double] inst




