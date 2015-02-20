

#include <cmath>
#include "vector3.h"

Vector3::Vector3(){
	_x = 0;
	_y = 0;
	_z = 0;	
	len = 0;	
}
Vector3::Vector3(float x, float y, float z){
	_x = x;
	_y = y;
	_z = z;	
}

Vector3::Vector3(const Vector3& myVec){
	_x = myVec._x;
	_y = myVec._y;
	_z = myVec._z;
}

Vector3& Vector3::operator=(const Vector3& myVec){
	_x = myVec._x;
	_y = myVec._y;
	_z = myVec._z;
	
	return *this;
}
/*
float Vector3::x() const{
	return _x;
}
float Vector3::y() const{
	return _y;
}
float Vector3::z() const{
	return _z;
}
*/
/*
void Vector3::setX(float x){
	_x = x;
}
void Vector3::setY(float y){
	_y = y;
}
void Vector3::setZ(float z){
	_z = z;
}*//*
float Vector3::dot(const Vector3& myVec) const{
	float retVal;
	retVal = (_x*myVec._x) + (_y*myVec._y) + (_z*myVec._z);
	return retVal;
}*/
/*
Vector3 Vector3::cross(const Vector3& myVec) const{
	Vector3 retVec;
	
	retVec._x = (_y*myVec._z) - (_z*myVec._y);
	retVec._y = (_z*myVec._x) - (_x*myVec._z);
	retVec._z = (_x*myVec._y) - (_y*myVec._x);
	
	return retVec;
}*/
float Vector3::length() const{
	float len;
	len = sqrt(_x*_x + _y*_y + _z*_z);
	return len;
}/*
void Vector3::normalize(){
	len = sqrt(_x*_x + _y*_y + _z*_z);
	if(len != 0)
	{
		_x = _x / len;
		_y = _y / len;
		_z = _z / len;
	}
}*/
Vector3 Vector3::operator+(const Vector3& myvec) const{
	Vector3 retvec;
	retvec._x = _x + myvec._x;
	retvec._y = _y + myvec._y;
	retvec._z = _z + myvec._z;
	
	return retvec;	
}
Vector3 Vector3::operator-(const Vector3& myvec) const{
	Vector3 retvec;
	retvec._x = _x - myvec._x;
	retvec._y = _y - myvec._y;
	retvec._z = _z - myvec._z;
	
	return retvec;	
}
Vector3& Vector3::operator+=(const Vector3& myvec){
	
	_x = _x + myvec._x;
	_y = _y + myvec._y;
	_z = _z + myvec._z;
	
	return *this;	
}
Vector3& Vector3::operator-=(const Vector3& myvec){
	
	_x = _x - myvec._x;
	_y = _y - myvec._y;
	_z = _z - myvec._z;
	
	return *this;	
}
Vector3 Vector3::operator*(float s) const{
	Vector3 retvec;
	retvec._x = _x * s;
	retvec._y = _y * s;
	retvec._z = _z * s;
	
	return retvec;
}
Vector3& Vector3::operator*=(float s){
		_x = _x * s;		
		_y = _y * s;
		_z = _z * s;
		
		return *this;
 }

Vector3 operator*(float s, const Vector3& myvec){
	Vector3 retvec;
	
	retvec.setX(myvec.x() * s);
	retvec.setY(myvec.y() * s);
	retvec.setZ(myvec.z() * s);
	
	return retvec;
}

