#pragma once
#include "sssf_VS\stdafx.h"

class PhysicalProperties
{
protected:
	float buoyancy;
	float mass;
	bool  collidable;
	float coefficientOfRestitution;
	float x;
	float originalX;
	float y;
	float originalY;
	float z;
	float velocityX;
	float velocityY;
	float accelerationX;
	float accelerationY;
	const float MAX_SPEED = 20.0f;
	boolean affectedByGravity;

public:
	PhysicalProperties()	{accelerationX = 0; accelerationY = 0;}
	~PhysicalProperties()	{}

	// INLINED METHODS
	float getBuoyancy()						{ return buoyancy;				}
	float getMass()							{ return mass;					}
	float getCoefficientOfRestitution()		{ return coefficientOfRestitution;	}
	bool isCollidable()						{ return collidable;			}
	float getX()							{ return x;						}
	float getOriginalX()					{ return originalX; }
	float getY()							{ return y;						}
	float getOriginalY()					{ return originalY; }
	float getZ()							{ return z;						}
	float getVelocityX()					{ return velocityX;				}
	float getVelocityY()					{ return velocityY;				}
	float getAccelerationX()				{ return accelerationX;			}
	float getAccelerationY()				{ return accelerationY;			}
	boolean isAffectedByGravity()			{ return affectedByGravity; }
	void setBuoyancy(float initBuoyancy)	{ buoyancy = initBuoyancy;		}
	void setMass(float initMass)			{ mass = initMass;				}
	void setCofficientOfRestitution(float cR) { coefficientOfRestitution = cR; }
	void setCollidable(bool initCollidable)	{ collidable = initCollidable;	}
	void setX(float initX)					{ x = initX;					}
	void setOriginalX(float initX)					{ originalX = initX; }
	void incX(float xToInc)					{ x += xToInc;					}
	void setY(float initY)					{ y = initY;					}
	void setOriginalY(float initY)					{ originalY = initY; }
	void incY(float yToInc)					{ y += yToInc;					}
	void setZ(float initZ)					{ z = initZ;					}
	void setAffectedByGravity(boolean affectedByGravity){ this->affectedByGravity = affectedByGravity; }

	int round(float realNum)
	{
		return (int)floor(realNum + 0.5f);
	}

	void setPosition(float initX, float initY)
	{
		x = initX;
		y = initY;
	}

	void setVelocity(float initVx, float initVy)
	{
		velocityX = initVx;
		velocityY = initVy;
	}

	void incVelocity(float incVx, float incVy)
	{ 
		velocityX += incVx;
		velocityY += incVy;
	}
	void setAccelerationX(float initAx)		{ accelerationX = initAx;		}
	void setAccelerationY(float initAy)		{ accelerationY = initAy;		}

	float calcTotalVelocity()
	{
		float xSquared = x * x;
		float ySquared = y * y;
		float totalVelocity = xSquared + ySquared;
		totalVelocity = sqrtf(totalVelocity);
		return totalVelocity;
	}

	void applyAcceleration()
	{
		if (velocityX + accelerationX <= MAX_SPEED
			&& velocityX + accelerationX >= -MAX_SPEED){
			velocityX += accelerationX;
		}
		else{
			if (velocityX > 0){
				velocityX = MAX_SPEED;
			}
			else{
				velocityX = -MAX_SPEED;
			}
		}
		velocityY += accelerationY;
	}
};