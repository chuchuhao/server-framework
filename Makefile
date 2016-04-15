EXEC = \
       test-async \
       test-reactor \
       test-buffer \
       test-protocol-server \
       test-parser \
       httpd

       OUT ?= .build
              .PHONY: all
              all: $(OUT) $(EXEC)

              CC ?= gcc
                    CFLAGS = -std=gnu99 -Wall -O2 -g -I .
                                  LDFLAGS = -lpthread

                                          ifeq ($(strip $(PROFILE)), 1)
                                          PROF_FLAGS = -pg
                                                  CFLAGS += $(PROF_FLAGS)
                                                          LDFLAGS += $(PROF_FLAGS)
                                                                  endif

                                                                  OBJS := \
                                                                          async.o \
                                                                          reactor.o \
                                                                          buffer.o \
                                                                          protocol-server.o \
                                                                          http-parser.o
                                                                          deps := $(OBJS:%.o=%.o.d)
                                                                                  OBJS := $(addprefix $(OUT)/,$(OBJS))
                                                                                          deps := $(addprefix $(OUT)/,$(deps))

                                                                                                  httpd: $(OBJS) httpd.c
                                                                                                  $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

                                                                                                  test-%: $(OBJS) tests/test-%.c
                                                                                                  $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

                                                                                                  $(OUT)/%.o: %.c
                                                                                                  $(CC) $(CFLAGS) -c -o $@ -MMD -MF $@.d $<

                                                                                                  $(OUT):
                                                                                                  @mkdir -p $@

                                                                                                  ab-test:
                                                                                                  @ab -c 32 -n 100 http://localhost:8080/

                                                                                                  doc:
                                                                                                  @doxygen

                                                                                                  clean:
                                                                                                  $(RM) $(EXEC) $(OBJS) $(deps)
                                                                                                  @rm -rf $(OUT)

                                                                                                  distclean: clean
                                                                                                  rm -rf html

                                                                                                  -include $(deps)
