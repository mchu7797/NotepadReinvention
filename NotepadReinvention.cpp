// NotepadReinvention.cpp : Defines the entry point for the application.
//

#include "NotepadReinvention.h"

#include "framework.h"

#include "TextManager.h"

#define MAX_LOADSTRING 100
#define FONT_SIZE 12

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
HFONT defaultFont;

HWND mainWindow;

TextManager TextBoard;

long TextPosX = 0;
long TextPosY = 0;

long TextHeight = 0;
long TextWidth = 0;

int CaretPosXByChar = 0;
int CaretPosYByChar = 0;

// Windows API 함수들
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

void GetFontSize(HWND hWnd, WCHAR character, int *height, int *width);
BOOL TryOpen();
BOOL TrySave();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_NOTEPADREINVENTION, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  if (!InitInstance(hInstance, nCmdShow)) {
    return FALSE;
  }

  HACCEL hAccelTable =
      LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NOTEPADREINVENTION));

  MSG msg;

  while (GetMessage(&msg, nullptr, 0, 0)) {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NOTEPADREINVENTION));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_NOTEPADREINVENTION);
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  hInst = hInstance;

  mainWindow = CreateWindowW(
      szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

  if (!mainWindow) {
    return FALSE;
  }

  ShowWindow(mainWindow, nCmdShow);
  UpdateWindow(mainWindow);

  return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  switch (message) {
  case WM_CREATE:
    defaultFont = CreateFont(FONT_SIZE, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0,
                             0, 0, 0, _T("굴림체"));
    SendMessage(hWnd, WM_SETFONT, (WPARAM)defaultFont, TRUE);
    TextBoard = TextManager();
    break;
  case WM_COMMAND: {
    int wmId = LOWORD(wParam);
    // Parse the menu selections:
    switch (wmId) {
    case IDM_ABOUT:
      DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
      break;
    case IDM_EXIT:
      DestroyWindow(hWnd);
      break;
    case IDM_FILE_OPEN:
      TextBoard.clear();
      
      if (!TryOpen()) {
        MessageBox(mainWindow, L"파일을 열 수 없습니다!", L"오류", MB_OK);
      }

      InvalidateRect(hWnd, NULL, true);
      break;
    case IDM_FILE_SAVE:
      if (!TrySave()) {
        MessageBox(mainWindow, L"파일을 저장할 수 없습니다!", L"오류", MB_OK);
      }
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
  } break;
  case WM_KEYDOWN:
    switch (wParam) {
    case VK_UP:
      if (CaretPosYByChar > 0) {
        --CaretPosYByChar;
      }

      if (TextBoard.getText(CaretPosYByChar).has_value()) {
        if (CaretPosXByChar >
            TextBoard.getText(CaretPosYByChar).value().length() - 1) {
          CaretPosXByChar = TextBoard.getText(CaretPosYByChar).value().length();
        }
      }

      InvalidateRect(hWnd, nullptr, true);
      break;
    case VK_DOWN:
      if (CaretPosYByChar < TextBoard.size() - 1) {
        ++CaretPosYByChar;
      }

      if (TextBoard.getText(CaretPosYByChar).has_value()) {
        if (CaretPosXByChar >
            TextBoard.getText(CaretPosYByChar).value().length() - 1) {
          CaretPosXByChar = TextBoard.getText(CaretPosYByChar).value().length();
        }
      }

      InvalidateRect(hWnd, nullptr, true);
      break;
    case VK_LEFT:
      if (CaretPosXByChar > 0) {
        --CaretPosXByChar;
      }

      InvalidateRect(hWnd, nullptr, true);
      break;
    case VK_RIGHT:
      if (TextBoard.getText(CaretPosYByChar).has_value()) {
        if (CaretPosXByChar <
            TextBoard.getText(CaretPosYByChar).value().length()) {
          ++CaretPosXByChar;
        }
      }

      InvalidateRect(hWnd, nullptr, true);
      break;
    case VK_BACK:
      TextBoard.handleHitBackspace(CaretPosXByChar, CaretPosYByChar);

      if (CaretPosXByChar > 0) {
        --CaretPosXByChar;
      } else if (CaretPosYByChar > 0) {
        --CaretPosYByChar;

        if (TextBoard.getText(CaretPosYByChar).has_value()) {
          CaretPosXByChar = TextBoard.getText(CaretPosYByChar).value().length();
        } else {
          CaretPosXByChar = 0;
        }
      } else {
      }

      InvalidateRect(hWnd, NULL, true);

      break;
    case VK_TAB:
      TextBoard.handleHitTab(CaretPosXByChar, CaretPosYByChar);

      CaretPosXByChar += 8;

      InvalidateRect(hWnd, NULL, true);

      break;
    case VK_RETURN:
      TextBoard.handleHitEnter(CaretPosXByChar, CaretPosYByChar);

      CaretPosXByChar = 0;
      ++CaretPosYByChar;

      InvalidateRect(hWnd, NULL, true);
      break;
    case VK_HOME:
      CaretPosXByChar = 0;
      InvalidateRect(hWnd, NULL, true);
      break;
    case VK_END:
      if (TextBoard.getText(CaretPosYByChar).has_value()) {
        CaretPosXByChar = TextBoard.getText(CaretPosYByChar).value().length();
      }
      InvalidateRect(hWnd, NULL, true);
      break;
    case VK_DELETE:
      TextBoard.handleHitDelete(CaretPosXByChar, CaretPosYByChar);

      InvalidateRect(hWnd, NULL, true);
      break;
    default:
      BYTE KeyState[256] = {0};
      WCHAR CharacterToType[2];
      if (ToUnicode(wParam, 0, KeyState, CharacterToType, 2, 0)) {
        TextBoard.handleWrite(CharacterToType, CaretPosXByChar,
                              CaretPosYByChar);
        ++CaretPosXByChar;
        InvalidateRect(hWnd, NULL, true);
      }
    }
    break;
  case WM_PAINT: {
    PAINTSTRUCT ps;
    TEXTMETRICW TextMetric;
    HDC hdc = BeginPaint(hWnd, &ps);

    GetTextMetrics(hdc, &TextMetric);

    // 글자 그리기
    for (int i = 0; i < TextBoard.size(); ++i) {
      std::optional<std::wstring> str = TextBoard.getText(i);

      if (!str.has_value()) {
        continue;
      }

      TextOutW(hdc, TextPosX, TextPosY + (i * TextMetric.tmHeight),
               str.value().c_str(), str.value().length());
    }

    // 커서 배치하기
    if (!TextBoard.getText(CaretPosYByChar).has_value()) {
      SetCaretPos(0, TextMetric.tmHeight * CaretPosYByChar);
      ShowCaret(hWnd);
    } else {
      std::wstring Text = TextBoard.getText(CaretPosYByChar).value();
      int CaretPosXByPixel = 0;
      int CharWidth, CharHeight;

      for (int i = 0; i < CaretPosXByChar; ++i) {
        if (CaretPosXByChar > Text.length()) {
          break;
        }

        GetFontSize(hWnd, Text[i], &CharHeight, &CharWidth);
        CaretPosXByPixel += CharWidth;
      }

      SetCaretPos(CaretPosXByPixel, TextMetric.tmHeight * CaretPosYByChar);
      ShowCaret(hWnd);
    }

    EndPaint(hWnd, &ps);
  } break;
  case WM_SETFOCUS:
    CreateCaret(hWnd, (HBITMAP)NULL, 2, 15);
    ShowCaret(hWnd);
    break;
  case WM_KILLFOCUS:
    DestroyCaret();
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  UNREFERENCED_PARAMETER(lParam);
  switch (message) {
  case WM_INITDIALOG:
    return (INT_PTR)TRUE;

  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
      EndDialog(hDlg, LOWORD(wParam));
      return (INT_PTR)TRUE;
    }
    break;
  }
  return (INT_PTR)FALSE;
}

void GetFontSize(HWND hWnd, WCHAR character, int *height, int *width) {
  HDC Hdc = GetDC(hWnd);

  TEXTMETRICW TextMetric;
  ABC Abc;

  GetCharABCWidths(Hdc, (UINT)character, (UINT)character, &Abc);
  GetTextMetrics(Hdc, &TextMetric);

  *height = TextMetric.tmHeight;
  *width = Abc.abcA + Abc.abcB + Abc.abcC;
}

BOOL TryOpen() {
  OPENFILENAME openFileName;
  TCHAR lpstrFile[256] = L"";

  memset(&openFileName, 0, sizeof(OPENFILENAME));
  openFileName.lStructSize = sizeof(OPENFILENAME);
  openFileName.hwndOwner = mainWindow;
  openFileName.lpstrFile = lpstrFile;
  openFileName.nMaxFile = 256;
  openFileName.lpstrInitialDir = L".";
  openFileName.lpstrDefExt = L"txt";
  openFileName.lpstrFilter = L"텍스트 파일\0*.txt\0모든 파일\0*.*";

  if (GetOpenFileName(&openFileName) == 0) {
    return false;
  }

  std::wifstream ReadStream(lpstrFile);

  if (!ReadStream.is_open()) {
    ReadStream.close();
    return false;
  }

  std::wstring Line;

  while (getline(ReadStream, Line)) {
    TextBoard.appendString(Line);
  }

  return true;
}

BOOL TrySave() {
  OPENFILENAME saveFileName;
  TCHAR lpstrFile[256] = L"";

  memset(&saveFileName, 0, sizeof(OPENFILENAME));
  saveFileName.lStructSize = sizeof(OPENFILENAME);
  saveFileName.hwndOwner = mainWindow;
  saveFileName.lpstrFile = lpstrFile;
  saveFileName.nMaxFile = 256;
  saveFileName.lpstrInitialDir = L".";
  saveFileName.lpstrDefExt = L"txt";
  saveFileName.lpstrFilter = L"텍스트 파일\0*.txt\0모든 파일\0*.*";

  if (GetSaveFileName(&saveFileName) == 0) {
    return false;
  }

  std::wofstream WriteStream(lpstrFile);

  if (!WriteStream.is_open()) {
    WriteStream.close();
    return false;
  }

  for (auto it = TextBoard.begin(); it < TextBoard.end(); ++it) {
    WriteStream << *it;
  }

  WriteStream.close();

  return true;
}