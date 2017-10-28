# The Salacia forwarder(tentative)
This is packet forwarder with Linux raw socket.  
Currently, support only Layer 2, but this will be multi layer switch in future.  

## Features
- Layer 2 switching

## Roadmap
- support VLAN tagging
- support Layer 3 switching (Rounting for connected network)
- support static routing
- support RIPv2 (Dynamic Routing Protocol)

## How to use
Download
```
$ git clone https://bitbucket.org/salacia/rawsocket_switch.git
or
$ git clone https://github.com/kamaboko123/salacia-forwarder.git
```

Build, Run
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

## Limitation
This forwarder is not tested in except develop enviroment.  
Especially, this is not support big endian CPU.  
(Probably, this work with intel CPU.)  

## Develop environment
- CPU : intel J1900
- RAM : DDR3 4GB
- OS : Ubuntu 16.04

