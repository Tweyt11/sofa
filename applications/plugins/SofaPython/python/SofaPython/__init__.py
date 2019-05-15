import __builtin__
import sys
import os
import inspect
import traceback
import Sofa
import importlib
import git

## @contributors
##   - Matthieu Nesme
##   - Maxime Tournier
##   - damien.marchal@univ-lille1.fr
##
## @date 2017

# Keep a list of the modules always imported in the Sofa-PythonEnvironment
try:
    __SofaPythonEnvironment_importedModules__
except:
    __SofaPythonEnvironment_importedModules__ = sys.modules.copy()

    # some modules could be added here manually and can be modified procedurally
    # e.g. plugin's modules defined from c++
    __SofaPythonEnvironment_modulesExcludedFromReload = []


def unloadModules():
    """ call this function to unload python modules and to force their reload
        (useful to take into account their eventual modifications since
        their last import).
    """
    global __SofaPythonEnvironment_importedModules__
    toremove = [name for name in sys.modules if not name in __SofaPythonEnvironment_importedModules__ and not name in __SofaPythonEnvironment_modulesExcludedFromReload ]
    for name in toremove:
        del(sys.modules[name]) # unload it


def formatStackForSofa(o):
    """ format the stack trace provided as a parameter into a string like that:
        in filename.py:10:functioname()
          -> the line of code.
        in filename2.py:101:functioname1()
            -> the line of code.
        in filename3.py:103:functioname2()
              -> the line of code.
    """
    ss='Python Stack: \n'
    for entry in o:
        ss+= ' in ' + str(entry[1]) + ':' + str(entry[2]) + ':'+ entry[3] + '()  \n'
        ss+= '  -> '+ entry[4][0] + '  \n'
        return ss


def getStackForSofa():
    """returns the current stack with a "informal" formatting. """
    ## we exclude the first level in the stack because it is the getStackForSofa() function itself.
    ss=inspect.stack()[1:]
    return formatStackForSofa(ss)


def getPythonCallingPointAsString():
    """returns the last entry with an "informal" formatting. """

    ## we exclude the first level in the stack because it is the getStackForSofa() function itself.
    ss=inspect.stack()[-1:]
    return formatStackForSofa(ss)


def getPythonCallingPoint():
    """returns the tupe with closest filename & line. """
    ## we exclude the first level in the stack because it is the getStackForSofa() function itself.
    ss=inspect.stack()[1]
    tmp=(os.path.abspath(ss[1]), ss[2])
    return tmp


# returns a dictionary of all callable objects in the module, with their type as key
def getPythonModuleContent(moduledir, modulename):
#    Sofa.msg_info("PythonAsset LOADING module " + modulename + " in " + moduledir)
    objects = {}
    # First let's load that script:
    try:
        sys.path.append(moduledir)

        if modulename in sys.modules:
            del(sys.modules[modulename])
        m = importlib.import_module(modulename)
    except ImportError, e:
        print ("PythonAsset ERROR: could not import module " + modulename)
        print (e)
        return objects
    except Exception, e:
        print ("Exception: in " + modulename + ":\n" + str(e))
        return objects

    # module loaded, let's see what's inside:
#    Sofa.msg_info("Module Loaded, let's see what's inside...")
    if "createScene" in dir(m):
        # print("We found a createScene entry point, let's load it")
        objects["createScene"] = "function"
    for i in dir(m):
        if i == "SofaObject" or i == "SofaPrefab" or inspect.isbuiltin(i) or not callable(getattr(m, i)):
            continue
        if inspect.isclass(eval("m." + i)):
            # A non-decorated class
            if issubclass(eval("m." + i), Sofa.PythonScriptController):
                objects[i] = "PythonScriptController"
            elif issubclass(eval("m." + i), Sofa.PythonScriptDataEngine):
                objects[i] = "PythonScriptDataEngine"
            else:
                objects[i] = "Class"
        else:
            class_ = getattr(m, i)
            # a class decorated with @SofaPrefab:
            if class_.__class__.__name__ == "SofaPrefab" \
               and i != "SofaPrefab":
                objects[i] = "SofaPrefab"
            else:
                objects[i] = "function"
