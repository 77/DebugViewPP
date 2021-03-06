// (C) Copyright Gert-Jan de Vos and Jan Wilmans 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Repository at: https://github.com/djeedjay/DebugViewPP/

#include "stdafx.h"
#include "Win32/gdi.h"
#include <string>

namespace fusion {
namespace graphics {

Window::Window(HINSTANCE hInstance, WNDPROC messageHandler, const std::wstring& uniqueClassName, const std::wstring& title, int width, int height)
{
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = messageHandler;
	wc.lpszClassName = uniqueClassName.c_str();
	wc.hInstance = hInstance;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hbrBackground = (HBRUSH)(BLACK_BRUSH);
	wc.lpszMenuName = NULL;

	RegisterClassEx(&wc);

	hwnd = CreateWindow(uniqueClassName.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);
}

void Window::Show(int nCmdShow)
{
	ShowWindow(hwnd, nCmdShow);
}

int MessageLoop::run()
{
	while (1)
	{
		if (GetMessage(&msg, NULL, 0, 0) != 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			return 0;
		}
	}
}

DeviceContext::DeviceContext(HDC hDC) : 
	hDC(hDC)
{
}

void DeviceContext::MoveTo(int x, int y)
{
	::MoveToEx(hDC, x, y, NULL);
}

void DeviceContext::LineTo(int x, int y)
{
	::LineTo(hDC, x, y);
}

void DeviceContext::DrawTextOut(const std::wstring& str, int x, int y)
{
	::TextOut(hDC, x, y, str.c_str(), str.size());
}

void DeviceContext::Rectangle(int x, int y, int width, int height)
{
	::Rectangle(hDC, x, y, width, height);
}

void DeviceContext::DrawPolygon(const std::vector<POINT>& points)
{
	::Polygon(hDC, points.data(), points.size());
}

void DeviceContextEx::DrawTimeline(const std::wstring& name, int x, int y, int width, COLORREF color)
{
	auto pen = CreatePen(PS_SOLID, 1, color);
	::SelectObject(hDC, pen);
	DrawTextOut(name, x, y -15);
	auto textWidth = 150;
	::Rectangle(hDC, x + textWidth, y, x + textWidth + width, y + 2);
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
	::SelectObject(hDC, pen);
	auto b = ::CreateSolidBrush(fill);
	::SelectObject(hDC, b);
	DrawSolidFlag(tooltip, x, y);
}

void DeviceContextEx::DrawFlag(const std::wstring& tooltip, int x, int y, COLORREF color, bool solid)
{
	auto pen = CreatePen(PS_SOLID, 1, color);
	::SelectObject(hDC, pen);
	if (solid)
		DrawSolidFlag(tooltip, x, y);
	else
		DrawFlag(tooltip, x, y);
}

} // namespace graphics
} // namespace fusion
