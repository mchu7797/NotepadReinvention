#ifndef NOTEPADREINVENTION_KEYBOARD_HANDLER_H	
#define NOTEPADREINVENTION_KEYBOARD_HANDLER_H

#include "framework.h"

// Keyboard typing
void WhenWrite(HWND);

// Keyboard mod keys
void WhenHitEnter(HWND);
void WhenHitTab(HWND);
void WhenHitInsert(HWND);
void WhenHitDelete(HWND);
void WhenHitHome(HWND);
void WhenHitEnd(HWND);
void WhenHitUp(HWND);
void WhenHitDown(HWND);
void WhenHitLeft(HWND);
void WhenHitRight(HWND);

void WhenScrollX(HWND);
void WhenScrollY(HWND);

#endif NOTEPADREINVENTION_KEYBOARD_HANDLER_H
