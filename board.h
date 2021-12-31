#ifndef _HEADER_BOARD_H_
#define _HEADER_BOARD_H_

#include <memory>
#include <iostream>

#include "move.h"
#include "piece.h"
#include "utilities.h"

// Window and lX11
#include "window.h"
class Xwindow;

class Piece;
class Move;
enum class ColorType;

const int BOARD_X = 8;
const int BOARD_Y = 8;

class Board {

	std::vector<Move> moveHistory;

	std::shared_ptr<Piece> internalBoard[BOARD_Y][BOARD_X];

	void loadBoard(char charBoard[8][8]);

	void charDraw();

	void updateCheckStatus();

	public:
		Board(char customBoard[8][8] = nullptr) {
			char defaultBoard[8][8] = {
				{'r','n','b','q','k','b','n','r'},
				{'p','p','p','p','p','p','p','p'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'P','P','P','P','P','P','P','P'},
				{'R','N','B','Q','K','B','N','R'}
			};

			if (customBoard == nullptr) {
				loadBoard(defaultBoard);
			} else {
				loadBoard(customBoard);
			}
		}
		bool tileExists(int x, int y) {
			if (((0 <= x) && (x < BOARD_X)) && ((0 <= y) && (y < BOARD_Y))) {
				return true;
			}
			else {
				return false;
			}
		}
		void draw() {
			charDraw();
		}
		void drawMoves(std::vector<Move> moves);
		std::shared_ptr<Piece> getAt(int x, int y) {
			return internalBoard[y][x];
		}
		int turnNumber = 0;
		std::vector<Move> getAllColorMoves(ColorType ct, bool noCastling = true);
		void enactMove(Move &mv);
		void undoLastMove();
		Move getLastMove();
		int numberOfMoves() {
			return moveHistory.size();
		}
		bool isWhiteInCheck = false;
		bool isBlackInCheck = false;
		bool isColorInCheck(ColorType ct);
		std::vector<Move> getAllValidColorMoves(ColorType ct, bool noCastling = true);
		int getTurnNumber() {
			return turnNumber;
		}
		void setTurnNumber(int value) {
			turnNumber = value;
		}
		bool isCheckmate(ColorType &thisColor);
		bool isStalemate(ColorType &thisColor);
		bool isInsuffiantMaterial(ColorType &thisColor);
		bool validate();
		void drawChars();
		void lX11Draw(Xwindow &xw);
		std::vector<Move> getMoveHistory() {
			return moveHistory;
		}
};


#endif // !_HEADER_BOARD_H_
