from libc.math cimport cos, sin, sqrt, fmod
from cpython.pycapsule cimport *
import gen
# distutils: language = c++
# distutils: sources = ./vector.cpp
cdef extern from "//home/dmarchal/projects/DEFROST/dev/sofa2/src/applications/plugins/SofaImplicitField/examples/vector.h" namespace "":
    cdef cppclass Vec3d:
            Vec3d()
            Vec3d(double x, double y, double z)
            double x,y,z
            void set(const double x, const double y, const double z)
            Vec3d operator+(const Vec3d)
            Vec3d operator-(const Vec3d)
            Vec3d mul(const double)const
            Vec3d div(const double)const
            double length() const
            Vec3d mod(const Vec3d)
            Vec3d vmin(const Vec3d) const
            Vec3d vmax(const Vec3d) const
            Vec3d vmin(const double) const
            Vec3d vmax(const double) const
            
            double dot(const Vec3d) const
            Vec3d cross(const Vec3d) const
            Vec3d normalized() const 
            
            Vec3d abs() const

cdef int idx = 0

cdef vec3ToStr(Vec3d p):
    return ""+str(p.x)+" "+str(p.y)+" "+str(p.z)

class ImplicitShape(object):
    def __init__(self, s):
        self.shapes = s

cdef class Shape:
    cdef str name
    cdef int uid
    def __init__(self):
        None

    cdef double evalFieldC(Shape tgt, Vec3d p):
        return 0.0

    cdef double evalFieldWithIdC(Shape tgt, Vec3d p, int c):
         return 0.0

    def evalField(self, p):
        return self.evalFieldC(Vec3d(p[0],p[1], p[2]))


cdef class Cylinder(Shape):
    cdef Vec3d c

    def __init__(self, c):
        self.c = Vec3d(c[0], c[1], c[2])

    cdef double evalFieldC(self, Vec3d p):
        cdef Vec3d d = Vec3d(p.x-self.c.x, p.y-self.c.y, 0)
        return d.length()-self.c.z

    def evalField(self, p):
        return self.evalFieldC(Vec3d(p[0],p[1], p[2]))

cdef class Plane(Shape):
    cdef Vec3d normal
    cdef float distance

    def __init__(self, c, d):
        global idx
        self.normal = Vec3d(c[0], c[1], c[2])
        self.distance=d
        idx+=1
        self.name = "Box"+str(idx)
        self.uid =idx
        
    cdef double evalFieldC(self, Vec3d p):
        return 0.0 
        
    def evalField(self, p):
        return 0.0

    def toGraph(self, p, input):
                n = p.addConstant("Vec3", [self.normal.x,self.normal.y,self.normal.z], ssaid=self.name+"_normal")
                d = p.addConstant("float", self.distance, ssaid=self.name+"_distance")
                return p.addInstruction("float", "sdPlane", [input, n, d], ssaid=self.name+"_outresult")

    def toShapeTree(self, node):
                union=node.createChild("Box")
                union.addNewData("size", "Shape", "The dimmensions of the box", "Vec3d", [self.b.x, self.b.y, self.b.z])     
                union.addNewData("ssai", "Shape", "The unique shape identifier", "s", self.name)     
                union.addNewData("ssai_vars", "Shape", "The unique shape identifier", "s", "size")                     
                return union    


cdef class Box(Shape):
    cdef Vec3d b

    def __init__(self, c):
        global idx
        self.b = Vec3d(c[0], c[1], c[2])
        idx+=1
        self.uid =idx
        self.name = "Box"+str(idx)

    cdef double evalFieldC(self, Vec3d p):
        cdef Vec3d d = p.abs() - self.b
        return d.vmax(0.0).length() + min( max(d.x,max(d.y,d.z)),0.0)

    def evalField(self, p):
        return self.evalFieldC(Vec3d(p[0],p[1], p[2]))

    def toGraph(self, p, input):
                c = p.addConstant("Vec3", [self.b.x,self.b.y,self.b.z], ssaid=self.name+"_size")
                return p.addInstruction("float", "sdBox", [input,c], ssaid=self.name+"_result")

    def toShapeTree(self, node):
                union=node.createChild("Box")
                union.addNewData("size", "Shape", "The dimmensions of the box", "Vec3d", [self.b.x, self.b.y, self.b.z])     
                union.addNewData("ssai", "Shape", "The unique shape identifier", "s", self.name)     
                union.addNewData("ssai_vars", "Shape", "The unique shape identifier", "s", "size")                     
                return union    

