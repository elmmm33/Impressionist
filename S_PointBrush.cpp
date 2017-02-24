//
// S_PointBrush.cpp
//
// The implementation of Scattered Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "s_pointbrush.h"

extern float frand();

S_PointBrush::S_PointBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void S_PointBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	savePaintForUndo();
	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);
}

void S_PointBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	int size;
	glGetIntegerv(GL_POINT_SIZE, &size);
	glPointSize(1);

	for (int i = 0; i<size; ++i)
		for (int j = 0; j<size; ++j)
		{
			if ((rand() % 50)<25)
			{
				ImpBrush::c_pBrushes[BRUSH_POINTS]->BrushMove(Point((source.x - size / 2.0) + i, (source.y - size / 2.0) + j), Point((target.x - size / 2.0) + i, (target.y - size / 2.0) + j));
			}
		}

	glPointSize(float(size));
}

void S_PointBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

