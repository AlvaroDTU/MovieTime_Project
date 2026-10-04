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

        // --- 4. RECUADRO NEGRO AMPLIADO (Filas 11 a 38, Cols 0 a 63) ---
        for (int f = 11; f <= 38; f++) {
            for (int c = 0; c <= 63; c++) {
                movie[f][c] = 12; // Panel de Fondo
            }
        }

        // --- 5. PARRILLA DE 12 CARTELERAS REDIMENSIONADAS ---

        // --- COLUMNA 1 (67 a 82) ---
        dibujar_marco_poster(movie, 67, 12, 82, 19, 3);
        for (int f = 14; f <= 17; f++) for (int c = 71; c <= 78; c++) movie[f][c] = 4;
        movie[15][73] = 3; movie[15][76] = 3; movie[17][74] = 3; movie[17][75] = 3;

        dibujar_marco_poster(movie, 67, 21, 82, 28, 6);
        for (int f = 23; f <= 26; f++) for (int c = 71; c <= 78; c++) movie[f][c] = 8;
        movie[24][73] = 3; movie[24][74] = 3; movie[25][73] = 3; movie[25][76] = 3;

        dibujar_marco_poster(movie, 67, 30, 82, 37, 9);
        for (int f = 33; f <= 35; f++) for (int c = 72; c <= 77; c++) movie[f][c] = 8;
        movie[32][74] = 8; movie[32][75] = 8; movie[35][70] = 8; movie[35][79] = 8;

        // --- COLUMNA 2 (88 a 103) ---
        dibujar_marco_poster(movie, 88, 12, 103, 19, 6);
        for (int f = 14; f <= 17; f++) for (int c = 92; c <= 99; c++) movie[f][c] = 8;
        movie[15][94] = 9; movie[15][97] = 9; movie[16][95] = 3; movie[16][96] = 3;

        dibujar_marco_poster(movie, 88, 21, 103, 28, 9);
        for (int f = 23; f <= 26; f++) for (int c = 92; c <= 99; c++) movie[f][c] = 11;
        movie[24][93] = 4; movie[25][96] = 4;

        dibujar_marco_poster(movie, 88, 30, 103, 37, 11);
        for (int f = 34; f <= 36; f++) for (int c = 93; c <= 98; c++) movie[f][c] = 3;
        movie[33][95] = 3; movie[33][96] = 3;

        // --- COLUMNA 3 (109 a 124) ---
        dibujar_marco_poster(movie, 109, 12, 124, 19, 6);
        for (int f = 14; f <= 17; f++) {
            movie[f][112] = 7; movie[f][113] = 7;
            movie[f][120] = 7; movie[f][121] = 7;
        }

        dibujar_marco_poster(movie, 109, 21, 124, 28, 9);
        for (int f = 23; f <= 26; f++) for (int c = 113; c <= 120; c++) movie[f][c] = 6;
        movie[24][114] = 8; movie[24][115] = 8; movie[25][118] = 8; movie[25][119] = 8;

        dibujar_marco_poster(movie, 109, 30, 124, 37, 3);
        for (int c = 111; c <= 122; c += 2) {
            movie[32][c] = 10; movie[34][c] = 10; movie[35][c] = 10;
        }

        // --- COLUMNA 4 (130 a 145) ---
        dibujar_marco_poster(movie, 130, 12, 145, 19, 8);
        movie[14][135] = 3; movie[14][140] = 3;
        movie[15][136] = 3; movie[15][139] = 3;
        movie[16][134] = 6; movie[16][141] = 6;

        dibujar_marco_poster(movie, 130, 21, 145, 28, 3);
        for (int i = 0; i < 5; i++) {
            movie[26 - i][134 + i] = 9;
            movie[26 - i][141 - i] = 10;
        }

        dibujar_marco_poster(movie, 130, 30, 145, 37, 9);
        for (int f = 32; f <= 35; f++) for (int c = 134; c <= 141; c++) movie[f][c] = 6;
        for (int f = 33; f <= 34; f++) for (int c = 135; c <= 140; c++) movie[f][c] = 4;
        movie[33][137] = 9; movie[34][138] = 9;

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