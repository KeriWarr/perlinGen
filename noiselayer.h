#ifndef __NOISELAYER_H__
#define __NOISELAYER_H__
#include "randmath.h"
#include <iostream>

class NoiseLayer {

		int **layer;
		
		int height, width;
		
	public:
	
		NoiseLayer(int height, int width, bool fillWithRand);
		
		~NoiseLayer();
		
		int **getLayer() const;
		
		int getHeight() const;
		
		int getWidth() const;
				
		void print();	
		
		void addFrom(NoiseLayer *nl);
		
		// deletes nl
		void addOn(NoiseLayer *nl);
		
		void scale(float factor);
		
		// newHeight & newWidth must be >= this->height, this->width respectively
		void interpolate(int newHeight, int newWidth);

};

#endif
