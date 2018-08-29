#cython: language=c++
from libcpp.memory cimport shared_ptr
from cython.operator cimport dereference as deref, preincrement as inc, address as address

import numpy as np

cdef class Exact_double:

    def __init__(self, *args):

        if not args:

            self._init_0(*args)

        elif (len(args)==1) and (isinstance(args[0], Exact_double)):

            self._init_1(*args)

        elif (len(args)==1) and (isinstance(args[0], list)) and ((isinstance(args[0][0], Exact_double))):

            self._init_1(args[0][0])

        elif (len(args)==1) and ((isinstance(args[0], float))):
            self._init_bis(args[0])

        elif (len(args)==1) and (isinstance(args[0], list)) and ((isinstance(args[0][0], float))):
            self._init_bis(args[0][0])

        else:
            raise Exception('can not handle type of %s' % (args,))


    def _init_0(self):
        self.inst = shared_ptr[cpp_kernel.Exact_double](new cpp_kernel.Exact_double())

    def _init_1(self, Exact_double n):
        self.inst = shared_ptr[cpp_kernel.Exact_double](new cpp_kernel.Exact_double((deref(n.inst.get()))))

    def _init_bis(self, double t):
        self.inst = shared_ptr[cpp_kernel.Exact_double](new cpp_kernel.Exact_double(t))

