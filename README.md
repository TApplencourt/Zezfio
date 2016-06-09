# Boilerplate

## Generate the config file
./legacy2json.py nuclei.ezfio_interface_config > nuclei.json

## Generate the fortran file
./honda.py nuclei.json > fortran/zezfio.f90

#Run

## Python
./zezfio.py <address> <path_config> <db_path_rel>

## Fortran

- Link with zezfio.f90
- export ZEZFIOADR=<adress>
- Run 