from collections import namedtuple
import array
from ctypes import c_char, c_int, c_long, c_float, c_double

def bool_to_int(b):
  if b in 'Tt1':
     return 1
  elif b in 'Ff0':
     return 0
  else:
     raise TypeError("bool_to_int : %s should be in (T|F)"%b)

Type = namedtuple("Type", ["code", "size", "python", "c_type", "fortran"])
typec2stuff = {
    "char[512]": Type('c', 512, lambda s: "%s"%(s.ljust(512)), c_char, "CHARACTER*(512)"),
    "char[256]": Type('c', 256, lambda s: "%s"%(s.ljust(256)), c_char, "CHARACTER*(256)"),
    "char[128]": Type('c', 128, lambda s: "%s"%(s.ljust(128)), c_char, "CHARACTER*(128)"),
    "char[64]" : Type('c',  64, lambda s: "%s"%(s.ljust(64)),  c_char, "CHARACTER*(64)"),
    "char[32]" : Type('c',  32, lambda s: "%s"%(s.ljust(32)),  c_char, "CHARACTER*(32)"),
    "char[16]" : Type('c',  16, lambda s: "%s"%(s.ljust(16)),  c_char, "CHARACTER*(16)"),
    "char[8]"  : Type('c',   8, lambda s: "%s"%(s.ljust(8)),   c_char, "CHARACTER*(8)"),
    "char[4]"  : Type('c',   4, lambda s: "%s"%(s.ljust(4)),   c_char, "CHARACTER*(4)"),
    "char[2]"  : Type('c',   2, lambda s: "%s"%(s.ljust(2)),   c_char, "CHARACTER*(2)"),
    "char"     : Type('c',   1, str,                           c_char, "CHARACTER"),
    "bool"     : Type('i',   4, bool_to_int,                   c_int, "LOGICAL"),
    "int"      : Type('i',   4, int,                           c_int, "INTEGER"),
    "long"     : Type('l',   8, int,                           c_long, "INTEGER*8"),
    "float"    : Type('f',   4, float,                         c_float, "REAL"),
    "double"   : Type('d',   8, float,                         c_double, "DOUBLE PRECISION")
}

def data_str2py(str_data, type_):
    fct = typec2stuff[type_].python
    if isinstance(str_data, (list, tuple)):
        return map(fct, str_data)
    else:
        return fct(str_data)

def data_py2c(py_data, type_):
    code = typec2stuff[type_].code

    if isinstance(py_data, (list, tuple)):
        if code == "c":
            size = typec2stuff[type_].size
            return array.array(code, "".join(d.ljust(size) for d in py_data))
        else:
            return array.array(code, py_data)
    else:
        if code == "c":
            return array.array(code, py_data)
        else:
            return array.array(code, [py_data])


def data_c2py(ar, dimension):
    " array -> python "

    if isinstance(dimension, (list, tuple)):
        return ar.tolist()
    else:
        return ar.tolist().pop()

def data_mv2c(machine_value, type_):
    " machine code -> array"
    code = typec2stuff[type_].code
    return array.array(code, machine_value)


def dimension2len(dimension):
    from operator import mul
    return reduce(mul, dimension) if isinstance(dimension, (list, tuple)) else 1

def type_fortran2c(ftype):
    for ctype, t in typec2stuff.iteritems():
        if ftype.lower() == t.fortran.lower():
            return ctype
    raise AttributeError, "Not C type for Fortran type: %s" % ftype
