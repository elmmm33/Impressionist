// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"

// Include individual brush headers here.
#include "PointBrush.h"
#include "LineBrush.h"
#include "CircleBrush.h"
#include "S_CircleBrush.h"
#include "S_LineBrush.h"
#include "S_PointBrush.h"


#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }
const double PI = 3.141592653589793;

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;

	// initialize the pramaters by sherry
	m_ucAngle = NULL;
	m_ucGray = NULL;
	m_ucBlur = NULL;
	lineDirectPattern = 0;


	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]				
		= new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]				
		= new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]	
		= new S_PointBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]		
		= new S_LineBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]	
		= new S_CircleBrush( this, "Scattered Circles" );

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];

}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
	if (type != BRUSH_LINES && type != BRUSH_SCATTERED_LINES) {
		m_pUI->m_StrokeDirectionChoice->deactivate();
		m_pUI->m_LineWidthSlider->deactivate();
		m_pUI->m_LineAngleSlider->deactivate();
	}
	else
	{
		m_pUI->m_StrokeDirectionChoice->activate();
		m_pUI->m_LineWidthSlider->activate();
		m_pUI->m_LineAngleSlider->activate();
	}
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

// Add by sherry

// Return the width with the line brush
int ImpressionistDoc::getLineWidth()
{
	return m_pUI->getLineWidth();
}

// Return the angle with the line brush
int ImpressionistDoc::getLineAngle()
{
	return m_pUI->getLineAngle();
}

// Return the alpha value with the brush
double ImpressionistDoc::getAlpha()
{
	return m_pUI->getAlpha();	
}


// Return RGB color value
double ImpressionistDoc::getR_Color()
{
	return m_pUI->getR_Color();
}

double ImpressionistDoc::getG_Color()
{
	return m_pUI->getB_Color();
}

double ImpressionistDoc::getB_Color()
{
	return m_pUI->getG_Color();
}


// Set brush size
void ImpressionistDoc::setSize(int size)
{
	m_pUI->setSize(size);
}

// set stroke direction type
void ImpressionistDoc::setStrokeDirect(int type)
{
	switch (type)
	{
		case STROKE_SLIDER:
		{
			lineDirectPattern = STROKE_SLIDER;
			//printf("lineDirectPattern is %d/n", lineDirectPattern);
			break;
		}
		case STROKE_GRAD:
		{
			lineDirectPattern = STROKE_GRAD;
			//printf("lineDirectPattern is %d/n", lineDirectPattern);
			break;
		}
		case STROKE_BRUSH:
		{
			lineDirectPattern = STROKE_BRUSH;
			//printf("lineDirectPattern is %d/n", lineDirectPattern);
			break;
		}
		default:
		{
			printf("The stroke direction is not identifiable\n");
		}
	}
}

// set line brush width
void ImpressionistDoc::setLineWidth(int width)
{
	m_pUI->setLineWidth(width);
}

// set line brush angle
void ImpressionistDoc::setLineAngle(int angle)
{
	m_pUI->setLineAngle(angle);
}





// End by Sherry

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	if ( m_ucBitmap ) delete [] m_ucBitmap;
	if ( m_ucPainting ) delete [] m_ucPainting;
	if (m_ucAngle) delete[] m_ucAngle;
	if (m_ucGray) delete[] m_ucGray;
	if (m_ucBlur) delete[] m_ucBlur;


	m_ucBitmap		= data;

	m_ucPainting = new unsigned char[width*height * 3];
	m_ucGray = new unsigned char[width*height * 3];
	m_ucBlur = new unsigned char[width*height * 3];
	m_ucAngle = new int[width*height];

	// allocate space for draw view
	m_ucPainting	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);
	memset(m_ucAngle, 0, width*height * sizeof(int));
	memset(m_ucGray, 0, width*height * 3);
	memset(m_ucBlur, 0, width*height * 3);

	// add by sherry
	for (int i = 0; i < width*height; i++)
	{
		unsigned char r, g, b, lum;

		for (int j = 0; j<3; j++)
		{
			switch (j)
			{
			case 0:
				r = data[i * 3 + j];
				break;
			case 1:
				g = data[i * 3 + j];
				break;
			case 2:
				b = data[i * 3 + j];
				break;
			}
		}
		lum = (r*0.30) + (g*0.59) + (b*0.11);
		for (int j = 0; j<3; j++)
		{
			m_ucGray[i * 3 + j] = lum;
		}
	}

	for (int j = 1; j<height - 1; j++)
		for (int i = 1; i<width - 1; i++)
		{
			double sum = 0;
			for (int l = 0; l<3; l++)
				for (int k = 0; k<3; k++)
				{
					int x = i - 1 + k;
					int y = j - 1 + l;

					sum += m_ucGray[(x + y*width) * 3];
				}
			unsigned char blurVal = sum / 9;

			for (int m = 0; m<3; m++)
			{
				m_ucBlur[(i + j*width) * 3 + m] = blurVal;
			}
		}

	for (int j = 1; j<height - 1; j++)
		for (int i = 1; i<width - 1; i++)
		{
			double xDiff =
				m_ucBlur[(i + j*width) * 3] - m_ucBlur[(i - 1 + j*width) * 3];
			double yDiff =
				m_ucBlur[(i + j*width) * 3] - m_ucBlur[(i + (j - 1)*width) * 3];

			int angle = 0;
			if (xDiff == 0) angle = 90;
			else
			{
				double temp = atan2(yDiff, xDiff);
				angle = temp / (2 * PI) * 360;
			}

			angle = (angle + 90) % 360;
			m_ucAngle[i + j*width] = angle;

		}


	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();

	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}

