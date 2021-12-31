#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <X11/Xlib.h>
#include <iostream>
#include <string>

#include "board.h"
#include "piece.h"

enum class ChessColor;
enum class PieceType;
enum class ColorType;

class Board;

const int TOTAL_COLORS = 6;

class Xwindow
{
    Display *dis;
    Window w;
    int screen;
    GC gc;
    unsigned long colors[TOTAL_COLORS];

    std::pair<PieceType, ColorType> oldBoard[8][8];
    std::pair<PieceType, ColorType> newBoard[8][8];

    // Frame dimensions;
    const int pixelSize = 4;
    const int tileHeight = pixelSize * 20;  // 80 (multiply by 20 since the graphics are 20x20) view drawPiece for the graphics
    const int tileWidth = pixelSize * 20;   // 80
    const int frameHeight = tileHeight * 8; // 640 (multiply by 8 since the board is 8x8)
    const int frameWidth = tileWidth * 8;   // 640

    // Will drawn on the Xwindow a piece of type pType with color cType at the given x and y
    void drawPiece(int x, int y, PieceType pType, ColorType cType);

    // Draws the difference between the current board and the past board
    // Use this to cut down on redrawing pieces and tiles
    void diffDraw();

    // Draws a rectangle
    void fillRectangle(int x, int y, int width, int height, int color = Black);

public:
    Xwindow(Board &board);
    void load(Board &board);

    ~Xwindow();
    Xwindow(const Xwindow &) = delete;
    Xwindow &operator=(const Xwindow &) = delete;

    // Available colours.
    enum
    {
        White = 0,
        Black,
        Brown,
        Burlywood,
        SlateGray,
        Seashell
    };
};

enum class ChessColor
{
    WhiteTile = Xwindow::Burlywood,
    BlackTile = Xwindow::SlateGray,
    WhitePiece = Xwindow::Seashell,
    BlackPiece = Xwindow::Black
};

#endif
