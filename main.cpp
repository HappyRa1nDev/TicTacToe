//
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#define KEY_SHIFTED     0x8000
#define KEY_TOGGLED     0x0001

const TCHAR szWinClass[] = _T("TicTacToe");
const TCHAR szWinName[]  = _T("TicTac");

WNDCLASS wincl;
HDC hdc; // контекст рисования
HWND hwnd;               /* This is the handle for our window */
HBRUSH hBrush;           /* Current brush */
HPEN hPenGrid;          // цвет решетки
HPEN hCross;//цвет крестика
HBRUSH hbEl;//цвет круга
HPEN hEl;//цвет рамки

int n = 3;//размерность сетки
int screenW = 320;
int screenH = 240;
enum eCellType
{
    eNone=0,
    eEllipse,
    eCross
};

//флаги зажатия клавиш
bool shift = false;
bool c = false;
bool ctrl = false;
bool q = false;
//

std::vector<eCellType> grid_mas;
int step_x;
int step_y;
RECT rect;// рект окна
int heightW;//высота рабочего окна
int widthW;//ширина рабочего окна
int size_line = 3;

int Napravlenie = 1;
int R = 239;
int G = 48;
int B = 56;
bool Rbool = true;
bool Gbool = false;
bool Bbool = true;

/* Runs Notepad */
void RunNotepad(void)
{
    STARTUPINFO sInfo;
    PROCESS_INFORMATION pInfo;

    ZeroMemory(&sInfo, sizeof(STARTUPINFO));

    puts("Starting Notepad...");
    CreateProcess(_T("C:\\Windows\\Notepad.exe"),
        NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo);
}

