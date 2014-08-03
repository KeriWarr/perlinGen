#include "randmath.h"
#include <iostream>

void print(int **layer, int dim);	
void delLayer(int **layer, int dim);
int **genLayer(int dim);
int **interpolate(int **layer, int dim1, int dim2, int weight);
void addTo(int **from, int **to, int dim);

int main() {
	
	randMath::seedRand();

	int dim = 1024;
	int size = 620;


	int **grid = new int*[dim+1];

	for(int i = 0; i < dim+1; ++i) {
		
		grid[i] = new int[dim+1];
		
		for(int j = 0; j < dim+1; j++) {
			
			grid[i][j] = 0;
			
		}
	}
	
	int weight = 2;
	
	//int layers = 0;
	
	for(int i = dim/128; i <= dim/2; i *= 2) {
		//layers++;
		int **layer = genLayer(i+1);
		
		//print(layer,i+1);
		
		layer = interpolate(layer, i+1, dim+1, weight);
		
		addTo(layer,grid,dim+1);		
		
		delLayer(layer, dim+1);
		
		weight *= 2;
	
	}

	print(grid, size);
	//std::cout << layers << std::endl;
	delLayer(grid,dim+1);

}

void print(int **layer, int dim) {
	
	for(int i = 0; i < dim; ++i) {
		
		for(int j = 0; j < dim; j++) {
			
			if(layer[i][j] <= 28) std::cout << " ";
			else if(layer[i][j] <= 56) std::cout << "'";
			else std::cout << "M";
			
		}
		
		std::cout << std::endl;
		
	}	
	
}	


void delLayer(int **layer, int dim) {
	
	for(int i = 0; i < dim; ++i) {
		
		delete [] layer[i];
		layer[i] = NULL;
		
	}
	
	delete [] layer;
	layer = NULL;	
	
}


int **genLayer(int dim) {

	int **layer = new int*[dim];

	for(int i = 0; i < dim; ++i) {
		
		layer[i] = new int[dim];
		
		for(int j = 0; j < dim; j++) {
			
			layer[i][j] = randMath::getRand(0,100);
			
		}
	}
	
	return layer;

}

int **interpolate(int **layer, int dim1, int dim2, int weight) {
	
	int **newLayer = new int*[dim2];
	int width = ((dim2-1)/(dim1-1));
	
	//std::cout << "width: " << width << "," << dim1 << "," << dim2 << std::endl;
	
	for(int i = 0; i < dim2; ++i) {
		
		newLayer[i] = new int[dim2];	
		
	}
	
	
	for(int i = 0; i < dim1; ++i) {
		
		for(int j = 0; j < dim1; j++) {
			//std::cout <<layer[i][j];
			
			 newLayer[i*width][j*width] = layer[i][j];
			
		}
		//std::cout << std::endl;
	}
	
	
	for(int i = 0; i < dim1; ++i) {		
		
		for(int j = 0; j < dim2; j++) {			
			
			if((j % width) != 0) {
			
				int prev = layer[i][(j - j%width)/width];			
				int after = layer[i][(j - (j%width))/width + 1];
				//std::cout << i << "," << j << "," << i%width << ";" << prev << "," << after << ";";
				
				float cs = (cos(((float)(j%width))/((float)width)*3.14159)+1.0)/2.0;
				if(prev < after) cs = 1.0 - cs;
				
				newLayer[i*width][j] = (int)((cs*((float)abs(prev-after)))+(prev < after ? prev : after));
				//std::cout << cs << "," << newLayer[i*width][j]  << std::endl; 	
			}
			
		}
	}
	//std::cout << "-----------------------------------" <<std::endl;
	for(int i = 0; i < dim2; ++i) {		
		
		for(int j = 0; j < dim2; j++) {
			
			if((i % width) != 0) {
				
				int above = newLayer[i - (i%width)][j];
				int below = newLayer[i - (i%width)+width][j];
				
				//std::cout << i << "," << j << "," << i%width << ";" << above << "," << below << ";";
				
				float cs = (cos(((float)(i%width))/((float)width)*3.14159)+1.0)/2.0;
				if(above < below) cs = 1.0 - cs;
				
				newLayer[i][j] = (int)(((cs)*((float)abs(above-below)))+(above < below ? above : below));
				//std::cout << cs << "," << newLayer[i][j]  << std::endl; 	
			}
			
		}
		
	}	
	//print(newLayer,dim2);
	for(int i = 0; i < dim2; ++i) {		
		
		for(int j = 0; j < dim2; j++) {
			//std::cout << newLayer[i][j] << ",";
			newLayer[i][j] /= weight;
		
		}
		//	std::cout << std::endl;
	}
	delLayer(layer, dim1);
	//print(newLayer,dim2);
	return newLayer;
	
}

void addTo(int **from, int **to, int dim) {
	
	for(int i = 0; i < dim; ++i) {		
		
		for(int j = 0; j < dim; j++) {

			to[i][j] += from[i][j];
			
		}
		
	}
	
}
