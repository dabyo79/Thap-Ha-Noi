#include <iostream>
#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits>
#include <ios>
#include <vector>
struct Move { int from, to; };

using namespace std;
const int BANNER_X = 1;
const int BANNER_Y = 1;
const int INPUT_Y  = BANNER_Y + 12; 
const int THAP_Y   = INPUT_Y + 4; 
static int g_screenW = 160;
static int g_topY = THAP_Y;  
int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return (info.srWindow.Right - info.srWindow.Left + 1);
}
void genMoves(int n, int src, int dst, int aux, std::vector<Move>& mv) {
    if (n <= 0) return;
    genMoves(n-1, src, aux, dst, mv);
    mv.push_back({src, dst});
    genMoves(n-1, aux, dst, src, mv);
}

int popTop(int **thap, int chieuCao, int col) {
    for (int i = 0; i < chieuCao; ++i) {
        if (thap[col][i] != 0) {
            int d = thap[col][i];
            thap[col][i] = 0;
            return d;
        }
    }
    return 0; 
}
void pushTop(int **thap, int chieuCao, int col, int disk) {
    for (int i = chieuCao - 1; i >= 0; --i) {
        if (thap[col][i] == 0) { thap[col][i] = disk; return; }
    }
}
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int getConsoleHeight() {
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return (info.srWindow.Bottom - info.srWindow.Top + 1);
}
int computeTopY(int chieuCao) {
    int winH  = getConsoleHeight();
    int marginBottom = 3;                
    int maxTop = max(BANNER_Y + 6, 1);  
    int idealTop = THAP_Y;

    if (idealTop + chieuCao > winH - marginBottom) {
        idealTop = max(maxTop, (winH - marginBottom) - chieuCao);
    }
    return idealTop;
}
void clearRect(int x, int y, int w, int h) {
    g_screenW = getConsoleWidth();

    int safeW = max(0, min(w, g_screenW - x - 1));

    for (int r = 0; r < h; ++r) {
        gotoxy(x, y + r);
        if (safeW > 0) cout << string(safeW, ' ');
        gotoxy(x, y + r);
    }
}
void clearForTower(int chieuCao) {
    int winH = getConsoleHeight();
    int h = min(winH - g_topY, chieuCao + 5);
    if (h < 0) h = 0;
    clearRect(0, g_topY, g_screenW, h);
}


void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

static bool g_abort = false;

inline bool waitStep() {
    int k = _getch();
    if (k == 27) {
        g_abort = true;
        return false;
    }
    return true;
}

// =================== TIÊU ĐỀ ===================
void hienthide() {
    system("cls");
    setConsoleColor(14);
    gotoxy(3, 1);  printf("\t	                         ▓▓▓▓▓▓▓ ▓▓▓   ▓▓▓   ▓▓▓▓▓▓   ▓▓▓▓▓▓▓                                 \n"); Sleep(40);
    gotoxy(3, 2);  printf("\t	                           ▓▓▓   ▓▓▓   ▓▓▓  ▓▓▓  ▓▓▓  ▓▓▓  ▓▓▓                                \n"); Sleep(40); setConsoleColor(6);
    gotoxy(3, 3);  printf("\t	                           ▓▓▓   ▓▓▓▓▓▓▓▓▓  ▓▓▓▓▓▓▓▓  ▓▓▓▓▓▓▓                                 \n"); Sleep(40);
    gotoxy(3, 4);  printf("\t	                           ▓▓▓   ▓▓▓   ▓▓▓  ▓▓▓  ▓▓▓  ▓▓▓                                     \n"); Sleep(40);
    gotoxy(3, 5);  printf("\t	                           ▓▓▓   ▓▓▓   ▓▓▓  ▓▓▓  ▓▓▓  ▓▓▓                                     \n"); Sleep(40);
    setConsoleColor(14);
    gotoxy(3,6) ;  printf("\t                                                                                             \n");
    gotoxy(3,7) ;  printf("\t                        ▓▓▓   ▓▓▓   ▓▓▓▓▓▓       ▓▓▓▓  ▓▓▓  ▓▓▓▓▓▓▓▓▓▓  ▓▓▓                         \n"); Sleep(40); 
    gotoxy(3,8) ;  printf("\t                        ▓▓▓   ▓▓▓  ▓▓▓  ▓▓▓      ▓▓▓▓▓ ▓▓▓  ▓▓▓    ▓▓▓  ▓▓▓                         \n"); Sleep(40); setConsoleColor(6);
    gotoxy(3,9) ;  printf("\t                        ▓▓▓▓▓▓▓▓▓  ▓▓▓▓▓▓▓▓      ▓▓▓ ▓▓▓▓▓  ▓▓▓    ▓▓▓  ▓▓▓                         \n"); Sleep(40);
    gotoxy(3,10);  printf("\t                        ▓▓▓   ▓▓▓  ▓▓▓  ▓▓▓      ▓▓▓  ▓▓▓▓  ▓▓▓    ▓▓▓  ▓▓▓                         \n"); Sleep(40);
    gotoxy(3,11);  printf("\t                        ▓▓▓   ▓▓▓  ▓▓▓  ▓▓▓      ▓▓▓   ▓▓▓  ▓▓▓▓▓▓▓▓▓▓  ▓▓▓                         \n"); Sleep(40);
    setConsoleColor(7);
    gotoxy(BANNER_X, INPUT_Y);
}
// ===============================================

void setTextColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void hienThiCot(int chieuCao, int dia, int x, int y, int color) {
    gotoxy(x + chieuCao, y - 1);
    setTextColor(240); cout << " ";
    setTextColor(7);

    gotoxy(x, y);
    setTextColor(color);

    for (int i = 0; i < chieuCao - dia; i++) cout << " ";
    for (int i = 0; i < dia; i++)        cout << "█";

    setTextColor(240); cout << " ";
    setTextColor(color);
    for (int i = 0; i < dia; i++)        cout << "█";

    setTextColor(7);
}

void drawBoardNoWait(int **thap, int soDia, int chieuCao) {
    g_screenW = getConsoleWidth();
    g_topY = computeTopY(chieuCao);
    clearForTower(chieuCao);

    int coreW = 2 * chieuCao + 1;
    int gap   = 3;
    int totalW = coreW * 3 + gap * 2;
    while (totalW > g_screenW - 2 && gap > 1) {
        gap--;
        totalW = coreW * 3 + gap * 2;
    }
    int X_START = max(1, (g_screenW - totalW) / 2);

    int x = X_START;
    for (int j = 0; j < 3; j++) {
        int y = g_topY;
        for (int i = 0; i < chieuCao; i++) {
            int dia   = thap[j][i];
            int color = dia ? (dia % 15 + 1) : 7;
            hienThiCot(chieuCao, dia, x, y, color);
            y++;
        }
        x += (j < 2 ? coreW + gap : coreW);
    }

    gotoxy(BANNER_X, INPUT_Y + 3);
}



void hienThiThap(int **thap, int soDia, int chieuCao) {
    g_screenW = getConsoleWidth();

    g_topY = computeTopY(chieuCao);

    clearForTower(chieuCao);

    int coreW = 2 * chieuCao + 1;  
    int gap   = 3;
    int totalW = coreW * 3 + gap * 2;
    while (totalW > g_screenW - 2 && gap > 1) {
        gap--;
        totalW = coreW * 3 + gap * 2;
    }
    int X_START = max(1, (g_screenW - totalW) / 2);

    int x = X_START;
    for (int j = 0; j < 3; j++) {
        int y = g_topY;
        for (int i = 0; i < chieuCao; i++) {
            int dia   = thap[j][i];
            int color = dia ? (dia % 15 + 1) : 7;
            hienThiCot(chieuCao, dia, x, y, color);
            y++;
        }
        x += (j < 2 ? coreW + gap : coreW);
    }

    gotoxy(BANNER_X, INPUT_Y + 3);

    if (!waitStep()) return;
}






void diChuyenDia(int nguon, int dich, int **thap, int soDia, int &chieuCao) {
    if (g_abort) return; 

    int dia = 0;
    for (int i = 0; i < chieuCao; i++) {
        if (thap[nguon][i] != 0) {
            dia = thap[nguon][i];
            thap[nguon][i] = 0;
            break;
        }
    }
    for (int i = chieuCao - 1; i >= 0; i--) {
        if (thap[dich][i] == 0) {
            thap[dich][i] = dia;
            break;
        }
    }
    hienThiThap(thap, soDia, chieuCao); 
}


