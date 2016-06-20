![](http://i.imgur.com/XUeaDoy.gif)

The lovely client/server version of [Ezfio](http://irpf90.ups-tlse.fr/index.php?title=EZFIO)


# Dependency
- [IRPy](https://github.com/TApplencourt/IRPy)
- [Jinja2](http://jinja.pocoo.org/docs/dev/)
- [Zeromq](http://zeromq.org/)
- [Pyzmq](https://github.com/zeromq/pyzmq)
- [f77_zmq](https://github.com/zeromq/f77_zmq)

You can use [pip](https://pip.pypa.io/en/stable/installing/) to install `zezfio` and all the Python Dependencies and 
- `pip install zezfio`

Or you can use [conda](http://conda.pydata.org/miniconda.html) to install the Python Dependencies (+ ZMQ)
- `conda install -c https://conda.anaconda.org/tapplencourt jinja2 pyzmq  irpy`

*PS: You can replace the name of the package by the `tar.gz` precedingly downloaded from the [anaconda cloud](https://anaconda.org)*

# Boilerplate


## Generate the config file
```bash
./zezfio_legacy2json <legacy_config> > <json_config>
```

## Generate and Compile the fortran file

- Honda is the best electric (and fortran) generator around!
```bash
./zezfio_honda <json_config> > <file.f90>
```
- Compile the library (just don't forget to include f77_zmq.h)
- Link it with your `main` fortran file

# Run Forrest, Run!

## Python server (Paris is the best city in the world regard too waiter)
```bash
./zezfio_paris <address> <json_config> <db_path>
```

## Fortran

- `export EZFIO_ADDRESS=<address>`
- Exec your main as usual

# Code architecture

```
   +---------------------+
   | {{ variable.name }} |
   +---------------------+
   | Value of variable   |   +-----------------+
   | in Python format    |                     |
   +---------------------+                     |
              +                                |
              |                                |
              v                                v
  +-----------------------+    +----------------------------------------+
  | {{ variable.name }}_c |    | {{ aonother_variable.name }}_dimension |
  +-----------------------+    +----------------------------------------+
  | Value of variable     |    | Dimension of the variable in Python    |
  | in C format           |    | format                                 |
  +-----------------------+    +----------------------------------------+
             +
             |
             v
 +--------------------------+
 | {{ variable.name }}_csze |
 +--------------------------+
 | Number of bytes in the   |
 | C format variable        |
 +--------------------------+
```
