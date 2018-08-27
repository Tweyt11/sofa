#cython: language=c++
from libcpp.memory cimport shared_ptr
from cython.operator cimport typeid
from cython.operator cimport dereference as deref, preincrement as inc, address as address
from SofaGeometry.CGAL cimport cpp_kernel
from SofaGeometry.CGAL cimport cpp_point2D
from SofaGeometry.CGAL cimport cpp_polygon2D
from SofaGeometry.CGAL cimport point2D


import numpy as np


cdef class Polygon2:

    #cdef shared_ptr[cpp_polygon2D.Polygon2] inst

    def __init__(self, *args):

        if not args:

            self._init_0(*args)

        elif (len(args)==1) and (isinstance(args[0], Polygon2)):

            self._init_1(*args)

        elif (len(args)==1) and (isinstance(args[0], point2D.Point2._memoryviewslice)):
            print("non copy case")
            print(str(type(args[0][0])))
            self._init_2(args[0])

        else:
            raise Exception('can not handle type of %s' % (args,))


    def _init_0(self):
        self.inst = shared_ptr[cpp_polygon2D.Polygon2](new cpp_polygon2D.Polygon2())

    def _init_1(self, Polygon2 pgn_0):
        self.inst = shared_ptr[cpp_polygon2D.Polygon2](new cpp_polygon2D.Polygon2((deref(pgn_0.inst.get()))))

    def _init_2(self,  point2D.Point2[:] list_pts):
        self._init_0()
        cdef shared_ptr[_vector[cpp_point2D.Point2]] ptr_vect
        cdef int i
        cdef point2D.Point2 pt

        for i in range(list_pts.size()):
            pt = list_pts[i]
            ptr_vect.get().push_back(deref(pt.inst.get()))

        cdef _vector[cpp_point2D.Point2].iterator first = ptr_vect.get().begin()
        cdef _vector[cpp_point2D.Point2].iterator last = ptr_vect.get().end()

        self.inst = shared_ptr[cpp_polygon2D.Polygon2](new cpp_polygon2D.Polygon2(first, last))
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
