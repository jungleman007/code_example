#ifndef _DATARASTERH_
#define _DATARASTERH_
//================================================================
//
// File: DataRaster.h
// Created: 12/15/2015 
// Purpose: An abstraction of a data raster on top of gdal
//
// Modified:  cpadwick  12/15/2105  Original Definition.
//
//================================================================

#include <stdlib.h>
#include <stdio.h>
#include <gdal_priv.h>
#include "Exception.h"
#include "RasterDims.h"

//forward declarations
//class IPL_DataRasterIterator;
template <typename T> class DataBuffer;
class RasterDims;

/** DataRaster: a class that models an image.  */
class DataRaster
{

private:

  int ns_;
  int nl_;
  int nb_;
  
  RasterDims dims_;

  GDALDataset* gdalDataset_;
  GDALRasterBand* gdalRasterBand_;
  
protected:

  /** Write a rectangular region of interest to the file
   * @param tileData Pointer to a memory location containing the tile data 
   * @param tileDims Reference to a DataRasterDims object containing the dimensions of the tileData
   * @param outputDims Reference to a DataRasterDims object containing the rectangel to write to in the output file
   * @param outBand Integer specifying the output band.  This follows GDAL and is 1 based.
   * @param dt DataType
   */
  template <typename T> 
  void writeSubrect(T* tileData, const RasterDims& tileDims, const RasterDims& outputDims, 
    int outBand, GDALDataType dt) throw(Exception)
  {
    if (!tileData)
      throw Exception("DataRaster: Error: tileData not valid");

    int tileWidth = tileDims.width();
    int tileHeight = tileDims.height();
    int width = outputDims.width();
    int height = outputDims.height();

    if (tileWidth == width && tileHeight == height)  //no subsetting required.  just write out the buffer.
    {
      setData(tileData, outBand, outputDims, dt);
    }
    else  //We need to pull out the subrect from the input buffer and write it out.
    {
      int xoff = outputDims.startSample() - tileDims.startSample();
      int yoff = outputDims.startLine() - tileDims.startLine();

      T* outData = new T[width*height];
      T* id = tileData + (yoff * tileWidth);  //wind forward by yoff lines
      for (int yy=0; yy<height; yy++)
      {
        memcpy((void*)(outData + (yy * width)), //wind forward by yy lines
          (void*)(id + xoff), //start the copy at the xoff pixel
          width * sizeof(T)); //copy width pixels of data
        id += tileWidth; //wind forward by 1 line
      }
      setData(outData, outBand, outputDims, dt);
      delete[](outData);
    }
  };
  
  /** Writes data to a file
   * @param data Pointer to a memory location containing the data 
   * @param band Integer specifying the band in the file to write to.  This is a 1-based index.
   * @param dims Reference to a DataRasterDims object containing the rectangle to write to in the output file
   * @param dt DataType
   */
  void setData(void* data, int band, const RasterDims& dims, GDALDataType dt) throw(Exception)
  {
    if (!gdalDataset_)
      throw Exception("Null pointer exception");
 
    //grab the band that we need.
    gdalRasterBand_ = gdalDataset_->GetRasterBand(band);
    if (!gdalRasterBand_)
      throw Exception("Null pointer exception");

    if (gdalRasterBand_->RasterIO(GF_Write, dims.startSample(), dims.startLine(),
      dims.width(), dims.height(), data, dims.width(), dims.height(), dt, 0, 0) != 0)
      throw Exception("Error encountered writing data.");  
  };


public:

/** Constructor.
 * @param filename The filename of the image to open
 * @param gdalMode The desired access mode for the image
 */
  DataRaster(void)
  {
    GDALAllRegister();
    gdalDataset_ = NULL;
    gdalRasterBand_ = NULL;
    nl_ = 0;
    ns_ = 0;
    nb_ = 0;
  };

/** Destructor.  Takes no arguments.  */
  ~DataRaster(void)
  {
    if (gdalDataset_)
    {
      GDALClose(gdalDataset_);
      gdalDataset_ = NULL;
    }
  };
  
