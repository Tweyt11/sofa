# -*- coding: ASCII -*-
from libcpp.vector cimport vector
from  libcpp cimport bool
from SofaGeometry.CGAL cimport cpp_point2D
from SofaGeometry.CGAL cimport  cpp_kernel

cdef extern from "<SofaGeometry/CGAL/Polygon2.h>" namespace "SofaGeometryCGAL":

    cdef cppclass Polygon2 "SofaGeometryCGAL::Polygon2":

        Polygon2() except +

        Polygon2( Polygon2&) except +

        Polygon2(vector[cpp_point2D.Point2].iterator, vector[cpp_point2D.Point2].iterator) except +


        double area()


    bool eq (const Polygon2&, const Polygon2&)
    bool neq (const Polygon2&, const Polygon2&)



