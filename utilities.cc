#include "utilities.h"

std::shared_ptr<Piece> pieceConstructor(char pieceCharacter, int x, int y) {
	
	if (pieceCharacter == '_') {
		// empty character has color of ColorType::NONE
	}

	// convert pieceCharacter to lower case
	ColorType cType = ColorType::BLACK;
	if (('A' <= pieceCharacter) && (pieceCharacter <= 'Z') && (pieceCharacter != '_')) {
		cType = ColorType::WHITE;
		pieceCharacter = pieceCharacter - 'A' + 'a';
	}

	std::shared_ptr<Piece> sharedPiece;

	// Convert lowercase char to piece
	switch (pieceCharacter) {
		case 'p':
			sharedPiece = std::make_shared<Pawn>(Pawn(cType, x, y));
			break;
		case 'n':
			sharedPiece =  std::make_shared<Knight>(Knight(cType, x, y));
			break;
		case 'b':
			sharedPiece = std::make_shared<Bishop>(Bishop(cType, x, y));
			break;
		case 'r':
			sharedPiece = std::make_shared<Rook>(Rook(cType, x, y));
			break;
		case 'q':
			sharedPiece = std::make_shared<Queen>(Queen(cType, x, y));
			break;
		case 'k':
			sharedPiece = std::make_shared<King>(King(cType, x, y));
			break;
		case '_':
			sharedPiece = std::make_shared<EmptyTile>(EmptyTile(ColorType::NONE, x, y));
			break;
		default:
			sharedPiece = std::make_shared<EmptyTile>(EmptyTile(ColorType::NONE, x, y));
			break;
	}

	return sharedPiece;
}

char charFromPiece(std::shared_ptr<Piece> pc) {
	PieceType pType = pc->getPieceType();
	ColorType cType = pc->getColorType();

	char pChar = '_';

	// Get character
	switch (pType) {
		case PieceType::PAWN:
			pChar = 'p';
			break;
		case PieceType::KNIGHT:
			pChar = 'n';
			break;
		case PieceType::BISHOP:
			pChar = 'b';
			break;
		case PieceType::ROOK:
			pChar = 'r';
			break;
		case PieceType::QUEEN:
			pChar = 'q';
			break;
		case PieceType::KING:
			pChar = 'k';
			break;
		case PieceType::EMPTY_TILE:
			return '_';
			break;
	}

	// Change character to uppercase if cType == WHITE
	if (cType == ColorType::WHITE) {
		pChar = pChar - 'a' + 'A';
	}

	return pChar;
}

ColorType oppositeColor(ColorType color) {
	switch (color) {
		case ColorType::WHITE:
			return ColorType::BLACK;
			break;
		case ColorType::BLACK:
			return ColorType::WHITE;
			break;
		case ColorType::NONE:
			return ColorType::NONE;
			break;
		default:
			return ColorType::NONE;
			break;
	}
}

ColorType intToColorType(int value) {

	// Reduce to only white or black
	value = value % 2;

	if (value == 0) {
		return ColorType::WHITE;
	}
	else if (value == 1) {
		return ColorType::BLACK;
	}
	else {
		return ColorType::NONE;
	}
}

int getPiecePoints(PieceType pType) {
	switch (pType) {
		case PieceType::PAWN:
			return 1;
			break;
		case PieceType::KNIGHT:
			return 3;
			break;
		case PieceType::BISHOP:
			return 3;
			break;
		case PieceType::ROOK:
			return 5;
			break;
		case PieceType::QUEEN:
			return 9;
			break;
		case PieceType::KING:
			return 0; // The king is unvaluble
			break;
		case PieceType::EMPTY_TILE:
			return 0;
			break;
		default:
			return 0;
			break;
	}
}

