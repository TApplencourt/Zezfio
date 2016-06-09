#!/usr/bin/env python

import zmq
import time
from hurry.filesize import size
import statistics

if __name__ == '__main__':
    from ctypes import c_int

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

    c_1 = c_int(1)
    c_126 = c_int(126)
    c_0 = c_int(0)

    send = sock.send

    start_time = time.time()
    message_size = 0
    message_cnt = 0
    l_message_sze = []


    while  True:
        message = sock.recv()


        #Get the info        
        action, file = message.split()
        str_instance, name = file.split(".")

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

                try:
                    message_size += array.buffer_info()[1] * array.itemsize
                except:
                    message_size += 4

                message_cnt += 1

        elif action == "set":

            data = sock.recv()

            try:
                getattr(instance,"set_%s" % name)(data)
            except IOError:
                sock.send(c_1)
            except:
                sock.send(c_126)
                raise
            else:
                sock.send(c_0)

        elif action == "has":
            try:
                getattr(instance,name)
            except IOError:
                sock.send(c_1)
            except:
                sock.send(c_126)
            else:
                sock.send(c_0)

        else:
            raise NotImplementedError

        elapsed_time = time.time() - start_time

        if elapsed_time > 1:

            l_message_sze.append(message_size)
            mu = statistics.mean(l_message_sze)
            try:
                var = statistics.stdev(l_message_sze)
            except  statistics.StatisticsError:
                var = 0

            print "%s/s (%s/s +/- %s) -  %s"%(size(message_size),size(mu), size(var), message_cnt)
            message_size = 0
            message_cnt = 0
            start_time = time.time()