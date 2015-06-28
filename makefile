TARGET = libbingsprec.so
OBJECTS = src/cJSON.o src/curlutils.o src/recognizer.o src/msauth.o
CFLAGS = -fPIC -c -Wall -Iinclude
LDFLAGS = -shared -fPIC -lcurl -luuid -lm
CC = c99
LD = $(CC)

all: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)

simple: examples/simple.o $(TARGET)
	$(LD) -o $@ $< -lbingsprec

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^

install: $(TARGET)
	cp $(TARGET) /usr/lib/
	cp -r include/bingsprec /usr/include/	

debug : $(OBJECTS)
	$(LD) -o $(TARGET) $^ $(LDFLAGS) -g
	
simpledebug: examples/simple.o $(TARGET)
	$(LD) -o simple $< -lbingsprec -g

clean:
	rm -f $(TARGET) src/*.o simple examples/*.o .saved_instance_id