cdef class PythonShape(Shape):
    cdef double evalFieldC(self, Vec3d p):
        return  self.evalField((p.x, p.y, p.z))

cdef class Tiling(Shape):
    cdef Vec3d c
    cdef Shape target

    def __init__(self, target, c):
        self.c = Vec3d(c[0], c[1], c[2])
        self.target = target
        global idx
        idx+=1
        self.name = "Tiling"+str(idx)
        self.uid =idx

        
    cdef double evalFieldC(self, Vec3d p):
        cdef Vec3d q = p.mod(self.c) - self.c.mul(0.5)
        return  self.target.evalFieldC(q)
 
    def toGraph(self, p, input):
                c = p.addConstant("Vec3", [self.c.x,self.c.y,self.c.z], ssaid=self.name+"_mod")
                r = p.addInstruction("Vec3", "spaceTiling", [input,c], ssaid=self.name+"_result") 
                return self.target.toGraph(p, r)
                
    def toShapeTree(self, node):
                union=node.createChild("Tiling")
                union.addNewData("mod", "Shape", "The module of the tile", "Vec3d", [self.c.x, self.c.y, self.c.z])     
                union.addNewData("ssai", "Shape", "The unique shape identifier", "s", self.name)     
                union.addNewData("ssai_vars", "Shape", "The unique shape identifier", "s", "mod")                     
                return self.target.toShapeTree(union)
 
cdef class Twist(Shape):
    cdef double angle
    cdef Shape target

    def __init__(self, target, angle):
        self.angle = angle
        self.target = target
        global idx
        idx+=1
        self.name = "Twist"+str(idx)
        self.uid =idx

    cdef double evalFieldC(self, Vec3d p):
        f = p.y * self.angle
        cdef Vec3d q = Vec3d(  p.x*cos(f)-p.z*sin(f),
                               p.x*sin(f)+p.z*cos(f),
                               p.y ) 
        return  self.target.evalFieldC(q)   
        
    def toGraph(self, p, input):
        a=p.addConstant("float", self.angle, ssaid=self.name+"_angle")
        t = p.addInstruction("Vec3", "spaceTwist", [input, a], ssaid=self.name+"_result")
        return self.target.toGraph(p, t)   
    
    def toShapeTree(self, node):
                twist=node.createChild("Twist")
                twist.addNewData("angle", "Shape", "The angle (in rad) of rotation", "Vec3d", self.angle)     
                twist.addNewData("ssai", "Shape", "The unique shape identifier", "s", self.name)     
                twist.addNewData("ssai_vars", "Shape", "The unique shape identifier", "s", "angle")                     
                self.target.toShapeTree(twist)
                
                return twist 

cdef class Translate(Shape):
    cdef Vec3d translation
    cdef Shape target

    def __init__(self, target, t):
        self.translation = Vec3d(t[0], t[1], t[2])
        self.target = target
        global idx
        idx+=1
        self.name = "Translation"+str(idx)
        self.uid =idx

    cdef double evalFieldC(self, Vec3d p):
        return self.target.evalFieldC( p - self.translation )

    def toGraph(self, p, input):
        s=p.addConstant("Vec3", [self.translation.x,
                                 self.translation.y,self.translation.z], ssaid=self.name+"_vector")
        pos = p.addInstruction("Vec3", "sub", [input, s], ssaid=self.name+"_add")
        return self.target.toGraph(p, pos)

    def toShapeTree(self, node):
        union=node.createChild("Translate")                
        union.addNewData("translation", "Shape", "The offset factor", "Vec3d", [self.translation.x,self.translation.y,self.translation.z])     
        union.addNewData("ssai", "Shape", "The unique shape identifier", "s", self.name)     
        union.addNewData("ssai_vars", "Shape", "The unique shape identifier", "s", "translation")     
        return self.target.toShapeTree(union)  

