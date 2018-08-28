# -*- coding: ASCII -*-

from  libcpp cimport bool
from SofaGeometry.CGAL cimport cpp_vector2D

cdef extern from "<SofaGeometry/CGAL/Point2.h>" namespace "SofaGeometryCGAL":

    cdef cppclass Point2 "SofaGeometryCGAL::Point2":

        Point2() except +

        Point2(const Point2&) except +

        Point2(int , int ) except +
        
        Point2(double , double ) except +



        double x()
        double y()

        bool operator == (const Point2&) const


        bool operator != (const Point2&) const

    bool geq (const Point2&, const Point2&)
    bool seq (const Point2&, const Point2&)
    bool greater (const Point2&, const Point2&)
    bool smaller (const Point2&, const Point2&)
    cpp_vector2D.Vector2 diff (const Point2&, const Point2&)
    Point2 add(const Point2&, const cpp_vector2D.Vector2&)





