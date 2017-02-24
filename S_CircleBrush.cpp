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
	BrushMove(source, target);
}

void S_CircleBrush::BrushMove(const Point source, const Point target)
{
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
	float theta = (float)(2 * 3.1415926) / float((int)(10 * sqrtf(r)));
	float x = r;
	float y = 0;

	glBegin(GL_TRIANGLE_FAN);
	SetColor(source);
	glVertex2f((GLfloat)target.x, (GLfloat)target.y);
	for (int ii = 0; ii < (int)(10 * sqrtf(r)); ii++)
	{
		glVertex2f(x + target.x, y + target.y);
		float tx = -y;
		float ty = x;
		x += tx * tanf(theta);
		y += ty * tanf(theta);
		x *= cosf(theta);
		y *= cosf(theta);
	}
	glVertex2f((GLfloat)(target.x + r), (GLfloat)target.y);
	glEnd();
}

