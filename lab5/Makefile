# Make file for Lab 4b

lab5.bin:	lab5final.s
		nasm lab5final.s -o lab5.bin -l lab5.lst

lab5final.s:	clib.s myisr.s myinth.s yaks.s lab5_app.s yakc.s
		cat clib.s myisr.s myinth.s yakc.s yaks.s lab5_app.s > lab5final.s

myinth.s:	myinth.c
		cpp myinth.c myinth.i
		c86 -g myinth.i myinth.s

lab5_app.s:	lab5_app.c yakk.h
		cpp lab5_app.c lab5_app.i
		c86 -g lab5_app.i lab5_app.s

yakc.s:		yakc.c yakk.h
		cpp yakc.c yakc.i
		c86 -g yakc.i yakc.s

clean:
		rm lab5.bin lab5.lst lab5final.s \
		myinth.s myinth.i \
		yakc.s yakc.i \
		lab5_app.s lab5_app.i
