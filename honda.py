#!/usr/bin/env python
if __name__ == '__main__':

    #  _             
    # |_) _. _|_ |_  
    # |  (_|  |_ | | 
    #                
    import sys, os
    path_config = sys.argv[1]


    #                                     _       
    # |_|  _. ._   _| |  _     _  _  ._ _|_ o  _  
    # | | (_| | | (_| | (/_   (_ (_) | | |  | (_| 
    #                                          _| 
    from src.chandler import get_json
    json_config = get_json(path_config)

    import src.template
    print src.template.generate_fortran(json_config)
