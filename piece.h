#ifndef _HEADER_PIECE_H_
#define _HEADER_PIECE_H_

#include <vector>

// REMOVE THIS
#include <iostream>

#include "move.h"
#include "board.h"

class Board;
class Move;
enum class MoveType;

enum class PieceType { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY_TILE };
enum class ColorType { WHITE, BLACK, NONE };

class Piece {

	protected:
		PieceType pType;
		ColorType cType;
		bool hasMoved = false;
		int firstMoveTurn = -1;
		int lastMovedTurn = -1;
		MoveType lastMoveType;
		std::pair<int, int> position;
		int forwardsDirection = 1;

		// Used to initialize values
		void init();

		// Used for the copying of most basic piece data
		// Can be removed: the method clone() is never used
		void defaultClone(Piece &pc);

	public:
		// The method getAllMoves() returns all moves that a piece could make even it they are invalid
		// This is used to determine whether a tile is attacked or not
		virtual std::vector<Move> getAllMoves(Board& brd) = 0;
		PieceType getPieceType();
		ColorType getColorType();
		std::pair<int, int> getPosition() {
			return position;
		}
		void setPosition(std::pair<int, int> newPosition) {
			position = newPosition;
		}
		MoveType getLastMoveType() {
			return lastMoveType;
		}
		void setLastMoveType(MoveType mt) {
			lastMoveType = mt;
		}
		int getLastMovedTurn() {
			return lastMovedTurn;
		}
		// Used to set when a piece last moved
		// It is able to set and unset the hasMoved variable
		void setLastMovedTurn(int lastTurn) {

			// This is a horrible function, but I live with it :)
			// It is also currently broken and lots of code depends on it

			if (lastTurn <= -1) {
				hasMoved = false;
				firstMoveTurn = -1;
				lastMovedTurn = -1;

				return;
			}

			if (firstMoveTurn == -1) {
				firstMoveTurn = lastTurn;
				lastMovedTurn = lastTurn;
				hasMoved = true;
			} else if (firstMoveTurn == lastTurn) {
				lastMovedTurn = lastTurn;

				hasMoved = false;
			}
		}
		virtual std::shared_ptr<Piece> clone() = 0;
		// Returns all legal moves a piece can make
		std::vector<Move> getValidMoves(Board &brd);
		bool moved() {
			if (getLastMovedTurn() > 1) {
				return true;
			}
			return hasMoved;
		}
};

// The pawn class
class Pawn : public Piece {
	public:
		Pawn(ColorType ct, int x, int y);
		std::vector<Move> getAllMoves(Board& brd);
		std::shared_ptr<Piece> clone();
};

// The knight class
class Knight : public Piece {
	public:
		Knight(ColorType ct, int x, int y);
		std::vector<Move> getAllMoves(Board& brd);
		std::shared_ptr<Piece> clone();
};

// The Bishop class
class Bishop : public Piece {
	public:
		Bishop(ColorType ct, int x, int y);
		std::vector<Move> getAllMoves(Board& brd);
		std::shared_ptr<Piece> clone();
};

// The Rook class
class Rook : public Piece {
	public:
		Rook(ColorType ct, int x, int y);
		std::vector<Move> getAllMoves(Board& brd);
		std::shared_ptr<Piece> clone();
};

// The Queen class
class Queen : public Piece {
	public:
		Queen(ColorType ct, int x, int y);
		std::vector<Move> getAllMoves(Board& brd);
		std::shared_ptr<Piece> clone();
};

// The King class
class King : public Piece {
	public:
		King(ColorType ct, int x, int y);
		std::vector<Move> getAllMoves(Board& brd);
		std::shared_ptr<Piece> clone();
};

// returns all moves a king could make (regardless of legality) excluding castling
std::vector<Move> kingMovesNoCastle(std::shared_ptr<Piece> king, Board& brd);

// The EmptyTile class
class EmptyTile : public Piece {
	public:
		EmptyTile(ColorType ct, int x, int y);
		std::vector<Move> getAllMoves(Board& brd);
		std::shared_ptr<Piece> clone();
};

#endif // !_HEADER_PIECE_H_