void FindIndex(int x, int y, int& indexI, int& indexJ) {
    indexI = x / step_x;
    indexJ = y / step_y;
}
void DrawLine(LONG x1, LONG y1, LONG x2, LONG y2) {
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
}
void CreateGrid() {
    SelectObject(hdc, hPenGrid);
    int start_x = rect.left;
    for (int i = 0; i < n; i++) {
        DrawLine( start_x, rect.top, start_x, rect.bottom);
        start_x += step_x;
    }
    DrawLine(rect.right, rect.top, rect.right, rect.bottom);

    int start_y = rect.top;
    for (int i = 0; i < n; i++) {
        DrawLine( rect.left, start_y, rect.right, start_y);
        start_y += step_y;
    }
    DrawLine(rect.left, rect.bottom, rect.right, rect.bottom);
    int kek = 0;
}
void DrawEllipse(int x, int y) {
    int i = rect.left + step_x * x;
    int j = rect.top + step_y * y;

    int s_x = step_x / 10;
    int s_y = step_y / 10;

    SelectObject(hdc, hbEl);
    SelectObject(hdc, hEl);

    Ellipse(hdc, i + s_x, j + s_x, i + step_x-s_x, j + step_y-s_y);
}
void DrawCross(int x, int y) {
    int i = rect.left+step_x * x;
    int j = rect.top+step_y * y;
    int s_x = step_x / 10;
    int s_y = step_y / 10;

    SelectObject(hdc, hCross);
    DrawLine(i + s_x, j + s_y, i + step_x - s_x, j + step_y - s_y);
    DrawLine(i+step_x - s_x, j + s_y, i + s_x, j + step_y - s_y);
}
void DrawMas() {
    for (int y = 0; y < n; y++) {
        for (int x= 0; x < n; x++) {
            if (grid_mas[y * n + x] == eEllipse) {
                DrawEllipse(x, y);
            }
            if (grid_mas[y * n + x] == eCross) {
                DrawCross(x, y);
            }
        }
    }
}
/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
    case WM_DESTROY:
        PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
        return 0;
    case WM_SIZE: {
        if (hwnd) {
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        POINT pt;
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        int Iindex, Jindex;

        FindIndex(pt.x, pt.y, Iindex, Jindex);
        if (grid_mas[Jindex*n+Iindex] != eEllipse) {
            grid_mas[Jindex * n + Iindex] = eEllipse;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;
    }
    case WM_RBUTTONDOWN:
    {
        POINT pt;
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        int Iindex, Jindex;

        FindIndex(pt.x, pt.y, Iindex, Jindex);
        if (grid_mas[Jindex * n + Iindex] != eCross) {
            grid_mas[Jindex * n + Iindex] = eCross;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;
    }
    /*case WM_ERASEBKGND:
    {
        RECT rc; GetClientRect(hwnd, &rc);
        hBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        FillRect((HDC)(wParam), &rc, hBrush);
        return 0;
    }*/
    case WM_KEYDOWN:
    {
        switch (wParam) {
        case VK_RETURN: {
            DeleteObject(hBrush);
            hBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
            SetClassLongPtrA(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBrush);
           // WM_ERASEBKGND
            break;
        }
        case VK_ESCAPE:
            TerminateProcess(GetCurrentProcess(), 0);
            break;
        case 0x43:
            if (shift) {
                SHORT s;
                RunNotepad();
            }
            c = true;
            break;
        case VK_SHIFT:
            if (c) {
                RunNotepad();
            }
            shift = true;
            break;
        case VK_CONTROL:
            if (q) {
                TerminateProcess(GetCurrentProcess(), 0);
            }
            ctrl = true;
            break;
        case 0x51:
            if (ctrl) {
                TerminateProcess(GetCurrentProcess(), 0);
            }
            q = true;
            break;
        }
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }

    case WM_KEYUP:
    {
        switch (wParam) {
        case 0x43:
            c = false;
            break;
        case VK_SHIFT:
            shift = false;
            break;
        case 0x51:
            q = false;
            break;
        case VK_CONTROL:
            ctrl = false;
            break;
        }
        return 0;
    }

    case WM_MOUSEWHEEL:
    {
        int tmp = 3;
        if (GET_WHEEL_DELTA_WPARAM(wParam) * Napravlenie <= 0) {
            Napravlenie = Napravlenie * -1;
            Rbool = !Rbool;
            Gbool = !Gbool;
            Bbool = !Bbool;
        }
        if (Rbool) {
            if (R + tmp <= 255)
            {
                R += tmp;
            }
            else {
                Rbool = !Rbool;
            }
        }
        else {
            if (R - tmp >= 0)
            {
                R -= tmp;
            }
            else {
                Rbool = !Rbool;
            }
        }
        //
        if (Gbool) {
            if (G + tmp <= 255)
            {
                G += tmp;
            }
            else {
                Gbool = !Gbool;
            }
        }
        else {
            if (G - tmp >= 0)
            {
                G -= tmp;
            }
            else {
                Gbool = !Gbool;
            }
        }
        //
        if (Bbool) {
            if (B + tmp <= 255)
            {
                B += tmp;
            }
            else {
                Bbool = !Bbool;
            }
        }
        else {
            if (B - tmp >= 0)
            {
                B -= tmp;
            }
            else {
                Bbool = !Bbool;
            }
        }
        DeleteObject(hPenGrid);
        hPenGrid = CreatePen(PS_SOLID, size_line, RGB(R, G, B));
        InvalidateRect(hwnd, NULL, TRUE);
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hwnd, &ps);
       // FillRect(hdc, &ps.rcPaint, hBrush);
        GetClientRect(hwnd, &rect);
        
        widthW = rect.right - rect.left;
        heightW = rect.bottom - rect.top;


        step_x = widthW / n;
        step_y = heightW / n;

        CreateGrid();
        DrawMas();
        EndPaint(hwnd, &ps);
        break;
    }
    }
    /* for messages that we don't deal with */
    return DefWindowProc(hwnd, message, wParam, lParam);
}

