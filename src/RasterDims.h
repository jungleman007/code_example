#ifndef _RASTERDIMSH_
#define _RASTERDIMSH_
//================================================================
//
// File: RasterDims.h
// Created: 12/15/2015 
// Purpose: An object for dealing with image dimensions
//
// Modified: 	cpadwick	12/15/2105	Original Definition.
//
//================================================================

#include <iostream>

/** RasterDims: a class that models image dimensions  */
class RasterDims
{

private:

	int startSample_;
  int endSample_;
  int startLine_;
  int endLine_;

public:
	
		/** Constructor.
		 * @param x0 The starting column
		 * @param x1 The ending column
		 * @param y0 The starting row
		 * @param y1 The ending row
		 */
		RasterDims(int x0, int x1, int y0, int y1)
		{
			startSample_ = x0;
			endSample_ = x1;
			startLine_ = y0;
			endLine_ = y1;
		};
		
		/** Constructor. */
		RasterDims(void)
		{
			startSample_ = 0;
			endSample_ = 0;
			startLine_ = 0;
			endLine_ = 0;
		};
		
		/** Copy constructor.
		 * @param obj DataRasterDims object to copy from
		 */
		RasterDims(const RasterDims& obj)
		{
			startSample_ = obj.startSample_;
			endSample_ = obj.endSample_;
			startLine_ = obj.startLine_;
			endLine_ = obj.endLine_;
		};
		
		//* Destructor */
		virtual ~RasterDims(void) {};
		
		/** Get Accessors */
		int startSample(void) const { return startSample_; };
		int endSample(void) const { return endSample_; };
		int startLine(void) const { return startLine_; };
		int endLine(void) const { return endLine_; };
		
		/** Set Accessors */
		void setStartSample(int val) { startSample_ = val; };
		void setEndSample(int val) { endSample_ = val; };
		void setStartLine(int val) { startLine_ = val; };
		void setEndLine(int val) { endLine_ = val; };
		
		/** Return width */
		int width(void) const { return(endSample_ - startSample_ + 1); };
		
		/** Return height */
		int height(void) const { return(endLine_ - startLine_ + 1); };
		
		void print(void)
		{
			std::cout << "startSample: " << startSample_ << " endSample: " << endSample_ << std::endl;
			std::cout << "startLine: " << startLine_ << " endLine: " << endLine_ << std::endl;
		};
		
};
#endif