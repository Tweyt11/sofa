#cython: language=c++
from libcpp.memory cimport shared_ptr
from cython.operator cimport typeid
from cython.operator cimport dereference as deref, preincrement as inc, address as address
from SofaGeometry.CGAL cimport  cpp_point2D
import numpy as np
cimport numpy as np

cdef class Point2:

    def __init__(self, *args):

        STUFF = "Hi"


        if not args:

            self._init_0(*args)
        elif (len(args)==1) and (isinstance(args[0], Point2)):

            self._init_1(*args)
        elif (len(args)==1) and (isinstance(args[0], list)) and ((isinstance(args[0][0], (float, int)))):

            self._init_2(args[0][0], args[0][1])

        elif (len(args)==2) and isinstance(args[0], (float,int)) and isinstance(args[1], (float,int)):
            self._init_2(*args)
        else:
               raise Exception('can not handle type of %s' % (args,))


    def _init_0(self):
        self.inst = shared_ptr[cpp_point2D.Point2](new cpp_point2D.Point2())

    def _init_1(self, Point2 p_0):
        self.inst = shared_ptr[cpp_point2D.Point2](new cpp_point2D.Point2((deref(p_0.inst.get()))))

    def _init_2(self, x, y):
        if isinstance(x, int) and isinstance(y, int):

            self.inst = shared_ptr[cpp_point2D.Point2](new cpp_point2D.Point2((<int>x), (<int>y)))
        else:
            self.inst = shared_ptr[cpp_point2D.Point2](new cpp_point2D.Point2((<double>x), (<double>y)))

    def __dealloc__(self):
        print("dealloc called for point")
        self.inst.reset()


    def x(self):
        cdef double _r = self.inst.get().x()
        py_result = <double>_r
        return py_result


    def y(self):
        cdef double _r = self.inst.get().y()
        py_result = <double>_r
        return py_result

    def equal(self, Point2 x):
        cdef bool temp = deref(self.inst.get()) == deref(x.inst.get())
        return temp

    def __eq__(self, Point2 x):
        cdef bool temp = self.equal(x)
        return temp


    def  nonequal(self, Point2 x):
        cdef bool temp = deref(self.inst.get())!=deref(x.inst.get())
        return temp

    def __ne__(self, Point2 x):
        return self.nonequal(x)

    def gequal(self, Point2 y):
        cdef bool temp = cpp_point2D.geq(deref(self.inst.get()), deref(y.inst.get()))
        return temp

    def __ge__(self, Point2 y):
        return self.gequal(y)

    def lequal(self, Point2 y):
        cdef bool temp = cpp_point2D.seq(deref(self.inst.get()), deref(y.inst.get()))
        return temp

    def __le__(self, Point2 y):
        return self.lequal(y)

    def greater(self, Point2 y):
        cdef bool temp = cpp_point2D.greater(deref(self.inst.get()), deref(y.inst.get()))
        return temp

    def __gt__(self, Point2 y):
        return self.greater(y)


    def __smaller__(self, Point2 y):
        cdef bool temp = cpp_point2D.smaller(deref(self.inst.get()), deref(y.inst.get()))
        return temp

    def __lt__(self, Point2 y):
        return self.smaller(y)
