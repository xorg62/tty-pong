files = ttypong.c

ttypong : $(files) ttypong.h -lncurses
	    cc -o ttypong $(files) -lncurses

.PHONY : clean
clean :
	-rm ttypong
