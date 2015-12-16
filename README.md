# code_example
This repo contains some example code for explanation purposes.  The code_example repo shows a sample geospatial imaging framework that allows clients to focus on building algorithms rather than concentrating on reading and writing data.  The code in this library uses GDAL (http://gdal.org/), the Geospatial Abstraction Data Layer library.

The main classes are the following:

DataRaster.h: an encaspsulation of an image.  It wraps a set of lower level GDAL functions and provides a single interface for dealing with raster data.

RasterDims.h: A class for storing the dimensions of an image, or a subrect.

DataBuffer.h: A class that holds a buffer object for interacting with imagery data.

DataRasterIterator.h: A class that is capable of "iterating" over an image by reading the image in chunks rather than reading the entire image into memory.  The iterator supports non-zero overlap between adjacent chunks if desired.

```DataRaster dr;
    dr.open(std::string("ms_chip"), GA_ReadOnly);
    
    //test accessors
    if (dr.nsamples() != 400 ||
        dr.nlines()   != 400 ||
        dr.nbands()   != 4   || 
        dr.dataType() != GDT_UInt16)
      CPPUNIT_FAIL("Parameters do not match");		
      
    double ulx, uly, lrx, lry;
    dr.getBounds(ulx, uly, lrx, lry);
    std::cout << "DataRaster Bounds are: " << ulx << " " << uly << " " << lrx << " " << lry << std::endl;
    
    //test accessor for dims object
    if (dr.dims().width()  != 400 ||
        dr.dims().height() != 400)
      CPPUNIT_FAIL("Parameters do not match");		
      
    //Allocate a buffer to hold the imagery data
    DataBuffer<unsigned short> databuffer(dr.dims(), dr.nbands());
    //read the data into each band
    for (int band=0; band<dr.nbands(); band++)
      dr.getData(databuffer, band+1, dr.dataType(), band);
      
    //compute the mean for each band
    std::vector<double> meanvals;
    computeMean<unsigned short>(databuffer, meanvals);
    
    std::cout << "Mean Values are: ";
    for (int band=0; band<dr.nbands(); band++)
      std::cout << "Band " << band << " :" << meanvals[band] << " ";
    std::cout << std::endl;```


