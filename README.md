# Boilerplate

## Generate the config file
./legacy2json.py <legacy_config> > <json_config>

## Generate the fortran file
./honda.py <json_config> > <file.f90>

#Run

## Python server
./zezfio.py <address> <json_config> <db_path>

## Fortran

- Link with zezfio.f90
- export ZEZFIOADR=<adress>
- Run 
