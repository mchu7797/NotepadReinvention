#include "TextManager.h"

#include <optional>
#include <string>
#include <vector>

TextManager::TextManager() {
  TextBoard = std::vector<std::wstring>();

  TextBoard.push_back(std::wstring());

  IsOverrideMode = false;
}

size_t TextManager::getMaxHeight() { return TextBoard.size(); }

size_t TextManager::getMaxWidth() {
  size_t MaxWidth = 0;

  for (std::vector<std::wstring>::iterator iter = TextBoard.begin();
       iter != TextBoard.end(); ++iter) {
    if (MaxWidth < iter->length()) {
      MaxWidth = iter->length();
    }
  }

  return MaxWidth;
}

std::optional<std::wstring> TextManager::getText(int Row) {
  if (Row < 0 || Row >= TextBoard.size()) {
    return std::nullopt;
  }

  return TextBoard[Row];
}

void TextManager::handleWrite(wchar_t *Character, int X, int Y) {
  std::wstring StringToType(Character);

  if (Y < 0 || Y > TextBoard.size()) {
    return;
  }

  if (X < 0) {
    return;
  }

  if (X > TextBoard[Y].length()) {
    TextBoard[X].append(Character);
  }

  TextBoard[Y].insert(X, StringToType);
}

void TextManager::handleHitEnter(int X, int Y) {
  if (Y < 0) {
    return;
  }

  if (Y > TextBoard.size() - 1) {
    TextBoard.push_back(std::wstring());
  }

  if (X > TextBoard[Y].length()) {
    TextBoard.insert(TextBoard.begin(), Y, std::wstring());
  }

  std::wstring temp = TextBoard[Y].substr(X, std::string::npos);
  TextBoard[Y].erase(X, std::string::npos);

  if (Y > TextBoard.size() - 1) {
    TextBoard.push_back(temp);
  } else {
    TextBoard.insert(TextBoard.begin() + Y + 1, temp);
  }
}

void TextManager::handleHitBackspace(int X, int Y) {
  if (Y < 0 || Y > TextBoard.size()) {
    return;
  }

  if (X < 0) {
    return;
  }

  if (X == 0) {
    if (Y == 0) {
      return;
    }

    if (TextBoard[Y].size() > 0) {
      TextBoard[Y - 1].append(TextBoard[Y]);
      TextBoard.erase(TextBoard.begin() + Y);
    } else {
      TextBoard.erase(TextBoard.begin() + Y);
    }

    return;
  }

  if (X > TextBoard[Y].length()) {
    TextBoard[Y].pop_back();
    return;
  }

  TextBoard[Y].erase(X - 1, 1);
}

void TextManager::handleHitTab(int X, int Y) {
  if (Y < 0 || Y > TextBoard.size()) {
    return;
  }

  if (X < 0) {
    return;
  }

  if (X > TextBoard[Y].length()) {
    TextBoard[Y].append(L"        ");
    return;
  }

  TextBoard[Y].insert(X, L"        ");
}

void TextManager::handleHitInsert() { IsOverrideMode = !IsOverrideMode; }

void TextManager::handleHitDelete(int X, int Y) {
  if (Y < 0 || Y > TextBoard.size()) {
    return;
  }

  if (X > TextBoard[Y].length()) {
    if (Y + 1 > TextBoard.size()) {
      return;
    }

    TextBoard[Y].append(TextBoard[Y + 1]);
    TextBoard.erase(TextBoard.begin() + Y + 1);

    return;
  }

  TextBoard[Y].erase(X, 1);
}