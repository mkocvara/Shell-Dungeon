
// Shapes.cpp
// Chris Rook
// Last modified 06/11/2018
//	Added AngledRectangle2D and Intersects(AngledRectangle2D) method for other classes

#include "Shapes.h"
#include <limits.h>
#include <iostream>
#include <cmath>
using namespace std;


// ***************************************************************
// Member functions for IShape ***********************************
// ***************************************************************

// Virtual destructor for usual reasons
IShape2D::~IShape2D()
{

}


// ***************************************************************
// Member functions for Point2D **********************************
// ***************************************************************

Point2D::Point2D()
{
	mPosition.set(0,0);
}

Point2D::Point2D(float x, float y)
{
	mPosition.set(x,y);
}

Point2D::Point2D(const Vector2D& rCopy)
{
	mPosition = rCopy;
}

void Point2D::PlaceAt(const Vector2D& rPosition)
{
	mPosition = rPosition;
}

Vector2D Point2D::GetPosition() const
{
	return mPosition;
}

bool Point2D::Intersects(const Point2D& rOther) const
{
	return(mPosition == rOther.mPosition);
}

float Point2D::Distance(const Point2D& rOther) const
{
	return (mPosition - rOther.mPosition).magnitude();
}

Vector2D Point2D::Intersection(const Point2D& rOther) const
{	
	// Uhh... Closest point on a point IS the point.
	return mPosition;
}

bool Point2D::Intersects(const Segment2D& rOther) const
{
	double t1 = rOther.GetTFromX(this->mPosition.XValue);
	double t2 = rOther.GetTFromY(this->mPosition.YValue);
	if(t2 == t1)	// On LINE
	{
		return (t2>=0 && t2<1.0);	// Within ends of segment
	}
	else
		return false;
}

bool Point2D::Intersects(const IShape2D& rOther) const
{
	// Is it a rectangle?
	if (typeid(rOther) == typeid(Rectangle2D))
		return dynamic_cast<const Rectangle2D*> (&rOther)->Intersects(*this);

	// Is it a circle?
	if (typeid(rOther) == typeid(Circle2D))
		return dynamic_cast<const Circle2D*> (&rOther)->Intersects(*this);

	// Is it a segment?
	if (typeid(rOther) == typeid(Segment2D))
		return dynamic_cast<const Segment2D*> (&rOther)->Intersects(*this);

	// Is it a point?
	if (typeid(rOther) == typeid(Point2D))
		return dynamic_cast<const Point2D*> (&rOther)->Intersects(*this);

	// Is it a point?
	if (typeid(rOther) == typeid(Rectangle2D))
		return dynamic_cast<const Rectangle2D*> (&rOther)->Intersects(*this);

	// Is it an angled rectangle?
	if (typeid(rOther) == typeid(AngledRectangle2D))
		return dynamic_cast<const AngledRectangle2D*> (&rOther)->Intersects(*this);

	return false;
}

float Point2D::Distance(const Segment2D& rOther) const
{
	// Project the point onto the line and find the parameter, t
	// using parametric equation.
	// Hmm. Should be ((C - S) . ( E - S ) ) / L^2

	return rOther.Distance(*this);
}

Vector2D Point2D::Intersection(const Segment2D& rOther) const
{
	return mPosition;
}

bool Point2D::Intersects(const Circle2D& rOther) const
{

	return ((mPosition - rOther.GetCentre()).magnitude() < rOther.GetRadius());
}

float Point2D::Distance(const Circle2D& rOther) const
{
	return ( (mPosition - rOther.GetCentre()).magnitude() - rOther.GetRadius() );
}

Vector2D Point2D::Intersection(const Circle2D& rOther) const
{
	// Uhh... Closest point on a point IS the point.
	return mPosition;
}

bool Point2D::Intersects(const Rectangle2D& rOther) const
{
	if( mPosition.XValue < rOther.GetCorner1().XValue
		|| mPosition.XValue > rOther.GetCorner2().XValue
		|| mPosition.YValue < rOther.GetCorner1().YValue
		|| mPosition.YValue > rOther.GetCorner2().YValue)
		return false;
	else
		return true;
}

bool Point2D::Intersects(const AngledRectangle2D& rOther) const
{
   return rOther.Intersects(*this);
}

float Point2D::Distance(const Rectangle2D& rOther) const
{
	// 9 possibilities
	if(mPosition.XValue < rOther.GetCorner1().XValue)
	{	// Left side
		if(mPosition.YValue < rOther.GetCorner1().YValue)
		{	// Top left
			return (mPosition - rOther.GetCorner1()).magnitude();
		}
		else if(mPosition.YValue >= rOther.GetCorner2().YValue)
		{	// Bottom left
			Vector2D bl(rOther.GetCorner1().XValue, rOther.GetCorner2().YValue);
			return (mPosition-bl).magnitude();
		}
		else
		{	// Middle left
			return rOther.GetCorner1().XValue-mPosition.XValue;
		}

	}
	else if(mPosition.XValue >= rOther.GetCorner2().XValue)
	{	// Right side
		if(mPosition.YValue< rOther.GetCorner1().YValue)
		{	// Top right
			Vector2D tr(rOther.GetCorner2().XValue, rOther.GetCorner1().YValue);
			return (mPosition-tr).magnitude();
		}
		else if(mPosition.YValue >= rOther.GetCorner2().YValue)
		{	// Bottom right
			return (mPosition - rOther.GetCorner2()).magnitude();
		}
		else
		{	// Middle right
			return mPosition.XValue - rOther.GetCorner2().XValue;
		}
	}
	else
	{	// Centre line
		if(mPosition.YValue < rOther.GetCorner1().YValue)
		{	// Top 
			return rOther.GetCorner1().YValue-mPosition.YValue;
		}
		else if(mPosition.YValue >= rOther.GetCorner2().YValue)
		{	// Bottom
			return mPosition.YValue - rOther.GetCorner2().YValue;
		}
		else
		{	// Inside rectangle  get distances to edges
			float t,b,l,r;
			t = mPosition.YValue - rOther.GetCorner1().YValue;
			b = rOther.GetCorner2().YValue-mPosition.YValue;
			l = mPosition.XValue - rOther.GetCorner1().XValue;
			r = rOther.GetCorner2().XValue-mPosition.XValue;

			float smallest = t;
			if(smallest>b)
				smallest =b;
			if(smallest>l)
				smallest = l;
			if(smallest>r)
				smallest =r;

			return -smallest;		// Negative, as inside the rectangle
		}
	}
}

