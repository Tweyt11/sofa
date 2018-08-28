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

    Vector2 mul_right(const Vector2&, const cpp_kernel.FT&)

    Vector2 mul_left(const cpp_kernel.FT&, const Vector2&)


