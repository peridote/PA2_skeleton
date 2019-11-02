#include "Node.h"

#include <GL/glut.h>
#include "cloth.h"
void Node::draw()
{
	glDisable(GL_LIGHTING);
	glColor3f(0.97, 0.95, 0.15);
	glPointSize(2.0);

	glBegin(GL_POINTS);	
	glVertex3f(getPosX(), getPosY(), getPosZ());
	glEnd();
	glEnable(GL_LIGHTING);
}
void Node::draw(Node* second, Node* third, std::vector<vec3> *facePos, int i)
{
	//glColor3f(0.9, 0.3, 0.1);
	glNormal3f(normal.x, normal.y, normal.z);
	glTexCoord2f(1.0 - (*facePos)[i].x, (*facePos)[i].y);
	glVertex3f(getPosX(), getPosY(), getPosZ());
	glNormal3f(second->normal.x, second->normal.y, second->normal.z);
	glTexCoord2f(1.0 - (*facePos)[i+1].x, (*facePos)[i+1].y);
	glVertex3f(second->getPosX(), second->getPosY(), second->getPosZ());
	glNormal3f(third->normal.x, third->normal.y, third->normal.z);
	glTexCoord2f(1.0 - (*facePos)[i+2].x, (*facePos)[i+2].y);
	glVertex3f(third->getPosX(), third->getPosY(), third->getPosZ());
}
void mass_spring::draw()
{
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(2.0);

 	glBegin(GL_LINES);
	glVertex3f(p1->position.x, p1->position.y, p1->position.z);
	glVertex3f(p2->position.x, p2->position.y, p2->position.z);
  	glEnd();	 
	glEnable(GL_LIGHTING);

}

void mass_cloth::draw(GLuint tex)
{	
	switch (drawMode)
	{
	case DRAW_MASS_NODES:
		glDisable(GL_LIGHTING);
		for (int i = 0; i < nodes.size(); i++)
			nodes[i]->draw();
		glEnable(GL_LIGHTING);
		break;
	case DRAW_SPRINGS:
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		for (int i = 0; i < spring.size(); i++)
			spring[i]->draw();
		glEnable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		break;
	case DRAW_FACES:
		//Basic Implements 3-3. Draw Call for Cloth
		//glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_TRIANGLES);
		//glBindTexture(GL_TEXTURE_2D, tex);
		for (int i = 0; i < faces.size(); i+= 3)
			faces[i]->draw(faces[i+1], faces[i+2], &facesRelPos,i);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		//glDisable(GL_LIGHTING);
		//Additional Implements 4-3. Texture Coordinate Mapping
		break;
	default:
		break;
	}
	glPopMatrix();
}

