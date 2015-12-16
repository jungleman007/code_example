# code_example
This repo contains some example code for explanation purposes.  The code_example repo shows a sample geospatial imaging framework that allows clients to focus on building algorithms rather than concentrating on reading and writing data.  The code in this library uses GDAL (http://gdal.org/), the Geospatial Abstraction Data Layer library.

The main classes are the following:

DataRaster.h: an encaspsulation of an image.  It wraps a set of lower level GDAL functions and provides a single interface for dealing with raster data.

RasterDims.h: A class for storing the dimensions of an image, or a subrect.

DataBuffer.h: A class that holds a buffer object for interacting with imagery data.

DataRasterIterator.h: A class that is capable of "iterating" over an image by reading the image in chunks rather than reading the entire image into memory.  The iterator supports non-zero overlap between adjacent chunks if desired.


