# -*- coding: ASCII -*-
from libcpp.vector cimport vector
from  libcpp cimport bool
from SofaGeometry.CGAL cimport cpp_point2D
from SofaGeometry.CGAL cimport cpp_kernel

cdef extern from "<SofaGeometry/CGAL/Polygon2.h>" namespace "SofaGeometryCGAL":

    cdef cppclass Polygon2 "SofaGeometryCGAL::Polygon2":

        Polygon2() except +

        Polygon2( Polygon2&) except +

        Polygon2(vector[cpp_point2D.Point2].iterator, vector[cpp_point2D.Point2].iterator, cpp_kernel.Kernel) except +


        double area()

        bool is_convex()
        bool is_simple()
        double orientation()
        void reverse_orientation ()


    bool eq (const Polygon2&, const Polygon2&)
    bool neq (const Polygon2&, const Polygon2&)



