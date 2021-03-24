// SPro_work.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "SPro_work.h"
#include "Checkers.h"


#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPROWORK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPROWORK));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPROWORK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(243, 221, 205));
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SPROWORK);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_MINIMIZEBOX | WS_SYSMENU,
      CW_USEDEFAULT, CW_USEDEFAULT, 782, 826, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void DrawBoard(HDC hdc, int dx, int dy, int size = 70)
{

    
  //  HBRUSH BoardBrush = CreateSolidBrush(RGB(24, 10, 0));
    HBRUSH DarkBrush = CreateSolidBrush(RGB(128, 64, 0));
    HBRUSH LightBrush = CreateSolidBrush(RGB(224, 192, 128));
    HBRUSH GreenBrush = CreateSolidBrush(RGB(33, 137, 51));
    HBRUSH RedBrush = CreateSolidBrush(RGB(166, 11, 11));
    RECT BackBoard = {0, 0, size * 11 + dx / 2, size * 11 + dy / 2};
    RECT field;

    //FillRect(hdc, &BackBoard, BoardBrush);
    Rectangle(hdc, dx, dy, 10 * size + dx, 10 * size + dy);
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            field = { i * size + dx , j * size + dy, (i + 1) * size + dx, (j + 1) * size + dy };
            if ((i + j) & 0b1)
                FillRect(hdc, &field, DarkBrush);
            else
                FillRect(hdc, &field, LightBrush);
            
            if (board[i][j].state == CHOOSEN)
            {
                FillRect(hdc, &field, GreenBrush);
            }

            if (board[i][j].state == ATTACKABLE)
            {
                FillRect(hdc, &field, RedBrush);
            }
        }
    }
    
    DeleteObject(DarkBrush);
    DeleteObject(LightBrush);
    DeleteObject(GreenBrush);
    DeleteObject(RedBrush);
 }



void DrawChecker(HDC hdc, int x, int y)
{
    HBRUSH Brush1, Brush2;

    if (board[x][y].checker->color)
    {
         Brush1 = CreateSolidBrush(RGB(230, 230, 230));
         Brush2 = CreateSolidBrush(RGB(20, 20, 20));
    }
    else 
    {
         Brush1 = CreateSolidBrush(RGB(20, 20, 20));
         Brush2 = CreateSolidBrush(RGB(230, 230, 230));
    }

    SelectObject(hdc, Brush1);
    Ellipse(hdc, 36 + x * 70, 36 + y * 70, 105 + x * 70, 105 + y * 70);
    SelectObject(hdc, Brush2);
    Ellipse(hdc, 40 + x * 70, 40 + y * 70, 101 + x * 70, 101 + y * 70);
    SelectObject(hdc, Brush1);
    Ellipse(hdc, 42 + x * 70, 42 + y * 70, 99 + x * 70, 99 + y * 70);
    SelectObject(hdc, Brush2);
    Ellipse(hdc, 52 + x * 70, 52 + y * 70, 89 + x * 70, 89 + y * 70);
    SelectObject(hdc, Brush1);
    Ellipse(hdc, 54 + x * 70, 54 + y * 70, 87 + x * 70, 87 + y * 70);

    if (typeid(*board[x][y].checker) == typeid (King))
    {
        SelectObject(hdc, Brush2);
        Ellipse(hdc, 54 + x * 70, 54 + y * 70, 87 + x * 70, 87 + y * 70);
    }
   
    DeleteObject(Brush1);
    DeleteObject(Brush2);
}







//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP hBoardBitmap;
    static POINT userSelect;
    static Game controller;
    static bool isChoosen;
    static POINT lastSelect;

    static int dx = 35, dy = 35;
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
       // DrawBitmap(hdc, hBoardBitmap, 0, 0);
        DrawBoard(hdc, dx, dy);
        SetBkMode(hdc, TRANSPARENT);
        TextOut(hdc, 0, 5, L"Ходять:", 8);
        HBRUSH playerBrush = controller.playerColor ? CreateSolidBrush(RGB(230, 230, 230))
                                                   : CreateSolidBrush(RGB(10, 10, 10));
        SelectObject(hdc, playerBrush);
        Ellipse(hdc, 55, 4, 75, 24);
        DeleteObject(playerBrush);
       /* char txt[3];
        _itoa_s(controller.amount[BLACK], txt, 10);
        TextOutA(hdc, 0, 0, txt, 3);
        _itoa_s(controller.amount[WHITE], txt, 10);
        TextOutA(hdc, 0, 20, txt, 3);*/
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                if (!board[i][j].isFree())
                {
                    DrawChecker(hdc, i, j);  
                }
            }
        }
        if (controller.IsGameOver())
        { 
            if (MessageBox(hWnd, L"Гра завершена. Бажаєте почати наново?", L"Чудова партія", MB_YESNO) == IDYES)
            {
                //MessageBox(hWnd, L"Privat: 4149 4991 3092 2337 \nMonobank: 5375 4141 2115 1350", L"За ідею автору", MB_OK);
                
                controller.Restart();
            }
            else
            {
                ShellExecute(NULL, L"open", L"https://youtu.be/VJCtRXrXkQU", NULL, NULL, SW_SHOWNORMAL);
            }
        }
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_LBUTTONDOWN:
    {
        GetCursorPos(&userSelect);
        ScreenToClient(hWnd, &userSelect);
        userSelect.x = (userSelect.x - 35) / 70;
        userSelect.y = (userSelect.y - 35) / 70;
        
        if(isChoosen && userSelect.x < 10 && userSelect.y < 10)
        {
              controller.Move(lastSelect.x, lastSelect.y, userSelect.x, userSelect.y);
           
              controller.Deselect();
              isChoosen = !isChoosen;
        }
    
            if (userSelect.x < 10 && userSelect.y < 10)
            {
                if (board[userSelect.x][userSelect.y].checker)
                    if (board[userSelect.x][userSelect.y].checker->color == controller.playerColor)
                    {
                        controller.Select(userSelect.x, userSelect.y);
                                          lastSelect = userSelect;
                                          isChoosen = !isChoosen;
                    }
                    else
                    {
                        controller.Deselect();

                    }
                  
            }



        InvalidateRect(hWnd, NULL, TRUE);
        UpdateWindow(hWnd);
    }
    break;
    case WM_CREATE:
    {
     
    }
    break;

    case WM_DESTROY:
    {
      
        PostQuitMessage(0);
    }
    break;
    default:
    {
        return DefWindowProc(hWnd, message, wParam, lParam);

    }
    }
    return 0;
}
// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
