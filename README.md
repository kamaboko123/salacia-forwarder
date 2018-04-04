# The Salacia forwarder
This is packet forwarder with Linux raw socket.  
Currently, support only Layer 2, but this will be multi layer switch in future.  

## Features
- Layer 2 switching
- VLAN tagging(test version)

## Roadmap
- support Layer 3 switching (Rounting for connected network)
- support static routing
- support RIPv2 (Dynamic Routing Protocol)

## How to use
### 1. Download
```
$ git clone https://github.com/kamaboko123/salacia-forwarder.git
```

### 2. Build, Run

#### L2SW(no VLAN)
```
$ cd salacia-forwarder/L2SW
$ make
$ ./bin/l2sw enp3s0 enp4s0
```
You need to specify interfaces as argument.  
The interfaces is need to set promiscuous mode.  
Example:
```
$ ifconfig enp3s0 promisc
$ ifconfig enp4s0 promisc
```

#### L2SW with VLAN(test version)
VLAN function is supported as test implementation.  
Currently, configuration is only supported by hardcode.  
You need to configuration by edit source code.  
```
$ cd salacia-forwarder/forwarder/test/vlan_switch
# Please edit source code for your environment
$ vim vlan_switch.cpp
$ make
$ bin/switch
```

## Limitation
This forwarder is not tested in except develop enviroment.  
Especially, this is not support big endian CPU.  
(Probably, this work with intel CPU.)  

## Develop environment
- CPU : intel J1900
- RAM : DDR3 4GB
- OS : Ubuntu 16.04

