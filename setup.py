#from distutils.core import setup
from setuptools import setup

setup(
  name = 'zezfio',
  packages = ['zezfio'],
  package_data={'zezfio': ['templates/*']},
  version = '0.3',
  description = 'The lovely client/server version of Ezfio',
  author = 'Thomas applencourt',
  author_email = 'applencourtirsamc.ups-tlse.fr',
  url = 'https://github.com/TApplencourt/Zezfio',
  download_url = 'https://github.com/TApplencourt/Zezfio/archive/master.zip', # I'll explain this in a second
  keywords = ['programming', 'fortran', 'EZFIO'], # arbitrary keywords
  classifiers = [],
  scripts = ["zezfio_honda", "zezfio_legacy2json", "zezfio_paris"],
  install_requires=['irpy','pyzmq', 'jinja2'],
)