Vector2D Point2D::Intersection(const Rectangle2D& rOther) const
{
	// Uhh... Closest point on a point IS the point.
	return mPosition;
}

// ****************************************************************
// Member functions for Segment2D *********************************
// ****************************************************************

Segment2D::Segment2D(): mStart(0,0), mEnd(0,0)
{

}

void Segment2D::PlaceAt(const Vector2D &rStart, const Vector2D &rEnd)
{
	mStart=rStart;
	mEnd=rEnd;
}

Vector2D Segment2D::GetStart() const
{
	return mStart;
}

Vector2D Segment2D::GetEnd() const
{
	return mEnd;
}

void Segment2D::ClipTo(float minT, float maxT)
{
	Vector2D end;
	end.set(mStart.XValue + maxT* (mEnd.XValue-mStart.XValue) , mStart.YValue + maxT* (mEnd.YValue-mStart.YValue) );

	SetStartTo(minT);
	mEnd = end;
}

float Segment2D::GetLength() const
{
	return (mStart-mEnd).magnitude();
}

bool Segment2D::Intersects(const Point2D& rOther) const
{

	return rOther.Intersects(*this);
}

float Segment2D::Distance(const Point2D& rOther) const
{

	return (rOther.GetPosition() - Intersection(rOther) ).magnitude();
}

Vector2D Segment2D::Intersection(const Point2D& rOther) const
{
	// Special case - line with length of zero
	if(mEnd==mStart) 
		return mEnd;
	else    // If line length is not zero
	{
		// Get unit direction vector from Start to End of the line
		Vector2D Direction = (mEnd - mStart).unitVector();

		// Projection of the point onto the line
		float dProjection = (rOther.GetPosition() - mStart) * Direction;

		float t = dProjection/GetLength();
			Vector2D vec = mStart + Direction*dProjection;
			vec=vec*2;
		if(t<0) 
			return mStart;
		else if(t>1.0)
			return mEnd;
		else
		{

			return mStart + Direction*dProjection;
		}
	}	// End if line length is not zero
}

bool Segment2D::Intersects(const IShape2D& rOther) const
{
	// Is it a rectangle?
	if (typeid(rOther) == typeid(Rectangle2D))
		return dynamic_cast<const Rectangle2D*> (&rOther)->Intersects(*this);

	// Is it a circle?
	if (typeid(rOther) == typeid(Circle2D))
		return dynamic_cast<const Circle2D*> (&rOther)->Intersects(*this);

	// Is it a segment?
	if (typeid(rOther) == typeid(Segment2D))
		return dynamic_cast<const Segment2D*> (&rOther)->Intersects(*this);

	// Is it a point?
	if (typeid(rOther) == typeid(Point2D))
		return dynamic_cast<const Point2D*> (&rOther)->Intersects(*this);

	// Is it a point?
	if (typeid(rOther) == typeid(Rectangle2D))
		return dynamic_cast<const Rectangle2D*> (&rOther)->Intersects(*this);

	// Is it an angled rectangle?
	if (typeid(rOther) == typeid(AngledRectangle2D))
		return dynamic_cast<const AngledRectangle2D*> (&rOther)->Intersects(*this);

	// Undefined shape
	return false;

}

bool Segment2D::Intersects(const Segment2D& rOther) const
{
	// Check that lines are not parallel

	if(!ParallelTo(rOther))
	{
		//Find the t (parametric equation parameter) of the
		// point of intersection on this line

		// Find t. Big equation from
		// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
		float t1 =(   (rOther.mEnd.XValue - rOther.mStart.XValue)*(this->mStart.YValue - rOther.mStart.YValue)
			- (rOther.mEnd.YValue - rOther.mStart.YValue) * (this->mStart.XValue - rOther.mStart.XValue)    )
			/ (    (rOther.mEnd.YValue - rOther.mStart.YValue) * (this->mEnd.XValue - this->mStart.XValue)
			- (rOther.mEnd.XValue - rOther.mStart.XValue) * (this->mEnd.YValue - this->mStart.YValue)  );

		Vector2D p1(this->PointFromT(t1));		// Point of intersection

		float t2 = rOther.GetTFromX(p1.XValue);

		if(t1>=0 && t1<1.0 && t2>=0 && t2<1.0)	// Intersection
			return true;
		else
			return false;
	}
	else		// Parallel lines
		return false;
}

float Segment2D::Distance(const Segment2D& rOther) const
{
	// Check that lines are not parallel

	if(!ParallelTo(rOther))
	{
		//Find the t (parametric equation parameter) of the
		// point of intersection on this line

		// Find t. Big equation from
		// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
		float t1 =(   (rOther.mEnd.XValue - rOther.mStart.XValue)*(mStart.YValue - rOther.mStart.YValue)
			- (rOther.mEnd.YValue - rOther.mStart.YValue) * (mStart.XValue - rOther.mStart.XValue)    )
			/ (    (rOther.mEnd.YValue - rOther.mStart.YValue) * (mEnd.XValue - mStart.XValue)
			- (rOther.mEnd.XValue - rOther.mStart.XValue) * (mEnd.YValue - mStart.YValue)  );

		Vector2D p1(this->PointFromT(t1));		// Point of intersection

		float t2 = rOther.GetTFromX(p1.XValue);

		if(t1>=0 && t1<1.0 && t2>=0 && t2<1.0)	// Intersection
			return 0;							// Return zero separation
	}
		
	// Intersection not on segments or lines parallel
	float distances[4];				// Between four points
	distances[0] = rOther.Distance(Point2D(mStart));
	distances[1] = rOther.Distance(Point2D(mEnd));
	distances[2] = this->Distance(Point2D(rOther.mStart));
	distances[3] = this->Distance(Point2D(rOther.mEnd));

	// Find smallest
	int smallest = 0;
	for(int i=1;i<4;i++)
	{
		if(distances[i]<distances[smallest])
		smallest =i;
	}

	return distances[smallest];
}