#    Sofa.msg_info(str(objects))
    return objects


import code
def getPythonModuleDocstring(mpath):
    "Get module-level docstring of Python module at mpath, e.g. 'path/to/file.py'."
    print mpath
    co = compile(open(mpath).read(), mpath, 'exec')
    if co.co_consts and isinstance(co.co_consts[0], basestring):
        docstring = co.co_consts[0]
    else:
        docstring = ""
    return str(docstring)


def sendMessageFromException(e):
    exc_type, exc_value, exc_tb = sys.exc_info()
    sofaExceptHandler(exc_type, exc_value, exc_tb)


def sofaFormatHandler(type, value, tb):
    global oldexcepthook
    """This exception handler, convert python exceptions & traceback into more classical sofa error messages of the form:
       Message Description
       Python Stack (most recent are at the end)
          File file1.py line 4  ...
          File file1.py line 10 ...
          File file1.py line 40 ...
          File file1.py line 23 ...
            faulty line
    """
    s="\nPython Stack (most recent are at the end): \n"
    for line in traceback.format_tb(tb):
        s += line

    return repr(value)+" "+s


def getSofaFormattedStringFromException(e):
    exc_type, exc_value, exc_tb = sys.exc_info()
    return sofaFormatHandler(exc_type, exc_value, exc_tb)

def sofaExceptHandler(type, value, tb):
    global oldexcepthook
    """This exception handler, convert python exceptions & traceback into more classical sofa error messages of the form:
       Message Description
       Python Stack (most recent are at the end)
          File file1.py line 4  ...
          File file1.py line 10 ...
          File file1.py line 40 ...
          File file1.py line 23 ...
            faulty line
    """
    h = type.__name__

    if str(value) != '':
        h += ': ' + str(value)
    
    s = ''.join(traceback.format_tb(tb))
    
    Sofa.msg_error(h + '\n' + s, "line", 7)

sys.excepthook=sofaExceptHandler

class Controller(Sofa.PythonScriptController):

    def __init__(self, node, *args, **kwargs):
        Sofa.msg_warning('SofaPython', 'SofaPython.Controller is intended as compatibility class only')

        # setting attributes from kwargs
        for name, value in kwargs.iteritems():
            setattr(self, name, value)

        # call createGraph for compatibility purposes
        self.createGraph(node)

        # check whether derived class has 'onLoaded'
        cls = type(self)
        if not cls.onLoaded is Sofa.PythonScriptController.onLoaded:
            Sofa.msg_warning('SofaPython',
                             '`onLoaded` is defined in subclass but will not be called in the future' )

class DataEngine(Sofa.PythonScriptDataEngine):

    def __init__(self, node, *args, **kwargs):
        Sofa.msg_warning('SofaPython', 'SofaPython.DataEngine is intended as compatibility class only')

        # setting attributes from kwargs
        for name, value in kwargs.iteritems():
            setattr(self, name, value)

        # call createGraph for compatibility purposes
        self.createGraph(node)

        # check whether derived class has 'onLoaded'
        cls = type(self)
        if not cls.onLoaded is Sofa.PythonScriptDataEngine.onLoaded:
            Sofa.msg_warning('SofaPython',
                             '`onLoaded` is defined in subclass but will not be called in the future' )


def getAbsPythonCallPath(node, rootNode):
    if rootNode.getPathName() == "/":
        return "root" + node.getPathName().replace("/", ".")[1:]
    else:
        # example:
        # rootNode.getPathName() = "/Snake/physics"
        # node.getPathName() = "/Snake/physics/visu/eye"
        # relPath = physics.visu.eye
        return rootNode.name + node.getPathName().replace(rootNode.getPathName(), "").replace("/", ".")

