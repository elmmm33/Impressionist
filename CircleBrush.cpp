//
// CircleBrush.cpp
//
// The implementation of Circle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "circlebrush.h"

extern float frand();
const double t_Pi = 6.2831853;
const int triangles = 100;

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	double x = target.x;
	double y = target.y;

	int diam;
	glGetIntegerv(GL_POINT_SIZE, &diam);
	double radius = diam / 2.0;

	int windowH = dlg->m_mainWindow->h();
	int drawHeight = dlg->m_paintView->GetDrawHeight();

	int startCol = dlg->m_paintView->GetStartCol();
	int endCol = dlg->m_paintView->GetEndCol();
	int startRow = dlg->m_paintView->GetStartRow() + windowH - drawHeight - 25;
	int endRow = startRow + drawHeight;

	glBegin(GL_TRIANGLE_FAN);
	SetColor(source);
	glVertex2f(x, y);

	for (int i = 0; i <= triangles; i++) {
		double xNew = x + (radius * cos(i * t_Pi / triangles));
		double yNew = y + (radius * sin(i * t_Pi / triangles));

		if (xNew<startCol || xNew>endCol || yNew<startRow || yNew>endRow)
		{
			xNew = x;
			yNew = y;
		}

		glVertex2f(xNew, yNew);
	}
	glEnd();
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