cdef class Scale(Shape):
    cdef double factor
    cdef Shape target

    def __init__(self, target, factor):
        self.factor = factor
        self.target = target
        global idx
        idx+=1
        self.name = "Scale"+str(idx)
        self.uid =idx

    cdef double evalFieldC(self, Vec3d p):
        return self.target.evalFieldC(p.div(self.factor))*self.factor;

    def toGraph(self, p, input):
        s=p.addConstant("float", self.factor, ssaid=self.name+"_factor")
        pos = p.addInstruction("Vec3", "div", [input, s], ssaid=self.name+"_pfactor")
        res = self.target.toGraph(p, pos) 
        return p.addInstruction("float", "mul", [res, s], ssaid=self.name+"_mulpro")      

    def toShapeTree(self, node):
        union=node.createChild("Scale")                
        union.addNewData("factor", "Shape", "The offset factor", "f", self.factor)     
        union.addNewData("ssai", "Shape", "The unique shape identifier", "s", self.name)     
        union.addNewData("ssai_vars", "Shape", "The unique shape identifier", "s", "factor")     
        return self.target.toShapeTree(union)    

cdef class Offset(Shape):
    cdef double factor
    cdef Shape target

    def __init__(self, target, factor):
        self.factor = factor
        self.target = target
        global idx
        idx+=1
        self.name = "Offset"+str(idx)
        self.uid =idx

    cdef double evalFieldC(self, Vec3d p):
        return self.target.evalFieldC(p.div(self.factor))*self.factor;

    def toGraph(self, p, input):
        res = self.target.toGraph(p, input) 
        s=p.addConstant("float", self.factor, ssaid=self.name+"_offset")
        s=p.addInstruction("float", "sdOffset", [input, res, s], ssaid=self.name+"_outresult")      
        return s
        
    def toShapeTree(self, node):
        union=node.createChild("Offset")                
        union.addNewData("factor", "Shape", "The offset factor", "f", self.factor)     
        union.addNewData("ssai", "Shape", "The unique shape identifier", "s", self.name)     
        union.addNewData("ssai_vars", "Shape", "The unique shape identifier", "s", "factor")     
        return self.target.toShapeTree(union)

cdef class Rotate(Shape):
    cdef double angle
    cdef Shape target
    
    def __init__(self, target, x=None, y=None, z=None):
        self.angle = x
        self.target = target
        global idx
        idx+=1
        self.name = "Rotate"+str(idx)
        self.uid =idx

    cdef double evalFieldC(self, Vec3d p):
        cdef double c = cos(self.angle)
        cdef double s = sin(self.angle)
        cdef Vec3d pp
        pp.set(c*p.x - s*p.y, s*p.x+ c*p.y, p.z)
        return self.target.evalFieldC( pp );

    def toGraph(self, p, input):
        s=p.addConstant("float", self.angle, ssaid=self.name+"_factor")
        pos = p.addInstruction("Vec3", "rotate", [input, s], ssaid=self.name+"_pfactor")
        res = self.target.toGraph(p, pos) 
        return res
    
    def toShapeTree(self, node):
                union=node.createChild("Rotate")
                
                return self.target.toShapeTree(union)
        
cdef class Sphere(Shape):
        cdef Vec3d center
        cdef double radius
        def __init__(self, *args, **kwargs):
            global idx
            self.center = Vec3d(0,0,0)
            self.radius = 1.0
            idx+=1
            self.name = "Sphere"+str(idx)
            self.uid =idx


            if len(args) >= 1:
                self.center = Vec3d(args[0][0], args[0][1], args[0][2])

            if len(args) >= 2:
                self.radius = args[1]


            for k in kwargs:
                vv = eval(str(kwargs[k]))

                if k=="center":
                    if isinstance(vv, list):
                        self.center = Vec3d(vv[0], vv[1], vv[2])
                    else:
                        print("PROBLEM")
                elif k=="radius":
                    self.radius = vv

        def setAttr(self, d, v):
                if d == "center" and len(v) == 3:
                        self.center = Vec3d(v[0], v[1], v[2])
                elif d == "radius":
                        self.radius = float(v)

        def getAttr(self, d):
                if d == "center":
                        c=self.center
                        return [c.x, c.y, c.z] 
                elif d == "radius":
                        return self.radius 

        def setCenter(self, a):
            self.center.x = self.center.x + a

        cdef double evalFieldC(self, Vec3d p):
            return  (self.center - p).length() - self.radius

        def toGraph(self, p, input):
                r = p.addConstant("float", self.radius, ssaid=self.name+"_radius")
                c = p.addConstant("Vec3", [self.center.x,self.center.y,self.center.z], ssaid=self.name+"_center")
                return p.addInstruction("float", "sdSphere", [input,c,r], ssaid=self.name+"_result")

        def toShapeTree(self, node):
                union=node.createChild("Sphere")
                union.addNewData("center", "Shape", "The position of the center of the Sphere", "Vec3d", [self.center.x, self.center.y, self.center.z])     
                union.addNewData("radius", "Shape", "The position of the center of the Sphere", "f", self.radius)     
                union.addNewData("ssai", "Shape", "The unique shape identifier", "s", self.name)     
                union.addNewData("ssai_vars", "Shape", "The unique shape identifier", "s", "center radius")                     
                return union

