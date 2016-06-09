from jinja2 import Environment, FileSystemLoader

import os
file_dir = os.path.dirname(os.path.abspath(__file__))
template_dir = os.path.join(file_dir,"..","templates")
env = Environment(loader=FileSystemLoader(template_dir))

# Custom filter method
def to_assumed_shape(array):
    """ [x,y,...] -> (:,:,...)"""
    return "({0})".format(",".join([":"]*( array.count(",")+1)))


def generate_server(json_config,db_path):

    template = env.get_template('server.jinja2.py').render(json_config=json_config,
                                                           db_path=db_path)
    return template

def generate_fortran(json_config):

    from src.convert import typec2stuff 

    env.filters['to_assumed_shape'] = to_assumed_shape
    template = env.get_template('fortran.jinja2').render(json_config=json_config,
                                                         typec2stuff=typec2stuff)
    return template

def importCode(code,name):

    import sys,imp

    module = imp.new_module(name)
    compiled_code = compile(code, '<string>', 'exec')
    exec compiled_code in module.__dict__

    return module
