#ifndef NOTEPAD_REINVENTION_TEXT_MANAGER_H
#define NOTEPAD_REINVENTION_TEXT_MANAGER_H

#include "framework.h"

class TextManager {
public:
  TextManager();

  std::optional<std::wstring> getText(int Row);
  size_t getMaxWidth();
  size_t getMaxHeight();

  std::vector<std::wstring>::iterator begin() { return TextBoard.begin(); }
  std::vector<std::wstring>::iterator end() { return TextBoard.end(); }
  size_t size() { return TextBoard.size(); }

  void appendString(std::wstring string) { TextBoard.push_back(string); }
  void clear() { TextBoard.clear(); }

  void handleWrite(wchar_t *Character, int X, int Y);
  void handleHitEnter(int X, int Y);
  void handleHitBackspace(int X, int Y);
  void handleHitTab(int X, int Y);
  void handleHitInsert();
  void handleHitDelete(int X, int Y);

private:
  bool IsOverrideMode;
  std::vector<std::wstring> TextBoard;
};

#endif // NOTEPAD_REINVENTION_TEXT_MANAGER_H