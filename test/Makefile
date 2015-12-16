CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp test_raster_dims.cpp test_data_buffer.cpp test_data_raster.cpp
OBJECTS=$(SOURCES:.cpp=.o)
INCLUDES=-I../src -I/mnt/tier2/staging/neon0/apps/cppunit/include -I/dg/local/cots/osgeo/gdal-1.8.1/include
LINC=-L/mnt/tier2/staging/neon0/apps/cppunit/lib -L/dg/local/cots/osgeo/gdal-1.8.1/lib
LIBS=-lcppunit -lgdal -ldl
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) $(LINC) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	/bin/rm *.o $(EXECUTABLE)

