CC=$(CROSS_COMPILE)gcc
OBJS := main.o
ledprivateapp: $(OBJS)
	$(CC) -o ledprivateapp $(CFLAGS) $(LDFLAGS) $(OBJS)
$(OBJS) : %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@
