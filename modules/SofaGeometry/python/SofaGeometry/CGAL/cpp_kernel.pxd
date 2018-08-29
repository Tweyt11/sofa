# -*- coding: ASCII -*-


cdef extern from "<SofaGeometry/CGAL/Kernel.h>" namespace "SofaGeometryCGAL":

    cdef cppclass Kernel "SofaGeometryCGAL::Kernel":
        Kernel() except +

    cdef cppclass FT "SofaGeometryCGAL::FT":
        FT() except +

    cdef cppclass Exact_double "SofaGeometryCGAL::Exact_double":
        Exact_double() except +
        Exact_double(double i) except +
        Exact_double(Exact_double) except +
        FT exact()
