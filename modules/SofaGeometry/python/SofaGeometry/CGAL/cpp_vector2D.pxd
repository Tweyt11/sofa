# -*- coding: ASCII -*-
from libcpp.vector cimport vector
from  libcpp cimport bool
from SofaGeometry.CGAL cimport cpp_point2D
from SofaGeometry.CGAL cimport cpp_kernel

cdef extern from "<SofaGeometry/CGAL/Vector2.h>" namespace "SofaGeometryCGAL":

    cdef cppclass Vector2 "SofaGeometryCGAL::Vector2":

        Vector2() except +

        Vector2(const Vector2&) except +

        Vector2(const cpp_point2D.Point2&, const cpp_point2D.Point2&) except +

        Vector2(const cpp_kernel.FT&, const cpp_kernel.FT&) except +

        double x()

        double y()

    Vector2 mul(const Vector2&, const cpp_kernel.FT&)

    Vector2 rmul(const cpp_kernel.FT&, const Vector2&)

    cpp_kernel.FT squared_length ()