def buildDataParams(datas, indent, scn):
    s = ""
    for data in datas:
        if data.hasParent():
            scn[0] += indent + "### THERE WAS A LINK. "
            scn[0] += data.getParentPath() + "=>" + data.getLinkPath() + "\n"
            if data.name != "name" and data.isPersistant():
                s += ", " + data.name + "=" + repr(data.getParentPath())
        else:
            if data.name != "name" and data.isPersistant():
                if " " not in data.name and data.name != "Help":
                    if data.name != "modulepath":
                        if data.name != "depend":
                            s += ", " + data.name + "=" + repr(data.value)
    return s


def saveRec(node, indent, modules, modulepaths, scn, rootNode):
    for o in node.getObjects():
        s = buildDataParams(o.getListOfDataFields(), indent, scn)
        print 'createObject'
        scn[0] += indent + getAbsPythonCallPath(node, rootNode) + ".createObject('"
        scn[0] += o.getClassName() + "', name='" + o.name + "'" + s + ")\n"

    for child in node.getChildren():
        s = buildDataParams(child.getListOfDataFields(), indent, scn)
        if child.getData("Prefab type") is not None:
            print 'createPrefab'
            scn[0] += (indent + "####################### Prefab: " +
                       child.name + " #########################\n")
            scn[0] += (indent + child.getData("Prefab type").value +
                       "(" + getAbsPythonCallPath(node, rootNode) +
                       ".createChild('" + child.name + "'))\n")
            scn[0] += ("\n")
            modules.append(child.getData("Defined in").value)
            modulepaths.append(child.getData("modulepath").value)
        else:
            print ("createNode")
            scn[0] += (indent + "####################### Node: " + child.name +
                       " #########################\n")
            scn[0] += (indent + getAbsPythonCallPath(node, rootNode) +
                       ".createChild('" + child.name + "')\n")
            saveRec(child, indent, modules, modulepaths, scn, rootNode)
            scn[0] += ("\n")


def getRelPath(path, relativeTo):
    # example:
    # path =       /home/bruno/dev/myproject/scripts
    # relativeTo = /home/bruno/dev/myproject/scenes/MyCoolFile.py
    # return "../scripts/"
    if os.path.isfile(relativeTo):
        relativeTo = os.path.dirname(relativeTo)

    if path == relativeTo:
        return ""
    commonPrefix = os.path.commonprefix([path, relativeTo])
    path = path.replace(commonPrefix, "")  # /scripts
    if path.startswith('/'):
        path = path[1:]  # scripts
    relativeTo = relativeTo.replace(commonPrefix, "")  # scenes
    newPath = ""
    if relativeTo != "":
        newPath += "../"
        for d in relativeTo.split('/'):
            newPath += "../"

    newPath += path
    return newPath


def saveAsPythonScene(fileName, node):
    try:
        root = node.getRootContext()
        fd = open(fileName, "w+")

        fd.write("import sys\n")
        fd.write("import os\n")

        modules = []
        modulepaths = []
        scn = [""]
        saveRec(root, "\t", modules, modulepaths, scn, root)

        fd.write("# all Paths\n")
        for p in list(dict.fromkeys(modulepaths)):
            if p != "":
                fd.write("sys.path.append('" + getRelPath(p, fileName) +
                         "')\n")

        fd.write('# all Modules:\n')
        for m in list(dict.fromkeys(modules)):
            fd.write("from " + m + " import *\n")

        fd.write("\n\ndef createScene(root):\n")
        fd.write(scn[0])
        return True
    except Exception, e:
        Sofa.msg_error(e)
        return False


