#!/usr/bin/env python

import zmq

if __name__ == '__main__':

    #  _             
    # |_) _. _|_ |_  
    # |  (_|  |_ | | 
    #                
    import sys, os
    address, path_config, db_path_rel= sys.argv[1:]
    db_path = os.path.abspath(db_path_rel)


    #                                     _       
    # |_|  _. ._   _| |  _     _  _  ._ _|_ o  _  
    # | | (_| | | (_| | (/_   (_ (_) | | |  | (_| 
    #                                          _| 
    from src.chandler import get_json
    json_config = get_json(path_config)

    import src.template

    code=src.template.generate_server(json_config,db_path)

    m = src.template.importCode(code,"ZEFIO")
    d_instance = m.d_instance

    #  __                 
    # (_   _  ._    _  ._ 
    # __) (/_ | \/ (/_ |  
    #                     

    context = zmq.Context(io_threads=8)
    sock = context.socket(zmq.REP)
    sock.bind(address)
    send = sock.send

    #  _             
    # /     o ._ _|_ 
    # \_    | | | |_ 
    #    __          

    from ctypes import c_int
    c_1 = c_int(1)
    c_126 = c_int(126)
    c_0 = c_int(0)

    while  True:

        #Get the info

        l = sock.recv_multipart()

        try:
            action, str_instance, name = l
        except ValueError:
            action, str_instance, name, data = l

        instance = d_instance[str_instance]
        #Get the instance
        if action == "get":
            
            try:
                array = getattr(instance,"%s_ctype" % name)
            except IOError:
                send(c_1)
            except:
                send(c_126)
                raise
            else:
                send(c_0,zmq.SNDMORE)
                send(array, copy=False)

        elif action == "set":

            try:
                getattr(instance,"set_%s" % name)(data)
            except IOError:
                send(c_1)
            except:
                send(c_126)
                raise
            else:
                send(c_0)

        elif action == "has":
            try:
                getattr(instance,name)
            except IOError:
                send(c_1)
            except:
                send(c_126)
            else:
                send(c_0)

        else:
            raise NotImplementedError