.PHONY: clean lock-test

DEFAULT: lock-test
# Define the tools we are going to use
CC= g++
LD = g++

LIBS = -lm -lpthread -lrt -latomic
CFLAGS = -g -Wall -Wno-unused -Wno-sign-compare -std=c++11 -I. -O3
OBJDIR = ../obj

OBJS += $(OBJDIR)/lock_test.o

JUNK = *~ $(OBJDIR)/*.o $(OBJDIR)/*.a $(OBJDIR)/*_ut  *-test

lock-test:  $(OBJDIR)/lock_test.o
	$(LD) $(CFLAGS) -o $@ $^ $(LIBS)
	./lock-test

$(OBJDIR)/lock_test.o: lock_test.cc
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f  test $(JUNK)