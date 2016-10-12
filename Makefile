CXX		= g++
CXXFLAGS	= -O2 -Wall 
LDFLAGS		= -lgpib
SRCFILES	= PelztierControl.cpp PelztierControlMain.cpp
OBJFILES    = PelztierControl.o PelztierControlMain.o

PelztierControl:		$(OBJFILES)
		$(CXX) $(OBJFILES) $(LDFLAGS) -o $@

clean:
		rm -f $(OBJFILES) PelztierControl 

PelztierControl.o:	PelztierControl.cpp PelztierControl.h

PelztierControlMain.o: PelztierControlMain.cpp PelztierControl.h
