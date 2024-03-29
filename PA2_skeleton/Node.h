

#pragma once
#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include "vector.h"
class Node
{
public:
	double	mass;
	vec3	impulseforce, constantforce, force;
	vec3	position, prepos, rk1pos;
	vec3	velocity, rk1vel;
	vec3	acceleration, rk1acc;
	vec3	normal;
	bool	isFixed;

public:
	Node(void)
	{
		isFixed = false;
		force.setZeroVector();
		constantforce.setZeroVector();
		impulseforce = vec3(0, 0, 0);
		velocity = vec3(0, 0, 0);
		acceleration = vec3(0, 0, 0);
		normal.setZeroVector();
		prepos = position;
		mass = 1.0;
	}
	Node(vec3 init_pos)
	{
		isFixed = false;
		position = init_pos;
		force.setZeroVector();
		constantforce.setZeroVector();
		impulseforce = vec3(0, 0, 0);
		velocity = vec3(0, 0, 0);
		acceleration = vec3(0, 0, 0);
		normal.setZeroVector();
		prepos = position;
		mass = 1.0;
	}
 
	~Node(void)
	{
	}

	double	getPosX(void) { return position.getX(); }
	double	getPosY(void) { return position.getY(); }
	double	getPosZ(void){ return position.getZ(); }

	void add_force(vec3 additional_force)
	{
		force += additional_force;
	}
	void add_impulse(vec3 force)
	{
		impulseforce += force;
	}
	void integrate(double dt)
	{
		prepos = position;
		if (!isFixed)
		{
			acceleration = force / mass;
			velocity += acceleration * dt;
			position += velocity * dt;

			//Basic Implements 2-2. Integration
		}
		/*initialize Force*/
		force.setZeroVector();
	}
	void rk1(double dt)
	{
		prepos = position;
		if (!isFixed)
		{
			rk1pos = position;
			rk1vel = velocity;
			acceleration = force / mass;
			velocity += acceleration * dt;
			position += velocity * dt;
			rk1acc = acceleration;

		}
		force.setZeroVector();
	}
	void rk2(double dt)
	{
		if (!isFixed)
		{
			acceleration = force / mass;
			position = rk1pos + (velocity+rk1vel) * dt / 2;
			velocity = rk1vel + (acceleration + rk1acc) * dt / 2;
		}
		/*initialize Force*/
		force.x = force.y = force.z = 0.0;
	}

	void draw();
	void draw(Node* second, Node* third, std::vector<vec3>* facePos,int i);
};
