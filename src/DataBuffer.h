#ifndef _DATABUFFERH_
#define _DATABUFFERH_
//================================================================
//
// File: DataBuffer.h
// Created: 09/15/2015
// Purpose: An encapsulation of a data buffer.
//
// Modified:  cpadwick  09/15/2015  Original Definition.
//
//================================================================

#include <iostream>
#include <memory.h>
#include "Exception.h"
#include "RasterDims.h"

/** DataBuffer: An encapsulation of a DataBuffer, 
 *  typically read from an image.
*/
template <typename T> class DataBuffer
{

private:

  T* data_;
  RasterDims dims_;
  long long int sz_, width_, height_, nbands_, first_, last_;

public:

/**
 * Constructor.
 * @param dims A DataRasterDims object
 * @param nbands The number of bands
 * @param bzero A boolean, set to true to zero out the allocated array.
 */
  DataBuffer(const RasterDims& dims, int nbands = 1, bool bzero = true) throw(Exception)
    : nbands_(nbands)
  {
    dims_.setStartSample(dims.startSample());
    dims_.setEndSample(dims.endSample());
    dims_.setStartLine(dims.startLine());
    dims_.setEndLine(dims.endLine());

    width_ = dims_.width();
    height_ = dims_.height();
    sz_ = width_ * height_;
    data_ = new T[sz_*nbands_];
    if (bzero)
      memset((void*)data_, 0, sizeof(T) * sz_ * nbands_);
    first_ = 0LL;
    last_ = sz_ * nbands_ - 1LL;
  };

/** Destructor */
  virtual ~DataBuffer(void) throw(Exception)
  {
    if (data_)
      delete[](data_);
  };

/** operator [], used for indexing an element of the data array */
  inline T& operator[](int elem) throw(Exception)
  {
    if (elem < first_ || elem > last_)
      throw Exception("DataBuffer: Error: out of bounds index attempt");
    return(data_[elem]);
  };

/** Accessors */
  const RasterDims& dims(void) const { return(dims_); };
  int nbands(void) const { return(nbands_); };
  int width(void) const { return(width_); };
  int height(void) const {return(height_); };

/** Accessor, returns non const pointer to underlying input data */
  T* data(void) { return(data_); };

};
#endif
