#ifndef _DATARASTERITERATORH_
#define _DATARASTERITERATORH_
//================================================================
//
// File: DataRasterIterator.h
// Created: 12/15/2015
// Purpose: A class that handles iterating over a raster
//
// Modified:  cpadwick  12/15/2015  Original Definition.
//
//================================================================

#include "DataRaster.h"
#include "RasterDims.h"
#include "Exception.h"

enum TilingMode
{
  TilingModeAllBands = 0,      //reads all bands at once
  TilingModeSingleBand = 1     //reads one band at a time
};


/** DataRaster: a class that is capable of iterating over an image  */
class DataRasterIterator
{

private:

  /** Returns the size in bytes corresponding to the data type
   * @param dt The GDAL data type
   */
  int getDataTypeSize(GDALDataType dt) throw(Exception)
  {
    switch(dt)
    {
      case (GDT_Byte):
      {
        char c;
        return (sizeof(c));
        break;
      }
      case (GDT_UInt16):
      {
        short s;
        return (sizeof(s));
        break;
      }
      case (GDT_Int16):
      {
        short s;
        return (sizeof(s));
        break;
      }
      case (GDT_UInt32):
      {
        unsigned int i;
        return (sizeof(i));
        break;
      }
      case (GDT_Int32):
      {
        int i;
        return (sizeof(i));
        break;
      }
      case (GDT_Float32):
      {
        float f;
        return (sizeof(f));
        break;
      }
      case (GDT_Float64):
      {
        double d;
        return (sizeof(d));
        break;
      }
      default:
        throw Exception("DataRasterIterator::getDataTypeSize Error: data type not implemented");
        break;
    }
    return(0);
  };


  int lineChunkSize_;
  int nTiles_;
  int ns_;
  int nl_;
  int overlap_;

public:

  /** Constructor
   * @param source DataRaster object representing the raster to be iterated over.
   * @param memsize The memsize in bytes of the desired chunk size
   * @param overlap The desired overlap in pixels between adjacent tiles
   * @param mode The desired tiling mode for iterating over the raster
   */
  DataRasterIterator(const DataRaster& source, int memsize, int overlap, TilingMode mode = TilingModeSingleBand) throw (Exception)
  {
    ns_ = source.nsamples();
    nl_ = source.nlines();
    overlap_ = overlap;
    int dtSize = getDataTypeSize(source.dataType());

    // Determine tile size by taking memsize and overlap into account.  Scanline tiling.
    lineChunkSize_ = (int)ceil(memsize / ((double)ns_ * (double)dtSize));
    if (mode == TilingModeAllBands)
      lineChunkSize_ /= (double)(source.nbands());

    if (lineChunkSize_ - (2 * overlap) <= 0)
    {
      std::ostringstream ostr;
      ostr << "Error: DataRasterIterator::initialize(): Requested overlap of "
           << overlap << " lines is too large. Requested memsize of " << memsize
           << " bytes only allows for " << lineChunkSize_ << " lines per tile." << std::endl
           << "Try increasing the requested memsize or decreasing the overlap." << std::endl;
      throw Exception(ostr.str());
    }

    lineChunkSize_ -= (2 * overlap);
    if (lineChunkSize_ > nl_)
    {
      lineChunkSize_ = nl_;
    }
    nTiles_ = (int)ceil((double)nl_ / (double)lineChunkSize_);

  };
  
  /** Destructor */
  virtual ~DataRasterIterator(void) {} ;
  
  /** Retrieves the tile dimensions given the tile number
   * @param tileNum The tile number to be processed
   * @param input_tile_dims The dimensions of the input tile
   * @param output_tile_dims The dimensions of the output tile.  In the case of overlap the input_tile_dims will not be the same as the output_tile_dims
   */
  void getTileDims(int tileNum, RasterDims& input_tile_dims, RasterDims* output_tile_dims = 0)
  {
    input_tile_dims.setStartSample(0);
    input_tile_dims.setEndSample(ns_ - 1);
    input_tile_dims.setStartLine(tileNum *  lineChunkSize_);
    input_tile_dims.setEndLine(input_tile_dims.startLine() + lineChunkSize_ - 1);

    if(output_tile_dims)
    {
      output_tile_dims->setStartSample(input_tile_dims.startSample());
      output_tile_dims->setEndSample(input_tile_dims.endSample());
      output_tile_dims->setStartLine(input_tile_dims.startLine());
      output_tile_dims->setEndLine(input_tile_dims.endLine());
    }

    //handle overlap
    input_tile_dims.setStartLine(input_tile_dims.startLine() - overlap_);
    input_tile_dims.setStartLine( (input_tile_dims.startLine() < 0) ? 0 : input_tile_dims.startLine() );
    input_tile_dims.setEndLine(input_tile_dims.endLine() + overlap_);
    input_tile_dims.setEndLine( (input_tile_dims.endLine() > nl_ - 1) ? nl_ - 1 : input_tile_dims.endLine() );

    if(output_tile_dims)
    {
      if (output_tile_dims->endLine() > nl_ - 1)
        output_tile_dims->setEndLine(nl_ - 1);
    }
  };

  /** Returns the number of tiles for this source raster */
  int ntiles(void) { return(nTiles_); };

  /** Returns the overlap in pixels between adjacent tiles for this source raster */
  int overlap(void) { return(overlap_); };

  /** Sets the overlap in pixels betwen adjacent tiles for this source raster */
  void setOverlap(int overlap) { overlap_ = overlap;} ;
  
};
#endif
