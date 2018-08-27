from  libcpp cimport bool
from libcpp.memory cimport shared_ptr
from libcpp.vector cimport vector as _vector
from cython.operator cimport dereference as deref, preincrement as inc, address as address

from SofaGeometry.CGAL cimport point2D
from SofaGeometry.CGAL cimport cpp_point2D
from SofaGeometry.CGAL cimport cpp_polygon2D
cimport numpy as np

cdef class Polygon2:

    cdef shared_ptr[cpp_polygon2D.Polygon2] inst