int main(int argc, char* argv[])
{
    std::ifstream inp("config.txt");

    COLORREF cBrush = 0x00E16941;
    COLORREF cPenGrid = 0x000000FF;
    int index_arg = 1;
    inp >> n;
    if (index_arg < argc)
        n =std::stoi(argv[index_arg++]);
    inp >> screenW;
    if (index_arg < argc)
        screenW= std::stoi(argv[index_arg++]);
    inp >> screenH;
    if (index_arg < argc)
        screenH = std::stoi(argv[index_arg++]);
    inp >> std::hex >> cBrush;
    if (index_arg < argc)
        cBrush = (COLORREF)std::stoi(argv[index_arg++],0,16);
    inp >> std::hex >> cPenGrid;
    if (index_arg < argc)
        cPenGrid = (COLORREF)std::stoi(argv[index_arg++],0,16);

    
    inp.close();
    grid_mas = std::vector<eCellType>(n * n, eNone);
    hBrush = CreateSolidBrush(cBrush);
    hPenGrid = CreatePen(PS_SOLID, size_line, cPenGrid);

    BOOL bMessageOk;
    MSG message;            /* Here message to the application are saved */
    wincl = { 0 };         /* Data structure for the windowclass */

    /* Harcode show command num when use non-winapi entrypoint */
    int nCmdShow = SW_SHOW;
    /* Get handle */
    HINSTANCE hThisInstance = GetModuleHandle(NULL);

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szWinClass;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by Windows */

    /* Use custom brush to paint the background of the window */
   
    hCross = CreatePen(PS_SOLID, size_line, RGB(0, 255, 0));
    hbEl = CreateSolidBrush(RGB(250, 215, 0));
    hEl = CreatePen(PS_SOLID, size_line, RGB(250, 215, 0));//цвет круга
    wincl.hbrBackground = hBrush;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClass(&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindow(
        szWinClass,          /* Classname */
        szWinName,       /* Title Text */
        WS_OVERLAPPEDWINDOW, /* default window */
        CW_USEDEFAULT,       /* Windows decides the position */
        CW_USEDEFAULT,       /* where the window ends up on the screen */
        screenW,                 /* The programs width */
        screenH,                 /* and height in pixels */
        HWND_DESKTOP,        /* The window is a child-window to desktop */
        NULL,                /* No menu */
        hThisInstance,       /* Program Instance handler */
        NULL                 /* No Window Creation data */
    );

    /* Make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while ((bMessageOk = GetMessage(&message, NULL, 0, 0)) != 0)
    {
        /* Yep, fuck logic: BOOL mb not only 1 or 0.
         * See msdn at https://msdn.microsoft.com/en-us/library/windows/desktop/ms644936(v=vs.85).aspx
         */
        if (bMessageOk == -1)
        {
            puts("Suddenly, GetMessage failed! You can call GetLastError() to see what happend");
            break;
        }
        /* Translate virtual-key message into character message */
        TranslateMessage(&message);
        /* Send message to WindowProcedure */
        DispatchMessage(&message);
    }
    std::ofstream out("config.txt");
    out << n << std::endl;
    out << screenW << std::endl;
    out << screenH << std::endl;
    //SelectObject(hdc, hBrush);
    LOGBRUSH lbr;
    GetObject(hBrush, sizeof(lbr), &lbr);
    COLORREF tmp_brush = lbr.lbColor;
    //COLORREF tmp_brush= GetDCBrushColor(hWnd);
    auto lamb = [](COLORREF tmp_brush) {
        std::stringstream stream;

        stream << "0x"
            << std::setfill('0') << std::setw(sizeof(tmp_brush) * 2)
            << std::hex << tmp_brush;
        /*int r = (int)GetRValue(tmp_brush);
        int g = (int)GetGValue(tmp_brush);
        int b = (int)GetBValue(tmp_brush);
        stream << "0x00";
        stream <<std::setfill('0')<< std::setw(2) << std::hex<< b;
        stream << std::setfill('0') <<std::setw(2) << std::hex << g;
        stream << std::setfill('0')<< std::setw(2) << std::hex << r;*/
        //stream<<std::hex<< (int)GetGValue(tmp_brush) << (int)GetRValue(tmp_brush);
        return stream.str();
    };
    out << lamb(tmp_brush) << std::endl;
    LOGPEN lbr2;
    GetObject(hPenGrid, sizeof(lbr2), &lbr2);
    COLORREF tmp_pen = lbr2.lopnColor;
    out << lamb(tmp_pen) << std::endl;
    /* Cleanup stuff */
    DestroyWindow(hwnd);
    UnregisterClass(szWinClass, hThisInstance);
    DeleteObject(hBrush);
    DeleteObject(hPenGrid);
    DeleteObject(hCross);
    DeleteObject(hEl);
    //RunNotepad();

    return 0;
}

