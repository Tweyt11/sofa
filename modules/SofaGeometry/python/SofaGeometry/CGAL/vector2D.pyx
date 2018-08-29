#cython: language=c++
from libcpp.memory cimport shared_ptr
from cython.operator cimport typeid
from cython.operator cimport dereference as deref, preincrement as inc, address as address
from SofaGeometry.CGAL cimport  point2D
from SofaGeometry.CGAL cimport  kernel
import numpy as np


cdef class Vector2:

    def __init__(self, *args):

        if not args:

            self._init_0(*args)

        elif (len(args)==1) and (isinstance(args[0], Vector2)):

            self._init_1(*args)

        elif (len(args)==1) and (isinstance(args[0], list)) and ((isinstance(args[0][0], point2D.Point2))):

            self._init_2(args[0][0], args[0][1])

        elif (len(args)==2) and ((isinstance(args[0], point2D.Point2))) and ((isinstance(args[1], point2D.Point2))):
            self._init_2(args[0], args[1])

        elif (len(args)==1) and (isinstance(args[0], list)) and ((isinstance(args[0][0], Vector2))) and ((isinstance(args[0][1], float))):
            self._init_mul(args[0][0], args[0][1])

        elif (len(args)==2) and ((isinstance(args[0], Vector2))) and ((isinstance(args[1], float))):
            self._init_mul(args[0], args[1])

        elif (len(args)==1) and (isinstance(args[0], list)) and ((isinstance(args[0][0], float))) and ((isinstance(args[0][1], Vector2))):
            self._init_rmul(args[0][0], args[0][1])

        elif (len(args)==2) and ((isinstance(args[0], float))) and ((isinstance(args[1], Vector2))):
            self._init_rmul(args[0], args[1])

        else:
            raise Exception('can not handle type of %s' % (args,))


    def _init_0(self):
        self.inst = shared_ptr[cpp_vector2D.Vector2](new cpp_vector2D.Vector2())

    def _init_1(self, Vector2 v_0):
        self.inst = shared_ptr[cpp_vector2D.Vector2](new cpp_vector2D.Vector2((deref(v_0.inst.get()))))

    def _init_2(self, point2D.Point2 p, point2D.Point2 q):

        self.inst = shared_ptr[cpp_vector2D.Vector2](new cpp_vector2D.Vector2(deref(p.inst.get()), deref(q.inst.get())))

    def _init_mul(self, Vector2 v, double t):
        cdef cpp_vector2D.Vector2 vect = cpp_vector2D.mul(deref(v.inst.get()), kernel.Exact_double(t).inst.get().exact())
        self.inst = shared_ptr[cpp_vector2D.Vector2](new cpp_vector2D.Vector2(vect))

    def _init_rmul(self, Vector2 v, double t):
        cdef cpp_vector2D.Vector2 vect = cpp_vector2D.rmul(kernel.Exact_double(t).inst.get().exact(), deref(v.inst.get()))
        self.inst = shared_ptr[cpp_vector2D.Vector2](new cpp_vector2D.Vector2(vect))

    def __dealloc__(self):
        print("dealloc called for point")
        self.inst.reset()

    def x(self):
        cdef double _r = self.inst.get().x()
        py_result = <double> _r
        return py_result

    def y(self):
        cdef double _r = self.inst.get().y()
        py_result = <double> _r
        return py_result

    def mul(self, double t):
        cdef Vector2 vect = Vector2(self, t)
        return vect

    def __mul__(self, t):
        return self.mul(t)

    def rmul(self, double t):
        cdef  Vector2 vect = Vector2(t, self)
        return vect

    def __rmul__(self, t):
        return self.rmul(t)