bool Segment2D::ParallelTo(const Segment2D& rOther) const
{
	return ParallelTo(rOther.mEnd - rOther.mStart);
}

bool Segment2D::ParallelTo(const Vector2D& rOther) const
{
	Vector2D direction = mEnd - mStart;
	double dotProduct = direction.unitVector()* rOther.unitVector();
	if(dotProduct<1.00001 && dotProduct>0.99999)
		return true;
	if(dotProduct>-1.00001 && dotProduct<-0.99999)
		return true;
	else return false;
}

Vector2D Segment2D::Intersection(const Segment2D& rOther) const
{
	// Check that lines are not parallel

	if(!ParallelTo(rOther))
	{
		//Find the t (parametric equation parameter) of the
		// point of intersection on this line

		// Find t. Big equation from
		// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
		float t1 =(   (rOther.mEnd.XValue - rOther.mStart.XValue)*(mStart.YValue - rOther.mStart.YValue)
			- (rOther.mEnd.YValue - rOther.mStart.YValue) * (mStart.XValue - rOther.mStart.XValue)    )
			/ (    (rOther.mEnd.YValue - rOther.mStart.YValue) * (mEnd.XValue - mStart.XValue)
			- (rOther.mEnd.XValue - rOther.mStart.XValue) * (mEnd.YValue - mStart.YValue)  );

		Vector2D p1(this->PointFromT(t1));		// Point of intersection

		float t2 = rOther.GetTFromX(p1.XValue);

		if(t1>=0 && t1<1.0 && t2>=0 && t2<1.0)	// Intersection
			return p1;							// Return the point
	}
		
	// Intersection not on segments or lines parallel
	double distances[4];				// Between four points
	distances[0] = rOther.Distance(Point2D(mStart));
	distances[1] = rOther.Distance(Point2D(mEnd));
	distances[2] = this->Distance(Point2D(rOther.mStart));
	distances[3] = this->Distance(Point2D(rOther.mEnd));

	// Find smallest
	int smallest = 0;
	for(int i=1;i<4;i++)
	{
		if(distances[i]<distances[smallest])
		smallest =i;
	}

	if(smallest == 0)			// my start closest to other segment
		return mStart;
	else if(smallest == 1)		// My end closest to other segment
		return mEnd;
	else if(smallest == 2)		// Other line's start closest to my segment
		return Intersection(Point2D(rOther.mStart));
	else						// other line's end closest to my segment
		return Intersection(Point2D(rOther.mEnd));
	
}

bool Segment2D::Intersects(const Circle2D& rOther) const
{
	return (Distance(rOther)<0);
}

float Segment2D::Distance(const Circle2D& rOther) const
{
	// Get distance to the centre and subtract the radius
	return (Distance( Point2D(rOther.GetCentre())) - rOther.GetRadius());;
}

Vector2D Segment2D::Intersection(const Circle2D& rOther) const
{

	return Intersection(Point2D(rOther.mCentre));
}

bool Segment2D::Intersects(const Rectangle2D& rOther) const
{
	if(GetLength()==0)	// Special case
	{
		return(rOther.Intersects(Point2D(mEnd)));
	}
	Segment2D clipped = rOther.Clip(*this);
	return (clipped.GetLength()>0);
}

bool Segment2D::Intersects(const AngledRectangle2D& rOther) const
{
   return rOther.Intersects(*this);
}

float Segment2D::Distance(const Rectangle2D& rOther) const
{
	// Slow
	if(Intersects(rOther))
		return 0.0;
	return Distance(Point2D(Intersection(rOther)));
}

Vector2D Segment2D::FirstIntersection(const Rectangle2D& rOther) const
{
	Segment2D clipped = rOther.Clip(*this);
	if(clipped.GetLength()==0)	// No intersection
		return mEnd;
	else
		return clipped.mStart;
}

Vector2D Segment2D::Intersection(const Rectangle2D& rOther) const
{
	Segment2D clipped = rOther.Clip(*this);
	if(clipped.GetLength()>0)	// If intersects
		return (clipped.PointFromT(0.5)); // return mid-point of the clipped line

	// What if end of line is closest to an edge, not a corner?
	else						// Not intersects
	{
		double distances[6];	// From each corner to the line and
								// each line edge to the rectangle

		// Get distances - this is gonna be slow
		distances[0] = Distance(Point2D(rOther.GetTopLeft()));
		distances[1] = Distance(Point2D(rOther.GetTopRight()));
		distances[2] = Distance(Point2D(rOther.GetBottomLeft()));
		distances[3] = Distance(Point2D(rOther.GetBottomRight()));
		distances[4] = Point2D(mStart).Distance(rOther);
		distances[5] = Point2D(mEnd).Distance(rOther);

		// Find smallest
		int smallest = 0;
		for(int i=1;i<6;i++)
		{
			if(distances[i]<distances[smallest])
				smallest =1;
		}

		if(smallest == 0)	// Top Left
		{
			return Intersection(Point2D(rOther.GetTopLeft()));
		}
		else if(smallest == 1)	// Top right
		{
			return Intersection(Point2D(rOther.GetTopRight()));
		}
		else if(smallest == 2)	// Bottom left
		{
			return Intersection(Point2D(rOther.GetBottomLeft()));
		}
		else if(smallest == 3)	// Bottom right
		{
			return Intersection(Point2D(rOther.GetBottomRight()));
		}
		else if(smallest == 4)	// Start of line closest to an edge
		{
			return Point2D(mStart).Intersection(rOther);
		}
		else 	//End of line closest to an edge
		{
			return Point2D(mEnd).Intersection(rOther);
		}
	}
}

