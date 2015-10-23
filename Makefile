
SYSTEMC = /usr/local/systemc231
INC_DIRS = -I$(SYSTEMC)/include
LIB_DIRS = -L$(SYSTEMC)/lib-linux64
LIBS = -lsystemc
CC = g++
CFLAGS = -Wall -g

TARGET = noc
OBJECTS = main.o \
            cs_router.o\
             core.o \

default : $(TARGET)

clean: 
	@rm $(OBJECTS) $(TARGET)

$(TARGET): $(OBJECTS)
	@$(CC) $(LIB_DIRS) $(INC_DIRS) -Wl,-rpath,$(SYSTEMC)/lib_linux64 -o $(TARGET) $(OBJECTS) $(LIBS)

main.o:main.cpp
	@$(CC) -g $(LIB_DIRS) $(INC_DIRS) $(LIBS) -Wl,-rpath,$(SYSTEMC)/lib_linux64 -c $(CFLAGS) $< -o $@
cs_router.o:cs_router.cpp
	@$(CC) -g $(LIB_DIRS) $(INC_DIRS) $(LIBS) -Wl,-rpath,$(SYSTEMC)/lib_linux64 -c $(CFLAGS) $< -o $@
core.o:core.cpp
	@$(CC) -g $(LIB_DIRS) $(INC_DIRS) $(LIBS) -Wl,-rpath,$(SYSTEMC)/lib_linux64 -c $(CFLAGS) $< -o $@
