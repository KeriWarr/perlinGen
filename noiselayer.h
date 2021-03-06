#ifndef __NOISELAYER_H__
#define __NOISELAYER_H__
#include "randmath.h"
#include "vector.h"
#include <vector>
#include <iostream>
#include <cmath>

class NoiseLayer {

		// noise data goes here
		float **layer;
		
		// array size
		int height, width;
		
	public:
	
		// constructor
		NoiseLayer(int height, int width, bool fillWithRand);
		
		// destructor
		~NoiseLayer();
		
		// returns layer
		float **getLayer() const;
		
		// returns height
		int getHeight() const;
		
		// returns width
		int getWidth() const;
				
		// prints a visual display of the state of layer
		void print();	

		// prints a ditribution of the values in layer
		void printStats();
		
		// outputs a generated HTML file with coloured table cells 
		//  corresponding to data points in layer
		void printHTMLTable();
		
		// adds the values from nl onto layer
		void addFrom(NoiseLayer *nl);
		
		// deletes nl and calls addFrom
		void addOn(NoiseLayer *nl);
		
		// multiplies all the values in layer by factor
		void scale(float factor);
		
		// expands layer to size (newHeight,newWidth) and fills 
		//  intermediate positions with data interpolated from nearby 
		//  positions
		// newHeight & newWidth must be >= this->height, this->width 
		//  respectively
		void interpolate(int newHeight, int newWidth);

		// adds a 'river' from a mountain to a body fo water
		void addRiver();

		// modulates the values in layer according to their distance from a pair of foci near the center of the grid
		//  this results in most of the values near the edges of the grid being 0 or close to 0, whereas values
		//  near the center can remain higher
		void modulateEllipse();

};

#endif
