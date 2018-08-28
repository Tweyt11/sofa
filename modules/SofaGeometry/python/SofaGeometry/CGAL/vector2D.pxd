from  libcpp cimport bool
from libcpp.memory cimport shared_ptr
from cython.operator cimport typeid
from cython.operator cimport dereference as deref, preincrement as inc, address as address
from SofaGeometry.CGAL cimport cpp_vector2D
from SofaGeometry.CGAL cimport point2D

cdef class Vector2:
    cdef shared_ptr[cpp_vector2D.Vector2] inst



