# Make file for Lab 4b

lab6.bin:	lab6final.s
		nasm lab6final.s -o lab6.bin -l lab6.lst

lab6final.s:	clib.s myisr.s myinth.s yaks.s lab6_app.s yakc.s
		cat clib.s myisr.s myinth.s yakc.s yaks.s lab6_app.s > lab6final.s

myinth.s:	myinth.c
		cpp myinth.c myinth.i
		c86 -g myinth.i myinth.s

lab6_app.s:	lab6app.c yakk.h
		cpp lab6app.c lab6_app.i
		c86 -g lab6_app.i lab6_app.s

yakc.s:		yakc.c yakk.h
		cpp yakc.c yakc.i
		c86 -g yakc.i yakc.s

clean:
		rm lab6.bin lab6.lst lab6final.s \
		myinth.s myinth.i \
		yakc.s yakc.i \
		lab6_app.s lab6_app.i
