from  libcpp cimport bool
from libcpp.memory cimport shared_ptr
from cython.operator cimport typeid
from cython.operator cimport dereference as deref, preincrement as inc, address as address
from SofaGeometry.CGAL cimport cpp_point2D

cdef class Point2:
    cdef shared_ptr[cpp_point2D.Point2] inst



