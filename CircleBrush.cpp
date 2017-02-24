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

	int s_Col = dlg->m_paintView->GetStartCol();
	int s_Row = dlg->m_paintView->GetStartRow() + windowH - drawHeight - 25;
	int e_Col = dlg->m_paintView->GetEndCol();
	int e_Row = s_Row + drawHeight;

	glBegin(GL_TRIANGLE_FAN);
	SetColor(source);
	glVertex2f(x, y);

	for (int i = 0; i <= triangles; i++) {
		double n_x = x + (radius * cos(i * t_Pi / triangles));
		double n_y = y + (radius * sin(i * t_Pi / triangles));

		if (n_x<s_Col || n_y<s_Row || n_x>e_Col || n_y>e_Row)
		{
			glVertex2f(x, y);
		}
		glVertex2f(n_x, n_y);
					
	}
	glEnd();
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

