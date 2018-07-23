# distutils: language=c++
from SofaEditor cimport SofaEditor, SofaEditorState
                
def createId():
        return SofaEditor.createId(NULL)

def getCurrentSelection():
        cdef const SofaEditorState* state = SofaEditor.getState(0)
        if state == NULL:
            return []
        return state.getSelection()

def toto():
        return None

