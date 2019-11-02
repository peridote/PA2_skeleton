
#pragma once

#include "spring.h"
#include "Node.h"
#include "sphere.h"
#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include "stb_image.h"
class mass_cloth
{
public:

	std::vector<Node *> nodes;
	std::vector<mass_spring *> spring;
	std::vector<Node*> faces;
	std::vector<vec3> facesRelPos;
	int			size_x, size_y, size_z;
	double		dx, dy, dz;
	double		structural_coef;
	double		shear_coef;
	double		bending_coef;
	int			iteration_n;
	int			drawMode;
	clock_t start, end;
	double result;
	double ground_offset, sphere_offset;

	mass_cloth()
	{ 	 
	}
	~mass_cloth()
	{ 
		for (int i = 0; i < nodes.size(); i++){ delete nodes[i]; }
		for (int i = 0; i < spring.size(); i++){ delete spring[i]; }
		nodes.clear();
		spring.clear();
		faces.clear();
	}
	enum DrawModeEnum{
		DRAW_MASS_NODES,
		DRAW_SPRINGS,
		DRAW_FACES
	};
 
public:
	void init()
	{
		//Basic Implements 1. Init Nodes and Shear and Structural Springs
		for(int k = 0; k < size_z; k++)
			for(int j = 0; j < size_y;j++)
				for (int i = 0; i < size_x; i++)
				{
					Node* node = new Node(vec3(j, k+40, i-25));
					nodes.push_back(node);
				}
		for(int k =0; k< size_z; k++)
			for(int j = 0; j < size_y-1 ; j++)
				for (int i = 0; i < size_x - 1; i++)
				{
					mass_spring* sp = new mass_spring(nodes[i+j*size_x], nodes[i+j*size_x+1]);
					sp->spring_coef = structural_coef;
					spring.push_back(sp);
					sp = new mass_spring(nodes[i + j * size_x], nodes[i + (j+1) * size_x]);
					sp->spring_coef = structural_coef;
					spring.push_back(sp);
					sp = new mass_spring(nodes[i + j * size_x], nodes[i + (j + 1) * size_x +1]);
					sp->spring_coef = shear_coef;
					spring.push_back(sp);
					sp = new mass_spring(nodes[i + j * size_x+1], nodes[i + (j + 1) * size_x]);
					sp->spring_coef = shear_coef;
					spring.push_back(sp);
				}
		for (int j = 1; j < size_y; j++)
		{
			mass_spring* sp = new mass_spring(nodes[j*size_x - 1], nodes[(j + 1)*size_x - 1]);
			sp->spring_coef = structural_coef;
			spring.push_back(sp);
		}
		for (int i = 0; i < size_x - 1; i++)
		{
			mass_spring* sp = new mass_spring(nodes[(size_y-1)*size_x + i], nodes[(size_y-1)*size_x + i + 1]);
			sp->spring_coef = structural_coef;
			spring.push_back(sp);
		}
		for (int k = 0; k < size_z; k++)
			for (int j = 0; j < size_y - 1; j++)
				for (int i = 0; i < size_x-2; i += 2)
				{
					mass_spring* sp = new mass_spring(nodes[i + size_x * j], nodes[i + size_x * j + 2]);
					sp->spring_coef = bending_coef;
					spring.push_back(sp);
				}
		for (int k = 0; k < size_z; k++)
			for (int j = 0; j < size_y - 2; j+=2)
				for (int i = 0; i < size_x - 1; i++)
				{
					mass_spring* sp = new mass_spring(nodes[i + size_x * j], nodes[i + size_x * (j + 2)]);
					sp->spring_coef = bending_coef;
					spring.push_back(sp);
				}
		for (int k = 0; k < size_z; k++)
			for (int j = 0; j < size_y-1; j++)
				for (int i = 0; i < size_x-1; i++)
				{
					faces.push_back(nodes[i + j*size_x]);
					faces.push_back(nodes[i + j * size_x + 1]);
					faces.push_back(nodes[i + (j + 1)*size_x]);
					facesRelPos.push_back(vec3(i/(float)size_x, j/ (float)size_y, k/ (float)size_z));
					facesRelPos.push_back(vec3((i+1) / (float)size_x, j / (float)size_y, k / (float)size_z));
					facesRelPos.push_back(vec3(i / (float)size_x, (j+1) / (float)size_y, k / (float)size_z));
					faces.push_back(nodes[i + (j + 1)*size_x]);
					faces.push_back(nodes[i + j * size_x + 1]);
					faces.push_back(nodes[i + (j + 1)*size_x + 1]);
					facesRelPos.push_back(vec3(i / (float)size_x, (j + 1) / (float)size_y, k / (float)size_z));
					facesRelPos.push_back(vec3((i+1) / (float)size_x, j / (float)size_y, k / (float)size_z));
					facesRelPos.push_back(vec3((i + 1) / (float)size_x, (j+1) / (float)size_y, k / (float)size_z));
					/*faces.push_back(nodes[(size_y - j)*size_x - i - 1]);
					faces.push_back(nodes[(size_y - j)*size_x - i - 2]);
					faces.push_back(nodes[(size_y - j - 1)*size_x - i - 1]);*/
				}
		nodes[size_x-1]->isFixed = true;
		nodes[0]->isFixed = true;
		//Additional Implements 1. Init Bending Spring
		/*
			Node *xp = new Node(vec3(x, y, z));
		
			mass_spring *sp = new mass_spring(p[Node_Index_A], p[Node_Index_B]);
 			sp->spring_coef = spring_Type_coef;
 			spring.push_back(sp);
		*/
		//Basic Implements 3-1. Generate Faces
		/*
			faces.push_back(p[Node_Index_A]);
			faces.push_back(p[Node_Index_C]);
			faces.push_back(p[Node_Index_B]);
		*/
		//Additional Implements 4-2. Initialize Texture Coordinates
	}
	
