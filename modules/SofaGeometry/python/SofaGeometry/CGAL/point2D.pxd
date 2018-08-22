from  libcpp cimport bool
from libcpp.memory cimport shared_ptr
from cython.operator cimport typeid
from cython.operator cimport dereference as deref, preincrement as inc, address as address
from cpp_point2D cimport Point2 as _Point2
from cpp_point2D cimport geq as _geq, seq as _seq, smaller as _smaller, greater as _greater
#from cpp_point2D cimport FT as _FT

cdef class Point2:

    cdef shared_ptr[_Point2] inst

#    def bool equal(self, Point2)
#    def bool not_equal(self, Point2)

#cpdef bool bigger_or_equal(Point2, Point2)
#    cpdef bool bigger(Point2, Point2)
#    cpdef bool smaller_or_equal(Point2, Point2)
#    cpdef bool smaller_or_equal(Point2, Point2)