float Segment2D::GetTFromX(float x) const
{
	if(mEnd.XValue-mStart.XValue == 0.0)
	{
		if(x == mEnd.XValue)
			return 1.0;
		else
			return 0;
	}
	else
		return (x - mStart.XValue) / (mEnd.XValue - mStart.XValue);
}

float Segment2D::GetTFromY(float y) const
{
	if(mEnd.YValue - mStart.YValue == 0.0)
	{
		if(y==mEnd.YValue)
			return 1.0;
		else
			return 0;
	}
	else
		return (y-mStart.YValue)/(mEnd.YValue-mStart.YValue);
}

Vector2D Segment2D::PointFromT(float t) const
{
	return (mStart+(mEnd-mStart)*t);
}

void Segment2D::SetStartTo(float t)
{
	mStart.set(mStart.XValue + t* (mEnd.XValue-mStart.XValue) , mStart.YValue + t* (mEnd.YValue-mStart.YValue) );
}

void Segment2D::SetEndTo(float t)
{
	mEnd.set(mStart.XValue + t* (mEnd.XValue-mStart.XValue) , mStart.YValue + t* (mEnd.YValue-mStart.YValue) );
}

// *********************************************************************
// Member functions for Circle2D
// *********************************************************************

Circle2D::Circle2D(): mdRadius(0)
{
	mCentre.set(0,0);
}

Circle2D::Circle2D(const Vector2D& rCentre, float radius)
{
	mCentre = rCentre;
	if(radius>=0)
		mdRadius=radius;
	else
		mdRadius=-radius;
}


void Circle2D::PlaceAt(const Vector2D& rCentre, float radius)
{
	PlaceAt(rCentre);
	if(radius >= 0)
		mdRadius = radius;
	else
		mdRadius =- radius;
}

void Circle2D::PlaceAt(const Vector2D& rCentre)
{
	mCentre = rCentre;
}


Vector2D Circle2D::GetCentre() const
{

	return mCentre;
}

float Circle2D::GetRadius() const
{

	return mdRadius;
}
float Circle2D::GetArea() const
{
	return 3.141593f * mdRadius*mdRadius;
}

bool Circle2D::Intersects(const Point2D& rOther) const
{
	return((rOther.GetPosition()-mCentre).magnitude()<mdRadius);
}

float Circle2D::Distance(const Point2D& rOther) const
{
	return((rOther.GetPosition()-mCentre).magnitude()-mdRadius);
}

Vector2D Circle2D::Intersection(const Point2D& rOther) const
{
	Vector2D toPoint = rOther.mPosition - mCentre;
	toPoint = toPoint.unitVector();
	return mCentre+toPoint*mdRadius;
}

bool Circle2D::Intersects(const Segment2D& rOther) const
{
	return rOther.Intersects(*this);
}

bool Circle2D::Intersects(const IShape2D& rOther) const
{
	// Is it a rectangle?
	if(typeid(rOther)==typeid(Rectangle2D))
		return dynamic_cast<const Rectangle2D*> (&rOther)->Intersects(*this);

	// Is it a circle?
	if (typeid(rOther) == typeid(Circle2D))
		return dynamic_cast<const Circle2D*> (&rOther)->Intersects(*this);

	// Is it a segment?
	if (typeid(rOther) == typeid(Segment2D))
		return dynamic_cast<const Segment2D*> (&rOther)->Intersects(*this);

	// Is it a point?
	if (typeid(rOther) == typeid(Point2D))
		return dynamic_cast<const Point2D*> (&rOther)->Intersects(*this);

	// Is it a point?
	if (typeid(rOther) == typeid(Rectangle2D))
		return dynamic_cast<const Rectangle2D*> (&rOther)->Intersects(*this);

	// Is it an angled rectangle?
	if (typeid(rOther) == typeid(AngledRectangle2D))
		return dynamic_cast<const AngledRectangle2D*> (&rOther)->Intersects(*this);

	// Undefined shape
	return false;
}

float Circle2D::Distance(const Segment2D& rOther) const
{
	return Distance(rOther.Intersection(mCentre));
}

Vector2D Circle2D::Intersection(const Segment2D& rOther) const
{
	return Intersection(rOther.Intersection(*this));	// Implicit cast to Point2D
}

bool Circle2D::Intersects(const Circle2D& rOther) const
{
	return( (rOther.GetCentre()-mCentre).magnitude() < (mdRadius + rOther.GetRadius()) );
}

float Circle2D::Distance(const Circle2D& rOther) const
{
	return( (rOther.GetCentre()-mCentre).magnitude() - (mdRadius+ rOther.GetRadius()) );
}

Vector2D Circle2D::Intersection(const Circle2D& rOther) const
{
	return Intersection(Point2D(rOther.mCentre));
}

bool Circle2D::Intersects(const Rectangle2D& rOther) const
{
	Point2D c(this->mCentre);
	if(c.Distance(rOther)<this->mdRadius)
		return true;
	else
		return false;
}

bool Circle2D::Intersects(const AngledRectangle2D& rOther) const
{
   return rOther.Intersects(*this);
}

float Circle2D::Distance(const Rectangle2D& rOther) const
{
	Point2D c(this->mCentre);
	return(c.Distance(rOther)-mdRadius);
}

Vector2D Circle2D::Intersection(const Rectangle2D& rOther) const
{
	Vector2D otherEdge = rOther.Intersection(*this)-mCentre;
	return (mCentre + otherEdge.unitVector()*mdRadius);
}

Vector2D Circle2D::CollisionNormal(const Point2D& rOther) const
{
	return (rOther.mPosition-mCentre).unitVector();
}

