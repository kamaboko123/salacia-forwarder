#!/usr/bin/python

import sys
import yaml

def err_exit(code):
    sys.stderr.write("Error!\n")
    sys.exit(code)

if len(sys.argv) < 2:
    sys.stderr.write("invalid args\n")
    sys.stderr.write("usage : %s <config_file>\n" % sys.argv[0])
    err_exit(-1)

try :
    conf_file = open(sys.argv[1], "r")
    y_data = yaml.load(conf_file)
    
except :
    sys.stderr.write("Cannot read config file.\n")
    sys.stderr.write("file name : %s\n" % sys.argv[1])
    err_exit(-1)
    
conf = y_data["config"]

output = ""
port_id = 0
port_name_list = []

port_mode = ["access", "trunk"]

for port in conf["ports"]:
    if not "name" in port.keys():
        continue
    
    if port["name"] in port_name_list :
        sys.stderr.write("[Port Duplicate] name : %s\n" % port["name"])
        err_exit(-1)
    port_name_list.append(port["name"])
    
    output += ("port-name_%d : %s\n" % (port_id, port["name"]))
    if not "mode" in port.keys():
        sys.stderr.write("parameter [mode] is not defined for port [%s]\n" % port["name"])
        err_exit(-1)
        
    output += ("port-mode_%d : %s\n" % (port_id, port["mode"]))
    
    if not port["mode"] in port_mode:
        sys.stderr.write("invalid port mode for port [%s]\n" % port["name"])
        err_exit(-1)
    
    if port["mode"] == "access":
        if not "vlan" in port.keys():
            sys.stderr.write("parameter [vlan] is not defined for port [%s]\n" % port["name"])
            err_exit(-1)
        if not port["vlan"] in range(1, 4095):
            sys.stderr.write("invalid [vlan_id] for port [%s]\n" % port["name"])
            err_exit(-1)
            
        output += ("port-vlan_%d : %s\n" % (port_id, port["vlan"]))
    
    port_id += 1

print output

