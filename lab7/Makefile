# Make file for Lab 4b

lab7.bin:	lab7final.s
		nasm lab7final.s -o lab7.bin -l lab7.lst

lab7final.s:	clib.s myisr.s myinth.s yaks.s lab7_app.s yakc.s
		cat clib.s myisr.s myinth.s yakc.s yaks.s lab7_app.s > lab7final.s

myinth.s:	myinth.c
		cpp myinth.c myinth.i
		c86 -g myinth.i myinth.s

lab7_app.s:	lab7app.c yakk.h
		cpp lab7app.c lab7_app.i
		c86 -g lab7_app.i lab7_app.s

yakc.s:		yakc.c yakk.h
		cpp yakc.c yakc.i
		c86 -g yakc.i yakc.s

clean:
		rm lab7.bin lab7.lst lab7final.s \
		myinth.s myinth.i \
		yakc.s yakc.i \
		lab7_app.s lab7_app.i