void thapHN(int m, int nguon, int dich, int trungGian, int **thap, int soDia, int &chieuCao) {
    if (g_abort) return;       
    if (m > 0) {
        thapHN(m - 1, nguon, trungGian, dich, thap, soDia, chieuCao);
        if (g_abort) return;

        diChuyenDia(nguon, dich, thap, soDia, chieuCao);
        if (g_abort) return;

        thapHN(m - 1, trungGian, dich, nguon, thap, soDia, chieuCao);
    }
}
void printHotkeyHint() {
    setConsoleColor(8);
    gotoxy(BANNER_X+35, INPUT_Y + 1);
    cout << "Enter: Tiến  |  Backspace: Lùi  |  ESC: Thoát";
    setConsoleColor(7);
}
void thapHaNoi(int soDia, int cotNguon, int cotDich, int cotTrungGian) {

    int **thap = new int *[3];
    for (int i = 0; i < 3; i++) {
        thap[i] = new int[soDia];
        for (int j = 0; j < soDia; j++) thap[i][j] = 0;
    }
    for (int i = soDia - 1; i >= 0; i--) thap[cotNguon][i] = i + 1;
    std::vector<Move> moves;
    genMoves(soDia, cotNguon, cotDich, cotTrungGian, moves);
    int k = 0;
    g_abort = false;
    printHotkeyHint();
    drawBoardNoWait(thap, soDia, soDia); 

    while (true) {
        int key = _getch();
        if (key == 27) { 
            g_abort = true;
            break;
        } else if (key == 13) { 
            if (k < (int)moves.size()) {
                int disk = popTop(thap, soDia, moves[k].from);
                pushTop(thap, soDia, moves[k].to, disk);
                k++;
                drawBoardNoWait(thap, soDia, soDia);
            }
        } else if (key == 8) { 
            if (k > 0) {
                k--;
                int disk = popTop(thap, soDia, moves[k].to);
                pushTop(thap, soDia, moves[k].from, disk);
                drawBoardNoWait(thap, soDia, soDia);
            }
        }

        if (!g_abort && k == (int)moves.size()) {
            setConsoleColor(10);

            gotoxy(max(3, (g_screenW - 50)/2), g_topY + soDia + 2);
            cout << "🎉 Chuc mung! Da hoan thanh Thap Ha Noi 🎉";
            setConsoleColor(7);
            gotoxy(max(3, (g_screenW - 40)/2), g_topY + soDia + 4);
            cout << "Nhan phim bat ky de quay lai phan nhap...";
            _getch();
            break;
        }
    }


    clearRect(0, g_topY, g_screenW, soDia + 8);


    for (int i = 0; i < 3; i++) delete[] thap[i];
    delete[] thap;
}



inline void resetToStart() {
    system("cls");
    hienthide();             
}
void disableWrapAtEOL() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    mode &= ~ENABLE_WRAP_AT_EOL_OUTPUT;
    SetConsoleMode(hOut, mode);
}
bool readIntInlineAt(int x, int y, const std::string& prompt,
                     int& out, int minV, int maxV,
                     const std::string& errLabel = "Gia tri khong hop le!") {
    std::string buf;
    while (true) {
        clearRect(x, y, g_screenW - x, 1);
        gotoxy(x, y);
        std::cout << prompt << buf;

        int ch = _getch();
        if (ch == 27) {
            return false;
        }
        if (ch == '\r') { 
            if (buf.empty()) {
                clearRect(x, y + 1, g_screenW - x, 1);
                gotoxy(x, y + 1);
                setConsoleColor(12);
                std::cout << "Vui long nhap so trong khoang [" << minV << ".." << maxV << "].";
                setConsoleColor(7);
                continue;
            }
            long long val = 0;
            try { val = std::stoll(buf); } catch (...) { val = (long long)minV - 1; }
            if (val < minV || val > maxV) {
                clearRect(x, y + 1, g_screenW - x, 1);
                gotoxy(x, y + 1);
                setConsoleColor(12);
                std::cout << errLabel << " [" << minV << ".." << maxV << "].";
                setConsoleColor(7);
                continue;
            }
            out = (int)val;
            clearRect(x, y + 1, g_screenW - x, 1); 
            return true;
        }
        if (ch == 8) { 
            if (!buf.empty()) buf.pop_back();
            continue;
        }
        if (ch >= '0' && ch <= '9') {
            if (buf.size() < 9) buf.push_back((char)ch);
        }
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    g_screenW = getConsoleWidth();
    disableWrapAtEOL();
    while (true) {
        resetToStart();
        int soDia;
if (!readIntInlineAt(BANNER_X, INPUT_Y,
    "Nhap so dia (3..10): ", soDia, 3, 10)) 
    continue;
}

int viTriNguon;
if (!readIntInlineAt(BANNER_X, INPUT_Y + 1,
    "Chon cot nguon (1..3): ", viTriNguon, 1, 3)) {
    continue;
}

int viTriDich;
        while (true) {
            if (!readIntInlineAt(BANNER_X, INPUT_Y + 2,
                "Chon cot dich (1..3, khac nguon): ", viTriDich, 1, 3)) {
                goto NEXT_ROUND; 
            }
            if (viTriDich != viTriNguon) break;
   
            clearRect(BANNER_X, INPUT_Y + 3, g_screenW - BANNER_X, 1);
            gotoxy(BANNER_X, INPUT_Y + 3);
            setConsoleColor(12);
            std::cout << "Cot dich phai KHAC cot nguon (" << viTriNguon << ").";
            setConsoleColor(7);
        }
        clearRect(BANNER_X, INPUT_Y + 3, g_screenW - BANNER_X, 1);
        printHotkeyHint();   
        thapHaNoi(soDia, viTriNguon - 1, viTriDich - 1, 6 - viTriNguon - viTriDich - 1);
        NEXT_ROUND: ;
        
        if (g_abort) {       
            resetToStart();  
            continue;        
        }
    }
    return 0;
}

