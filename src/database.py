import os
import gzip

def read_array(path_gz):

    if not os.path.isfile(path_gz):
      raise IOError("%s doesn't exist"% path_gz)

    import tempfile
    import io
    import subprocess

    tmpdir = tempfile.mkdtemp()
    tmp_fifo = os.path.join(tmpdir, 'fifo')

    os.mkfifo(tmp_fifo)
            
    try:   
        p = subprocess.Popen("gzip --stdout -d %s > %s" % (path_gz,tmp_fifo), shell=True)
        f = io.open(tmp_fifo, "r")
        
        data = []
        while True:
            line=f.readline()
            if not line:
                break
            else:
                data.append(line.strip())
    finally:
        f.close()
        p.wait()
        os.remove(tmp_fifo)
        os.rmdir(tmpdir)

    return data

def read_scalar(path):

    if not os.path.isfile(path):
      raise IOError("%s doesn't exist"% path)

    with open(path, 'r') as f:
      data = f.read().strip()
    return data



class LegacyFolderHieracy(object):

    def __init__(self, path):
        self.path = path

    def read(self,instance,category,name):
      """Read str"""

      dimension=getattr(instance,"%s_dimension"%name)
      path=os.path.join(self.path,category,name)

      if isinstance(dimension, (list, tuple)):
        data = read_array("%s.gz" % path)[2:]
      else:
        data = read_scalar(path)
      return data

    def write(self,instance,category,name,py_data):

      dimension=getattr(instance,"%s_dimension"%name)
      path=os.path.join(self.path,category,name)

      if not os.path.exists(os.path.dirname(path)):
          try:
              os.makedirs(os.path.dirname(path))
          except OSError as exc: # Guard against race condition
              if exc.errno != errno.EEXIST:
                  raise

      if isinstance(dimension, (list, tuple)):
        header = [str(len(dimension)), " "*4 +" ".join(map(str,dimension))]
        data_str = [value for value in map(str,py_data)]

        with gzip.open("%s.gz" % path, "wb") as f:
            f.write("\n".join(header+data_str)+"\n")
      else:
        data_str_flaten = str(py_data).strip()
        with open(path, "w") as f:
          f.write(data_str_flaten+"\n")
