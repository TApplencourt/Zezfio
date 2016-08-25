import os
from zezfio.convert import str2py

from ctypes import cdll, c_int, POINTER, byref

path_dll = os.path.join(os.path.dirname(__file__),'c','gz2ar.so')
dll = cdll.LoadLibrary(path_dll)

class LegacyFolderHieracy(object):
    def __init__(self, root_path):

        if not os.path.isdir(root_path):
            raise  IOError("Error: %s is not a directory" % root_path)

        self.root_path = root_path

    def read_scalar(self,category,name,type_):
        "Return Py Object"

        path = os.path.join(self.root_path, category, name)
        if not os.path.isfile(path):
            raise IOError("%s doesn't exist" % path)
    
        with open(path, 'r') as f:
            data = f.read().strip()

        return str2py(data, type_)

    def read_array(self, category, name, type_, dimension):
    
        path = os.path.join(self.root_path, category, name+".gz")
        if not os.path.isfile(path):
            raise IOError("%s doesn't exist" % path)

        error_code = c_int()
        try:
            dll.ezfio_extract_int.restype = POINTER(c_int * dimension)
        except OverflowError:
            raise OverflowError("Check the dimension of the array (%s)"%dimension)
    
        data = dll.ezfio_extract_int(path,dimension, byref(error_code)).contents
        
        if error_code.value:
            raise RuntimeError("Error when running the C code.")
    
        return data