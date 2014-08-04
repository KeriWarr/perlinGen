#include "noiselayer.h"
#include <iostream>

const int DETAIL_LEVEL_INVERSE = 20; // 8+


int main() {

	int dim = 213; // dim >= DETAIL_LEVEL_INVERSE + 2
	int height = 58;
	int width = dim;

	NoiseLayer *grid = new NoiseLayer(height,width,false);
	
	dim += DETAIL_LEVEL_INVERSE-2;
	for(int i = dim/DETAIL_LEVEL_INVERSE, weight = 2; i <= dim/2; i *= 2, weight *= 2) {
		
		NoiseLayer *layer = new NoiseLayer(i,i,true);
		
		layer->interpolate(dim,dim);
		
		layer->scale(1.0/(float)weight);
		
		grid->addOn(layer);
	
	}

	grid->print();
	delete grid;

}
