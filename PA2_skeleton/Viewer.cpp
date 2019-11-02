
#pragma once
#include <iostream>
#include "Viewer.h"
#include "Windows.h"
using namespace std;

Viewer::Viewer(void)
{
	m_Zoom = 120;
	for (int i = 0; i < 2; i++)
	{
		m_Rotate[i] = 0.0f;
		m_Translate[i] = 0.0f;
		m_Mouse_Coord[i] = 0.0f;
	}
	m_Mouse_Event[0] = m_Mouse_Event[1] = m_Mouse_Event[2] = 0;
	m_start = false;
}

Viewer::~Viewer(void)
{
}

void Viewer::Initialize(void)
{
	S_Simulator.Initialize();
	S_Simulator.Lighting();
	//Additional Implements 4-1. OpenGL Initialization for Texture Mapping
}

void Viewer::Render(void)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -m_Zoom);
	
	glTranslatef(m_Translate[0], m_Translate[1], 0.0);
	glPushMatrix();
	glRotatef(m_Rotate[0], 1.0, 0.0, 0.0);
	glRotatef(m_Rotate[1], 0.0, 1.0, 0.0);
	S_Simulator.Render();
	glPopMatrix();
	float light_pos[] = { 150.0, 150.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glutSwapBuffers();
	glutPostRedisplay();
}

void Viewer::Reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w / h, 10.0, 500);
	glViewport(0, 0, w, h);
}

void Viewer::Mouse(int mouse_event, int state, int x, int y)
{
	m_Mouse_Coord[0] = x;
	m_Mouse_Coord[1] = y;
	switch (mouse_event)
	{
	case GLUT_LEFT_BUTTON:
		m_Mouse_Event[0] = ((GLUT_DOWN == state) ? 1 : 0);
		break;
	case GLUT_MIDDLE_BUTTON:
		m_Mouse_Event[1] = ((GLUT_DOWN == state) ? 1 : 0);
		break;
	case GLUT_RIGHT_BUTTON:
		m_Mouse_Event[2] = ((GLUT_DOWN == state) ? 1 : 0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

bool interationMode = false;
void Viewer::Motion(int x, int y)
{
	int diffx = x - m_Mouse_Coord[0];
	int diffy = y - m_Mouse_Coord[1];
	
	m_Mouse_Coord[0] = x;
	m_Mouse_Coord[1] = y;

	if (m_Mouse_Event[0])
	{
		if (!interationMode)
		{
			m_Rotate[0] += (float) 0.1 * diffy;
			m_Rotate[1] += (float) 0.1 * diffx;
		}
		else if (interationMode)
		{
			vec3 force = vec3(0.3*diffx, -0.3*diffy, 0);
			glPushMatrix();
			glLoadIdentity();
			GLfloat temp_matrix[16];
			glRotatef(m_Rotate[0], 1.0, 0.0, 0.0);
			glRotatef(-m_Rotate[1], 0.0, 1.0, 0.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, temp_matrix);
			/*glTranslatef(0.0, 0.0, -m_Zoom);
			glTranslatef(m_Translate[0], m_Translate[1], 0.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, temp_matrix);*/
			glPopMatrix();
			vec3 computedF = vec3();
			computedF.x = temp_matrix[0] * force.x + temp_matrix[4] * force.y + temp_matrix[8] * force.z + temp_matrix[12] * 1;
			computedF.y = temp_matrix[1] * force.x + temp_matrix[5] * force.y + temp_matrix[9] * force.z + temp_matrix[13] * 1;
			computedF.z = temp_matrix[2] * force.x + temp_matrix[6] * force.y + temp_matrix[10] * force.z + temp_matrix[14] * 1;
			
			S_Simulator.cloth->add_impulse(computedF);
			//Basic Implements 5. User Interaction
		}
 	}
	else if (m_Mouse_Event[1])
	{
		m_Translate[0] += (float) 0.1 * diffx;
		m_Translate[1] -= (float) 0.1 * diffy;
	}
	else if (m_Mouse_Event[2])
	{
		m_Zoom += (float) 0.1 * diffy;
	}
	glutPostRedisplay();
}

void Viewer::Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		S_Simulator.cloth->drawMode = mass_cloth::DRAW_MASS_NODES;
		break;
	case '2':
		S_Simulator.cloth->drawMode = mass_cloth::DRAW_SPRINGS;
		break;
	case '3':
		S_Simulator.cloth->drawMode = mass_cloth::DRAW_FACES;
		break;
	case 27:
	case 'q':
	case 'Q':
		exit(0);
		break;
	case' ':
		m_start = !m_start;
		break;
	case 'r':
	case 'R':
		S_Simulator.Initialize();
		break;
	case'f':
	case'F':
		interationMode = !interationMode;
		break;
	}
	glutPostRedisplay();
}

void Viewer::Update()
{
	if (m_start == true)
	{
		S_Simulator.Update();
	}
}