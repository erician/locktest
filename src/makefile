.PHONY: clean  lock-test rwlock-test cas-test thread-clock-test

DEFAULT: lock-test
# Define the tools we are going to use
CC= g++
LD = g++

LIBS = -lm -lpthread -lrt -latomic
CFLAGS = -g -Wall -Wno-unused -Wno-sign-compare -std=c++17 -I. -O3
OBJDIR = ../obj

OBJS += $(OBJDIR)/lock_test.o

JUNK = *~ $(OBJDIR)/*.o $(OBJDIR)/*.a $(OBJDIR)/*_ut  *-test

thread-clock-test: $(OBJDIR)/thread_clock_test.o
	$(LD) $(CFLAGS) -o $@ $^ $(LIBS)
	./thread-clock-test

lock-test:  $(OBJDIR)/lock_test.o
	$(LD) $(CFLAGS) -o $@ $^ $(LIBS)
	./lock-test

cas-test:  $(OBJDIR)/cas_test.o
	$(LD) $(CFLAGS) -o $@ $^ $(LIBS)
	./cas-test

rwlock-test:  $(OBJDIR)/rwlock_test.o
	$(LD) $(CFLAGS) -o $@ $^ $(LIBS)
	./rwlock-test

$(OBJDIR)/thread_clock_test.o: thread_clock_test.cc
	$(CC) $(CFLAGS) -c -o $@ $<
$(OBJDIR)/lock_test.o: lock_test.cc
	$(CC) $(CFLAGS) -c -o $@ $<
$(OBJDIR)/cas_test.o: cas_test.cc
	$(CC) $(CFLAGS) -c -o $@ $<
$(OBJDIR)/rwlock_test.o: rwlock_test.cc
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f  test $(JUNK)