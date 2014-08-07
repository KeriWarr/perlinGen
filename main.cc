#include "noiselayer.h"
#include <iostream>

const int DETAIL_LEVEL_INVERSE = 30; // 8+


int main() {

	int dim = 300; // dim >= DETAIL_LEVEL_INVERSE + 2

	NoiseLayer *grid = new NoiseLayer(dim,dim,false);
	
	dim += DETAIL_LEVEL_INVERSE-2;
	for(int i = dim/DETAIL_LEVEL_INVERSE, weight = 2; i <= dim/2; i *= 2, weight *= 2) {
		
		NoiseLayer *layer = new NoiseLayer(i,i,true);
		
		layer->interpolate(dim,dim);
		
		layer->scale(1.1/(float)weight);
		
		grid->addOn(layer);
	
	}
	
	//grid->addRiver();
	//grid->printStats();
	grid->modulateEllipse();
	//grid->printStats();
	//grid->printHTMLTable();
	grid->print();
	delete grid;

}
