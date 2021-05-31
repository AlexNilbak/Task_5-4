#pragma once
#include "figure.h"
#include <iostream>
#include <cmath>
#include <float.h>
#include <algorithm>
#include <vector>
using namespace std;

class Square : public Figure {
	Vector v1;
	Vector v2;
	Vector v3;
	int xl, yl, zl;
	double Ap, Bp, Cp, Dp;
public:
	Square(int xl, int yl, int zl, Vector v0, Vector v1, Vector v2, Vector v3, Vector O, Color col = Color(0, 255, 0)) {
		this->xl = xl;
		this->yl = yl;
		this->zl = zl;
		this->v0 = v0;
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;
		this->col = col;
		Ap = (v1.y - v0.y) * (v2.z - v0.z) - (v1.z - v0.z) * (v2.y - v0.y);
		Bp = (v1.z - v0.z) * (v2.x - v0.x) - (v1.x - v0.x) * (v2.z - v0.z);
		Cp = (v1.x - v0.x) * (v2.y - v0.y) - (v1.y - v0.y) * (v2.x - v0.x);
		if ((v0.x - O.x) * Ap + (v0.y - O.y) * Bp + (v0.z - O.z) * Cp < 0) {
			Ap = -Ap;
			Bp = -Bp;
			Cp = -Cp;
		}
		Dp = -Ap * v0.x - Bp * v0.y - Cp * v0.z;
	}

	bool intersect(double A, double B, double C, double xc, double yc, double zc, double& touch, bool& tch) override {
		double edge = (Ap * A + Bp * B + Cp * C);
		if (abs(edge) < DBL_MIN) {
			return false;
		}
		double t = -Dp / edge;
		double xo = A * t;
		double yo = B * t;
		double zo = C * t;
		Vector o(xo, yo, zo);
		Vector a(v0.x, v0.y, v0.z);
		Vector b(v1.x, v1.y, v1.z);
		Vector d(v2.x, v2.y, v2.z);
		Vector c(v3.x, v3.y, v3.z);
		Vector n1 = (o - a).crossProduct(o - b);
		Vector n2 = (o - b).crossProduct(o - c);
		Vector n3 = (o - c).crossProduct(o - d);
		Vector n4 = (o - d).crossProduct(o - a);
		if (n1 * n2 < 0 || n1 * n3 < 0 || n1 * n4 < 0 || n2 * n3 < 0 || n2 * n4 < 0 || n3 * n4 < 0) {
			touch = 0;
			tch = false;
			return false;
		}
		touch = yo;
		tch = false;
		return true;
	}
	Color pixelColor(double x, double y, double z) override {
		Vector n(Ap, Bp, Cp);
		n = n.normalize();
		Vector a(x - xl, y - yl, z - zl);
		a = a.normalize();
		Color c = col;
		double light = n * a;
		if (light < 0) {
			light = 0;
		}
		c.R = min(c.R * light + 10, 255.0);
		c.G = min(c.G * light + 10, 255.0);
		c.B = min(c.B * light + 10, 255.0);
		return c;
	}
	void setColor(Color c) override {
		col = c;
	}
	Vector getCenter() override {
		Vector center;
		center.x = (v1.x + v2.x + v3.x + v0.x) / 4;
		center.y = (v1.y + v2.y + v3.y + v0.y) / 4;
		center.z = (v1.z + v2.z + v3.z + v0.z) / 4;
		return center;
	}
};


class Box : public Figure {
	Square* s1;
	Square* s2;
	Square* s3;
	Square* s4;
	Square* s5;
	Square* s6;
	Vector center;
public:
	Box(int xl, int yl, int zl, vector<Figure*>& objs, Vector center, Vector v0, Vector v1) {
		this->v0 = v0;
		this->center = center;
		Vector v110(v1.x, v1.y, v0.z);
		Vector v101(v1.x, v0.y, v1.z);
		Vector v100(v1.x, v0.y, v0.z);
		Vector v011(v0.x, v1.y, v1.z);
		Vector v010(v0.x, v1.y, v0.z);
		Vector v001(v0.x, v0.y, v1.z);

		s1 = new Square(xl, yl, zl, v0, v100, v001, v101, center);
		s3 = new Square(xl, yl, zl, v0, v001, v010, v011, center);
		s2 = new Square(xl, yl, zl, v0, v100, v010, v110, center);
		s4 = new Square(xl, yl, zl, v001, v101, v011, v1, center);
		s5 = new Square(xl, yl, zl, v100, v101, v110, v1, center);
		s6 = new Square(xl, yl, zl, v010, v110, v011, v1, center);

		objs.push_back(s1);
		objs.push_back(s2);
		objs.push_back(s3);
		objs.push_back(s4);
		objs.push_back(s5);
		objs.push_back(s6);
	}
	bool intersect(double, double, double, double, double, double, double&, bool&) override {
		return false;
	}
	Color pixelColor(double, double, double) override {
		return { 0,0,0 };
	}
	void setColor(Color c) override {
		col = c;
		s1->col = c;
		s2->col = c;
		s3->col = c;
		s4->col = c;
		s5->col = c;
		s6->col = c;
	}
	Vector getCenter() override {
		return center;
	}
};

