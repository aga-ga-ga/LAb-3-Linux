TARGET_SERVER=server
TARGET_CLIENT=client

THREAD=0

DTHREAD=-Dthread_def=$(THREAD)

all:
	$(CROSS_COMPILE)$(CC) client.c -o client $(DTHREAD)
	$(CROSS_COMPILE)$(CC) server.c -o server $(DTHREAD) -lpthread

clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT)