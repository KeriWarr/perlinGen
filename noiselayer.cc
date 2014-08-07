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

	std::cout << this->height << "," << this->width << std::endl;
	
	for(int i = 0; i < this->height; ++i) {
		
		for(int j = 0; j < this->width; ++j) {
			
			if(this->layer[i][j] == -1.0) std::cout << "~";
			else if(this->layer[i][j] <= 20) std::cout << " ";
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

void NoiseLayer::printStats() {

	std::cout << "Height: " << this->height << std::endl << "Width: " << this->width << std::endl;

	int *stats = new int[100];
	int counted = 0;

	for(int i = 0; i < 100; i++) {

		stats[i] = 0;

	}

	for(int i = 0; i < this->height; ++i) {
		
		for(int j = 0; j < this->width; ++j) {

			if(this->layer[i][j] >= 0 && this->layer[i][j] < 100) {

				stats[(int)this->layer[i][j]]++;
				counted++;

			}
	
		}

	}	

	std::cout << "tiles in range [0-100): " << counted << std::endl;

	for(int i = 0; i < 100; i++) {

		std::cout << i << ": " << stats[i] << std::endl;
	}

	delete [] stats;

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

			if(this->layer[i][j] <= 35) std::cout << "<td style=\"background-color:rgb(10,40,200);\"></td>";
			else std::cout << "<td style=\"background-color:rgb(" << (int)(this->layer[i][j]*4)-35 << ",255," << (int)(this->layer[i][j]*4)-35 << ");\"></td>";
			
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
			if(this->layer[i][j] <= 0) this->layer[i][j] = 0;
			else if(this->layer[i][j] >= 100) this->layer[i][j] = 99.9;
			
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

void NoiseLayer::addRiver() {
	
	int X = 0, Y = 0;
	int attempts = 0;
	
	while(this->layer[X = randMath::getRand(0,this->height-1)][Y = randMath::getRand(0,this->width-1)] <= 70) {
		
		attempts++;
		if(attempts > 100) return;
		
	}
	
	std::cout << X << "," << Y <<  std::endl;
	
	std::vector<Vector> river;
	river.push_back(Vector(X,Y));
	
	int prevX = X; 
	int prevY = Y;
	
	while(X > 0 && X < this->height && Y > 0 && Y < this->width && this->layer[X][Y] > 30) {
		
		int nextX = X;
		int nextY = Y;
		
		std::cout << X << "," << Y << ": " << this->layer[X][Y] << std::endl;
		
		for(int i = X-1; i <= X+1; i++) {
			
			for(int j = Y-1; j <= Y+1; j++) {
				if(abs(prevX-i) + abs(prevY-j)) std::cout << "  " << i << "," << j << ": " << this->layer[i][j] << std::endl;
				if((abs(prevX-i) + abs(prevY-j)) > 1 && this->layer[i][j] < this->layer[nextX][nextY]) {
					
					nextX = i; nextY = j;
					
				}
				
			}
			
		}
		
		if(nextX == X && nextY == Y) {
			
			for(int i = X-2; i <= X+2; i++) {
				
				for(int j = Y-2; j <= Y+2; j++) {	
					
					if(abs(prevX-i) == 2 || abs(prevY-j) == 2 && this->layer[i][j] < this->layer[nextX][nextY]) {
						
						nextX = i; nextY = j;
						
					}		
					
				}
				
			}
			
		}
		
		if(nextX == X && nextY == Y) break;
		
		
		else {
			
			prevX = X;
			prevY = Y;
			X = nextX;
			Y = nextY;
			river.push_back(Vector(X,Y));
			
		}
			
		
	}
	
	for(std::vector<Vector>::iterator t = river.begin(); t != river.end(); t++) {
				
		for(int i = t->getX()-1; i <= t->getX()+1; i++) {
			
			for(int j = t->getY()-1; j <= t->getY()+1; j++) {
				
				if((abs(i-t->getX()) + abs(j-t->getY())) <= 1) this->layer[i][j] = -1.0;
		
			}
			
		}
		
	}
	
}


void NoiseLayer::modulateEllipse() {

	Vector foci1 = Vector(this->height/2,this->width/3);
	Vector foci2 = Vector(this->height/2,2*this->width/3);

	for(int i = 0; i < this->height; ++i) {		
		
		for(int j = 0; j < this->width; ++j) {

			this->layer[i][j] -= (((sqrt((abs(foci1.getX()-i)*abs(foci1.getX()-i))+(abs(foci1.getY()-j)*abs(foci1.getY()-j))) + sqrt((abs(foci2.getX()-i)*abs(foci2.getX()-i))+(abs(foci2.getY()-j)*abs(foci2.getY()-j)))) / this->width) * 80) - 25;
			if(this->layer[i][j] <= 0) this->layer[i][j] = 0;

		}

	}

}