#include "move.h"

MoveType Move::getMoveType() {
	return mType;
}

std::pair<int, int> Move::getFromPosition() {
	return fromPosition;
}

std::pair<int, int> Move::getDestinationPosition() {
	return destinationPosition;
}

std::pair<int, int> Move::getCapturePosition() {
	return capturePosition;
}

Move::Move(MoveType mt, std::shared_ptr<Piece> from, std::shared_ptr<Piece> destination, std::shared_ptr<Piece> capture, PieceType promote) : mType{ mt } {

	// Load data into from
	fromPosition = from->getPosition();
	fromColor = from->getColorType();
	fromPiece = from->getPieceType();

	// Load data into destination
	destinationPosition = destination->getPosition();
	destinationColor = destination->getColorType();
	destinationPiece = destination->getPieceType();

	// Load data into capture
	capturePosition = capture->getPosition();
	captureColor = capture->getColorType();
	capturePiece = capture->getPieceType();

	promotePiece = promote;
}

std::shared_ptr<Piece> Move::recreateCapturedPiece() {
	
	std::shared_ptr<Piece> recreatedPiece;

	// Create a shared_ptr based on the captured piece type
	switch (capturePiece) {
		case PieceType::PAWN:
			recreatedPiece = std::make_shared<Pawn>(Pawn(captureColor, capturePosition.first, capturePosition.second));
			break;
		case PieceType::KNIGHT:
			recreatedPiece = std::make_shared<Knight>(Knight(captureColor, capturePosition.first, capturePosition.second));
			break;
		case PieceType::BISHOP:
			recreatedPiece = std::make_shared<Bishop>(Bishop(captureColor, capturePosition.first, capturePosition.second));
			break;
		case PieceType::ROOK:
			recreatedPiece = std::make_shared<Rook>(Rook(captureColor, capturePosition.first, capturePosition.second));
			break;
		case PieceType::QUEEN:
			recreatedPiece = std::make_shared<Queen>(Queen(captureColor, capturePosition.first, capturePosition.second));
			break;
		case PieceType::KING:
			recreatedPiece = std::make_shared<King>(King(captureColor, capturePosition.first, capturePosition.second));
			break;
		case PieceType::EMPTY_TILE:
			recreatedPiece = std::make_shared<EmptyTile>(EmptyTile(captureColor, capturePosition.first, capturePosition.second));
			break;
	}

	return recreatedPiece;
}

bool Move::isChecking() {
	// All moves that attack a king have a capture piece of type king
	return capturePiece == PieceType::KING;
}
