/**
 * Windows specific console API for lv.
 *
 * @author Hirokazu Yamagishi<hien.ayatuki@gmail.com
 */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef WIN32
#include <assert.h>

#include <windows.h>
#include <wincon.h>

/* for private, public */
#include <import.h>

#include <win32console.h>

private HANDLE g_lv_console_handle;
private HANDLE g_original_console_handle;
private WORD g_fg_color;
private WORD g_bg_color;

private void Win32ConsoleSetColor(WORD fg, WORD bg) {
  assert(g_lv_console_handle != INVALID_HANDLE_VALUE);

  SetConsoleTextAttribute(g_lv_console_handle, (fg << 4 | bg));
}

private WORD getFGColor(WORD wAttr) {
  return (wAttr >> 4) & 0x0f;
}

private WORD getBGColor(WORD wAttr) {
  return wAttr & 0x0f;
}

public void Win32ConsoleInit() {
  CONSOLE_SCREEN_BUFFER_INFO cinfo;

  g_lv_console_handle = INVALID_HANDLE_VALUE;
  g_original_console_handle = GetStdHandle( STD_OUTPUT_HANDLE );
  if (g_original_console_handle == INVALID_HANDLE_VALUE)
    return;

  GetConsoleScreenBufferInfo(g_original_console_handle, &cinfo);
  g_fg_color = getFGColor(cinfo.wAttributes);
  g_bg_color = getBGColor(cinfo.wAttributes);

  Win32ConsoleSetUp();

  Win32ConsoleSetColor( g_fg_color, g_bg_color );
}


public void Win32ConsoleSetUp() {
  COORD size;
  CONSOLE_SCREEN_BUFFER_INFO cinfo;

  GetConsoleScreenBufferInfo(g_original_console_handle, &cinfo);

  if (g_lv_console_handle == INVALID_HANDLE_VALUE) {
    g_lv_console_handle = CreateConsoleScreenBuffer(
      GENERIC_READ | GENERIC_WRITE,
      FILE_SHARE_READ | FILE_SHARE_WRITE,
      NULL,
      CONSOLE_TEXTMODE_BUFFER,
      NULL);
  }
  size.X = cinfo.srWindow.Right - cinfo.srWindow.Left + 1;
  size.Y = cinfo.srWindow.Bottom - cinfo.srWindow.Top + 1;
  SetConsoleScreenBufferSize(g_lv_console_handle, size);
  SetConsoleActiveScreenBuffer(g_lv_console_handle);
}

public void Win32ConsoleSetDown() {
  if (g_original_console_handle == INVALID_HANDLE_VALUE) {
    return;
  }
  if (g_lv_console_handle != INVALID_HANDLE_VALUE) {
    SetConsoleActiveScreenBuffer(g_original_console_handle);
    CloseHandle(g_lv_console_handle);
    g_lv_console_handle = INVALID_HANDLE_VALUE;
  }
}


public void Win32ConsoleGetWindowSize() {
  CONSOLE_SCREEN_BUFFER_INFO cinfo;
  GetConsoleScreenBufferInfo(g_lv_console_handle, &cinfo);
  WIDTH  = cinfo.srWindow.Right - cinfo.srWindow.Left + 1;
  HEIGHT = cinfo.srWindow.Bottom - cinfo.srWindow.Top + 1;
}

public int Win32ConsolePrint(byte ch) {
  DWORD write = 0;
  BOOL ret = WriteConsole(g_lv_console_handle, &ch, 1, &write, NULL);
  return write;
}

public void Win32ConsoleClearScreen() {
  CONSOLE_SCREEN_BUFFER_INFO cinf;
  COORD startPoint;
  DWORD screenSize = 0;
  DWORD dwWriteSize = 0;
  DWORD dwAttribute = (g_fg_color << 4) | g_bg_color;

  GetConsoleScreenBufferInfo(g_lv_console_handle, &cinf);

  startPoint.X = startPoint.Y = 0;
  screenSize = cinf.dwSize.X * cinf.dwSize.Y;

  FillConsoleOutputCharacter(g_lv_console_handle, ' ', screenSize, startPoint, &dwWriteSize);

  SetConsoleTextAttribute(g_lv_console_handle, dwAttribute);
  FillConsoleOutputAttribute(g_lv_console_handle, dwAttribute, screenSize, startPoint, &dwWriteSize);
}


public void Win32ConsoleClearRight() {
  CONSOLE_SCREEN_BUFFER_INFO cinfo;
  DWORD clearSize = 0;
  DWORD dwWriteSize = 0;
  DWORD dwAttribute = (g_fg_color << 4) | g_bg_color;

  GetConsoleScreenBufferInfo(g_lv_console_handle, &cinfo);

  clearSize = cinfo.dwSize.X - cinfo.dwCursorPosition.X;
  FillConsoleOutputCharacter(g_lv_console_handle, ' ', clearSize, cinfo.dwCursorPosition, &dwWriteSize);

  SetConsoleTextAttribute(g_lv_console_handle, dwAttribute);
  FillConsoleOutputAttribute(g_lv_console_handle, dwAttribute, clearSize, cinfo.dwCursorPosition, &dwWriteSize);
}


public void Win32ConsoleSetCur(int x, int y) {
  COORD pos;
  pos.X = x; pos.Y = y;
  SetConsoleCursorPosition(g_lv_console_handle, pos);
}

public void Win32ConsoleEnterStandoutMode() {
  DWORD dwAttribute = 0;

  dwAttribute = (g_bg_color << 4) | g_fg_color;
  SetConsoleTextAttribute(g_lv_console_handle, dwAttribute);
}

public void Win32ConsoleExitStandoutMode() {
  DWORD dwAttribute = 0;

  dwAttribute = (g_fg_color << 4) | g_bg_color;
  SetConsoleTextAttribute(g_lv_console_handle, dwAttribute);
}

#endif /* WIN32 */
