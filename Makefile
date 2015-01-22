CC := gcc
CFLAGS := -std=gnu99 -O2 -Wall
LDFLAGS := -std=gnu99
RM := rm

#
# Define our target app.
#
APP := hrh

#
# Define the source files for our build.
#
HRH_SRCS := hrh.c

#
# Create a list of object files from source files.
#
HRH_OBJS := $(patsubst %.c,%.o,$(filter %.c,$(HRH_SRCS)))

HRH_LIBS := -lmpfr

%.o : %.c
	$(CC) $(CFLAGS) -MD -c $< -o $@

.PHONY: all

all: $(APP)

hrh: $(HRH_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(HRH_OBJS) $(HRH_LIBS)

-include $(HRH_OBJS:.o=.d) 

.PHONY: clean

clean:
	$(RM) -f $(APP) *.o $(patsubst %,%/*.o,$(SUBDIRS))
	$(RM) -f $(APP) *.d $(patsubst %,%/*.d,$(SUBDIRS))

.PHONY: realclean

realclean: clean
	$(RM) -f *~ $(patsubst %,%/*~,$(SUBDIRS))
	$(RM) -f *.txt $(patsubst %,%/*~,$(SUBDIRS))

