```
 o8o                                                      
 `"'                                                      
oooo   .oooo.o  .ooooo.  oo.ooooo.   .ooooo.  ooo. .oo.   
`888  d88(  "8 d88' `88b  888' `88b d88' `88b `888P"Y88b  
 888  `"Y88b.  888   888  888   888 888ooo888  888   888  
 888  o.  )88b 888   888  888   888 888    .o  888   888  
o888o 8""888P' `Y8bod8P'  888bod8P' `Y8bod8P' o888o o888o 
                          888                             
                         o888o                            
                                                          
```
isopen - a tool for reading port banners


About: 

I wrote this as a replacement for telnet. 

Originally, telnet was used to connect to hosts, but SSH has replaced that over time, because of encryption. Now, I use telnet mostly to connect to a port on a host to see if it's open and possibly what service is running on the port.  
After connecting with telnet hitting ^] and typing 'exit' is a bummer. I wrote this so I won't have to anymore.



Use:

```
$ isopen 22
Trying 127.0.0.1:22...
Connected to 127.0.0.1:22
SSH-2.0-OpenSSH_7.6p1 Ubuntu-4

$ isopen 25
Trying 127.0.0.1:25...
Connected to 127.0.0.1:25
220 mako ESMTP Postfix (Ubuntu)

$ isopen keepbusy.fun 443
Trying 35.201.65.17:443... 
Port 443 is listening, but no data recieved.
```

Compile:
`make`

Install:
(as root)
```
make install
make replace_telnet
```

Undo:
(as root)
```
make uninstall
```
