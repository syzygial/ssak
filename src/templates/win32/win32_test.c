#include <windows.h>

#define CLASS_NAME "Hello world"

LRESULT CALLBACK handle_event(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  if (message == WM_DESTROY) PostQuitMessage(0);
  return DefWindowProc(hWnd, message, wParam, lParam);
}

ATOM register_class(HINSTANCE hInst) {
  WNDCLASSEX wc = {
    .cbSize = sizeof(WNDCLASSEX),
    .style = CS_HREDRAW | CS_VREDRAW,
    .lpfnWndProc = handle_event,
    .lpszClassName = CLASS_NAME,
    .hInstance = hInst,
    .hCursor = LoadCursor(NULL, IDC_ARROW),
    .hbrBackground = GetStockObject(WHITE_BRUSH)
  };
  return RegisterClassEx(&wc);
}

BOOL init_window(HINSTANCE hInst, int nCmdShow) {
  HWND hWnd = CreateWindow(
    CLASS_NAME,
    CLASS_NAME,
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    NULL,
    NULL,
    hInst,
    NULL);
  if (!hWnd) return FALSE;
  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);
  return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {
  if (hPrevInstance != NULL) return 0;
  MSG msg;
  register_class(hInstance);
  if(!init_window(hInstance, nCmdShow)) return 1;
  
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
  }