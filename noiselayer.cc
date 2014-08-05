#include "noiselayer.h"

NoiseLayer::NoiseLayer(int height, int width, bool fillWithRand) : height(height), width(width) {
	
	this->layer = new float*[height];

	for(int i = 0; i < height; ++i) {
		
		this->layer[i] = new float[width];
		
		for(int j = 0; j < width; ++j) {
			
			if(fillWithRand) this->layer[i][j] = randMath::getRand((float)0.0,(float)99.9);
			else this->layer[i][j] = 0.0;
			
		}
	}
	
}

NoiseLayer::~NoiseLayer() {
	
	for(int i = 0; i < this->height; ++i) {
		
		delete [] this->layer[i];
		this->layer[i] = NULL;
		
	}
	
	delete [] this->layer;
	this->layer = NULL;		
	
}

float **NoiseLayer::getLayer() const {
	
	return this->layer;
	
}

int NoiseLayer::getHeight() const {
	
	return this->height;
	
}

int NoiseLayer::getWidth() const {
	
	return this->width;
	
}

void NoiseLayer::print() {
	
	for(int i = 0; i < this->height; ++i) {
		
		for(int j = 0; j < this->width; ++j) {
			
			if(this->layer[i][j] <= 20) std::cout << " ";
			else if(this->layer[i][j] <= 30) std::cout << ".";
			else if(this->layer[i][j] <= 40) std::cout << "-";
			else if(this->layer[i][j] <= 50) std::cout << "\"";
			else if(this->layer[i][j] <= 60) std::cout << "{";
			else if(this->layer[i][j] <= 70) std::cout << "T";
			else std::cout << "M";
			
		}
		
		std::cout << std::endl;
		
	}	
	
}

void NoiseLayer::printHTMLTable() {
	
	static bool first = true;
	
	if(first) {
		std::cout << "<head><style>table{border-spacing:0;}td{width:2px;height:2px;}</style></head>";
		first = false;
	}
	
	std::cout << "<table>";
	
	for(int i = 0; i < this->height; ++i) {
		
		std::cout << "<tr>";
		
		for(int j = 0; j < this->width; ++j) {
			
			std::cout << "<td style=\"background-color:rgb(" << (int)(this->layer[i][j]*2.55) << "," << (int)(this->layer[i][j]*2.55) << "," << (int)(this->layer[i][j]*2.55) << ");\"></td>";
			
		}
		
	std::cout << "</tr>";
	
	}
	
	std::cout << "</table>" << std::endl;
	
}

void NoiseLayer::addFrom(NoiseLayer *nl) {
	
	for(int i = 0; i < this->height; ++i) {		
		
		for(int j = 0; j < this->width; ++j) {

			if(i < nl->getHeight() && j < nl->getWidth()) this->layer[i][j] += nl->getLayer()[i][j];
			
		}
		
	}
	
}

void NoiseLayer::addOn(NoiseLayer *nl) {
	
	this->addFrom(nl);
	delete nl;
	
}

void NoiseLayer::scale(float factor) {
	
	for(int i = 0; i < this->height; ++i) {		
		
		for(int j = 0; j < this->width; ++j) {
			
			this->layer[i][j] = (int)((float)this->layer[i][j] * factor);
			
		}
		
	}	
	
}

void NoiseLayer::interpolate(int newHeight, int newWidth) {
	
	int vertiGap = (newHeight - 2)/(this->height - 1) + 1;
	int horizGap = (newWidth - 2)/(this->width - 1) + 1;
	
	int fullHeight = vertiGap*(this->height-1) + 1;
	int fullWidth = horizGap*(this->width-1) + 1;
	
	float **newLayer = new float*[fullHeight];
	
	for(int i = 0; i < fullHeight; ++i) {
		
		newLayer[i] = new float[fullWidth];	
		
	}
	
	
	for(int i = 0; i < this->height; ++i) {
		
		for(int j = 0; j < this->width; ++j) {
			
			 newLayer[i*vertiGap][j*horizGap] = this->layer[i][j];
			
		}

	}
		
	
	for(int i = 0; i < this->height; ++i) {		
		
		for(int j = 0; j < fullWidth; ++j) {			
			
			if((j % horizGap) != 0) {
			
				int prev = layer[i][j / horizGap];			
				int after = layer[i][(j / horizGap) + 1];
				
				float cs = (cos(((float)(j%horizGap))/((float)horizGap)*3.14159)+1.0)/2.0;
				if(prev < after) cs = 1.0 - cs;
				
				newLayer[i*vertiGap][j] = (cs*((float)abs(prev-after)))+(float)(prev < after ? prev : after);
				
			}
			
		}
		
	}


	for(int i = 0; i < fullHeight; ++i) {		
		
		for(int j = 0; j < fullWidth; ++j) {
			
			if((i % vertiGap) != 0) {
				
				int above = newLayer[i - (i%vertiGap)][j];
				int below = newLayer[i - (i%vertiGap) + vertiGap][j];
				
				
				float cs = (cos(((float)(i%vertiGap))/((float)vertiGap)*3.14159)+1.0)/2.0;
				if(above < below) cs = 1.0 - cs;
				
				newLayer[i][j] = ((cs)*((float)abs(above-below)))+(float)(above < below ? above : below);
				
			}
			
		}
		
	}	
	
	
	float **sizedLayer = new float*[newHeight];

	for(int i = 0; i < newHeight; ++i) {
		
		sizedLayer[i] = new float[newWidth];
		
		for(int j = 0; j < newWidth; ++j) {
			
			sizedLayer[i][j] = newLayer[i][j];
			
		}
	}

	
	for(int i = 0; i < this->height; ++i) {
		
		delete [] this->layer[i];
		this->layer[i] = NULL;
		
	}
	
	delete [] this->layer;
	this->layer = NULL;	
	
	for(int i = 0; i < fullHeight; ++i) {
		
		delete [] newLayer[i];
		newLayer[i] = NULL;
		
	}
	
	delete [] newLayer;
	newLayer = NULL;	
	
	this->layer = sizedLayer;
	this->height = newHeight;
	this->width = newWidth;
	
}