Vector2D Circle2D::CollisionNormal(const Circle2D& rOther) const
{
	return (rOther.mCentre-mCentre).unitVector();
}

Vector2D Circle2D::CollisionNormal(const Rectangle2D& rOther) const
{
	return (rOther.Intersection(*this)-mCentre).unitVector();
}

Vector2D Circle2D::CollisionNormal(const Segment2D& rOther) const
{
	return (rOther.Intersection(*this)-mCentre).unitVector();
}


// ********************************************************************
// Member functions for Rectangle2D
// ********************************************************************

Rectangle2D::Rectangle2D()
{
	this->mCorner1.set(0,0);
	this->mCorner2.set(0,0);
}

void Rectangle2D::PlaceAt(float top, float left, float bottom, float right)
{
	PlaceAt(Vector2D(left,top), Vector2D(right, bottom));
}

Vector2D Rectangle2D::GetCentre() const
{
	return (mCorner1+mCorner2)/2;
}

void Rectangle2D::PlaceAt(Vector2D bottomLeft, Vector2D topRight)
{
	// Verify corner 1 is bottom left and corner 2 is top right
	float top, left, bottom, right;
	if(bottomLeft.XValue<=topRight.XValue)
	{
		left = bottomLeft.XValue;
		right = topRight.XValue;
	}
	else
	{
		right = bottomLeft.XValue;
		left = topRight.XValue;
	}

	if(bottomLeft.YValue<=topRight.YValue)
	{
		bottom = bottomLeft.YValue;
		top = topRight.YValue;
	}
	else
	{
		top = bottomLeft.YValue;
		bottom = topRight.YValue;
	}
	
	mCorner1.set(left, bottom);
	mCorner2.set(right, top);
}

Vector2D Rectangle2D::GetCorner1() const
{
	return mCorner1;
}

Vector2D Rectangle2D::GetCorner2() const
{
	return mCorner2;
}

Vector2D Rectangle2D::GetBottomLeft() const
{
	return mCorner1;
}

Vector2D Rectangle2D::GetTopRight() const
{
	return mCorner2;
}

Vector2D Rectangle2D::GetTopLeft() const
{
	return Vector2D(mCorner1.XValue, mCorner2.YValue);
}

Vector2D Rectangle2D::GetBottomRight() const
{
	return Vector2D(mCorner2.XValue, mCorner1.YValue);
}

float Rectangle2D::GetArea() const
{
	float h = mCorner1.XValue - mCorner2.XValue;
	float w = mCorner1.YValue - mCorner2.YValue;

	float a = h* w;

	if(a<0) return -a;
	else return a;
}

bool Rectangle2D::Intersects(const Point2D& rOther) const
{
	return rOther.Intersects(*this);
}

Vector2D Rectangle2D::CollisionNormal(const Point2D& rOther)const
{
	Vector2D answer;
		double t,b,l,r;
		t = rOther.GetPosition().YValue-GetCorner1().YValue;
		b = GetCorner2().YValue - rOther.GetPosition().YValue;
		l = rOther.GetPosition().XValue-GetCorner1().XValue;
		r = GetCorner2().XValue - rOther.GetPosition().XValue;

		double smallest = t;
		{
			answer.set(0,1);
		}
		if(smallest>b)
		{
			answer.set(0,-1);
			smallest =b;
		}
		if(smallest>l)
		{
			answer.set(-1,0);
			smallest = l;
		}
		if(smallest>r)
		{
			answer.set(1,0);
			smallest =r;
		}
		if(smallest<0)
		{
			answer.set(0,0);
		}
		return answer;
}

float Rectangle2D::Distance(const Point2D& rOther) const
{
		
	return rOther.Distance(*this);
}

Vector2D Rectangle2D::Intersection(const Point2D& rOther) const
{
	// 9 possibilities
	if(rOther.mPosition.XValue < mCorner1.XValue)
	{	// Left side
		if(rOther.mPosition.YValue<mCorner1.YValue)
		{	// Top left
			return mCorner1;
		}
		else if(rOther.mPosition.YValue>=GetCorner2().YValue)
		{	// Bottom left
			return GetBottomLeft();
		}
		else
		{	// Middle left
			return Vector2D(mCorner1.XValue, rOther.mPosition.YValue);
		}

	}
	else if(rOther.mPosition.XValue >= mCorner2.XValue)
	{	// Right side
		if(rOther.mPosition.YValue < mCorner1.YValue)
		{	// Top right
			return GetTopRight();
		}
		else if(rOther.mPosition.YValue >= mCorner2.YValue)
		{	// Bottom right
			return mCorner2;
		}
		else
		{	// Middle right
			return Vector2D(mCorner2.XValue, rOther.mPosition.YValue);
		}
	}
	else
	{	// Centre line
		if(rOther.mPosition.YValue < mCorner1.YValue)
		{	// Top 
			return Vector2D(rOther.mPosition.XValue, mCorner1.YValue);
		}
		else if(rOther.mPosition.YValue>=mCorner2.YValue)
		{	// Bottom
			return Vector2D(rOther.mPosition.XValue, mCorner2.YValue);
		}
		else
		{	// Inside rectangle  get distances to edges
			double t,b,l,r;
			t = rOther.mPosition.YValue-mCorner1.YValue;
			b = mCorner2.YValue - rOther.mPosition.YValue;
			l = rOther.mPosition.XValue-mCorner1.XValue;
			r = mCorner2.XValue - rOther.mPosition.XValue;

			double smallest = t;
			if(smallest>b)
				smallest =b;
			if(smallest>l)
				smallest = l;
			if(smallest>r)
				smallest =r;

			if(smallest == t)
				return Vector2D(rOther.mPosition.XValue, mCorner1.YValue);
			else if(smallest == b)
				return Vector2D(rOther.mPosition.XValue, mCorner2.YValue);
			else if(smallest == l)
				return Vector2D(mCorner1.XValue, rOther.mPosition.YValue);
			else		// Smallest ==r
				return Vector2D(mCorner2.XValue, rOther.mPosition.YValue);
		}
	}

}

