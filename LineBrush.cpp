//
// LineBrush.cpp
//
// The implementation of Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "linebrush.h"

extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	int width = GetDocument()->getLineWidth();
	double halfWidth = width / 2.0;   

	int length;
	glGetIntegerv(GL_POINT_SIZE, &length);
	double halfLength = length / 2.0; 


	int windowH = dlg->m_mainWindow->h();
	int drawHeight = dlg->m_paintView->GetDrawHeight();

	int startCol = dlg->m_paintView->GetStartCol();
	int endCol = dlg->m_paintView->GetEndCol();
	int startRow = dlg->m_paintView->GetStartRow() + windowH - drawHeight - 25;
	int endRow = startRow + drawHeight;

	// started line angle
	const double PI = 3.141592653589793;
	int angle = 0;

	switch (pDoc->lineDirectPattern)
	{
		case STROKE_SLIDER:
		{
			printf("line Direct Pattern is", pDoc->lineDirectPattern);
			angle = pDoc->getLineAngle();	
			break;
		}
		case STROKE_GRAD:
		{
			printf("line Direct Pattern is", pDoc->lineDirectPattern);
			int x = target.x - startCol;
			int y = target.y - startRow;
			int width = pDoc->m_nWidth;
			int height = pDoc->m_nHeight;

			if (x <= 0 || x > width) angle = 90;
			else if (y <= 0 || y > height) angle = 0;
			else angle = pDoc->m_ucAngle[x + y*width];
			break;
		}
		case STROKE_BRUSH:
		{
			printf("line Direct Pattern is", pDoc->lineDirectPattern);
			double xDiff = target.x - (pDoc->currentPoint.x);
			double yDiff = target.y - (pDoc->currentPoint.y);

			pDoc->currentPoint = target;

			if (xDiff == 0) angle = 90;
			else angle = atan2(yDiff, xDiff) / (2 * PI) * 360;
			break;
		}
		default:
		{
			printf("No such brush exist\n");
			break;
		}
	}

	double mathAngle = (angle % 360) / 360.0 * 2 * PI;
	double cosV = cos(mathAngle);
	double sinV = sin(mathAngle);
	//end line angle


	double axis[8];
	axis[0] = target.x - halfLength*cosV - halfWidth*sinV;
	axis[1] = target.y - halfLength*sinV + halfWidth*cosV;
	axis[2] = target.x - halfLength*cosV + halfWidth*sinV;
	axis[3] = target.y - halfLength*sinV - halfWidth*cosV;
	axis[4] = target.x + halfLength*cosV + halfWidth*sinV;
	axis[5] = target.y + halfLength*sinV - halfWidth*cosV;
	axis[6] = target.x + halfLength*cosV - halfWidth*sinV;
	axis[7] = target.y + halfLength*sinV + halfWidth*cosV;


	for (int i = 0; i<8; i += 2)
	{
		if (axis[i]<startCol) axis[i] = startCol;
		if (axis[i]>endCol) axis[i] = endCol;
	}
	for (int i = 1; i<8; i += 2)
	{
		if (axis[i]<startRow) axis[i] = startRow;
		if (axis[i]>endRow) axis[i] = endRow;
	}


	glBegin(GL_POLYGON);
	SetColor(source);
	glVertex2d(axis[0], axis[1]);
	glVertex2d(axis[2], axis[3]);
	glVertex2d(axis[4], axis[5]);
	glVertex2d(axis[6], axis[7]);
	glEnd();


}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

