#pragma once
#include <cmath>
#include <float.h>
using namespace std;

struct Color {
	int R;
	int G;
	int B;
	inline Color(int r = 0, int g = 0, int b = 0) {
		R = r;
		G = g;
		B = b;
	}
};

class Vector {
public:
	double x, y, z;
	Vector(double x = 0, double y = 0, double z = 0) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector normalize() {
		double r = sqrt(x * x + y * y + z * z);
		return Vector(x / r, y / r, z / r);
	}
	double operator*(const Vector& r) {
		double touch = x * r.x + y * r.y + z * r.z;
		return touch;
	}
	Vector operator*(double r) {
		return { x * r,y * r,z * r };
	}
	Vector operator-(const Vector& r) {
		return Vector(x - r.x, y - r.y, z - r.z);
	}
	Vector operator+(const Vector& r) {
		return Vector(x + r.x, y + r.y, z + r.z);
	}
	double length() const {
		return x * x + y * y + z * z;
	}
	bool operator>(const Vector& r) {
		if (length() > r.length()) {
			return true;
		}
		return false;
	}
	Vector crossProduct(Vector b) {
		Vector c;
		c.x = y * b.z - z * b.y;
		c.y = z * b.x - x * b.z;
		c.z = x * b.y - y * b.x;
		return c;
	}
	Vector operator-() {
		return Vector(-x, -y, -z);
	}
};

class Figure {
protected:
	Vector v0;
public:
	Color col;
	virtual bool intersect(double, double, double, double, double, double, double&, bool&) = 0;
	virtual Color pixelColor(double, double, double) = 0;
	virtual void setColor(Color) = 0;
	virtual Vector getCenter() = 0;
};

