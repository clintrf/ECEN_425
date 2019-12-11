# Make file for Lab 4b

lab8.bin:	lab8final.s
		nasm lab8final.s -o lab8.bin -l lab8.lst
		
lab8final.s:	clib.s myisr.s myinth.s yaks.s simptris.s lab8app.s yakc.s
		cat clib.s simptris.s myisr.s myinth.s yakc.s yaks.s lab8app.s > lab8final.s

myinth.s:	myinth.c
		cpp myinth.c myinth.i
		c86 -g myinth.i myinth.s

lab8_app.s:	lab8app.c yakk.h
		cpp lab8app.c lab8_app.i
		c86 -g lab8_app.i lab8_app.s

yakc.s:		yakc.c yakk.h
		cpp yakc.c yakc.i
		c86 -g yakc.i yakc.s

clean:
		rm lab8.bin lab8.lst lab8final.s \
		myinth.s myinth.i \
		yakc.s yakc.i \
		lab8_app.s lab8_app.i