bool Rectangle2D::Intersects(const Segment2D& rOther) const
{
	return rOther.Intersects(*this);
}

bool Rectangle2D::Intersects(const AngledRectangle2D& rOther) const
{
   return rOther.Intersects(*this);
}

bool Rectangle2D::Intersects(const IShape2D& rOther) const
{
	// Is it a rectangle?
	if (typeid(rOther) == typeid(Rectangle2D))
		return dynamic_cast<const Rectangle2D*> (&rOther)->Intersects(*this);

	// Is it a circle?
	if (typeid(rOther) == typeid(Circle2D))
		return dynamic_cast<const Circle2D*> (&rOther)->Intersects(*this);

	// Is it a segment?
	if (typeid(rOther) == typeid(Segment2D))
		return dynamic_cast<const Segment2D*> (&rOther)->Intersects(*this);

	// Is it a point?
	if (typeid(rOther) == typeid(Point2D))
		return dynamic_cast<const Point2D*> (&rOther)->Intersects(*this);

	// Is it a point?
	if (typeid(rOther) == typeid(Rectangle2D))
		return dynamic_cast<const Rectangle2D*> (&rOther)->Intersects(*this);

	// Is it an angled rectangle?
	if (typeid(rOther) == typeid(AngledRectangle2D))
		return dynamic_cast<const AngledRectangle2D*> (&rOther)->Intersects(*this);

	// Undefined shape
	return false;
}

Vector2D Rectangle2D::Intersection(const Segment2D& rOther) const
{
	// Get the intersection point on the other segment
	// and return the point closest to it
	return Intersection(rOther.Intersection(*this));
}

bool Rectangle2D::Intersects(const Circle2D& rOther) const
{
	return rOther.Intersects(*this);
}

float Rectangle2D::Distance(const Circle2D& rOther) const
{

	return rOther.Distance(*this);
}

Vector2D Rectangle2D::Intersection(const Circle2D& rOther) const
{
		// Return the point closest to the centre of the circle
	return Intersection(rOther.mCentre);
}

Segment2D Rectangle2D::Clip(Segment2D other) const
{
	
	float top =this->mCorner1.YValue;
	float bottom =this->mCorner2.YValue;
	float left =this->mCorner1.XValue;
	float right =this->mCorner2.XValue;
	Vector2D p1 = other.GetStart();
	Vector2D p2 = other.GetEnd();

	if(p1.XValue<left && p2.XValue<left)
		return Segment2D();				// Both ends left of rectangle - empty segment
	if(p1.XValue>=right && p2.XValue>=right)
		return Segment2D();				// Both ends right of rectangle - empty segment
	if(p1.YValue>=bottom && p2.YValue>=bottom)
		return Segment2D();				// Both ends below rectangle - empty segment
	if(p1.YValue<top && p2.YValue<top)
		return Segment2D();				// Both ends above rectangle - empty segment

	if( this->Intersects(Point2D(p1)) && this->Intersects(Point2D(p2)) )
		return other;					// Entirely inside rectangle - return entire segment

	// Time to get serious

	float maxT=1.0;
	float minT=0.0;
	float tempT=0;

	// Check for clip at the left edge
	if(p1.XValue<left && p2.XValue>=left)
	{
		minT = other.GetTFromX(left);
	}

	if(p2.XValue<left && p1.XValue>=left)
	{
		maxT = other.GetTFromX(left);
	}

	// Check for clip at the right edge
	if(p1.XValue<=right && p2.XValue>=right)
	{
		tempT = other.GetTFromX(right);
		if(tempT<maxT) maxT=tempT;

	}

	if(p2.XValue<=right && p1.XValue>=right)
	{
		tempT = other.GetTFromX(right);
		if(tempT>minT) minT=tempT;

	}

	// Check for clip at the top edge
	if(p1.YValue<top && p2.YValue>=top)
	{
		tempT = other.GetTFromY(top);
		if(tempT>minT) minT = tempT;
	}

	if(p2.YValue<top && p1.YValue>=top)
	{
		tempT = other.GetTFromY(top);
		if(tempT<maxT) maxT=tempT;

	}

	// Check for clip at bottom edge
	if(p1.YValue<=bottom && p2.YValue>=bottom)
	{
		tempT = other.GetTFromY(bottom);
		if(tempT<maxT) maxT=tempT;

	}

	if(p2.YValue<=bottom && p1.YValue>=bottom)
	{
		tempT = other.GetTFromY(bottom);
		if(tempT>minT) minT=tempT;
		cout << "b";
	}

	// Check for mint and maxt overlap - means you missed the rectangle
	if(minT > maxT)
	{
		other.PlaceAt(Vector2D(0,0), Vector2D(0,0));
	}
	else
	{
		other.ClipTo(minT, maxT);
	}

	return other;
}

bool Rectangle2D::Intersects(const Rectangle2D& rOther) const
{
	if(rOther.mCorner1.XValue >= this->mCorner2.XValue
		|| rOther.mCorner2.XValue <= this->mCorner1.XValue
		|| rOther.mCorner1.YValue >= this->mCorner2.YValue
		|| rOther.mCorner2.YValue <= this->mCorner1.YValue)
		return false;
	else
		return true;
}

