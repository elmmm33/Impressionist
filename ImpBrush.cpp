//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ImpBrush.h"

// Static class member initializations
int			ImpBrush::c_nBrushCount	= 0;
ImpBrush**	ImpBrush::c_pBrushes	= NULL;

const unsigned long RANGE=2*2*2*2*2*2*2*2-1;

ImpBrush::ImpBrush(ImpressionistDoc*	pDoc, 
				   char*				name) :
					m_pDoc(pDoc), 
					m_pBrushName(name)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColor (const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();
	GLubyte color[4];
	// glBlendFunc(GL_ZERO, GL_SRC1_ALPHA);
	memcpy ( color, pDoc->GetOriginalPixel( source ), 3 );
	double alpha = GetDocument()->getAlpha();
	double rColor = GetDocument()->getRColor();					// Red Filter
	double gColor = GetDocument()->getGColor();					// Green Filter
	double bColor = GetDocument()->getBColor();					// Blue Filter
	double brightness = GetDocument()->getBrightnessColor();	// Brightness Filter
	color[0] = GLubyte((color[0]/255.0*rColor*brightness)*RANGE);
	color[1] = GLubyte((color[1]/255.0*gColor*brightness)*RANGE);
	color[2] = GLubyte((color[2]/255.0*bColor*brightness)*RANGE);
	color[3] = GLubyte(alpha*RANGE);
	glColor4ubv( color );
}
//ADDED BY RYAN START
/*
void ImpBrush::SetColorAlpha(const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();

	//R,G,B,Alpha
	GLubyte color[4];

	memcpy(color, pDoc->GetOriginalPixel(source),3);

	double alpha=GetDocument()->getAlpha();
	color[3]=GLubyte(alpha*RANGE);

	//printf("color[0] is %d \n", color[0]);
	//printf("color[1] is %d \n", color[1]);
	//printf("color[2] is %d \n", color[2]);
	//printf("color[3] is %d \n", color[3]);

	//glClear(GL_COLOR_BUFFER_BIT);
	//glEnable (GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glColor4ubv(color);

	GLfloat colorNow[4];
	glGetFloatv(GL_CURRENT_COLOR, colorNow);

	for(int i=0;i<4;i++)
	{
		printf("colorNow[%d] is %f \n",i,colorNow[i]);
	}
	glFlush();
}
*/



//ADDED BY RYAN END
double ImpBrush::dist(Point a, Point b)
{
	double xDist=a.x-b.x;
	double yDist=a.y-b.y;
	return sqrt(xDist*xDist+yDist*yDist);
}

/* (Tim) Save Paint for Undo [START] */
void ImpBrush::savePaintForUndo() {
	ImpressionistDoc* pDoc = GetDocument();

	int dimension = pDoc->m_nPaintWidth * pDoc->m_nPaintHeight * 3;
	unsigned char* undo = new unsigned char [dimension];

	memcpy(undo, pDoc->m_ucPainting, dimension);
	pDoc->m_ucPainting_Undo.push(undo);
	
	/* ImpressionistDoc* pDoc = GetDocument();

	int dimension = pDoc->m_nPaintWidth * pDoc->m_nPaintHeight * 3;
	delete [] pDoc->m_ucPainting_Undo;
	pDoc->m_ucPainting_Undo = new unsigned char [dimension];
	memcpy(pDoc->m_ucPainting_Undo, pDoc->m_ucPainting, dimension); */
}
/* (Tim) Save Paint for Undo [END] */