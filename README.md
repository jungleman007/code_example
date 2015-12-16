# code_example

# Overview 

This repo contains some example code for explanation purposes.  The code_example repo shows a simple geospatial imaging framework that allows clients to focus on building algorithms rather than concentrating on reading and writing data.  The library is useful because it handles large images by providing easy to use utilities to process the data in chunks rather than reading the entire image into memory.  Overlap between chunks is also supported.  The code in this library uses GDAL (http://gdal.org/), the Geospatial Abstraction Data Layer library.

# Classes

The main classes are the following:

*DataRaster.h*: an encapsulation of an image.  It wraps a set of lower level GDAL functions and provides a single interface for dealing with raster data.

*RasterDims.h*: A class for storing the dimensions of an image, or a subrect.

*DataBuffer.h*: A class that holds a buffer object for interacting with imagery data.

*DataRasterIterator.h*: A class that is capable of "iterating" over an image by reading the image in chunks rather than reading the entire image into memory.  The iterator supports non-zero overlap between adjacent chunks if desired.

*Ndvi.h*: A class that computes the Normalized Difference Vegetation Index for a multi-spectral image.

# Building The Code

You will need to install the gdal and gdal-dev packages.  Typically you can get these through your package manager like yum or apt-get.  You will also need to install cppunit (http://sourceforge.net/projects/cppunit/).  Download it and follow the instructions for building it and install it.  You may need to edit the Makefile in the "test" directory to point the Makefile at the include and lib locations of gdal and cppunit.   When you determine these locations, simply edit the Makefile and change the INCLUDES and LINC lines to reflect the paths on your system.  To get the Doxygen docs to build, you will need to have Doxygen installed.  You should be able to get this with a package manager.

Then:

`make`

will build the tests and will also build the docs using Doxygen (the output will appear in docs/html/index.html).  You can run the tests by cd'ing into the test directory and typing `./test`.  The output from the cppunit tests should scroll by.

# Quickstart Example

Here is a quick example of how the NDVI computation is written.  The code to call the algorithm is extremely simple:

```
 try
  {
    Ndvi ndvicalc(std::string("ms_chip"), std::string("ndvi_output.tif"));
    ndvicalc.run();
  }
  catch (std::exception& e)
  {
    std::ostringstream ostr;
    ostr << "Exception thrown during ndvi calculation: " << e.what();
    CPPUNIT_FAIL(ostr.str().c_str());
  }
```

Here a class called Ndvi is constructed.  An input file name "ms_chip" is provided as the input image, and a filename of the outputfile "ndvi_output.tif" is also provided.  The NDVI image will be written to the output file.  Then the run() method is called.  The method is wrapped in a try/catch exception block to handle any errors that may occur.  Taking a look at Ndvi.h, we start with the private members declared in the header file:

```
private:

  DataRaster inputraster_;
  DataRaster outputraster_;
```
The class has an inputraster and an output raster, both of type DataRaster.  Onto the constructor:

```
Ndvi(const std::string& inputfilename, const std::string& outputfilename) throw(Exception)
{
  inputraster_.open(inputfilename, GA_ReadOnly);  //open the input raster
  if (inputraster_.nbands() != 4)
    throw Exception("The input data must contain 4 bands.");
    
  //create the output raster
  outputraster_.create(outputfilename, inputraster_.dims(), 1, GDT_Float32, "GTiff", &inputraster_);
};
```
This opens an existing data set specified by inputfilename and stores it in the inputraster object.  It also creates a new data set specified by outputfilename and store it in the outputraster object.  Clients would normally interact with this class by calling the run method:

```
void run(void)
{
  switch(inputraster_.dataType())
  {
    case (GDT_Byte):
    {
      generate<unsigned char>();
      break;
    }
    case (GDT_UInt16):
    {
      generate<unsigned short>();
      break;
    }
    case (GDT_Int16):
    {
      generate<short>();
      break;
    }
    case (GDT_UInt32):
    {
      generate<unsigned int>();
      break;
    }
    case (GDT_Int32):
    {
      generate<int>();
      break;
    }
    case (GDT_Float32):
    {
      generate<float>();
      break;
    }
    case (GDT_Float64):
    {
      generate<double>();
      break;
    }
    default:
    {
      throw Exception("Ndvi::run Error: data type not implemented");
      break;
    }
  }
  
};
```

This method switches on the data type of the inputraster and instantiates the appropriate template call to the generate() function.  On to the generate function!

```
template <typename T> void generate(void)
{
    //setup the memory chunk size.  This is the largest chunk we are willing to read into memory.
    int memsize = static_cast<int>(0.1 * 1024. * 1024.);
    DataRasterIterator iter(inputraster_, memsize, 0);
    
    for (int tilenum=0; tilenum<iter.ntiles(); tilenum++)
    {
      //get the dimensions of the chunk to be processed.
      RasterDims chunkdims;
      iter.getTileDims(tilenum, chunkdims);
      
      //read the data for the chunk from the input file.
      DataBuffer<T> inputdata(chunkdims, inputraster_.nbands());
      for (int band=0; band<inputraster_.nbands(); band++)
        inputraster_.getData(inputdata, band+1, inputraster_.dataType(), band);
        
      //allocate a data buffer for the ndvi result.
      DataBuffer<float> outputdata(chunkdims, 1);
      
      //process the data.
      processchunk(inputdata, outputdata);
      
      //write the data out to the output file.
      for (int band=0; band<outputraster_.nbands(); band++)
        outputraster_.setData(outputdata, chunkdims, band+1, outputraster_.dataType(), band);
    }
    
    //close the file to ensure the data is written to the file.
    outputraster_.close();
};
```
This method instantiates a DataRasterIterator object which knows how to iterate over the chunks in an image.  "Chunks" are defined by the user who decides the maximum amount of memory they are willing to work with at one time.  This is handled by setting the memsize parameter, which is in units of bytes.  The iterator computes how many chunks (or tiles) are needed to fit into the users memory constraints. 

Then we enter a loop over the tiles.  For each tile, the iterator is queried to determine the dimensions of the current chunk (or tile).  The data for the chunk is then read from the inputraster and stored in a DataBuffer object.  Once that is done a DataBuffer object is created to hold the output result for the current chunk.  Then a method called processchunk is called and the inputdata and outputdata buffers are passed into it.  Next the data for the current chunk is written to the outputfile, and the loop for the next tile is executed.  When all the chunks are processed the outputraster is closed, which ensures that the data will be written to the file.

The processchunk method is as follows:

```
template <typename T> static void processchunk(DataBuffer<T>& inputdata, DataBuffer<float>& outputdata)
{ 
  int sz = inputdata.dims().width() * inputdata.dims().height();  //the size of 1 band of data
  T* band4ptr = inputdata.data() + sz * 3;  //wind ptr forward to the 4th band
  T* band3ptr = inputdata.data() + sz * 2;  //wind ptr forward to the 3rd band
  float* outputptr = outputdata.data();  //ptr to the output data
  
  //loop through all the pixels in the band and compute the NDVI
  for (int idx=0; idx<sz; idx++)
  {
    float band4val = static_cast<float>(band4ptr[idx]);
    float band3val = static_cast<float>(band3ptr[idx]);
    outputptr[idx] = (band4val - band3val) / (band4val + band3val + 1e-6);  //NDVI calculation
  }

};
```

In this method we use bands 3 and 4 from the input image to compute the NDVI value for each pixel in the output image.  We are assuming that the user passed the data to us in the band order blue, green, red, NIR.  If this is not true we will be in trouble!  The calculation goes quickly since the calls to the data() method return native c++ pointers, so there is no penalty or overhead from calling accessors or other methods in the virtual function stack.  This computation could be implemented in a multi-threaded fashion using OpenMP in a very straightforward manner.

And that's it!  The library provides a clean, easy to use interface for implementing operations on imagery data.  The library handles large files by utilizing chunked IO based on user defined memory constraints.  The advantage of using this framework is that the algorithm implementer can focus on implementing the code for the algorithm and letting the library worry about the file IO.  Using this framework, the author has been able to reduce the time to develop new algorithms signficantly.

The next step in learning the library is to read the test code and understand how each class works and how it is used.  Then examine the header files in the src directory and familiarize yourself with those.  Then, implement your own stuff!
    