  /** Opens an existing file
   * @param filename Filename of the image file to open.
   * @param gdalMode Access mode for the file
  */
  void open(const std::string& filename, GDALAccess gdalMode) throw (Exception)
  {
    gdalDataset_ = (GDALDataset *) GDALOpen(filename.c_str(), gdalMode);
    if (!gdalDataset_)
      throw Exception(std::string("Unable to open file ") + filename);
    nb_ = gdalDataset_->GetRasterCount();
    ns_ = gdalDataset_->GetRasterXSize();
    nl_ = gdalDataset_->GetRasterYSize();
    dims_.setStartSample(0);
    dims_.setEndSample(ns_ - 1);
    dims_.setStartLine(0);
    dims_.setEndLine(nl_ - 1);
  };
  
  /** Closes a data raster which is currently open
  */
  void close(void)
  {
    if (gdalDataset_)
    {
      GDALClose(gdalDataset_);
      gdalDataset_ = NULL;
      gdalRasterBand_ = NULL;
      nl_ = 0;
      ns_ = 0;
      nb_ = 0;
    }
    
  };
  
  /** Creates a new file
   * @param filename Filename of the image file to create
   * @param dims RasterDims object specifying the spatial dimensions of the file
   * @param nbands The number of bands desired in the file
   * @param dataType The GDALDataType of the output file
   * @param format A valid GDAL format string, e.g. GTiff
   * @param parent If a valid pointer is passed then the new file will inherit the georeferencing and projection from the parent
  */
  void create(const std::string& filename,
              const RasterDims& dims,
              int nbands,
              GDALDataType dataType,
              const std::string& format,
              const DataRaster* parent = NULL)
  {

    GDALDriver *pDriver;
    pDriver = GetGDALDriverManager()->GetDriverByName(format.c_str());
    if (!pDriver)
      throw Exception(std::string("DataRaster::create Error: unable to get GDAL driver for driver ") + format);

    char **pMetadata = pDriver->GetMetadata();
    int bOK = CSLFetchBoolean(pMetadata, GDAL_DCAP_CREATE, FALSE);
    if (!bOK)
      throw Exception("This driver does not support file creation.");

    int xSize = dims.width();
    int ySize = dims.height();
    GDALDataset* outputDataset = pDriver->Create(filename.c_str(), xSize, ySize, nbands, dataType, NULL);
    if (!outputDataset)
      throw Exception(std::string("Creation of file ") + filename + std::string(" failed."));

    if (parent)
    {
      //transfer all the geo data to the new file
      double adfGeoTransform[6];
      if (parent->gdalDataset_->GetGeoTransform( adfGeoTransform ) == CE_None)
        outputDataset->SetGeoTransform( adfGeoTransform );
        
      if (parent->gdalDataset_->GetProjectionRef() != NULL)
        outputDataset->SetProjection(parent->gdalDataset_->GetProjectionRef());
    }

    //close the new file
    delete(outputDataset);
    outputDataset = NULL;

    open(filename, GA_Update);
  };

  

/** Retrieves data from the image.
 * @param buf Pointer to a DataBuffer object
 * @param imageband An integer specifying which band to read from the image.  This follows GDAL and is 1 based.
 * @param dataType The type of the data to read from the image
 * @param bufferBand An integer designating the target band for the DataBuffer object.  This is a zero based index.
 */
  template <typename T> void getData(DataBuffer<T>& buf, int imageband, GDALDataType dataType, int bufferBand = 0) throw (Exception)
  {
    if (!gdalDataset_)
      throw Exception("DataRaster::getData Error: gdalDataset_ object is NULL.");

    if (bufferBand > buf.nbands() - 1)
      throw Exception("DataRaster::getData(): Error: bufferBand exceeds dimensions of buffer.");

    //grab the band that we need.
    gdalRasterBand_ = gdalDataset_->GetRasterBand(imageband);
    if (!gdalRasterBand_)
       throw Exception("DataRaster::getData Error: bandPtr is NULL.");

    int xSize = buf.dims().width();
    int ySize = buf.dims().height();
    int sz = xSize * ySize;
    
    T* bufdata = buf.data();

    if (gdalRasterBand_->RasterIO(GF_Read, buf.dims().startSample(), buf.dims().startLine(), 
      xSize, ySize, (void*)(bufdata + sz * bufferBand), xSize, ySize, dataType, 0, 0) != 0)
      throw Exception("DataRaster::getData Error: RasterIO returned an error");
  };

