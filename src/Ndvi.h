#ifndef _NDVIH_
#define _NDVIH_
//================================================================
//
// File: Ndvi.h
// Created: 12/15/2015 
// Purpose: A class that computes the Normalized Difference Vegetation
//          Index for a multispectral image.
//
// Modified:  cpadwick  12/15/2105  Original Definition.
//
//================================================================

#include <iostream>
#include "Exception.h"
#include "DataRaster.h"
#include "DataBuffer.h"
#include "DataRasterIterator.h"
#include "RasterDims.h"

/** Ndvi: a class that compute the Normalized Difference Vegetation Index for a multispectral image. */
class Ndvi
{

private:

  DataRaster inputraster_;
  DataRaster outputraster_;
  
protected:

  /** Generate an outputfile containing the NDVI data. This is an internal method. */
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



public:
  
    /** Constructor.
     * @param inputfilename The pathname to the multispectral file that NDVI will be computed for.
     *   The input file must contain 4 bands and is assumed to be int he order Blue,Green,Red,NIR
     * @param outputfilename The filename of the output file that will contain the computed NDVI results.
     */
    Ndvi(const std::string& inputfilename, const std::string& outputfilename) throw(Exception)
    {
      inputraster_.open(inputfilename, GA_ReadOnly);  //open the input raster
      if (inputraster_.nbands() != 4)
        throw Exception("The input data must contain 4 bands.");
        
      //create the output raster
      outputraster_.create(outputfilename, inputraster_.dims(), 1, GDT_Float32, "GTiff", &inputraster_);
    };
    
    //* Destructor */
    virtual ~Ndvi(void) 
    {
      inputraster_.close();
      outputraster_.close();
    };
    
    /** Processes a chunk of imagery.  This method is public so it can be called by clients who just want a chunk of data processed rather than an output file.
     * @param inputdata A data buffer containing a chunk of imagery to be processed.
     * @param outputdata A data buffer for storing the output.
     */
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
    
    /** Runs the algorithm.  Most clients should call this method after constructing the object. */
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
    
    
    
   
    
};
#endif