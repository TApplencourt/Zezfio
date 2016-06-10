![](http://i.imgur.com/XUeaDoy.gif)

The client/server version of Ezfio (http://irpf90.ups-tlse.fr/index.php?title=EZFIO)

# Dependency
- IRPy (https://github.com/TApplencourt/IRPy)
`pip install IRPy`
- Jinja2 (http://jinja.pocoo.org/docs/dev/)
`pip install Jinja2`
- Zeromq (http://zeromq.org/)
- Pyzmq (https://github.com/zeromq/pyzmq)
`pip install pyzmq`
- f77_zmq (https://github.com/zeromq/f77_zmq)

# Boilerplate

## Generate the config file
```bash
./legacy2json.py <legacy_config> > <json_config>
```

## Generate and Compile the fortran file

- Honda is the best electric (and fortran) generator around!
```bash
./honda.py <json_config> > <file.f90>
```
- Compile the library (just don't forget to link with lib_zmq.f90)
- Link her with your `main` fortran file

# Run Forrest, Run!

## Python server
```bash
./zezfio.py <address> <json_config> <db_path>
```

## Fortran

-`bash export EZFIO_ADDRESS=<address>`
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
