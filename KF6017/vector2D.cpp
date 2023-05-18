// Modified 04/11/2018
//		Changed rotatedBy to use angles clockwise

// Modified by 19002175
//		Renamed variables to a style consistent with other work.

#include "vector2D.h"
#include "math.h"

Vector2D::Vector2D(): XValue(0), YValue(0)
{
}

Vector2D::Vector2D(float x, float y): XValue(x), YValue(y)
{
}

Vector2D::Vector2D(const Vector2D& rCopyVector)
{
	XValue = rCopyVector.XValue;
	YValue = rCopyVector.YValue;
}

void Vector2D::set(float x, float y)
{
	XValue=x;
	YValue=y;
}

void Vector2D::set(const Vector2D& rCopyVector)
{
	XValue = rCopyVector.XValue;
	YValue = rCopyVector.YValue;
}

float Vector2D::magnitude() const
{
	return float(sqrt(XValue * XValue + YValue * YValue));
}

	// Returns the square of the magnitude of the vector
	// (hence avoiding a square root.)
float Vector2D::magnitudeSquared() const
{
	return (XValue * XValue + YValue * YValue);
}


Vector2D Vector2D::unitVector() const
{
	float divideBy = magnitude();

	if(divideBy==0)
	{
		return Vector2D(0,0);
	}
		else
	return Vector2D(XValue/divideBy, YValue/divideBy);
}

Vector2D Vector2D::operator+(const Vector2D& rAddVector) const
{
	return Vector2D (XValue + rAddVector.XValue, YValue + rAddVector.YValue);
}

void Vector2D::operator+=(const Vector2D& rAddVector)
{
	XValue += rAddVector.XValue;
	YValue += rAddVector.YValue;
}

void Vector2D::operator-=(const Vector2D& rAddVector)
{
	XValue -= rAddVector.XValue;
	YValue -= rAddVector.YValue;
}

void Vector2D::operator/=(float divisor)
{
	XValue /= divisor;
	YValue /= divisor;
}

void Vector2D::operator/=(int divisor)
{
	XValue /= divisor;
	YValue /= divisor;
}

void Vector2D::operator/=(double divisor)
{
	XValue /= float(divisor);
	YValue /= float(divisor);
}

void Vector2D::operator*=(float multiplier)
{
	XValue *= multiplier;
	YValue *= multiplier;
}

void Vector2D::operator*=(double multiplier)
{
	XValue *= float(multiplier);
	YValue *= float(multiplier);
}

void Vector2D::operator*=(int multiplier)
{
	XValue *= multiplier;
	YValue *= multiplier;
}


Vector2D Vector2D::operator-(const Vector2D& rMinusVector) const
{
	return Vector2D (XValue - rMinusVector.XValue,YValue- rMinusVector.YValue);
}

Vector2D Vector2D::operator-() const
{
	return Vector2D (-this->XValue,-this->YValue);
}

Vector2D Vector2D::operator*(float multiplier) const
{
	return Vector2D(XValue*multiplier,YValue*multiplier);
}

Vector2D Vector2D::operator*(double multiplier) const
{
	return Vector2D(XValue * float(multiplier), YValue * float(multiplier));
}

Vector2D Vector2D::operator*(int multiplier) const
{
	return Vector2D(XValue*multiplier,YValue*multiplier);
}

float Vector2D::operator*(const Vector2D& rOtherVector) const
{
	return XValue * rOtherVector.XValue + YValue * rOtherVector.YValue;
}

bool Vector2D::operator==(const Vector2D& rOtherVector) const
{
	return (XValue == rOtherVector.XValue && YValue == rOtherVector.YValue);
}

Vector2D Vector2D::operator/(float divisor) const
{
	return Vector2D(XValue/divisor,YValue/divisor);
}

Vector2D Vector2D::operator/(double divisor) const
{
	return Vector2D(XValue / (float)divisor, YValue / (float)divisor);
}

Vector2D Vector2D::operator/(int divisor) const
{
	return Vector2D(XValue / (float)divisor, YValue / (float)divisor);
}

float Vector2D::angle() const
// Working in radians
{
	float answer;

	answer = float(atan2(XValue, YValue));

	while(answer < 0) answer += 6.28319f;
	while(answer >= 6.28319f) answer -= 6.28319f;

	return answer;
}

void Vector2D::setBearing(float angle, float magnitude)
{
// Working in radians
	XValue = magnitude * float(sin(angle));
	YValue = magnitude * float(cos(angle));
}

bool Vector2D::isParallelTo(const Vector2D &rOtherVector) const
{
	if(this->unitVector() == rOtherVector.unitVector()) return true;
	if(this->unitVector() == -rOtherVector.unitVector()) return true;
	else return false;
}

bool Vector2D::isPerpendicularTo(const Vector2D & rOtherVector) const
{
	return((*this * rOtherVector) == 0);
}

Vector2D Vector2D::perpendicularVector() const
{
	return Vector2D(this->YValue, -this->XValue);
}

Vector2D Vector2D::rotatedBy(float angle) const
{
	float s = float(sin(angle));
	float c = float(cos(angle));
	return Vector2D(this->XValue * c + this->YValue * s,- this->XValue * s + this->YValue * c);
	
}

Vector2D operator*(float multiplier, Vector2D v) 
{
	return v*multiplier;
}

Vector2D operator*(double multiplier, Vector2D v)
{
	return v * multiplier;
}

Vector2D operator*(int multiplier, Vector2D v)
{
	return v * multiplier;
}