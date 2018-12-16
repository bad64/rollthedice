# Roll the dice !

A super lightweight dice rolling program like you can find a lot of nowadays on Discord. Except this one is mine and written in C, as part of my experimentation on shared libraries.

main.c is merely a sample program that interfaces with the library, where the *true* magic happens. The lib comes with a help screen and callable random facility ~~that I took from StackOverflow~~.

# Installation

In case you don't know how a makefile works (hey, we've all started somewhere, haven't we ?), here is the rundown:

Linux: make && sudo make install && make clean  
Windows: mingw32-make -f Makefile.win && mingw32-make clean -f Makefile.win

Whatever you do, **do not pass any optimization flags (-O1, -O2)** to either makefile. I'm unsure why but this breaks the library thusly created.

**But what if I don't want your sample application ?**

Fear not, citizen, for I have thought of such a situation. You simply need to invoke `make lib` (or `mingw32-make lib` on Windows) to only produce the relevant shared object, which you can do as you please with.