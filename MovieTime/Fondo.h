#pragma once
#include <windows.h>

using namespace System;

class Fondo
{
public:
    void gotoxy(int x, int y) {
        Console::SetCursorPosition(x, y);
    }

    // Función auxiliar para dibujar marcos de póster
    void dibujar_marco_poster(int movie[40][150], int x1, int y1, int x2, int y2, int color_fondo) {
        for (int f = y1; f <= y2; f++) { movie[f][x1] = 3; movie[f][x2] = 3; }
        for (int c = x1; c <= x2; c++) { movie[y1][c] = 3; movie[y2][c] = 3; }
        for (int f = y1 + 1; f < y2; f++) {
            for (int c = x1 + 1; c < x2; c++) {
                movie[f][c] = color_fondo;
            }
        }
    }

    void imprime_movietime_cartelera(int x, int y) {
        
        
        int movie[40][150] = { 0 };

        // --- 1. TEXTO "MOVIE" (Columnas 2 a 48, Filas 3 a 9) ---
        for (int f = 3; f <= 9; f++) { movie[f][2] = 1; movie[f][3] = 1; movie[f][10] = 1; movie[f][11] = 1; }
        movie[4][4] = 1; movie[4][5] = 1; movie[5][6] = 1; movie[5][7] = 1; movie[4][8] = 1; movie[4][9] = 1; // M
        for (int f = 3; f <= 9; f++) { movie[f][14] = 1; movie[f][15] = 1; movie[f][21] = 1; movie[f][22] = 1; }
        for (int c = 14; c <= 22; c++) { movie[3][c] = 1; movie[9][c] = 1; } // O
        for (int f = 3; f <= 7; f++) { movie[f][25] = 1; movie[f][26] = 1; movie[f][32] = 1; movie[f][33] = 1; }
        movie[8][27] = 1; movie[8][28] = 1; movie[8][30] = 1; movie[8][31] = 1; movie[9][29] = 1; // V
        for (int f = 3; f <= 9; f++) { movie[f][36] = 1; movie[f][37] = 1; }
        for (int c = 35; c <= 38; c++) { movie[3][c] = 1; movie[9][c] = 1; } // I
        for (int f = 3; f <= 9; f++) { movie[f][41] = 1; movie[f][42] = 1; }
        for (int c = 41; c <= 48; c++) { movie[3][c] = 1; movie[6][c] = 1; movie[9][c] = 1; } // E

        // --- 2. TEXTO "TIME" (Columnas 54 a 93, Filas 3 a 9) ---
        for (int f = 3; f <= 9; f++) { movie[f][59] = 2; movie[f][60] = 2; }
        for (int c = 54; c <= 65; c++) { movie[3][c] = 2; movie[4][c] = 2; } // T
        for (int f = 3; f <= 9; f++) { movie[f][68] = 2; movie[f][69] = 2; }
        for (int c = 67; c <= 70; c++) { movie[3][c] = 2; movie[9][c] = 2; } // I
        for (int f = 3; f <= 9; f++) { movie[f][73] = 2; movie[f][74] = 2; movie[f][82] = 2; movie[f][83] = 2; }
        movie[4][75] = 2; movie[4][76] = 2; movie[5][77] = 2; movie[5][78] = 2; movie[4][79] = 2; movie[4][80] = 2; // M
        for (int f = 3; f <= 9; f++) { movie[f][86] = 2; movie[f][87] = 2; }
        for (int c = 86; c <= 93; c++) { movie[3][c] = 2; movie[6][c] = 2; movie[9][c] = 2; } // E

        // --- 3. CINTA DE PELÍCULA (Esquina Superior Derecha) ---
        for (int c = 110; c <= 145; c++) { movie[1][c] = 3; movie[3][c] = 3; movie[7][c] = 3; movie[9][c] = 3; }
        for (int f = 1; f <= 9; f++) { movie[f][110] = 3; movie[f][145] = 3; }
        for (int c = 111; c <= 144; c++) { movie[2][c] = 4; movie[8][c] = 4; }
        for (int c = 111; c <= 144; c += 4) { movie[2][c] = 3; movie[8][c] = 3; }
        for (int f = 4; f <= 6; f++) for (int c = 111; c <= 144; c++) movie[f][c] = 5;

        // --- 4. RECUADRO NEGRO PARA MENÚ/SISTEMA (Filas 11 a 38, Cols 2 a 50) ---
        for (int f = 11; f <= 38; f++) {
            for (int c = 0; c <= 53; c++) {
                movie[f][c] = 12; // Nuevo ID para el contenedor del menú
            }
        }

        // --- 5. PARRILLA CONTINUA DE 12 CARTELERAS ---

        // --- COLUMNA 1 (55 a 74) ---
        dibujar_marco_poster(movie, 55, 12, 74, 19, 3);
        for (int f = 14; f <= 17; f++) for (int c = 61; c <= 68; c++) movie[f][c] = 4;
        movie[15][63] = 3; movie[15][66] = 3; movie[17][64] = 3; movie[17][65] = 3;

        dibujar_marco_poster(movie, 55, 21, 74, 28, 6);
        for (int f = 23; f <= 26; f++) for (int c = 61; c <= 68; c++) movie[f][c] = 8;
        movie[24][63] = 3; movie[24][64] = 3; movie[25][63] = 3; movie[25][66] = 3;

        dibujar_marco_poster(movie, 55, 30, 74, 37, 9);
        for (int f = 33; f <= 35; f++) for (int c = 62; c <= 67; c++) movie[f][c] = 8;
        movie[32][64] = 8; movie[32][65] = 8; movie[35][60] = 8; movie[35][69] = 8;

        // --- COLUMNA 2 (78 a 97) ---
        dibujar_marco_poster(movie, 78, 12, 97, 19, 6);
        for (int f = 14; f <= 17; f++) for (int c = 84; c <= 91; c++) movie[f][c] = 8;
        movie[15][86] = 9; movie[15][89] = 9; movie[16][87] = 3; movie[16][88] = 3;

        dibujar_marco_poster(movie, 78, 21, 97, 28, 9);
        for (int f = 23; f <= 26; f++) for (int c = 84; c <= 91; c++) movie[f][c] = 11;
        movie[24][85] = 4; movie[25][88] = 4;

        dibujar_marco_poster(movie, 78, 30, 97, 37, 11);
        for (int f = 34; f <= 36; f++) for (int c = 85; c <= 90; c++) movie[f][c] = 3;
        movie[33][87] = 3; movie[33][88] = 3;

        // --- COLUMNA 3 (102 a 121) ---
        dibujar_marco_poster(movie, 102, 12, 121, 19, 6);
        for (int f = 14; f <= 17; f++) {
            movie[f][106] = 7; movie[f][107] = 7;
            movie[f][116] = 7; movie[f][117] = 7;
        }

        dibujar_marco_poster(movie, 102, 21, 121, 28, 9);
        for (int f = 23; f <= 26; f++) for (int c = 108; c <= 115; c++) movie[f][c] = 6;
        movie[24][109] = 8; movie[24][110] = 8; movie[25][113] = 8; movie[25][114] = 8;

        dibujar_marco_poster(movie, 102, 30, 121, 37, 3);
        for (int c = 105; c <= 118; c += 2) {
            movie[32][c] = 10; movie[34][c] = 10; movie[35][c] = 10;
        }

        // --- COLUMNA 4 (125 a 144) ---
        dibujar_marco_poster(movie, 125, 12, 144, 19, 8);
        movie[14][131] = 3; movie[14][138] = 3;
        movie[15][132] = 3; movie[15][137] = 3;
        movie[16][130] = 6; movie[16][139] = 6;

        dibujar_marco_poster(movie, 125, 21, 144, 28, 3);
        for (int i = 0; i < 5; i++) {
            movie[26 - i][130 + i] = 9;
            movie[26 - i][138 - i] = 10;
        }

        dibujar_marco_poster(movie, 125, 30, 144, 37, 9);
        for (int f = 32; f <= 35; f++) for (int c = 130; c <= 139; c++) movie[f][c] = 6;
        for (int f = 33; f <= 34; f++) for (int c = 132; c <= 137; c++) movie[f][c] = 4;
        movie[33][134] = 9; movie[34][135] = 9;

        // --- 6. RENDERIZADO 100% NATIVO ---
        for (int f = 0; f < 40; f++) {
            for (int c = 0; c < 150; c++) {
                gotoxy(x + c, y + f);

                switch (movie[f][c]) {
                case 0: Console::BackgroundColor = ConsoleColor::Yellow; break;     // Fondo Amarillo Base
                case 1: Console::BackgroundColor = ConsoleColor::Black; break;      // MOVIE
                case 2: Console::BackgroundColor = ConsoleColor::DarkRed; break;    // TIME
                case 3: Console::BackgroundColor = ConsoleColor::Black; break;      // Marcos
                case 4: Console::BackgroundColor = ConsoleColor::White; break;      // Blanco
                case 5: Console::BackgroundColor = ConsoleColor::DarkGray; break;   // Gris
                case 6: Console::BackgroundColor = ConsoleColor::Red; break;        // Rojo
                case 7: Console::BackgroundColor = ConsoleColor::White; break;      // Blanco
                case 8: Console::BackgroundColor = ConsoleColor::Yellow; break;     // Amarillo
                case 9: Console::BackgroundColor = ConsoleColor::Blue; break;       // Azul
                case 10: Console::BackgroundColor = ConsoleColor::Green; break;     // Verde
                case 11: Console::BackgroundColor = ConsoleColor::DarkYellow; break;// Naranja
                case 12: Console::BackgroundColor = ConsoleColor::Black; break;      // Panel Fondo Menú
                }
                Console::Write(" ");
            }
        }
        Console::ResetColor();
    }
};