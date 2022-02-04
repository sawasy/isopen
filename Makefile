isopen : src/isopen.c 
	gcc src/isopen.c -o isopen

clean : 
	rm isopen

install : 
	cp isopen /usr/local/bin
	if [ ! -d "/usr/local/share/man/man1" ]; then mkdir /usr/local/share/man/man1; fi
	cp man/isopen.1 /usr/local/share/man/man1

uninstall :
	rm /usr/local/bin/isopen
	rm /usr/local/share/man/man1/isopen.1

replace_telnet :
	mv /usr/bin/telnet /usr/bin/telnet-orig
	ln -s /usr/local/bin/isopen /usr/bin/telnet
