#cython: language=c++
from libcpp.memory cimport shared_ptr
from cython.operator cimport dereference as deref, preincrement as inc, address as address
#from SofaGeometry.CGAL cimport cpp_kernel
#from SofaGeometry.CGAL cimport cpp_point2D
#from SofaGeometry.CGAL cimport cpp_polygon2D
#from SofaGeometry.CGAL cimport point2D
from SofaGeometry.CGAL import point2D


import numpy as np


cdef class Polygon2:

    #cdef shared_ptr[cpp_polygon2D.Polygon2] inst

    def __init__(self, *args):

        if not args:

            self._init_0(*args)

        elif (len(args)==1) and (isinstance(args[0], Polygon2)):

            self._init_1(*args)

        elif (len(args)==1) and (isinstance(args[0][0], point2D.Point2)):
            print("non copy case")
            self._init_2(args[0])

        else:
            raise Exception('can not handle type of %s' % (args,))


    def _init_0(self):
        self.inst = shared_ptr[cpp_polygon2D.Polygon2](new cpp_polygon2D.Polygon2())

    def _init_1(self, Polygon2 pgn_0):
        self.inst = shared_ptr[cpp_polygon2D.Polygon2](new cpp_polygon2D.Polygon2((deref(pgn_0.inst.get()))))

    def _init_2(self,  point2D.Point2[:] list_pts):
        cdef int n = list_pts.shape[0]
        print(str(n))
        cdef shared_ptr[cpp_point2D.Point2[n]] ptr_vect = shared_ptr[cpp_point2D.Point2[n]](new cpp_point2D.Point2[n])

        cdef int i
        cdef point2D.Point2 pt

        for i in range(n):
            pt = list_pts[i]
            ptr_vect.get()[i] = deref(pt.inst.get())

#        cdef _vector[cpp_point2D.Point2].iterator first = ptr_vect.get()
#        cdef _vector[cpp_point2D.Point2].iterator last + nte     = #ptr_vect.get().end()
        self.inst = shared_ptr[cpp_polygon2D.Polygon2](new cpp_polygon2D.Polygon2(ptr_vect.get(), ptr_vect.get() + n, cpp_kernel.Kernel()))
        ptr_vect.reset()

    def __dealloc__(self):
        print("dealloc called for polygon")
        self.inst.reset()

    def equal(self, Polygon2 pgn):
        cdef bool temp = cpp_polygon2D.eq(deref(self.inst.get()), deref(pgn.inst.get()))
        return temp

    def __eq__ (self, Polygon2 pgn):
        return self.equal(pgn)

    def nonequal(self, Polygon2 pgn):
        cdef bool temp = cpp_polygon2D.neq(deref(self.inst.get()), deref(pgn.inst.get()))
        return temp

    def __ne__ (self, Polygon2 pgn):
        return self.nonequal(pgn)

    def area(self):
        return self.inst.get().area()

    def is_simple(self):
        cdef bool temp = self.inst.get().is_simple()
        return temp

    def is_convex(self):
        cdef bool temp  = self.inst.get().is_convex()
        return temp

    def orientation(self):
        cdef double temp  = self.inst.get().orientation()
        return temp

    def reverse_orientation(self):
        self.inst.get().reverse_orientation()