	void computeNormal()
	{
		//Basic Implements 3-2. Compute Vertex Normal
		/*
			for(each face)
			{
				compute face normal
			}
			for(each node)
			{
				인접한 face의 평균 normal
			}
		*/
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->normal.setZeroVector();
		}
		for (int i = 0; i < faces.size(); i += 3)
		{
			vec3 a = faces[i+1]->position - faces[i]->position;
			vec3 b = faces[i + 2]->position - faces[i]->position;
			vec3 c = a.Cross(b);
			c.Normalize();
			faces[i]->normal += c;
			faces[i+1]->normal += c;
			faces[i+2]->normal += c;
		}
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->normal.Normalize();
		}
	}
	
	void add_force(vec3 additional_force)
	{		 
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->add_force(additional_force);
		}
	}
	void add_impulse(vec3 additional_force)
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->add_impulse(additional_force);
		}
	}
	void compute_force(double dt, vec3 gravity)
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->add_force(gravity * nodes[i]->mass);
		}
		/* Compute Force for all springs */
		for (int i = 0; i < spring.size(); i++)
		{
			spring[i]->internal_force(dt);
		}
	}

	void integrate(double dt)
	{
		/* integrate Nodes*/
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->integrate(dt);
		}
	}
	void rk2(double dt, vec3 gravity)
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->rk1(dt);
		}
		compute_force(dt, gravity);
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->rk2(dt);
		}
	}
	void rk4(double dt, vec3 gravity)
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->rk1(dt/2);
		}
		compute_force(dt/2, gravity);
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->rk2(dt/2);
		}
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->rk1(dt / 2);
		}
		compute_force(dt / 2, gravity);
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->rk2(dt / 2);
		}
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->rk1(dt);
		}
		compute_force(dt, gravity);
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->rk2(dt);
		}
	}
	void collision_response(vec3 ground, Sphere* sphere)
	{
		//Basic Implements 4. Collision Check with ground
		for (int i = 0; i < nodes.size(); i++)
		{
			{
				vec3 p = nodes[i]->prepos;
				vec3 c = nodes[i]->position;
				double x = c.x - (c.x - p.x)*(ground.getY()+ground_offset-c.y) / (c.y - p.y);
				double z = c.z - (c.z - p.z)*(ground.getY()+ground_offset-c.y) / (c.y - p.y);
				if(p.y > ground.getY() + ground_offset && c.y < ground.getY() + ground_offset)
				if (x <= 250.0f&&x >= -250.0f&&z <= 250.0f&&z >= -250.0f) {
					
						vec3 vel = nodes[i]->velocity;
						nodes[i]->velocity = 0.2 * vel;
						nodes[i]->velocity.y *= -1;
						nodes[i]->position = vec3(x, ground.getY() + ground_offset, z);
						nodes[i]->integrate(0.01);
				}
			}
		}
		//Additional Implements 2. Collision Check with Sphere
		for (int i = 0; i < nodes.size(); i++)
		{
			vec3 lt = nodes[i]->position - sphere->position;
			if (lt.length() < sphere->r + ground_offset)
			{
				lt = (nodes[i]->position + nodes[i]->prepos) / 2;
				lt = lt - sphere->position;
				lt.Normalize();
				vec3 vel = nodes[i]->velocity;
				double d = lt.x*vel.x + lt.y*vel.y + lt.z*vel.z;
				vec3 newvel = vec3(vel.x - 2 * lt.x*d, vel.y - 2 * lt.y*d, vel.z - 2 * lt.z*d);
				nodes[i]->position = (nodes[i]->position + nodes[i]->prepos) / 2;
				nodes[i]->velocity = newvel;
				nodes[i]->integrate(0.01);
			}
		}
		//Additional Implements 3. Collision Check with Mesh Object
		/*
			if(Collision Detection)
			{
				Collision Response
			}
		*/
	}

	void draw(GLuint tex);
};