def createPrefabFromNode(fileName, node, name, help):
    try:
        print 'Saving prefab'
        fd = open(fileName, "w+")
        fd.write("import sys\n")
        fd.write("import os\n")

        modules = []
        modulepaths = []
        scn = [""]
        saveRec(node, "\t\t", modules, modulepaths, scn, node)

        fd.write("# all Paths\n")
        for p in list(dict.fromkeys(modulepaths)):
            fd.write("sys.path.append('" + getRelPath(p, fileName) + "')\n")

        fd.write('# all Modules:\n')
        for m in list(dict.fromkeys(modules)):
            fd.write("from " + m + " import *\n")

        fd.write("\n\n@SofaPrefab\n")
        fd.write("class " + name + "():\n")
        fd.write("\t\"\"\" " + help + " \"\"\"\n")
        fd.write("\tdef __init__(self, " + node.name + "):\n")
        fd.write("\t\tself.node = " + node.name + "\n")
        fd.write(scn[0])
        return True
    except Exception, e:
        Sofa.msg_error(e)
        return False


def loadMeshAsset(type, path, node):
    loader = node.createObject(type, name="loader", filename=path)
    vmodel = node.createObject("OglModel", name="vmodel", src=loader.getLinkPath())
    return node


def loadPythonAsset(moduledir, modulename, prefabname, node):
#    print "#############   LOADING PYTHON ASSET"
#    print "#############   moduledir: " + moduledir
#    print "#############   modulename: " + modulename
#    print "#############   prefabname: " + prefabname
#    print "#############   node.name: " + node.name
    functions = {}
    pythonScripts = {}
    classes = {}
    prefabs = {}

    # First let's load that script:
    try:
        sys.path.append(moduledir)
        m = importlib.import_module(modulename)
    except ImportError, e:
        Sofa.msg_error("PythonAsset ERROR: could not import module " + modulename)
        Sofa.msg_error(e)

    # module loaded, let's see what's inside:
    if "createScene" in dir(m):
        # print("We found a createScene entry point, let's load it")
        functions["createScene"] = m.createScene
    for i in dir(m):
        if i == "SofaObject" or i == "SofaPrefab":
            continue
        if inspect.isclass(eval("m." + i)):
            # A non-decorated class
            if issubclass(eval("m." + i), Sofa.PythonScriptController) or \
               issubclass(eval("m." + i), Sofa.PythonScriptDataEngine):
                pythonScripts[i] = getattr(m, i)
            else:
                classes[i] = getattr(m, i)
        else:
            class_ = getattr(m, i)
            # a class decorated with @SofaPrefab:
            if class_.__class__.__name__ == "SofaPrefab" \
               and i != "SofaPrefab":
                prefabs[i] = class_
            elif callable(class_) and not inspect.isbuiltin(i):
                functions[i] = class_

    # No Prefab name passed as argument. Loading 1 prefab:
    if "" == prefabname:
        print ('No prefab name provided. Loading by order of priority')
        # 1st, createScene if it exists:
        if 'createScene' in functions.keys():
            functions['createScene'](node)
            return node
        # 2nd, call the first prefab available:
        if len(prefabs) > 0:
            prefabs.items()[0][1](node)
            return node
        # 3rd, call the first PythonScript:
        if len(pythonScripts) > 0:
            pythonScripts.items()[0][1](node)
            return node
        # 4th, call the first class:
        if len(classes) > 0:
            classes.items()[0][1](node)
            return node
        # finally, try to call the first python function available:
        if len(functions) > 0:
            functions.items()[0][1](node)
            return node
        print ("PythonAsset ERROR: No callable found in " + str(m))
    else:
        pyName = prefabname
        print "#############" + prefabname
        try:
            if pyName == "createScene":
                print 'Loading ' + pyName
                functions["createScene"](node)
            elif pyName in prefabs.keys():
                print 'Loading ' + pyName
                prefabs[pyName](node)
            elif pyName in pythonScripts.keys():
                print 'Loading ' + pyName
                pythonScripts[pyName](node)
            elif pyName in classes.keys():
                print 'Loading ' + pyName
                classes[pyName](node)
            elif pyName in functions.keys():
                print 'Loading ' + pyName
                functions[pyName](node)
            else:
                print ("PythonAsset ERROR: No callable object with name " + pyName)
        except Exception, e:
            Sofa.msg_error(node, e)
    return node

