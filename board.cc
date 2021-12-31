#include "board.h"

void Board::loadBoard(char charBoard[8][8]) {
	for (int row = 0; row < BOARD_Y; row++) {
		for (int col = 0; col < BOARD_X; col++) {
			internalBoard[row][col] = pieceConstructor(charBoard[row][col], col, row);
		}
	}

	updateCheckStatus();
}

std::string boolToStr(bool value) {
	if (value) {
		return "true";
	} else {
		return "false";
	}
}

void Board::charDraw() {
	std::cout << "   W: " << boolToStr(isWhiteInCheck) << ", B: " << boolToStr(isBlackInCheck) << ", TURN: " << turnNumber << std::endl;
	for (int row = 0; row < BOARD_Y; row++) {
		std::cout << row << ": ";
		for (int col = 0; col < BOARD_X; col++) {
			std::cout << charFromPiece(internalBoard[row][col]);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << "   ";
	for (int col = 0; col < BOARD_X; col++) {
		std::cout << col;
	}
	std::cout << std::endl << std::endl;
}

void Board::drawMoves(std::vector<Move> moves) {
	for (int row = 0; row < BOARD_Y; row++) {
		std::cout << row << ": ";
		for (int col = 0; col < BOARD_X; col++) {

			bool foundMove = false;

			for (auto &move : moves) {
				std::pair<int, int> capturePos = move.getDestinationPosition();
				int captureX = capturePos.first;
				int captureY = capturePos.second;

				if ((captureX == col) && (captureY == row)) {
				
					std::cout << "X";

					foundMove = true;
					break;
				}
			}

			if (!foundMove) {
				std::cout << charFromPiece(internalBoard[row][col]);
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << "   ";
	for (int col = 0; col < BOARD_X; col++) {
		std::cout << col;
	}
	std::cout << std::endl << std::endl;
}


std::vector<Move> Board::getAllColorMoves(ColorType ct, bool noCastling) {
	std::vector<Move> moveAccumulator;

	for (int row = 0; row < BOARD_Y; row++) {
		for (int col = 0; col < BOARD_X; col++) {
			
			std::shared_ptr<Piece> pc = getAt(col, row);

			if (pc->getColorType() == ct) {
				std::vector<Move> pieceMoves;

				if (pc->getPieceType() == PieceType::KING) {
					if (noCastling) {
						pieceMoves = kingMovesNoCastle(pc, *this);
					} else {
						pieceMoves = pc->getAllMoves(*this);
					}
				} else {
					pieceMoves = pc->getAllMoves(*this);
				}

				moveAccumulator.insert(moveAccumulator.end(), pieceMoves.begin(), pieceMoves.end());
			}

		}
	}

	return moveAccumulator;
}

void Board::updateCheckStatus() {

	// If the Black King is attacked, then black is in check
	isBlackInCheck = false;
	std::vector<Move> whiteMoves = getAllColorMoves(ColorType::WHITE);
	for (Move &mv : whiteMoves) {
		if (mv.isChecking()) {
			isBlackInCheck = true;
			break;
		}
	}

	// If the White King is attacked, then white is in check
	isWhiteInCheck = false;
	std::vector<Move> blackMoves = getAllColorMoves(ColorType::BLACK);
	for (Move& mv : blackMoves) {
		if (mv.isChecking()) {
			isWhiteInCheck = true;
			break;
		}
	}
	
}

void Board::enactMove(Move& mv) {
	moveHistory.push_back(mv);

	std::pair<int, int> fromPosition = mv.getFromPosition();
	std::pair<int, int> destinationPosition = mv.getDestinationPosition();
	std::pair<int, int> capturePosition = mv.getCapturePosition();

	// Custom logic for en passant
	if (mv.getMoveType() == MoveType::EN_PASSANT) {

		std::pair<int, int> fromLoc = mv.getFromPosition();
		std::pair<int, int> destLoc = mv.getDestinationPosition();
		std::pair<int, int> captLoc = mv.getCapturePosition();

		std::shared_ptr<Piece> fromEmptyTile = std::make_shared<EmptyTile>(EmptyTile(ColorType::NONE, fromLoc.first, fromLoc.second));
		std::shared_ptr<Piece> captEmptyTile = std::make_shared<EmptyTile>(EmptyTile(ColorType::NONE, captLoc.first, captLoc.second));

		std::shared_ptr<Piece> thePawn = getAt(fromLoc.first, fromLoc.second);
		thePawn->setPosition(destLoc);
		thePawn->setLastMovedTurn(turnNumber);

		internalBoard[destLoc.second][destLoc.first] = thePawn;
		internalBoard[fromLoc.second][fromLoc.first] = fromEmptyTile;
		internalBoard[captLoc.second][captLoc.first] = captEmptyTile;

		updateCheckStatus();
		turnNumber++;
		return;
	}

	// Custom logic for castling
	if (mv.getMoveType() == MoveType::CASTLE_KING || mv.getMoveType() == MoveType::CASTLE_QUEEN) {
	
		// Kingside castling
		if (mv.getMoveType() == MoveType::CASTLE_KING) {

			// Change location of rook depending on color
			std::pair<int, int> rookCornerCoords = (mv.getFromPieceColor() == ColorType::WHITE) ? std::pair<int, int>(7, 7) : std::pair<int, int>(7, 0);

			// Move the king
			std::pair<int, int> newKingPosition = mv.getCapturePosition();
			std::pair<int, int> oldKingPosition = mv.getFromPosition();
			std::shared_ptr<Piece> theKing = getAt(oldKingPosition.first, oldKingPosition.second);
			theKing->setLastMovedTurn(turnNumber);
			theKing->setPosition(newKingPosition);
			internalBoard[newKingPosition.second][newKingPosition.first] = theKing;

			// Put emptyTile on old king position
			std::shared_ptr<Piece> kingEmptyTile = std::make_shared<EmptyTile>(EmptyTile(ColorType::NONE, oldKingPosition.first, oldKingPosition.second));
			internalBoard[oldKingPosition.second][oldKingPosition.first] = kingEmptyTile;

			// Move the Rook
			std::pair<int, int> newRookPosition = mv.getDestinationPosition();
			std::pair<int, int> oldRookPosition = rookCornerCoords;
			std::shared_ptr<Piece> theRook = getAt(oldRookPosition.first, oldRookPosition.second);
			theRook->setLastMovedTurn(turnNumber);
			theRook->setPosition(newRookPosition);
			internalBoard[newRookPosition.second][newRookPosition.first] = theRook;

			// Put emptyTile on old rook position
			std::shared_ptr<Piece> rookEmptyTile = std::make_shared<EmptyTile>(EmptyTile(ColorType::NONE, oldRookPosition.first, oldRookPosition.second));
			internalBoard[oldRookPosition.second][oldRookPosition.first] = rookEmptyTile;

		} else {
			// Queenside castling

			int backrow = 7;
			if (mv.getFromPieceColor() == ColorType::WHITE) {
				backrow = 7;
			} else {
				backrow = 0;
			}

			std::pair<int, int> oldKingPosition = mv.getFromPosition();
			std::pair<int, int> newKingPosition = mv.getCapturePosition();
			std::pair<int, int> oldRookPosition = std::pair<int, int>(0, backrow);
			std::pair<int, int> newRookPosition = mv.getDestinationPosition();

			std::shared_ptr<Piece> theKing = getAt(oldKingPosition.first, oldKingPosition.second);
			std::shared_ptr<Piece> theRook = getAt(oldRookPosition.first, oldRookPosition.second);

			// Move king to correct position and put emptyTile in old position
			internalBoard[newKingPosition.second][newKingPosition.first] = theKing;
			theKing->setLastMovedTurn(turnNumber);
			theKing->setPosition(newKingPosition);
			std::shared_ptr<Piece> kingEmptyTile = std::make_shared<EmptyTile>(EmptyTile(ColorType::NONE, oldKingPosition.first, oldKingPosition.second));
			internalBoard[oldKingPosition.second][oldKingPosition.first] = kingEmptyTile;

			// Move rook to correct position and put emptyTile in old position
			internalBoard[newRookPosition.second][newRookPosition.first] = theRook;
			theRook->setLastMovedTurn(turnNumber);
			theRook->setPosition(newRookPosition);
			std::shared_ptr<Piece> rookEmptyTile = std::make_shared<EmptyTile>(EmptyTile(ColorType::NONE, oldRookPosition.first, oldRookPosition.second));
			internalBoard[oldRookPosition.second][oldRookPosition.first] = rookEmptyTile;

		}

		updateCheckStatus();
		turnNumber++;
		return;
	}

	// Replace capturePosition with emptyTile
	std::shared_ptr<Piece> capturePiece = getAt(capturePosition.first, capturePosition.second);
	std::shared_ptr<Piece> captureEmptyTile = std::make_shared<EmptyTile>(EmptyTile(ColorType::NONE, capturePosition.first, capturePosition.second));

	internalBoard[capturePosition.second][capturePosition.first] = captureEmptyTile;

	// Replace destinationPosition with fromPosition
	std::shared_ptr<Piece> fromPiece = getAt(fromPosition.first, fromPosition.second);
	std::shared_ptr<Piece> destinationPiece = getAt(destinationPosition.first, destinationPosition.second);

	internalBoard[destinationPosition.second][destinationPosition.first] = fromPiece;

	// Update fromPiece's location
	fromPiece->setPosition(destinationPosition);
	fromPiece->setLastMovedTurn(turnNumber);
	
	// Replace old fromPosition to emptyTile
	std::shared_ptr<Piece> fromEmptyTile = std::make_shared<EmptyTile>(EmptyTile(ColorType::NONE, fromPosition.first, fromPosition.second));

	internalBoard[fromPosition.second][fromPosition.first] = fromEmptyTile;

	// Pawn promotion
	if (mv.getMoveType() == MoveType::PROMOTE) {
		
		std::shared_ptr<Piece> promotePiece;

		PieceType promoteType = mv.getPromoteType();

		if (promoteType == PieceType::BISHOP) {
			promotePiece = std::make_shared<Bishop>(Bishop(fromPiece->getColorType(), destinationPosition.first, destinationPosition.second));
		} else if (promoteType == PieceType::KNIGHT) {
			promotePiece = std::make_shared<Knight>(Knight(fromPiece->getColorType(), destinationPosition.first, destinationPosition.second));
		} else if (promoteType == PieceType::ROOK) {
			promotePiece = std::make_shared<Rook>(Rook(fromPiece->getColorType(), destinationPosition.first, destinationPosition.second));
		} else if (promoteType == PieceType::QUEEN) {
			promotePiece = std::make_shared<Queen>(Queen(fromPiece->getColorType(), destinationPosition.first, destinationPosition.second));
		} else {
			// Queen
			promotePiece = std::make_shared<Queen>(Queen(fromPiece->getColorType(), destinationPosition.first, destinationPosition.second));
		}

		promotePiece->setPosition(destinationPosition);
		promotePiece->setLastMovedTurn(turnNumber);

		internalBoard[destinationPosition.second][destinationPosition.first] = promotePiece;
	}

	// Detect checks and update bools
	// TODO:
	updateCheckStatus();

	turnNumber++;
}

void Board::undoLastMove() {

	// Exit if no moves
	if (moveHistory.size() == 0) {
		return;
	}

	Move lastMove = moveHistory.back();
	moveHistory.pop_back();

	std::pair<int, int> fromPosition = lastMove.getFromPosition();
	std::pair<int, int> destinationPosition = lastMove.getDestinationPosition();
	std::pair<int, int> capturePosition = lastMove.getCapturePosition();

	// Custom logic for en passant
	if (lastMove.getMoveType() == MoveType::EN_PASSANT) {

		std::pair<int, int> originalLocation = lastMove.getFromPosition();
		std::pair<int, int> capturePieceLocation = lastMove.getCapturePosition();
		std::pair<int, int> destinationLocation = lastMove.getDestinationPosition();

		// Move pawn back to original location
		std::shared_ptr<Piece> thePawn = getAt(destinationLocation.first, destinationLocation.second);
		thePawn->setPosition(originalLocation);
		thePawn->setLastMovedTurn(turnNumber - 1);
		internalBoard[originalLocation.second][originalLocation.first] = thePawn;

		// Put empty tile on destination location
		std::shared_ptr<Piece> pawnEmptyTile = std::make_shared<EmptyTile>(EmptyTile(ColorType::NONE, destinationLocation.first, destinationLocation.second));
		internalBoard[destinationLocation.second][destinationLocation.first] = pawnEmptyTile;

		// Recreate captured pawn
		std::shared_ptr<Piece> recreatedPawn = lastMove.recreateCapturedPiece();
		recreatedPawn->setPosition(capturePosition);
		recreatedPawn->setLastMovedTurn(turnNumber - 1);
		internalBoard[capturePosition.second][capturePosition.first] = recreatedPawn;


		updateCheckStatus();
		turnNumber--;
		return;
	}

	// Custom logic for castling
	if (lastMove.getMoveType() == MoveType::CASTLE_KING || lastMove.getMoveType() == MoveType::CASTLE_QUEEN) {

		// Kingside castle
		if (lastMove.getMoveType() == MoveType::CASTLE_KING) {
			
			// Set fixed positions based on color
			std::pair<int, int> colorBasedCurrentRookPosition = (lastMove.getFromPieceColor() == ColorType::WHITE) ? std::pair<int, int>(5, 7) : std::pair<int, int>(5, 0);
			std::pair<int, int> colorBasedNewRookPosition     = (lastMove.getFromPieceColor() == ColorType::WHITE) ? std::pair<int, int>(7, 7) : std::pair<int, int>(7, 0);
			std::pair<int, int> colorBasedCurrentKingPosition = (lastMove.getFromPieceColor() == ColorType::WHITE) ? std::pair<int, int>(6, 7) : std::pair<int, int>(6, 0);
			std::pair<int, int> colorBasedNewKingPosition     = (lastMove.getFromPieceColor() == ColorType::WHITE) ? std::pair<int, int>(4, 7) : std::pair<int, int>(4, 0);

			// Generic kingside castling undo code
			std::pair<int, int> currentRookPosition = colorBasedCurrentRookPosition;
			std::pair<int, int> newRookPosition = colorBasedNewRookPosition;
			std::shared_ptr<Piece> theRook = getAt(currentRookPosition.first, currentRookPosition.second);
			theRook->setLastMovedTurn(turnNumber - 1);
			theRook->setPosition(newRookPosition);
			internalBoard[newRookPosition.second][newRookPosition.first] = theRook;

			// Place emptyTile on current rook position
			std::shared_ptr<Piece> rookEmptyTile = std::make_shared<EmptyTile>(EmptyTile(ColorType::NONE, currentRookPosition.first, currentRookPosition.second));
			internalBoard[currentRookPosition.second][currentRookPosition.first] = rookEmptyTile;

			// Move king back to center
			std::pair<int, int> currentKingPosition = colorBasedCurrentKingPosition;
			std::pair<int, int> newKingPosition = colorBasedNewKingPosition;
			std::shared_ptr<Piece> theKing = getAt(currentKingPosition.first, currentKingPosition.second);
			theKing->setLastMovedTurn(turnNumber - 1);
			theKing->setPosition(newKingPosition);
			internalBoard[newKingPosition.second][newKingPosition.first] = theKing;

			// Place emptyTile on current king position
			std::shared_ptr<Piece> kingEmptyTile = std::make_shared<EmptyTile>(EmptyTile(ColorType::NONE, currentKingPosition.first, currentKingPosition.second));
			internalBoard[currentKingPosition.second][currentKingPosition.first] = kingEmptyTile;

			// Reset hasMoved on rook and king
			theKing->setLastMovedTurn(-2);
			theRook->setLastMovedTurn(-2);

		} else {
			// Queenside castling

			// Set where the pieces will move to based on color
			int backrow = 7;
			if (lastMove.getFromPieceColor() == ColorType::WHITE) {
				backrow = 7;
			} else {
				backrow = 0;
			}

			std::pair<int, int> oldKingPosition = std::pair<int, int>(2, backrow);
			std::pair<int, int> newKingPosition = std::pair<int, int>(4, backrow);
			std::pair<int, int> oldRookPosition = std::pair<int, int>(3, backrow);
			std::pair<int, int> newRookPosition = std::pair<int, int>(0, backrow);

			std::shared_ptr<Piece> theKing = getAt(oldKingPosition.first, oldKingPosition.second);
			std::shared_ptr<Piece> theRook = getAt(oldRookPosition.first, oldRookPosition.second);

			// Move king back to original position
			internalBoard[newKingPosition.second][newKingPosition.first] = theKing;
			theKing->setPosition(newKingPosition);
			std::shared_ptr<Piece> kingEmptyTile = std::make_shared<EmptyTile>(EmptyTile(ColorType::NONE, oldKingPosition.first, oldKingPosition.second));
			internalBoard[oldKingPosition.second][oldKingPosition.first] = kingEmptyTile;

			// Move rook back to original position
			internalBoard[newRookPosition.second][newRookPosition.first] = theRook;
			theRook->setPosition(newRookPosition);
			std::shared_ptr<Piece> rookEmptyTile = std::make_shared<EmptyTile>(EmptyTile(ColorType::NONE, oldRookPosition.first, oldRookPosition.second));
			internalBoard[oldRookPosition.second][oldRookPosition.first] = rookEmptyTile;

			// Reset hasMoved on rook and king
			theKing->setLastMovedTurn(-2);
			theRook->setLastMovedTurn(-2);
		
		}

		updateCheckStatus();
		turnNumber--;
		return;
	}

	// put destination piece on from location
	std::shared_ptr<Piece> movedPiece = getAt(destinationPosition.first, destinationPosition.second);
	internalBoard[destinationPosition.second][destinationPosition.first] = std::make_shared<EmptyTile>(EmptyTile(ColorType::NONE, destinationPosition.first, destinationPosition.second));
	internalBoard[fromPosition.second][fromPosition.first] = movedPiece;
	movedPiece->setPosition(fromPosition);
	movedPiece->setLastMovedTurn(turnNumber - 1);

	// Reset hasMoved for king
	if (lastMove.getMoveType() == MoveType::PAWN_BIGMOVE) {
		movedPiece->setLastMovedTurn(-1); // -1 resets the hasMoved bool to false
	}

	// Put piece back on capture location
	internalBoard[capturePosition.second][capturePosition.first] = lastMove.recreateCapturedPiece();

	// Undo promotion
	if (lastMove.getMoveType() == MoveType::PROMOTE) {
		std::shared_ptr<Piece> depromotedPawn = std::make_shared<Pawn>(Pawn(movedPiece->getColorType(), fromPosition.first, fromPosition.second));
		depromotedPawn->setPosition(fromPosition);
		depromotedPawn->setLastMovedTurn(turnNumber - 1);

		internalBoard[fromPosition.second][fromPosition.first] = depromotedPawn;
	}

	updateCheckStatus();

	turnNumber--;
}

Move Board::getLastMove() {
	return moveHistory.back();
}

bool Board::isColorInCheck(ColorType ct) {
	switch (ct) {
		case ColorType::WHITE:
			return isWhiteInCheck;
			break;
		case ColorType::BLACK:
			return isBlackInCheck;
			break;
		case ColorType::NONE:
			return false;
			break;
	}
	return false;
}

std::vector<Move> Board::getAllValidColorMoves(ColorType ct, bool noCastling) {
	std::vector<Move> moveAccumulator;
	
	for (int row = 0; row < BOARD_Y; row++) {
		for (int col = 0; col < BOARD_X; col++) {
			std::shared_ptr<Piece> pc = getAt(col, row);

			if (pc->getColorType() == ct) {
				std::vector<Move> pieceMoves;

				if (pc->getPieceType() == PieceType::KING) {
					if (noCastling) {
						std::vector<Move> unfilteredKingMoves = kingMovesNoCastle(pc, *this);
						std::vector<Move> accptedKingMoves;

						// Filter king moves for validity
						for (Move& mv : unfilteredKingMoves) {
							enactMove(mv);

							if (isColorInCheck(mv.getFromPieceColor())) {

							}
							else {
								accptedKingMoves.push_back(mv);
							}

							undoLastMove();
						}

						pieceMoves = accptedKingMoves;
					} else {
						pieceMoves = pc->getValidMoves(*this);
					}
				} else {
					pieceMoves = pc->getValidMoves(*this);
				}

				moveAccumulator.insert(moveAccumulator.end(), pieceMoves.begin(), pieceMoves.end());
			}
		}
	}

	return moveAccumulator;
}

bool Board::isCheckmate(ColorType& thisColor) {
	// Check to see if white is in checkmate
	if (isWhiteInCheck) {
		std::vector<Move> whiteMoves = getAllValidColorMoves(ColorType::WHITE);
		if (whiteMoves.size() == 0) {
			thisColor = ColorType::WHITE;
			return true;
		}
	}

	// Check to see if black is in checkmate
	if (isWhiteInCheck) {
		std::vector<Move> blackMoves = getAllValidColorMoves(ColorType::BLACK);
		if (blackMoves.size() == 0) {
			thisColor = ColorType::BLACK;
			return true;
		}
	}

	return false;
}
bool Board::isStalemate(ColorType& thisColor) {
	
	// Check to see if its white's turn and there are no available moves
	if (getTurnNumber() % 2 == 0) {
		std::vector<Move> whiteMoves = getAllValidColorMoves(ColorType::WHITE);
		if ((whiteMoves.size() == 0) && !isColorInCheck(ColorType::WHITE)) {
			thisColor = ColorType::WHITE;
			return true;
		}
	}

	// Check to see if its black's turn and there are no available moves
	if (getTurnNumber() % 2 == 1) {
		std::vector<Move> blackMoves = getAllValidColorMoves(ColorType::BLACK);
		if ((blackMoves.size() == 0) && !isColorInCheck(ColorType::BLACK )) {
			thisColor = ColorType::BLACK;
			return true;
		}
	}

	return false;
}

bool Board::isInsuffiantMaterial(ColorType& thisColor) {
	// View: https://support.chess.com/article/128-what-does-insufficient-mating-material-mean

	bool whiteHasMaterial = true;
	bool blackHasMaterial = true;

	int numberOfWhiteKnights = 0;
	int numberOfWhiteBishops = 0;

	int numberOfBlackKnights = 0;
	int numberOfBlackBishops = 0;

	for (int row = 0; row < BOARD_Y; row++) {
		for (int col = 0; col < BOARD_X; col++) {
			std::shared_ptr<Piece> currentPiece = getAt(col, row);
			if (currentPiece->getPieceType() == PieceType::KNIGHT) {
				if (currentPiece->getColorType() == ColorType::WHITE) {
					numberOfWhiteKnights++;
				} else {
					numberOfBlackKnights++;
				}
			} else if (currentPiece->getPieceType() == PieceType::BISHOP) {
				if (currentPiece->getColorType() == ColorType::WHITE) {
					numberOfWhiteBishops++;
				} else {
					numberOfBlackBishops++;
				}
			} else if (currentPiece->getPieceType() == PieceType::KING) {
				// There is always a king
			} else if (currentPiece->getPieceType() == PieceType::EMPTY_TILE) {
				// Ignore empty tiles
			} else {
				// Any other piece on the board means there is not insuffiant material
				return false;
			}
		}
	}

	// White doesn't have the material
	if ((numberOfWhiteBishops == 1 && numberOfWhiteKnights == 0) || (numberOfWhiteBishops == 0 && numberOfWhiteKnights == 1) || 
		(numberOfWhiteBishops == 0 && numberOfWhiteKnights == 0)) {

		whiteHasMaterial = false;
	}
	// Black doesn't have the material
	if ((numberOfBlackBishops == 1 && numberOfBlackKnights == 0) || (numberOfBlackBishops == 0 && numberOfBlackKnights == 1) || 
		(numberOfBlackBishops == 0 && numberOfBlackKnights == 0)) {

		blackHasMaterial = false;
	}

	return !whiteHasMaterial && !blackHasMaterial;
}


bool Board::validate() {
	// We must check that:
	// There is only one of each king
	// No pawns are on the last rows
	// No king is in check

	int numberOfWhiteKings = 0;
	int numberOfBlackKings = 0;

	bool noPawnsOnBackrow = true;

	for (int row = 0; row < BOARD_Y; row++) {
		for (int col = 0; col < BOARD_X; col++) {
			std::shared_ptr<Piece> pn = getAt(col, row);

			if (pn->getPieceType() == PieceType::PAWN) {
				// Check for pawns on 0th and 7th row
				if ((row == 0) || (row == 7)) {
					noPawnsOnBackrow = false;
				}
			}

			if (pn->getPieceType() == PieceType::KING) {
				if (pn->getColorType() == ColorType::WHITE) {
					numberOfWhiteKings++;
				} else {
					numberOfBlackKings++;
				}
			}
		}
	}

	updateCheckStatus();

	bool whiteCheck = isColorInCheck(ColorType::WHITE);
	bool blackCheck = isColorInCheck(ColorType::BLACK);
	bool correctKings = (numberOfBlackKings == numberOfWhiteKings) && (numberOfBlackKings == 1);

	return !whiteCheck && !blackCheck && correctKings && noPawnsOnBackrow;
}

void Board::drawChars() {
	std::cout << std::endl;
	for (int row = 0; row < BOARD_Y; row++) {
		int boardRowIndex = BOARD_Y - row;
		std::cout << boardRowIndex << " ";
		for (int col = 0; col < BOARD_X; col++) {
			int whiteOrBlackSquare = (col + row) % 2; // 0 is white, 1 is black
			ColorType tileColor = intToColorType(whiteOrBlackSquare);

			std::shared_ptr<Piece> piece = getAt(col, row);

			if (piece->getPieceType() == PieceType::EMPTY_TILE) {
				if (tileColor == ColorType::WHITE) {
					std::cout << " ";
				} else {
					std::cout << "_";
				}
			} else {
				char pieceSymbol = charFromPiece(piece);
				std::cout << pieceSymbol;
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << "  ";
	for (int col = 0; col < BOARD_X; col++) {
		char boardColIndex = 'a' + col;
		std::cout << boardColIndex;
	}
	std::cout << std::endl;
}

void Board::lX11Draw(Xwindow &xw) {
	// This function will draw a lX11 window and to the console


	// This is the console logic
	drawChars();

	// This updates the lX11 graphics
	xw.load(*this);
}





