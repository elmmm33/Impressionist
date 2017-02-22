//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include <cmath>
#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintview.h"
#include "ImpBrush.h"
#include <iostream>


#define MOUSE_MOVE			0
#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
static Point	coord;

PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;

}


void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	if(!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		ortho();

		glClear( GL_COLOR_BUFFER_BIT );		
	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();

	}

	if ( m_pDoc->m_ucPainting && isAnEvent) 
	{

		// Clear it after processing.
		isAnEvent	= 0;	

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );
		
		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			break;
		case LEFT_MOUSE_DRAG:
			m_pDoc->m_pCurrentBrush->BrushMove( source, target );
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );

			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			fixPoint=target;   //Define the initial fixPoint to create the effect
			break;
		case RIGHT_MOUSE_DRAG:
		{
			//Clear up the content before the next movement start
			RestoreContent();

			//glDrawBuffer(GL_BACK);  //Change the drawing buffer to back buffer.
			glLineWidth(1);

			glBegin( GL_LINES );
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex2d(fixPoint.x, fixPoint.y);
				glVertex2d(target.x, target.y);
			glEnd();
			break;
		}
		case RIGHT_MOUSE_UP:
		{
			RestoreContent();

			double xDist=target.x-fixPoint.x;
			double yDist=target.y-fixPoint.y;
			//Set the angle of Line
			if(xDist==0)
			{
				m_pDoc->setLineAngle(90);
			}
			else
			{
				double tanV=yDist/xDist;
				int angle=(atan2(yDist,xDist)/(2*PI)*360);
				m_pDoc->setLineAngle(angle);
			}

			//Set the size of line
			double dist;
			double tempDist=sqrt(xDist*xDist+yDist*yDist);
			if(tempDist<=1) 
			{
				dist=1;
				m_pDoc->setSize(1);
			}
			else
			{
				dist=(int)(tempDist + 0.5);
				m_pDoc->setSize(dist);
			}

			glClear(GL_BACK);
			break;
		}
		default:
			printf("Unknown event!!\n");		
			break;
		}
	}

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

}


int PaintView::handle(int event)
{
	switch(event)
	{
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		redraw();

		/* (Tim) Cursor on Origainl View [START] */
		m_pDoc->m_pUI->m_origView->isEvent = true;
		m_pDoc->m_pUI->m_origView->eventToDo = MOUSE_MOVE;
		m_pDoc->m_pUI->m_origView->cursor.x = coord.x;
		m_pDoc->m_pUI->m_origView->cursor.y = h() - coord.y;
		m_pDoc->m_pUI->m_origView->refresh();
		/* (Tim) Cursor on Origainl View [END] */

		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();

		/* (Tim) Cursor on Origainl View [START] */
		m_pDoc->m_pUI->m_origView->isEvent = true;
		m_pDoc->m_pUI->m_origView->eventToDo = MOUSE_MOVE;
		m_pDoc->m_pUI->m_origView->cursor.x = coord.x;
		m_pDoc->m_pUI->m_origView->cursor.y = h() - coord.y;
		m_pDoc->m_pUI->m_origView->refresh();
		/* (Tim) Cursor on Origainl View [END] */
		break;
	default:
		return 0;
		break;

	}



	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}


void PaintView::RestoreContent()
{
	//glDrawBuffer(GL_FRONT_AND_BACK);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

//	glDrawBuffer(GL_FRONT);
}

int PaintView::GetStartRow()
{
	return m_nStartRow;
}


int PaintView::GetEndRow()
{
	return m_nEndRow;
}
int PaintView::GetStartCol()
{
	return m_nStartCol;
}
int PaintView::GetEndCol()
{
	return m_nEndCol;
}
int PaintView::GetDrawHeight()
{
	return m_nDrawHeight;
}