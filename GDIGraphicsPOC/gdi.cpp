// (C) Copyright Gert-Jan de Vos and Jan Wilmans 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Repository at: https://github.com/djeedjay/DebugViewPP/

#include "stdafx.h"
#include "gdi.h"
#include <string>
#include "CobaltFusion/dbgstream.h"

namespace fusion {
namespace gdi {

BOOL DeviceContextEx::DrawTextOut(const std::wstring& str, int x, int y)
{
	return CDC::TextOut(x, y, str.c_str(), str.size());
}


BOOL DeviceContextEx::DrawPolygon(const std::vector<POINT>& points)
{
	return CDC::Polygon(points.data(), points.size());
}

void DeviceContextEx::DrawTimeline(const std::wstring& name, int x, int y, int width, COLORREF color)
{
	auto pen = CreatePen(PS_SOLID, 1, color);
	SelectPen(pen);
	DrawTextOut(name, x, y -15);
	auto textWidth = 150;
	Rectangle(x + textWidth, y, x + textWidth + width, y + 2);
}

void DeviceContextEx::DrawFlag(const std::wstring& /* tooltip */, int x, int y)
{
	MoveTo(x, y);
	LineTo(x, y - 20);
	LineTo(x + 7, y - 16);
	LineTo(x, y - 12);
}

void DeviceContextEx::DrawSolidFlag(const std::wstring& /* tooltip */, int x, int y)
{
	DrawPolygon({ { x, y - 20 },{ x + 7, y - 16 },{ x, y - 12 } });
	MoveTo(x, y);
	LineTo(x, y - 20);
}

void DeviceContextEx::DrawSolidFlag(const std::wstring& tooltip, int x, int y, COLORREF border, COLORREF fill)
{
	auto pen = CreatePen(PS_SOLID, 1, border);
	SelectPen(pen);
	auto b = ::CreateSolidBrush(fill);
	SelectBrush(b);
	DrawSolidFlag(tooltip, x, y);
}

void DeviceContextEx::DrawFlag(const std::wstring& tooltip, int x, int y, COLORREF color, bool solid)
{
	auto pen = CreatePen(PS_SOLID, 1, color);
	SelectPen(pen);
	if (solid)
		DrawSolidFlag(tooltip, x, y);
	else
		DrawFlag(tooltip, x, y);
}


LONG CTimelineView::GetTrackPos32(int nBar)
{
	SCROLLINFO si = { sizeof(si), SIF_TRACKPOS };
	GetScrollInfo(nBar, &si);
	return si.nTrackPos;
}

BOOL CTimelineView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	cdbg << "OnMouseWheel, zDelta: " << zDelta << "\n";			// todo: find out why these events are not captured like in CMainFrame::OnMouseWheel
	return TRUE;
}

void CTimelineView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
{
	if (nSBCode == SB_THUMBTRACK)
	{
		cdbg << "OnHScroll, nPos: " << nPos << "\n";	// received range is 1-100
		SetScrollPos(SB_HORZ, nPos);
		Invalidate();
	}
}

BOOL CTimelineView::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
	SetScrollRange(SB_HORZ, 1, 500);	// no effect??
	return 1;
}

void CTimelineView::OnPaint(CDCHandle cdc)
{
	using namespace fusion;
	PAINTSTRUCT ps;
	BeginPaint(&ps);
	graphics::DeviceContextEx dc(GetWindowDC());
	int y = 25 + GetTrackPos32(SB_HORZ);
	auto grey = RGB(160, 160, 170);
	dc.DrawTimeline(L"Move Sequence", 15, y, 500, grey);
	dc.DrawFlag(L"tag", 200, y);
	dc.DrawFlag(L"tag", 250, y);
	dc.DrawSolidFlag(L"tag", 260, y, RGB(255, 0, 0), RGB(0, 255, 0));
	dc.DrawFlag(L"tag", 270, y);

	y += 25;
	dc.DrawTimeline(L"Arbitrary data", 15, y, 500, grey);
	dc.DrawFlag(L"blueFlag", 470, y, RGB(0, 0, 255), true);

	EndPaint(&ps);
}

} // namespace graphics
} // namespace fusion
