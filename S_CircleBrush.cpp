//
// S_CircleBrush.cpp
//
// The implementation of Scattered Circle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

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
	savePaintForUndo();
	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);
}

void S_CircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	int length;
	glGetIntegerv(GL_POINT_SIZE, &length);
	double halfLength = length / 2.0;
	double startXS = source.x - halfLength;
	double startYS = source.y - halfLength;
	double startXT = target.x - halfLength;
	double startYT = target.y - halfLength;


	for (int i = 0; i<length; i++)
		for (int j = 0; j<length; j++)
		{
			Point newS = Point(startXS + i, startYS + j);
			Point newT = Point(startXT + i, startYT + j);

			double distance = sqrt((newT.x  - target.x) ^ 2 + (newT.y - target.x) ^ 2);
			if (distance>halfLength) continue;

			int seed = rand() % 50;
			if (seed<1)
			{
				ImpBrush::c_pBrushes[BRUSH_CIRCLES]->BrushMove(newS, newT);
			}
		}
}

void S_CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

