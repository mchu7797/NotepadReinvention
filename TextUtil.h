#ifndef NOTEPADREINVENTION_TEXT_UTIL_H
#define NOTEPADREINVENTION_TEXT_UTIL_H

#include "framework.h"

int GetPosByXPos() {}

int GetPosByYPos() {}

int GetTextHeight(TCHAR* string, int stringLength) {
  if (stringLength < 0) {
    return -1;
  }
 
  int heightCount = 0;
  int charIndex = 0;

  if (stringLength == 0) {
    while (string[charIndex] != 0) {
      if (string[charIndex] == '\n') {
        heightCount++;
      }

      charIndex++;
    }
  } else {
    while (charIndex < stringLength) {
      if (string[charIndex] == '\n') {
        heightCount++;
      }

      charIndex++;
    }
  }

  return heightCount;
}

int GetTextWidth(TCHAR* string, int stringLength) {
  if (stringLength < 0) {
    return -1;
  }

  int maxWidth = 0;
  int widthCount = 0;
  int charIndex = 0;

  if (stringLength == 0) {
    while (string[charIndex] != 0) {
      if (string[charIndex] == '\n') {
        if (maxWidth < widthCount) {
          maxWidth == widthCount;
        };
        widthCount = 0;
      }

      widthCount++;
      charIndex++;
    }
  } else {
    while (charIndex < stringLength) {
      if (string[charIndex] == '\n') {
        if (maxWidth < widthCount) {
          maxWidth == widthCount;
        };
        widthCount = 0;
      }

      widthCount++;
      charIndex++;
    }
  }

  return maxWidth;
}

#endif NOTEPADREINVENTION_TEXT_UTIL_H