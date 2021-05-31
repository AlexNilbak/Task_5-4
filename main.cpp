#include <cmath>
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <float.h>
#include <fstream>
#include "CImg.h"
#include "figure.h"
#include "box.h"
#include "tetrahedron.h"

using namespace std;

int main() {
	double PI = 3.14159;
	double maxDist = 0;
	double minDist = DBL_MAX;
	vector<Figure*> figs;
	Figure* f;
	double alpha;
	int x, y;
	int xc, yc, zc;
	int xl, yl, zl;

	ifstream in1("scene.txt");
	ifstream in2("figures.txt");
	string name;
	in1 >> name;
	in1 >> xc >> yc >> zc;
	in1 >> name;
	in1 >> alpha;
	in1 >> name;
	in1 >> x >> y;
	in1 >> name;
	in1 >> xl >> yl >> zl;

	if (x % 2 != 0) {
		x++;
	}
	if (y % 2 != 0) {
		y++;
	}

    cimg_library::CImg<unsigned char> image(x,y,1,3,0);
    cimg_library::CImgDisplay display(image, "Raytracing");
	double k = static_cast<double>(y) * sqrt( 2 / (1 - cos(alpha / 180 * PI)) - 1) / 2;
	printf("%f\n", k);
	while (!in2.eof()) {
		in2 >> name;
		if (name == "tetra") {
			double x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
			in2 >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3 >> x4 >> y4 >> z4;
			Vector v0;
			v0.x = (x1 + x2 + x3 + x4) / 4;
			v0.y = (y1 + y2 + y3 + y4) / 4;
			v0.z = (z1 + z2 + z3 + z4) / 4;
            if (v0.length() > maxDist) {
                maxDist = v0.length();
            }
            if (v0.length() < minDist) {
                minDist = v0.length();
            }
			figs.push_back(new Tetrahedron(xl, yl, zl, figs, v0, Vector(x1, y1, z1), Vector(x2, y2, z2), Vector(x3, y3, z3), Vector(x4,y4,z4)));
		}
		else if (name == "box") {
			double x1, y1, z1, x2, y2, z2;
			in2 >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
			Vector v0;
			v0.x = (x1 + x2) / 2;
			v0.y = (y1 + y2) / 2;
			v0.z = (z1 + z2) / 2;
			if (v0.length() > maxDist) {
				maxDist = v0.length();
			}
			if (v0.length() < minDist) {
				minDist = v0.length();
			}
			figs.push_back(new Box(xl, yl, zl, figs, v0, Vector(x1, y1, z1), Vector(x2, y2, z2)));
		}
	}
	for (Figure* o : figs) {
        double illumination;
        if (maxDist == minDist) {
			illumination = 0;
        } 
		else {
			illumination = (255.0 / (maxDist - minDist)) * o->getCenter().length() - (255.0 * minDist) / (maxDist - minDist);
        }
        if (illumination < 0) illumination = 0;
        Color c(0, 255 - illumination, illumination);
        o->setColor(c);
	}

	int c = figs.size();
	while (c != 1) {
		for (int i = 1; i < c; i++) {
			if ( (figs[i]->getCenter().y - k) < (figs[i - 1]->getCenter().y - k) ) {
				f = figs[i];
				figs[i] = figs[i - 1];
				figs[i - 1] = f;
			}
			c--;
		}
	}


	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
#pragma omp parallel for
	for (int i = -y / 2; i < y / 2; i++) {
#pragma omp parallel for
		for (int j = -x / 2; j < x / 2; j++) {
			bool pixelSet = false;
			for (Figure* o : figs) {
				double kdot;
				bool tch;
				if (o->intersect(j, k, i, xc, yc, zc, kdot, tch)) {
					Color c = o->pixelColor(static_cast<double>(j) * kdot / k, kdot, static_cast<double>(i) * kdot / k);
					int color[3] = {c.R, c.G, c.B};
					image.draw_point(x / 2 + j, y / 2 - i, color);
					pixelSet = true;
				}
			}
			if (!pixelSet) {
                int color[3] = {0, 0, 0};
                image.draw_point(x / 2 + j, y / 2 - i, color);
			}
		}
	}
	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	int elapsed_ms = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
	std::cout << "Time spent: " << elapsed_ms << " ms\n";
    display.display(image);
    image.save("picture.bmp");
	while (!display.is_closed())
        display.wait();
}