float Rectangle2D::Distance(const Rectangle2D& rOther) const
{
	Rectangle2D clipRect = rOther;		// A rectangle that describes the overlap
										// between the two rectangles
	// Clip the left side
	if(clipRect.mCorner1.XValue<this->mCorner1.XValue)
		clipRect.mCorner1.XValue=this->mCorner1.XValue;

	// Clip the right side
	if(clipRect.mCorner2.XValue>this->mCorner2.XValue)
		clipRect.mCorner2.XValue=this->mCorner2.XValue;

	// Clip the top side
	if(clipRect.mCorner1.YValue<this->mCorner1.YValue)
		clipRect.mCorner1.YValue=this->mCorner1.YValue;

	// Clip the bottom side
	if(clipRect.mCorner2.YValue>this->mCorner2.YValue)
		clipRect.mCorner2.YValue=this->mCorner2.YValue;

	// Check to see if the clip rect has been inverted horizontally
	if(clipRect.mCorner1.XValue< clipRect.mCorner2.XValue)
	{
		// If it HASN'T, flatten the rectangle horizontally
		clipRect.mCorner1.XValue = clipRect.mCorner2.XValue;
	}
	// Check to see if the clip rect has been inverted vertically
	if(clipRect.mCorner1.YValue< clipRect.mCorner2.YValue)
	{
		// If it HASN'T, flatten the rectangle vertically
		clipRect.mCorner1.YValue = clipRect.mCorner2.YValue;
	}

	// Return the diagonal of the resultant rectangle (this will be zero) if
	// there is overlap
	return (clipRect.mCorner1- clipRect.mCorner2).magnitude();
}

Vector2D Rectangle2D::Intersection(const Rectangle2D& rOther) const
{	
	Rectangle2D clipRect = rOther;		// A rectangle that describes the overlap
										// between the two rectangles
	// Clip the left side
	if(clipRect.mCorner1.XValue<this->mCorner1.XValue)
		clipRect.mCorner1.XValue=this->mCorner1.XValue;

	// Clip the right side
	if(clipRect.mCorner2.XValue>this->mCorner2.XValue)
		clipRect.mCorner2.XValue=this->mCorner2.XValue;

	// Clip the top side
	if(clipRect.mCorner1.YValue<this->mCorner1.YValue)
		clipRect.mCorner1.YValue=this->mCorner1.YValue;

	// Clip the bottom side
	if(clipRect.mCorner2.YValue>this->mCorner2.YValue)
		clipRect.mCorner2.YValue=this->mCorner2.YValue;

	// Note that clip rect may be "inverted" if there is no intersection
	// i.e. top > bottom or left>right
	// Not a problem.

	// Return the intersection point of the centre of the clipped rectangle

	return Intersection(clipRect.GetCentre());
}

float Rectangle2D::LengthThrough(const Segment2D& rOther) const
{
	Segment2D seg = this->Clip(rOther);
	return seg.GetLength();
}

Vector2D Rectangle2D::CollisionNormal(const Circle2D& rOther) const
{
	if(Intersects(Vector2D(rOther.mCentre)))		// Centre of circle inside rectangle
	{
		float t,b,l,r;
		t = rOther.mCentre.YValue-mCorner1.YValue;
		b = mCorner2.YValue - rOther.mCentre.YValue;
		l = rOther.mCentre.XValue - mCorner1.XValue;
		r = mCorner2.XValue - rOther.mCentre.XValue;

		float smallest = t;
		if(smallest>b)
			smallest =b;
		if(smallest>l)
			smallest = l;
		if(smallest>r)
			smallest =r;

		if(smallest == t)
			return Vector2D(0,1);
		else if(smallest == b)
			return Vector2D(0,-1);
		else if(smallest == l)
			return Vector2D(-1,0);
		else		// Smallest ==r
			return Vector2D(1,0);
	}
	else		// Centre of circle outside rectangle
	{
		Vector2D edge = Intersection(Point2D(rOther.mCentre));
		Vector2D normal = rOther.mCentre - edge;
		return normal.unitVector();
	}
}
Vector2D Rectangle2D::CollisionNormal(const Rectangle2D& rOther) const
{
	Vector2D answer(0,1);		// Assume an upwards collision
	// Get distance between this bottom and other top
	float distance = abs(this->mCorner1.YValue - rOther.mCorner2.YValue);
	
	// See if downwards collision is smaller
	if(distance > abs(this->mCorner2.YValue - rOther.mCorner1.YValue))
	{
		answer.set(0, -1);
		distance = abs(this->mCorner2.YValue - rOther.mCorner1.YValue);
	}

	// See if leftwards collision is smaller
	if(distance > abs(this->mCorner1.XValue - rOther.mCorner2.XValue))
	{
		answer.set(-1, 0);
		distance = abs(this->mCorner1.XValue - rOther.mCorner2.XValue);
	}

	// See if rightwards collision is smaller
	if(distance > abs(this->mCorner2.XValue - rOther.mCorner1.XValue))
	{
		answer.set(1, 0);
		distance = abs(this->mCorner2.XValue - rOther.mCorner1.XValue);
	}

	return answer;
}
Vector2D Rectangle2D::CollisionNormal(const Segment2D& rOther) const
{
	return CollisionNormal(rOther.Intersection(*this));
}


// Angled rectangle ***************************************************

AngledRectangle2D::AngledRectangle2D()
{
   mWidth = 0;
   mHeight = 0;
   mAngle = 0;
   UpdateTrivialRejector();
}

// Constructs an AngledRectangle2D at rotation 0, with the given height
// and width and centre
AngledRectangle2D::AngledRectangle2D(Vector2D centre, float height, float width)
   :mWidth(width), mHeight(height), mCentre(centre)
{
   UpdateTrivialRejector();
   mLocalRectangle.PlaceAt(mHeight / 2, -mWidth / 2, -mHeight / 2, mWidth / 2);
}


void AngledRectangle2D::UpdateTrivialRejector()
{
   mTrivialRejector.PlaceAt(mCentre, sqrt(mWidth*mWidth/4 + mHeight*mHeight/4));
}

// Sets the height and width of the angled rectangle
void AngledRectangle2D::SetDimensions(float height, float width)
{
   mWidth = width;
   mHeight = height;
   UpdateTrivialRejector();
   mLocalRectangle.PlaceAt(mHeight / 2, -mWidth / 2, -mHeight / 2, mWidth / 2);
}

// Sets the centre of the angled rectangle
void AngledRectangle2D::SetCentre(Vector2D centre)
{
   mCentre = centre;
   UpdateTrivialRejector();
}

