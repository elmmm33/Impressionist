//
// S_CircleBrush.cpp
//
// The implementation of Scattered Circle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include <cstdlib>
#include <ctime>
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "s_circlebrush.h"


extern float frand();

S_CircleBrush::S_CircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void S_CircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	size = pDoc->getSize();
	num_points = 3;
	srand((unsigned int)(time(0)));
//	glPointSize((float)size);
	BrushMove(source, target);
}

void S_CircleBrush::BrushMove(const Point source, const Point target)
{
/*	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	int length;
	glGetIntegerv(GL_POINT_SIZE, &length);


	for (int i = 0; i<length; i++)
		for (int j = 0; j<length; j++)
		{
			Point newS = Point((source.x - length / 2.0) + i, (source.y - length / 2.0) + j);
			Point newT = Point((target.x - length / 2.0) + i, (target.y - length / 2.0) + j);

			double distance = sqrt((newT.x  - target.x) ^ 2 + (newT.y - target.x) ^ 2);
			if (distance>length / 2.0) continue;

			if ((rand() % 50)<1)
			{
				ImpBrush::c_pBrushes[BRUSH_CIRCLES]->BrushMove(Point((source.x - length / 2.0) + i, (source.y - length / 2.0) + j), Point((target.x - length / 2.0) + i, (target.y - length / 2.0) + j));
			}
		}
		*/
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredCircleBrush::BrushMove  document is NULL\n");
		return;
	}

	for (int i = 0; i < num_points; ++i) {
		int x = rand() % size - size / 2;
		int y = rand() % size - size / 2;
		Point paintPoint = Point((target.x + x), (target.y + y));
		DrawCircle(paintPoint, paintPoint, (float)size / (float)2.0);
	}
}

void S_CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

void S_CircleBrush::DrawCircle(Point source, Point target, float r) {
	//The brush is moved to a new place. I need to draw a filled circle there
	int num_segments = (int)(10 * sqrtf(r));//change the 10 to a smaller/bigger number as needed

	float theta = (float)(2 * 3.1415926) / float(num_segments);
	float tangetial_factor = tanf(theta);//calculate the tangential factor

	float radial_factor = cosf(theta);//calculate the radial factor

									  //set the initial point coordinate
	float x = r;//we start at angle = 0
	float y = 0;

	glBegin(GL_TRIANGLE_FAN);
	//set color
	SetColor(source);
	//first point is the center point
	glVertex2f((GLfloat)target.x, (GLfloat)target.y);
	//then loop to add the bounding points to the vertex map
	for (int ii = 0; ii < num_segments; ii++)
	{
		glVertex2f(x + target.x, y + target.y);//output vertex

											   //calculate the tangential vector
											   //remember, the radial vector is (x, y)
											   //to get the tangential vector we flip those coordinates and negate one of them

		float tx = -y;
		float ty = x;

		//add the tangential vector

		x += tx * tangetial_factor;
		y += ty * tangetial_factor;

		//correct using the radial factor

		x *= radial_factor;
		y *= radial_factor;
	}
	//last point should return to the first bounding point,
	//for GL_TRIANGLE_FAN doesn't close the loop altomatically
	glVertex2f((GLfloat)(target.x + r), (GLfloat)target.y);
	glEnd();
}

