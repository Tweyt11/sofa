"""type: SofaContent"""

import Sofa.Core

class UController(Sofa.Core.Controller):
        def __init__(self, target, *args, **kwargs):
                Sofa.Core.Controller.__init__(self, *args, **kwargs)
                self.target = target
                self.lastTime = 0

        def onAnimateBeginEvent(self, params):
                self.lastTime += params["dt"]
                # change the value
                self.target.points.value = [[self.lastTime,0.0,0.0]]*10
                
                # force the update. 
                len(self.target.values.value)

def Settings(name="Settings"):
        setting = Sofa.Core.Node(name)
        setting.add("RequiredPlugin", name="NodePhysics")
        setting.add("RequiredPlugin", name="SofaImplicitField")
        setting.add("RequiredPlugin", name="SofaImplicitField3")
        setting.add("RequiredPlugin", name="SofaOpenCL")        
        return setting

def Shape(name="Shape"):
        def fieldFunction(x,y,z):
                return 3.0
        shape = Sofa.Core.Node(name)
        c=shape.add("CustomField", name="field")
        c.pyEvalFunction.value = fieldFunction 
        return shape

def OpenCL(field, name="OpenCL"):
        self = Sofa.Core.Node(name)
        fs = self.add("NodePhysics.FileSource", filename="kernel.cl")
        l = self.add("NodePhysics.TextLoader", filename=fs.filename)
        self.add("OpenCLScalarField", name="afield", kernelCode=l.content, points=[[0.0,0.0,0.0]]*10)
        self.add(UController, target=field)
        return self

def createScene(root):
        root.add(Settings)
        root.add(Shape)
        root.add(OpenCL, field=root.Shape.field)
