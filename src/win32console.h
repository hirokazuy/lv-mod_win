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

#ifndef __WIN32_CONSOLE_H__
#define __WIN32_CONSOLE_H__

#include <import.h>

extern int WIDTH, HEIGHT;

public void Win32ConsoleInit();
public void Win32ConsoleSetUp();
public void Win32ConsoleSetDown();
public void Win32ConsoleGetWindowSize();
public int Win32ConsolePrint( byte ch );
public void Win32ConsoleClearScreen();
public void Win32ConsoleClearRight();
public void Win32ConsoleSetCur(int x, int y);
public void Win32ConsoleEnterStandoutMode();
public void Win32ConsoleExitStandoutMode();

#endif /* __WIN32_CONSOLE_H__ */
