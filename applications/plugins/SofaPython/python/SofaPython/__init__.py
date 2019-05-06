import __builtin__
import sys
import inspect
import traceback
import Sofa
import importlib

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
    tmp=(ss[1], ss[2])
    return tmp


# returns a dictionary of all callable objects in the module, with their type as key
def getPythonModuleContent(moduledir, modulename):
    objects = {}
    # First let's load that script:
    try:
        sys.path.append(moduledir)
        m = importlib.import_module(modulename)
    except ImportError, e:
        print ("PythonAsset ERROR: could not import module " + modulename)
        print (e)
        return objects
    except Exception, e:
        print ("Exception: in " + modulename + ":\n" + str(e))
        return objects

    # module loaded, let's see what's inside:
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
    return objects


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


def getAbsPythonCallPath(node):
    return "root" + ("." if len(node.getPathName()) > 1 else "") + node.getPathName().replace("/", ".")[1:]

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


def saveRec(node, indent, modules, scn):
    for o in node.getObjects():
        s = buildDataParams(o.getListOfDataFields(), indent, scn)
        print 'createObject'
        scn[0] += indent + getAbsPythonCallPath(node) + ".createObject('"
        scn[0] += o.getClassName() + "', name='" + o.name + "'" + s + ")\n"

    for child in node.getChildren():
        s = buildDataParams(child.getListOfDataFields(), indent, scn)
        if child.getData("Prefab type") is not None:
            print 'createPrefab'
            scn[0] += (indent + "####################### Prefab: " +
                       child.name + " #########################\n")
            scn[0] += (indent + child.getData("Prefab type").value +
                       "(" + getAbsPythonCallPath(node) + ".createChild('" +
                       child.name + "'))\n")
            scn[0] += ("\n")
            print ('addModule')
            modules.append('from ' + child.getData("Defined in").value
                           + ' import *\n')
        else:
            print ("createNode")
            scn[0] += (indent + "####################### Node: " + child.name +
                       " #########################\n")
            scn[0] += (indent + getAbsPythonCallPath(node) + ".createChild('" + child.name
                       + "')\n")
            saveRec(child, indent, modules, scn)
            scn[0] += ("\n")


def saveAsPythonScene(fileName, node):
    try:
        root = node.getRootContext()
        fd = open(fileName, "w+")

        fd.write("# all Paths\n")
        fd.write("import sys\n")
        fd.write("import os\n")
        for p in list(dict.fromkeys(sys.path)):
            if p.startswith('/usr/lib/') or p.startswith('/usr/local/lib'):
                continue
            if p == '':
                continue
            fd.write(("sys.path.append('" + str(p) + "')\n"))

        modules = []
        scn = [""]
        saveRec(root, "\t", modules, scn)

        fd.write('# all Modules:\n')
        for m in modules:
            fd.write(m)
        fd.write("\n\ndef createScene(root):\n")
        fd.write(scn[0])
        return True
    except Exception, e:
        print (e)
        return False
