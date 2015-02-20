#ifndef __VECTOR3_H__
#define __VECTOR3_H__


#include <string>
#include <fstream>
#include <vector>
#include <cmath>

class Vector3 {
public:
//Privatedan taşıma
    float _x, _y, _z;
    float len;



    // Constructors
    Vector3();
    Vector3(float, float, float);
    Vector3(const Vector3&);

    // Assignment operator (can be chained)
    Vector3& operator=(const Vector3&);
    bool operator==(const Vector3 &a){
		if (_x == a._x && _y == a._y && _z == a._z)
			return true;
		else
			return false;		
	};

    // Accessors
    float x() const{return _x;};
    float y() const{return _y;};
    float z() const{return _z;};

    void setX(float x){_x = x;};
    void setY(float y){_y = y;};
    void setZ(float z){_z = z;};

    // Misc. operations
    float dot(const Vector3& myVec) const
    {
		float retVal;
		retVal = (_x*myVec._x) + (_y*myVec._y) + (_z*myVec._z);
		return retVal;
	};
    Vector3 cross(const Vector3& myVec) const
    {
			
		Vector3 retVec;
		
		retVec._x = (_y*myVec._z) - (_z*myVec._y);
		retVec._y = (_z*myVec._x) - (_x*myVec._z);
		retVec._z = (_x*myVec._y) - (_y*myVec._x);
		
		return retVec;
	};

    void normalize()
    {
		len = sqrt(_x*_x + _y*_y + _z*_z);
		if(len)
		{
			_x = _x / len;
			_y = _y / len;
			_z = _z / len;
		}
	};
    float length() const;


    // Arithmetic
    Vector3 operator+(const Vector3&) const;
    Vector3 operator-(const Vector3&) const;
    Vector3 operator*(float) const;

    Vector3& operator+=(const Vector3&);
    Vector3& operator-=(const Vector3&);
    Vector3& operator*=(float);

private:
};

Vector3 operator*(float, const Vector3&);  // unfortunately this needs to be outside the class */

#endif