// Sets the angle of the rectangle
void AngledRectangle2D::SetAngle(float angle)
{
   mAngle = angle;
}

// Returns the current angle
float AngledRectangle2D::GetAngle() const
{
   return mAngle;
}

// Returns the current centre
Vector2D AngledRectangle2D::GetCentre() const
{
   return mCentre;
}

// Returns the current height
float AngledRectangle2D::GetHeight() const
{
   return mHeight;
}

// Returns the current width
float AngledRectangle2D::GetWidth() const
{
   return mHeight;
}

Vector2D AngledRectangle2D::TransformToLocal(Vector2D v) const
{
   v = v - mCentre;
   v = v.rotatedBy(-mAngle);
   return v;
}

Vector2D AngledRectangle2D::TranformBackFromLocal(Vector2D v) const
{

   v = v.rotatedBy(mAngle);
   v = v + mCentre;
   return v;
}


// Returns true if the AngledRectangle intersects with other shapes
bool AngledRectangle2D::Intersects(const IShape2D& rOther) const
{
	// Is it a rectangle?
	if (typeid(rOther) == typeid(Rectangle2D))
		return dynamic_cast<const Rectangle2D*> (&rOther)->Intersects(*this);

	// Is it a circle?
	if (typeid(rOther) == typeid(Circle2D))
		return dynamic_cast<const Circle2D*> (&rOther)->Intersects(*this);

	// Is it a segment?
	if (typeid(rOther) == typeid(Segment2D))
		return dynamic_cast<const Segment2D*> (&rOther)->Intersects(*this);

	// Is it a point?
	if (typeid(rOther) == typeid(Point2D))
		return dynamic_cast<const Point2D*> (&rOther)->Intersects(*this);

	// Is it a point?
	if (typeid(rOther) == typeid(Rectangle2D))
		return dynamic_cast<const Rectangle2D*> (&rOther)->Intersects(*this);

	// Is it an angled rectangle?
	if (typeid(rOther) == typeid(AngledRectangle2D))
		return dynamic_cast<const AngledRectangle2D*> (&rOther)->Intersects(*this);

	// Undefined shape
	return false;
}

bool AngledRectangle2D::Intersects(const Point2D& rOther) const
{
   if (!rOther.Intersects(mTrivialRejector))
   {
      return false;
   }
   else
   {
      Vector2D p = TransformToLocal(rOther.GetPosition());
      return mLocalRectangle.Intersects(p);
   }
}

bool AngledRectangle2D::Intersects(const Circle2D& rOther) const
{
   if (!rOther.Intersects(mTrivialRejector))
   {
      return false;
   }
   else
   {
      Circle2D c(TransformToLocal(rOther.mCentre), rOther.mdRadius);
      return mLocalRectangle.Intersects(c);
   }
}

bool AngledRectangle2D::Intersects(const Segment2D& rOther) const
{
   if (!rOther.Intersects(mTrivialRejector))
   {
      return false;
   }
   else
   {
      Segment2D s;
      s.PlaceAt(TransformToLocal(rOther.mStart), TransformToLocal(rOther.mEnd));
      return mLocalRectangle.Intersects(s);
   }
}

bool AngledRectangle2D::Intersects(const Rectangle2D& rOther) const
{
   if (!rOther.Intersects(mTrivialRejector))
   {
      return false;
   }
   else
   {
      // Do your corners intersect it?
      Vector2D p1(mWidth / 2, mHeight / 2);
      p1 = p1.rotatedBy(mAngle);
      if (rOther.Intersects(mCentre + p1))
         return true;
      Vector2D p3 = -p1;
      if (rOther.Intersects(mCentre + p3))
         return true;
      Vector2D p2(mWidth / 2, -mHeight / 2);
      p2 = p2.rotatedBy(mAngle);
      if (rOther.Intersects(mCentre + p2))
         return true;
      Vector2D p4 = -p2;
      if (rOther.Intersects(mCentre + p4))
         return true;

      // Do your segments intersect it?
      Segment2D s;
      s.PlaceAt(p1 + mCentre, p2 + mCentre);
      if (rOther.Intersects(s))
         return true;
      s.PlaceAt(p2 + mCentre, p3 + mCentre);
      if (rOther.Intersects(s))
         return true;
      s.PlaceAt(p3 + mCentre, p4 + mCentre);
      if (rOther.Intersects(s))
         return true;
      s.PlaceAt(p4 + mCentre, p1 + mCentre);
      if (rOther.Intersects(s))
         return true;
      return false;
   }
}

bool AngledRectangle2D::Intersects(const AngledRectangle2D& rOther) const
{
   if (!rOther.Intersects(mTrivialRejector))
   {
      return false;
   }
   else
   {
      // Do your corners intersect it?
      Vector2D p1(mWidth / 2, mHeight / 2);
      p1 = p1.rotatedBy(mAngle);
      if (rOther.Intersects(mCentre + p1))
         return true;
      Vector2D p3 = -p1;
      if (rOther.Intersects(mCentre + p3))
         return true;
      Vector2D p2(mWidth / 2, -mHeight / 2);
      p2 = p2.rotatedBy(mAngle);
      if (rOther.Intersects(mCentre + p2))
         return true;
      Vector2D p4 = -p2;
      if (rOther.Intersects(mCentre + p4))
         return true;

      // Do your segments intersect it?
      Segment2D s;
      s.PlaceAt(p1+mCentre, p2 + mCentre);
      if (rOther.Intersects(s))
         return true;
      s.PlaceAt(p2 + mCentre, p3 + mCentre);
      if (rOther.Intersects(s))
         return true;
      s.PlaceAt(p3 + mCentre, p4 + mCentre);
      if (rOther.Intersects(s))
         return true;
      s.PlaceAt(p4 + mCentre, p1 + mCentre);
      if (rOther.Intersects(s))
         return true;

      return false;
   }
}