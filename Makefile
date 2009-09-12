files = ttypong.c

ttypong : $(files) -lncurses
	    cc -o ttypong $(files) -lncurses

.PHONY : clean
clean :
	-rm ttypong