  /** Writes data to the image.
   * @param buf Reference to a DataBuffer object
   * @param outputBand An integer specifying which band to write to in the image.  This follows GDAL and is 1 based.
   * @param dataType The type of the data to be written to the image
   * @param bufferBand An integer designating the target band for the DataBuffer object.  This is a zero based index.
   */
  template <typename T> void setData(DataBuffer<T>& buf, const RasterDims& outputDims,
      int outputBand, GDALDataType dataType, int bufferBand = 0) throw (Exception)
  {
    if (bufferBand > buf.nbands() - 1)
      throw Exception("DataRaster::setData(): Error: bufferBand exceeds dimensions of buffer.");

    T* bandData = buf.data() + buf.dims().width() *  buf.dims().height() * bufferBand;
    writeSubrect(bandData, buf.dims(), outputDims, outputBand, dataType);
  };

  /** Retrieve the bounds/extents of the image.
   * @param ulx Reference to upper left corner x coordinate
   * @param uly Reference to upper left corner y coordinate
   * @param lrx Reference to lower right corner x coordinate
   * @param lry Reference to lower right corner y coordinate
  */  
  void getBounds(double & ulx, double & uly, double & lrx, double & lry) const throw (Exception)
  {
    if(!gdalDataset_)
      throw Exception("No image dataset is available");
    
    int nXSize = gdalDataset_->GetRasterXSize();
    int nYSize = gdalDataset_->GetRasterYSize();
    imageToMap(0, 0, &ulx, &uly);
    imageToMap(nXSize, nYSize, &lrx, &lry);
  };

  /** Converts image coordinates to map coordinates
  * @param sample The column coordinate of the point to be transformed 
  * @param line The row coordinate of the point in the image to be transformed
  * @param mapSample The transformed map column coordinate
  * @param mapLine The transformed line column coorinate
  */
  void imageToMap(int sample, int line, double* mapSample, double* mapLine) const
  {
    double dsample = static_cast<double>(sample);
    double dline = static_cast<double>(line);
    double adfGeoTransform[6];
    if (gdalDataset_->GetGeoTransform(adfGeoTransform) == CE_None)
    {
      *mapSample = adfGeoTransform[0] + dsample * adfGeoTransform[1] + dline * adfGeoTransform[2];
      *mapLine = adfGeoTransform[3] + dsample * adfGeoTransform[4] + dline * adfGeoTransform[5];
      return;
    }
    *mapSample = dsample;
    *mapLine = dline;
  };

  //tile iterator
  /*IPL_DataRasterIterator* createIterator(int memSize,
    int overlap = 0,
    int level = -1,
    int granularity = 1,
    bool b2DTiling = false,
    TilingMode mode = TilingModeSingleBand) throw (IPLException);*/

  /** Returns the number of samples (columns) for this DataRaster. */
  int nsamples(void) const { return ns_; };

  /** Returns the number of lines (rows) for this DataRaster. */
  int nlines(void) const { return nl_; };

  /** Returns the number of bands for this DataRaster. */
  int nbands(void) const { return nb_; };

  /** Returns the dimensions of this DataRaster. */
  const RasterDims& dims(void) const { return dims_; };

  /** Returns the dataType for this DataRaster. */
  GDALDataType dataType(int band=1) throw(Exception)
  {
    if (!gdalDataset_)
      throw Exception("DataRaster::dataType(): Error: null pointer exception");
    gdalRasterBand_ = gdalDataset_->GetRasterBand(band);
    if (!gdalRasterBand_)
      throw Exception("DataRaster::dataType(): Error: null pointer exception");
    GDALDataType dataType = gdalRasterBand_->GetRasterDataType();
    return(dataType);
  };
};

#endif

