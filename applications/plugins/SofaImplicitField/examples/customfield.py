# -*- coding: utf-8 -*-
import pyximport; pyximport.install(reload_support=True)
import sys
import dfgeom
from dfgeom import *
import gen
import math
from math import sqrt

def glslFunction():
    p=gen.Program()
    
    inp = p.addConstant("Vec3", [0,0,0], ssaid="pos")
    r = shape.toGraph(p, inp)
    v = p.addConstant("Vec3", [1.0,1.0,1.0])
    out = p.addInstruction("Vec4", "vec4",[r, v])
    
    p = gen.glslDump(p)
    #print("code" + str(p))
    return (p[0], p[1]+"\n return "+out.ssaid+";")

def evalField(x,y,z):
    return evalField(shape, x,y,z)

### OTHER STUFF



#### Shape description
b = Box([1.5,0.5,1.0])
u = Sphere([0.0,0.0,0.0], 0.9)
s = Sphere([1.0,0.0,0.0], 0.7)
shape = b

shape = Difference(
                Difference(b, u),
                Sphere([1.0,0.0,0.0], 1.0)
                )

shape = Difference(
                Union(s, shape),
                Box([1.0,0.1,1.2])
                )

shape2 = Rotate(shape, x=1.0)
shape1 = Rotate(shape, x=.0)


motif1 = Box([0.5,0.5,0.5])
motif1 = Twist(motif1, 1.0)
motif1 = Translate(motif1, [0,1,0])
motif2 = Box([0.5,0.5,0.5])
motif2 = Twist(motif2, -1.0)

motif = Union(motif1,motif2)
#shape = motif

motif = Tiling(motif, [1.1,1.1,1.4])
motif = Scale(motif, 0.1)
motif = Twist(motif, 2)

## VIEW POINT 1
#shape = Intersection(Box([1.0,1.0,1.0]), motif)

shape = Offset(shape, +0.02)
motif = Difference(shape, motif)

shape1 = Offset(shape, -0.06)
shape = Difference(shape, shape1)
shape = Offset(shape, -0.06)

shape = Union(motif, shape)

shape = Intersection(Box([2,1,0.7]), shape)

plane = Translate(Box([2.0,0.1,2.0]), [0,-0.7,0])
shape = Union(shape, plane)
