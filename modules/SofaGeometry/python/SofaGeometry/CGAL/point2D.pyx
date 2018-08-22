#cython: language=c++
from libcpp.memory cimport shared_ptr
from cython.operator cimport typeid
from cython.operator cimport dereference as deref, preincrement as inc, address as address
from cpp_point2D cimport Point2 as _Point2
#from cpp_point2D cimport to_double as _to_double
#from cpp_point2D cimport FT as _FT


cdef class Point2:

    #cdef shared_ptr[_Point2] inst

    def __init__(self, *args):
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
        self.inst = shared_ptr[_Point2](new _Point2())

    def _init_1(self, Point2 p_0):
        self.inst = shared_ptr[_Point2](new _Point2((deref(p_0.inst.get()))))

    def _init_2(self, x, y):
        if isinstance(x, int) and isinstance(y, int):

            self.inst = shared_ptr[_Point2](new _Point2((<int>x), (<int>y)))
        else:
            self.inst = shared_ptr[_Point2](new _Point2((<double>x), (<double>y)))

    def __dealloc__(self):
        print("dealloc called")
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

        cdef bool temp = deref(self.inst.get())==deref(x.inst.get())
        return temp

    def not_equal(self, Point2 x):

        cdef bool temp = deref(self.inst.get())!=deref(x.inst.get())
        return temp

def geq(Point2 x, Point2 y):
    cdef bool temp = _geq(deref(x.inst.get()), deref(y.inst.get()))
    return temp

def seq(Point2 x, Point2 y):

    cdef bool temp = _seq(deref(x.inst.get()), deref(y.inst.get()))
    return temp

def greater(Point2 x, Point2 y):

    cdef bool temp = _greater(deref(x.inst.get()), deref(y.inst.get()))
    return temp

def smaller(Point2 x, Point2 y):

    cdef bool temp = _smaller(deref(x.inst.get()), deref(y.inst.get()))
    return temp