cdef class Union(Shape):
        cdef Shape a
        cdef Shape b
        def __init__(self, a, b):
            global idx
            self.a = a
            self.b = b
            self.name = "Union"+str(idx)
            idx+=1
            self.uid =idx

        cdef double evalFieldC(self, Vec3d p):
                return  min(self.a.evalFieldC(p), self.b.evalFieldC(p))

        def toGraph(self,p,input):
                a = self.a.toGraph(p,input)
                b = self.b.toGraph(p,input)
                return p.addInstruction("float", "min", [a,b], ssaid=self.name+"_result")
        
        def toShapeTree(self, node):
                union=node.createChild("Union")
                union.addNewData("ssai", "Shape", "The unique shape identifier", "s", self.name)                     
                union.addNewData("ssai_vars", "Shape", "The unique shape identifier", "s", "")                     
                
                self.a.toShapeTree(union)
                self.b.toShapeTree(union)                

cdef class Difference(Shape):
        cdef Shape a
        cdef Shape b
        def __init__(self, a, b):
            global idx
            self.a = a
            self.b = b
            self.name = "Difference"+str(idx)
            idx+=1
            self.uid =idx


        cdef double evalFieldC(self, Vec3d p):
                return  max(self.a.evalFieldC(p), -self.b.evalFieldC(p))

        def toGraph(self,p,input):
                a = self.a.toGraph(p,input)
                b = self.b.toGraph(p,input)
                r = p.addInstruction("float", "-", [b]) 
                return p.addInstruction("float", "max", [r,a], ssaid=self.name+"_result")

        def toShapeTree(self, node):
                union=node.createChild("Difference")
                union.addNewData("ssai", "Shape", "The unique shape identifier", "s", self.name)                     
                union.addNewData("ssai_vars", "Shape", "The unique shape identifier", "s", "")                     
                
                self.a.toShapeTree(union)
                self.b.toShapeTree(union)                


cdef class Intersection(Shape):
        cdef Shape a
        cdef Shape b
        def __init__(self, a, b):
            self.a = a
            self.b = b
            global idx
            idx+=1            
            self.name = "Intersection"+str(idx)
            self.uid =idx


        cdef double evalFieldC(self, Vec3d p):
              return  max(self.a.evalFieldC(p), self.b.evalFieldC(p))

        def toGraph(self,p,input):
                a = self.a.toGraph(p,input)
                b = self.b.toGraph(p,input)
                return p.addInstruction("float", "max", [b,a], ssaid=self.name+"_result")

        def toShapeTree(self, node):
                inter=node.createChild("Intersection")
                inter.addNewData("ssai", "Shape", "The unique shape identifier", "s", self.name)                     
                inter.addNewData("ssai_vars", "Shape", "The unique shape identifier", "s", "")                     
                
                self.a.toShapeTree(inter)
                self.b.toShapeTree(inter)     

cdef double rawEvalFieldC(Shape tgt, double x, double y, double z):
    #with nogil:
    return tgt.evalFieldC(Vec3d(x,y,z))

cpdef double evalField(Shape tgt, x, y ,z):
    return rawEvalFieldC(tgt,x,y,z)


cdef getCythonRawFunctionC(Shape shape):
    return ( PyCapsule_New(<void*>rawEvalFieldC, "evalFunction", NULL), shape )

cpdef getCythonRawFunction(shape):
    print("INSTALLING A LOW LEVEL CYTHON HOOK TO: ")
    return getCythonRawFunctionC(shape)

