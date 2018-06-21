#!/usr/bin/env python
# -*- coding: utf-8 -*-

import Sofa

def WriteDataGraph(Node):
        DataGraphFile = open('DataGraph.dot', 'w') 
        DataGraphFile.write('digraph finite_state_machine {\n')
	DataGraphFile.write('rankdir=LR;\n')
	DataGraphFile.write('size="8,5";\n')
	DataGraphFile.write('node [shape = circle];\n')
        
        BuildGraph(Node, DataGraphFile)
        
        DataGraphFile.write('}\n')
        DataGraphFile.close()
        # run 'dot -Tsvg DataGraph.dot -o DataGraph.svg' in the terminal to generate the svg file.
    
def BuildGraph(Node, DataGraphFile):
 
        Components = Node.getObjects()
        NComponents = len(Components)
        
        for i in range(0,NComponents):
            CurrentComponent = Components[i] 
            DataFields = CurrentComponent.getListOfDataFields()
            NDataFields = len(DataFields)
            for i in range(0,NDataFields):
                HasParent = DataFields[i].hasParent()
                if HasParent:
                    #print(DataFields[i].name)
                    ReferencedDataPath = DataFields[i].getParentPath()
                    ReferencedComponentPath = DataFields[i].getLinkedComponentPath().replace('/','_')
                    ReferringComponentPath = CurrentComponent.getPathName().replace('/','_')
                    
                    DataGraphFile.write(ReferringComponentPath + ' -> ' + ReferencedComponentPath +  ' [ label = "' + DataFields[i].name + '"];\n')
                

        Children = Node.getChildren() 
        NChildren = len(Children)

        for i in range(0, NChildren):
            BuildGraph(Children[i], DataGraphFile)
            
       
        
        
class Controller(Sofa.PythonScriptController):

    def initGraph(self, node):
            self.node = node
            self.RootNode = self.node.getRoot()
	    WriteDataGraph(self.RootNode)
    
    def onBeginAnimationStep(self,deltaTime):
	   pass

    def onKeyPressed(self,c):
  
            
            if (c == "1"):
               pass
            elif (c == "2"):
               pass
            elif (c == "3"):
               pass
            elif (c == "4"):
               pass 
            elif (c == "5"):
                pass
            elif (c == "6"):
               pass
  
