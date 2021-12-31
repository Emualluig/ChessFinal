#ifndef _HEADER_UTILITIES_H_
#define _HEADER_UTILITIES_H_

#include <memory>

#include "piece.h"

enum class PieceType;
enum class ColorType;
class Piece;

// Creates a Piece based on the character and its location
std::shared_ptr<Piece> pieceConstructor(char pieceCharacter, int x, int y);

// Returns the char associated to that Piece
char charFromPiece(std::shared_ptr<Piece> pc);

// Returns the opposite color
// White -> Black
// Black -> White
// None -> None
ColorType oppositeColor(ColorType color);

// Convert a number to a color
// 0 -> White
// 1 -> Black
// Can be used to determine which color must play depending on turn number 
ColorType intToColorType(int value);

// Calculates the piece value of a PieceType
int getPiecePoints(PieceType pType);

#endif // !_HEADER_UTILITIES_H_

