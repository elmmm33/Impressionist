//
// S_LineBrush.cpp
//
// The implementation of Scattered Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "s_linebrush.h"

extern float frand();

S_LineBrush::S_LineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void S_LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);
}

void S_LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
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
			if ((rand() % 200)<1)
			{
				ImpBrush::c_pBrushes[BRUSH_LINES]->BrushMove(Point((source.x - length / 2.0) + i, (source.y - length / 2.0) + j), Point((target.x - length / 2.0) + i, (target.y - length / 2.0) + j));
			}
		}
}

void S_LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

