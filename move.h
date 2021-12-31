#ifndef _HEADER_MOVE_H_
#define _HEADER_MOVE_H_

#include <memory>

#include "piece.h"

class Board;
class Piece;
enum class ColorType;
enum class PieceType;

enum class MoveType { CAPTURE, MOVE, CASTLE_KING, CASTLE_QUEEN, EN_PASSANT, PAWN_BIGMOVE, NONE, PROMOTE };

class Move {
	MoveType mType;

	// Data about the from piece
	std::pair<int, int> fromPosition;
	ColorType fromColor;
	PieceType fromPiece;

	// Data about the destination piece
	std::pair<int, int> destinationPosition;
	ColorType destinationColor;
	PieceType destinationPiece;

	// Data about the capture piece
	std::pair<int, int> capturePosition;
	ColorType captureColor;
	PieceType capturePiece;

	// Data about the promotion (if applicable)
	PieceType promotePiece;
	ColorType promoteColor;

	public:
		MoveType getMoveType();
		std::pair<int, int> getFromPosition();
		std::pair<int, int> getDestinationPosition();
		std::pair<int, int> getCapturePosition();
		Move(MoveType mt, std::shared_ptr<Piece> from, std::shared_ptr<Piece> destination, std::shared_ptr<Piece> capture, PieceType promote = (PieceType)0); // (PieceType)0 is pawn

		// Returns a shared_ptr to a Piece that was previously captured
		std::shared_ptr<Piece> recreateCapturedPiece();
		
		// Determines if a move puts the king in check or not
		bool isChecking();

		// Information about the capture, from, promotion
		PieceType getCapturePieceType() {
			return capturePiece;
		}
		PieceType getFromPieceType() {
			return fromPiece;
		}
		ColorType getFromPieceColor() {
			return fromColor;
		}
		PieceType getPromoteType() {
			return promotePiece;
		}

};

#endif // !_HEADER_MOVE_H_
