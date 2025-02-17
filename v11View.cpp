#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "v11.h"
#endif

#include "v11Doc.h"
#include "v11View.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cv11View

IMPLEMENT_DYNCREATE(Cv11View, CView)

BEGIN_MESSAGE_MAP(Cv11View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cv11View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_REGISTERED_MESSAGE(AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM, Cv11View::OnHighlightRibbonListItem)
	ON_COMMAND(ID_SHAPE, Cv11View::OnShape)
	ON_COMMAND(ID_COLOR, Cv11View::OnColor)
END_MESSAGE_MAP()

// Cv11View construction/destruction

Cv11View::Cv11View() {
	this->s = 0;
	this->c = 0;
	this->s_p = 0;
	this->c_p = 0;
}

Cv11View::~Cv11View()
{
}

BOOL Cv11View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cv11View drawing

void Cv11View::OnDraw(CDC* pDC)
{
	CPen p;
	p.CreatePen(PS_SOLID, 2, c);
	pDC->SelectObject(p);
	CPoint tocka(100,100);

	switch (this -> s_p)
	{
	case 0:
		pDC->Rectangle(this -> rect);
		break;
	case 1:
		pDC->Ellipse(this->rect);
		break;
	case 2:
		pDC->RoundRect(this->rect, tocka);
	default:
		break;
	}
}


// Cv11View printing


void Cv11View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Cv11View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cv11View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cv11View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void Cv11View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cv11View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Cv11View diagnostics

#ifdef _DEBUG
void Cv11View::AssertValid() const
{
	CView::AssertValid();
}

void Cv11View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cv11Doc* Cv11View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cv11Doc)));
	return (Cv11Doc*)m_pDocument;
}
#endif //_DEBUG


void Cv11View::OnLButtonDown(UINT nFlags, CPoint tocka)
{
	CRectTracker tr;

	if (tr.TrackRubberBand(this, tocka))
	{
		rect = tr.m_rect;
		Invalidate();
	}
}

LRESULT Cv11View::OnHighlightRibbonListItem(WPARAM wp, LPARAM lp)
{
	int ind = (int)wp;
	CMFCRibbonBaseElement* pElem = (CMFCRibbonBaseElement*)lp;
	UINT id = pElem->GetID();

	switch (id)
	{
	case ID_COLOR:
		if (ind != -1)
		{
			CMFCRibbonColorButton* cButton = (CMFCRibbonColorButton*)pElem;
			c = cButton->GetHighlightedColor();
			
		}
		else
		{
			c = c_p;
		}
		break;
	case ID_SHAPE:
		if (ind != -1)
		{
			s = ind;
		}
		else
		{
			s = s_p;
			
		}
		break;
	}

	Invalidate();
	return 0;
}

void Cv11View::OnColor()
{
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_COLOR, arr);
	CMFCRibbonColorButton* pGallery = (CMFCRibbonColorButton*)arr.GetAt(0);

	c_p = pGallery->GetColor();
	c = c_p;

	Invalidate();
}

void Cv11View::OnShape()
{
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_SHAPE, arr);
	CMFCRibbonGallery* pGallery = (CMFCRibbonGallery*)arr.GetAt(0);

	s_p = pGallery->GetSelectedItem();
	s = s_p;

	Invalidate();
}