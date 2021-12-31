#include "piece.h"

void Piece::init() {
	PieceType pType = PieceType::EMPTY_TILE;
	ColorType cType = ColorType::NONE;
	bool hasMoved = false;
	int lastMovedTurn = -1;
	MoveType lastMoveType = MoveType::NONE;
}

PieceType Piece::getPieceType() {
	return pType;
}
ColorType Piece::getColorType() {
	return cType;
}

void Piece::defaultClone(Piece &pc) {
	pc.setLastMovedTurn(lastMovedTurn);
	pc.setLastMoveType(lastMoveType);
}

std::vector<Move> Piece::getValidMoves(Board& brd) {
	std::vector<Move> allPieceMoves = getAllMoves(brd);

	// At the end of each move you must be out of check
	std::vector<Move> validMoves;
	for (Move &mv : allPieceMoves) {

		// Enact the move then check if it puts or keeps the color in check
		brd.enactMove(mv);

		if (brd.isColorInCheck(getColorType())) {
			
		} else {
			// The move doesn't keep or put the color in check
			validMoves.push_back(mv);
		}

		// Undo the move
		brd.undoLastMove();
	}

	return validMoves;
}

/*
	Pawn
*/

Pawn::Pawn(ColorType ct, int x, int y) {
	init();
	pType = PieceType::PAWN;
	cType = ct;
	position = std::pair<int, int>(x, y);

	// Determine the forwards direction based on color
	switch (ct) {
		case ColorType::WHITE:
			forwardsDirection = -1;
			break;
		case ColorType::BLACK:
			forwardsDirection = 1;
			break;
	}
}

std::vector<Move> Pawn::getAllMoves(Board& brd) {
	std::vector<Move> moveCollector;

	int currentX = position.first;
	int currentY = position.second;

	std::shared_ptr<Piece> thisPiece = std::make_shared<Pawn>(*this);

	// Get left diagonal attack
	if (brd.tileExists(currentX - 1, currentY + forwardsDirection)) {
		std::shared_ptr<Piece> attackedPiece = brd.getAt(currentX - 1, currentY + forwardsDirection);

		if (attackedPiece->getColorType() != this->getColorType()) {
			if (attackedPiece->getPieceType() != PieceType::EMPTY_TILE) {

				// Check if on the last row
				if ((currentY + forwardsDirection == 0) || (currentY + forwardsDirection == 7)) {
					// Get all promotion moves
					Move mv1 = Move(MoveType::PROMOTE, thisPiece, attackedPiece, attackedPiece, PieceType::BISHOP);
					Move mv2 = Move(MoveType::PROMOTE, thisPiece, attackedPiece, attackedPiece, PieceType::KNIGHT);
					Move mv3 = Move(MoveType::PROMOTE, thisPiece, attackedPiece, attackedPiece, PieceType::ROOK);
					Move mv4 = Move(MoveType::PROMOTE, thisPiece, attackedPiece, attackedPiece, PieceType::QUEEN);

					moveCollector.push_back(mv1);
					moveCollector.push_back(mv2);
					moveCollector.push_back(mv3);
					moveCollector.push_back(mv4);
				}
				else {
					Move mv = Move(MoveType::CAPTURE, thisPiece, attackedPiece, attackedPiece);

					moveCollector.push_back(mv);
				}

			}
		}
	}

	// Get right diagonal attack
	if (brd.tileExists(currentX + 1, currentY + forwardsDirection)) {
		std::shared_ptr<Piece> attackedPiece = brd.getAt(currentX + 1, currentY + forwardsDirection);

		if (attackedPiece->getColorType() != this->getColorType()) {
			if (attackedPiece->getPieceType() != PieceType::EMPTY_TILE) {

				// Check if on the last row
				if ((currentY + forwardsDirection == 0) || (currentY + forwardsDirection == 7)) {
					// Get all promotion moves
					Move mv1 = Move(MoveType::PROMOTE, thisPiece, attackedPiece, attackedPiece, PieceType::BISHOP);
					Move mv2 = Move(MoveType::PROMOTE, thisPiece, attackedPiece, attackedPiece, PieceType::KNIGHT);
					Move mv3 = Move(MoveType::PROMOTE, thisPiece, attackedPiece, attackedPiece, PieceType::ROOK);
					Move mv4 = Move(MoveType::PROMOTE, thisPiece, attackedPiece, attackedPiece, PieceType::QUEEN);

					moveCollector.push_back(mv1);
					moveCollector.push_back(mv2);
					moveCollector.push_back(mv3);
					moveCollector.push_back(mv4);
				}
				else {
					Move mv = Move(MoveType::CAPTURE, thisPiece, attackedPiece, attackedPiece);

					moveCollector.push_back(mv);
				}

			}
		}
	}

	// Move one forwards if available
	bool canMoveOneForwards = false;
	if (brd.tileExists(currentX, currentY + forwardsDirection)) {
		std::shared_ptr<Piece> attackedPiece = brd.getAt(currentX, currentY + forwardsDirection);

		if (attackedPiece->getPieceType() == PieceType::EMPTY_TILE) {

			// Check if on the last row
			if ((currentY + forwardsDirection == 0) || (currentY + forwardsDirection == 7)) {
				Move mv1 = Move(MoveType::PROMOTE, thisPiece, attackedPiece, attackedPiece, PieceType::BISHOP);
				Move mv2 = Move(MoveType::PROMOTE, thisPiece, attackedPiece, attackedPiece, PieceType::KNIGHT);
				Move mv3 = Move(MoveType::PROMOTE, thisPiece, attackedPiece, attackedPiece, PieceType::ROOK);
				Move mv4 = Move(MoveType::PROMOTE, thisPiece, attackedPiece, attackedPiece, PieceType::QUEEN);

				moveCollector.push_back(mv1);
				moveCollector.push_back(mv2);
				moveCollector.push_back(mv3);
				moveCollector.push_back(mv4);
			} else {
				Move mv = Move(MoveType::MOVE, thisPiece, attackedPiece, attackedPiece);

				moveCollector.push_back(mv);
			}

			canMoveOneForwards = true;
		}
	}

	// Check if pawn is on starting row, if so, it can do the big pawn move
	std::pair<int, int> whiteStartingPosition = std::pair<int, int>(currentX, 6);
	std::pair<int, int> blackStartingPosition = std::pair<int, int>(currentX, 1);
	bool onStartingSquare = false;
	if (getColorType() == ColorType::WHITE && getPosition() == whiteStartingPosition) {
		onStartingSquare = true;
	} else if (getColorType() == ColorType::BLACK && getPosition() == blackStartingPosition) {
		onStartingSquare = true;
	} else {
		onStartingSquare = false;
	}

	// Move two forwards if available
	if ((onStartingSquare) && (canMoveOneForwards) && (brd.tileExists(currentX, currentY + 2 * forwardsDirection))) {
		std::shared_ptr<Piece> attackedPiece = brd.getAt(currentX, currentY + 2 * forwardsDirection);

		if (attackedPiece->getPieceType() == PieceType::EMPTY_TILE) {
			Move mv = Move(MoveType::PAWN_BIGMOVE, thisPiece, attackedPiece, attackedPiece);

			moveCollector.push_back(mv);
		}
	}

	// En passant
	// Check if the last move was a pawning moving by two
	if (brd.getMoveHistory().size() > 0) {

		Move lastMove = brd.getMoveHistory().back();

		if (lastMove.getMoveType() == MoveType::PAWN_BIGMOVE) {

			std::pair<int, int> bigMoveDesitnation = lastMove.getDestinationPosition();

			// Check left square
			std::pair<int, int> leftSquarePair = std::pair<int, int>(currentX - 1, currentY);
			if (leftSquarePair == lastMove.getDestinationPosition()) {
				std::shared_ptr<Piece> attackedPiece = brd.getAt(currentX - 1, currentY);
				std::shared_ptr<Piece> destinationPiece = brd.getAt(currentX - 1, currentY + forwardsDirection);

				Move mv = Move(MoveType::EN_PASSANT, thisPiece, destinationPiece, attackedPiece);
				moveCollector.push_back(mv);
			}

			// Check right square
			std::pair<int, int> rightSquarePair = std::pair<int, int>(currentX + 1, currentY);
			if (rightSquarePair == lastMove.getDestinationPosition()) {
				std::shared_ptr<Piece> attackedPiece = brd.getAt(currentX + 1, currentY);
				std::shared_ptr<Piece> destinationPiece = brd.getAt(currentX + 1, currentY + forwardsDirection);

				Move mv = Move(MoveType::EN_PASSANT, thisPiece, destinationPiece, attackedPiece);
				moveCollector.push_back(mv);
			}
		}
	}

	return moveCollector;
}

std::shared_ptr<Piece> Pawn::clone() {
	Pawn pn = Pawn(this->getColorType(), getPosition().first, getPosition().second);
	defaultClone(pn);

	std::shared_ptr<Piece> p = std::make_shared<Pawn>(pn);
	return p;
}

/*
	Knight
*/

Knight::Knight(ColorType ct, int x, int y) {
	init();
	pType = PieceType::KNIGHT;
	cType = ct;
	position = std::pair<int, int>(x, y);
}

std::vector<Move> Knight::getAllMoves(Board& brd) {
	std::vector<Move> moveCollector;

	int attackMap[5][5] = {
		{0, 1, 0, 1, 0},
		{1, 0, 0, 0, 1},
		{0, 0, 2, 0, 0},
		{1, 0, 0, 0, 1},
		{0, 1, 0, 1, 0}
	};

	for (int rawAttackY = 0; rawAttackY < 5; rawAttackY++) {
		int attackY = rawAttackY - (5 / 2);
		for (int rawAttackX = 0; rawAttackX < 5; rawAttackX++) {
			int attackX = rawAttackX - (5 / 2);

			// Skip over zeroes
			if (attackMap[rawAttackY][rawAttackX] == 0) {
				continue;
			}

			int relativeAttackX = position.first + attackX;
			int relativeAttackY = position.second + attackY;

			// Skip over non-existant tiles
			if (!brd.tileExists(relativeAttackX, relativeAttackY)) {
				continue;
			}

			std::shared_ptr<Piece> attackedPiece = brd.getAt(relativeAttackX, relativeAttackY);
			ColorType attackedColor = attackedPiece->getColorType();

			// Skip over tiles of the same color
			if (attackedColor == this->getColorType()) {
				continue;
			} else {
				std::shared_ptr<Piece> thisPiece = std::make_shared<Knight>(*this);
				// if the color is NONE, then the move type is MoveType::MOVE
				if (attackedColor == ColorType::NONE) {
					Move mv = Move(MoveType::MOVE, thisPiece, attackedPiece, attackedPiece);

					moveCollector.push_back(mv);
				} else {
					// The attackedColor is not NONE and is different from the color of this piece
					// then the MoveType is MoveType::Capture
					Move mv = Move(MoveType::CAPTURE, thisPiece, attackedPiece, attackedPiece);

					moveCollector.push_back(mv);
				}
			}
		}
	}

	return moveCollector;
}

std::shared_ptr<Piece> Knight::clone() {
	Knight pn = Knight(this->getColorType(), getPosition().first, getPosition().second);
	defaultClone(pn);

	std::shared_ptr<Piece> p = std::make_shared<Knight>(pn);
	return p;
}


/*
	Bishop
*/

Bishop::Bishop(ColorType ct, int x, int y) {
	init();
	pType = PieceType::BISHOP;
	cType = ct;
	position = std::pair<int, int>(x, y);
}

std::vector<Move> Bishop::getAllMoves(Board& brd) {
	std::vector<Move> moveCollector;

	int currentX = position.first;
	int currentY = position.second;

	std::shared_ptr<Piece> thisPiece = std::make_shared<Bishop>(*this);

	// TODO: refactor, the code is the same for each diagonal but with + instead of -

	// Calculate diagonals from this piece

	// top left diagonal
	for (int i = 1; i < BOARD_Y; i++) {
		if (brd.tileExists(currentX - i, currentY - i)) {
			std::shared_ptr<Piece> attackedPiece = brd.getAt(currentX - i, currentY - i);

			// Add 'move' moves
			if (attackedPiece->getPieceType() == PieceType::EMPTY_TILE) {
				Move mv = Move(MoveType::MOVE, thisPiece, attackedPiece, attackedPiece);

				moveCollector.push_back(mv);
			} else {
				// This is a capturing move

				if (attackedPiece->getColorType() != this->getColorType()) {
					Move mv = Move(MoveType::CAPTURE, thisPiece, attackedPiece, attackedPiece);

					moveCollector.push_back(mv);
				}

				// We hit a piece, bishop cannot go through pieces, so we break;
				break;
			}
		} else {
			break;
		}
	}

	// top right diagonal
	for (int i = 1; i < BOARD_Y; i++) {
		if (brd.tileExists(currentX + i, currentY - i)) {
			std::shared_ptr<Piece> attackedPiece = brd.getAt(currentX + i, currentY - i);

			// Add 'move' moves
			if (attackedPiece->getPieceType() == PieceType::EMPTY_TILE) {
				Move mv = Move(MoveType::MOVE, thisPiece, attackedPiece, attackedPiece);

				moveCollector.push_back(mv);
			} else {
				// This is a capturing move

				if (attackedPiece->getColorType() != this->getColorType()) {
					Move mv = Move(MoveType::CAPTURE, thisPiece, attackedPiece, attackedPiece);

					moveCollector.push_back(mv);
				}

				// We hit a piece, bishop cannot go through pieces, so we break;
				break;
			}
		} else {
			break;
		}
	}

	// bottom left diagonal
	for (int i = 1; i < BOARD_Y; i++) {
		if (brd.tileExists(currentX - i, currentY + i)) {
			std::shared_ptr<Piece> attackedPiece = brd.getAt(currentX - i, currentY + i);

			// Add 'move' moves
			if (attackedPiece->getPieceType() == PieceType::EMPTY_TILE) {
				Move mv = Move(MoveType::MOVE, thisPiece, attackedPiece, attackedPiece);

				moveCollector.push_back(mv);
			} else {
				// This is a capturing move

				if (attackedPiece->getColorType() != this->getColorType()) {
					Move mv = Move(MoveType::CAPTURE, thisPiece, attackedPiece, attackedPiece);

					moveCollector.push_back(mv);
				}

				// We hit a piece, bishop cannot go through pieces, so we break;
				break;
			}
		} else {
			break;
		}
	}

	// bottom right diagonal
	for (int i = 1; i < BOARD_Y; i++) {
		if (brd.tileExists(currentX + i, currentY + i)) {
			std::shared_ptr<Piece> attackedPiece = brd.getAt(currentX + i, currentY + i);

			// Add 'move' moves
			if (attackedPiece->getPieceType() == PieceType::EMPTY_TILE) {
				Move mv = Move(MoveType::MOVE, thisPiece, attackedPiece, attackedPiece);

				moveCollector.push_back(mv);
			} else {
				// This is a capturing move, bishops cannot go through other pieces

				if (attackedPiece->getColorType() != this->getColorType()) {
					Move mv = Move(MoveType::CAPTURE, thisPiece, attackedPiece, attackedPiece);

					moveCollector.push_back(mv);
				}

				// We hit a piece, bishop cannot go through pieces, so we break;
				break;
			}
		} else {
			break;
		}
	}

	return moveCollector;
}

std::shared_ptr<Piece> Bishop::clone() {
	Bishop pn = Bishop(this->getColorType(), getPosition().first, getPosition().second);
	defaultClone(pn);

	std::shared_ptr<Piece> p = std::make_shared<Bishop>(pn);
	return p;
}

/*
	Rook
*/

Rook::Rook(ColorType ct, int x, int y) {
	init();
	pType = PieceType::ROOK;
	cType = ct;
	position = std::pair<int, int>(x, y);
}

std::vector<Move> Rook::getAllMoves(Board& brd) {
	std::vector<Move> moveCollector;

	int currentX = position.first;
	int currentY = position.second;

	std::shared_ptr<Piece> thisPiece = std::make_shared<Rook>(*this);

	// TODO: refactor, the code is the same for each horizontal/vertical but with + instead of -

	// Get left horizontal
	for (int i = 1; i < BOARD_X; i++) {
		if (brd.tileExists(currentX - i, currentY)) {
			std::shared_ptr<Piece> attackedPiece = brd.getAt(currentX - i, currentY);

			if (attackedPiece->getPieceType() == PieceType::EMPTY_TILE) {
				Move mv = Move(MoveType::MOVE, thisPiece, attackedPiece, attackedPiece);

				moveCollector.push_back(mv);
			} else {
				// We hit a piece, so this could be a capturing move

				if (attackedPiece->getColorType() != this->getColorType()) {
					Move mv = Move(MoveType::CAPTURE, thisPiece, attackedPiece, attackedPiece);

					moveCollector.push_back(mv);
				}

				// We hit a piece, rooks cannot go through pieces, so we break;
				break;
			}
		} else {
			break;
		}
	}

	// Get right horizontal
	for (int i = 1; i < BOARD_X; i++) {
		if (brd.tileExists(currentX + i, currentY)) {
			std::shared_ptr<Piece> attackedPiece = brd.getAt(currentX + i, currentY);

			if (attackedPiece->getPieceType() == PieceType::EMPTY_TILE) {
				Move mv = Move(MoveType::MOVE, thisPiece, attackedPiece, attackedPiece);

				moveCollector.push_back(mv);
			} else {
				// We hit a piece, so this could be a capturing move

				if (attackedPiece->getColorType() != this->getColorType()) {
					Move mv = Move(MoveType::CAPTURE, thisPiece, attackedPiece, attackedPiece);

					moveCollector.push_back(mv);
				}

				// We hit a piece, rooks cannot go through pieces, so we break;
				break;
			}
		} else {
			break;
		}
	}

	// Get up vertical
	for (int i = 1; i < BOARD_X; i++) {
		if (brd.tileExists(currentX, currentY - i)) {
			std::shared_ptr<Piece> attackedPiece = brd.getAt(currentX, currentY - i);

			if (attackedPiece->getPieceType() == PieceType::EMPTY_TILE) {
				Move mv = Move(MoveType::MOVE, thisPiece, attackedPiece, attackedPiece);

				moveCollector.push_back(mv);
			} else {
				// We hit a piece, so this could be a capturing move
				if (attackedPiece->getColorType() != this->getColorType()) {
					Move mv = Move(MoveType::CAPTURE, thisPiece, attackedPiece, attackedPiece);

					moveCollector.push_back(mv);
				}
				// We hit a piece, rooks cannot go through pieces, so we break;

				break;
			}
		} else {
			break;
		}
	}

	// Get down vertical
	for (int i = 1; i < BOARD_X; i++) {
		if (brd.tileExists(currentX, currentY + i)) {
			std::shared_ptr<Piece> attackedPiece = brd.getAt(currentX, currentY + i);

			if (attackedPiece->getPieceType() == PieceType::EMPTY_TILE) {
				Move mv = Move(MoveType::MOVE, thisPiece, attackedPiece, attackedPiece);

				moveCollector.push_back(mv);
			} else {
				// We hit a piece, so this could be a capturing move
				if (attackedPiece->getColorType() != this->getColorType()) {
					Move mv = Move(MoveType::CAPTURE, thisPiece, attackedPiece, attackedPiece);

					moveCollector.push_back(mv);
				}
				// We hit a piece, rooks cannot go through pieces, so we break;

				break;
			}
		} else {
			break;
		}
	}


	return moveCollector;
}

std::shared_ptr<Piece> Rook::clone() {
	Rook pn = Rook(this->getColorType(), getPosition().first, getPosition().second);
	defaultClone(pn);

	std::shared_ptr<Piece> p = std::make_shared<Rook>(pn);
	return p;
}

/*
	Queen
*/

Queen::Queen(ColorType ct, int x, int y) {
	init();
	pType = PieceType::QUEEN;
	cType = ct;
	position = std::pair<int, int>(x, y);
}

std::vector<Move> Queen::getAllMoves(Board& brd) {
	std::vector<Move> moveCollector;

	int currentX = position.first;
	int currentY = position.second;

	std::shared_ptr<Piece> thisPiece = std::make_shared<Queen>(*this);

	// Create temporary bishop and rook then combine the possible moves to get the Queen's moves
	Bishop tmpBishop = Bishop(this->getColorType(), currentX, currentY);
	Rook tmpRook = Rook(this->getColorType(), currentX, currentY);

	std::vector<Move> diagonalMoves = tmpBishop.getAllMoves(brd);
	std::vector<Move> straightMoves = tmpRook.getAllMoves(brd);

	// Add moves to moveCollector
	moveCollector.insert(moveCollector.end(), diagonalMoves.begin(), diagonalMoves.end());
	moveCollector.insert(moveCollector.end(), straightMoves.begin(), straightMoves.end());

	return moveCollector;
}

std::shared_ptr<Piece> Queen::clone() {
	Queen pn = Queen(this->getColorType(), getPosition().first, getPosition().second);
	defaultClone(pn);

	std::shared_ptr<Piece> p = std::make_shared<Queen>(pn);
	return p;
}

/*
	King
*/

King::King(ColorType ct, int x, int y) {
	init();
	pType = PieceType::KING;
	cType = ct;
	position = std::pair<int, int>(x, y);
}

// King's non castling moves
std::vector<Move> kingMovesNoCastle(std::shared_ptr<Piece> king, Board& brd) {
	std::vector<Move> moveCollector;

	std::pair<int, int> position = king->getPosition();

	int currentX = position.first;
	int currentY = position.second;

	std::shared_ptr<Piece> thisPiece = king;

	// Use attack map to simply logic, 1 is attacked, 0 is not attacked
	char attackMap[3][3] = {
		{1, 1, 1},
		{1, 0, 1},
		{1, 1, 1}
	};

	// Move and attacks
	for (int i = 0; i < 3; i++) {
		int relativeX = i - 1;
		for (int j = 0; j < 3; j++) {
			int relativeY = j - 1;

			// Skip non-attacked square
			if (attackMap[i][j] == 0) {
				continue;
			}

			// Skip tiles that do not exist
			if (!brd.tileExists(currentX + relativeX, currentY + relativeY)) {
				continue;
			}

			std::shared_ptr<Piece> attackedPiece = brd.getAt(currentX + relativeX, currentY + relativeY);

			// Determine whether the move is a capture or a move
			if (attackedPiece->getPieceType() == PieceType::EMPTY_TILE) {
				Move mv = Move(MoveType::MOVE, thisPiece, attackedPiece, attackedPiece);

				moveCollector.push_back(mv);
			}
			else {

				// Is touching another piece, determine if can capture
				if (attackedPiece->getColorType() != thisPiece->getColorType()) {
					Move mv = Move(MoveType::CAPTURE, thisPiece, attackedPiece, attackedPiece);

					moveCollector.push_back(mv);
				}
			}
		}
	}

	return moveCollector;
}

std::vector<Move> King::getAllMoves(Board& brd) {
	std::vector<Move> moveCollector;

	int currentX = position.first;
	int currentY = position.second;

	std::shared_ptr<Piece> thisPiece = std::make_shared<King>(*this);

	std::vector<Move> noCastle = kingMovesNoCastle(thisPiece, brd);

	moveCollector.insert(moveCollector.end(), noCastle.begin(), noCastle.end());

	// TEMP only allow white to castle for now
	bool tempBool = true;//getColorType() == ColorType::WHITE;

	// Determine if the king is on the right square
	std::pair<int, int> whiteKingPosition = std::pair<int, int>(4, 7);
	std::pair<int, int> blackKingPosition = std::pair<int, int>(4, 0);
	std::pair<int, int> kingPositionToCastle = std::pair<int, int>(0, 0);
	if (getColorType() == ColorType::WHITE) {
		kingPositionToCastle = whiteKingPosition;
	} else {
		kingPositionToCastle = blackKingPosition;
	}

	// stop the king from castling himself into the void
	bool isOnGoodLocationKing = getPosition() == kingPositionToCastle;

	std::vector<Move> playedMoves = brd.getMoveHistory();
	bool kingHasMoved = false;
	for (Move &mv : playedMoves) {
		if (mv.getFromPieceType() == PieceType::KING && mv.getFromPieceColor() == getColorType()) {
			kingHasMoved = true;
			break;
		}
	}

	// Get castling moves
	// Can't castle if the king has moved or the king is in check
	if (!kingHasMoved && !brd.isColorInCheck(getColorType()) && tempBool && isOnGoodLocationKing) {

		std::vector<Move> opponentMoves = brd.getAllColorMoves(oppositeColor(getColorType()));
		
		// Kingside castling
		{
			// King side castling
			std::pair<int, int> kingCoords = std::pair<int, int>(0, 0);
			std::pair<int, int> rookCoords = std::pair<int, int>(0, 0);
			std::pair<int, int> leftTileCoords = std::pair<int, int>(0, 0);
			std::pair<int, int> rightTileCoords = std::pair<int, int>(0, 0);

			// Change coords pending on color
			if (getColorType() == ColorType::WHITE) {
				// White
				kingCoords = std::pair<int, int>(4, 7);
				rookCoords = std::pair<int, int>(7, 7);
				leftTileCoords = std::pair<int, int>(5, 7);
				rightTileCoords = std::pair<int, int>(6, 7);
			}
			else {
				// Black
				kingCoords = std::pair<int, int>(4, 0);
				rookCoords = std::pair<int, int>(7, 0);
				leftTileCoords = std::pair<int, int>(5, 0);
				rightTileCoords = std::pair<int, int>(6, 0);
			}

			// The rook
			std::shared_ptr<Piece> theRook = brd.getAt(rookCoords.first, rookCoords.second);

			// Determine if the rook has moved
			bool hasRookMoved = false;
			for (Move &mv : playedMoves) {
				if (mv.getFromPieceType() == PieceType::ROOK && mv.getFromPosition() == rookCoords && mv.getFromPieceColor() == getColorType()) {
					hasRookMoved = true;
					break;
				}
			}

			bool isOnGoodLocationRook = theRook->getPosition() == rookCoords && theRook->getPieceType() == PieceType::ROOK;

			if (!hasRookMoved && isOnGoodLocationRook) {
				// The two empty tiles

				std::shared_ptr<Piece> leftTile = brd.getAt(leftTileCoords.first, leftTileCoords.second);
				std::shared_ptr<Piece> rightTile = brd.getAt(rightTileCoords.first, rightTileCoords.second);

				bool canCastle = true;

				// First check if can castle
				if (leftTile->getPieceType() != PieceType::EMPTY_TILE) {
					canCastle = false;
				}

				if (rightTile->getPieceType() != PieceType::EMPTY_TILE) {
					canCastle = false;
				}

				// Second check if can castle
				if (canCastle) {
					// Check if the tile are attacked
					for (Move& mv : opponentMoves) {
						// The empty tiles are attacked
						if (mv.getDestinationPosition() == leftTile->getPosition()) {
							canCastle = false;
							break;
						}
						if (mv.getDestinationPosition() == rightTile->getPosition()) {
							canCastle = false;
							break;
						}
					}
				}

				// We can castle kingside
				if (canCastle) {
					Move mv = Move(MoveType::CASTLE_KING, thisPiece, leftTile, rightTile);

					moveCollector.push_back(mv);
				}
			}
		}

		// Queenside castling
		{
			int backrow = 7;
			if (getColorType() == ColorType::WHITE) {
				// White has backrow of 7
				backrow = 7;
			} else {
				// Black has backrow of 0
				backrow = 0;
			}

			std::pair<int, int> oldRookPosition    = std::pair<int, int>(0, backrow);
			std::pair<int, int> leftTilePosition   = std::pair<int, int>(1, backrow);
			std::pair<int, int> middleTilePosition = std::pair<int, int>(2, backrow);
			std::pair<int, int> rightTilePosition  = std::pair<int, int>(3, backrow);
			std::pair<int, int> oldKingPosition    = std::pair<int, int>(4, backrow);

			// The rook
			std::shared_ptr<Piece> theRook = brd.getAt(oldRookPosition.first, oldRookPosition.second);

			// Determine if the rook has moved
			bool hasRookMoved = false;
			for (Move &mv : playedMoves) {
				if (mv.getFromPieceType() == PieceType::ROOK && mv.getFromPosition() == oldRookPosition && mv.getFromPieceColor() == getColorType()) {
					hasRookMoved = true;
					break;
				}
			}

			bool isOnGoodLocationRook = theRook->getPosition() == oldRookPosition && theRook->getPieceType() == PieceType::ROOK;

			if (!hasRookMoved && isOnGoodLocationRook) {
				// Check if the tile are empty and not attacked

				bool canCastle = true;
				std::shared_ptr<Piece> leftTile = brd.getAt(leftTilePosition.first, leftTilePosition.second);
				std::shared_ptr<Piece> middleTile = brd.getAt(middleTilePosition.first, middleTilePosition.second);
				std::shared_ptr<Piece> rightTile = brd.getAt(rightTilePosition.first, rightTilePosition.second);

				// Check that they are empty tiles
				if (leftTile->getPieceType() != PieceType::EMPTY_TILE) {
					canCastle = false;
				}
				if (middleTile->getPieceType() != PieceType::EMPTY_TILE) {
					canCastle = false;
				}
				if (rightTile->getPieceType() != PieceType::EMPTY_TILE) {
					canCastle = false;
				}

				if (canCastle) {
					// Check that the empty tiles are not attacked

					for (Move& mv : opponentMoves) {
						if (mv.getDestinationPosition() == leftTile->getPosition()) {
							canCastle = false;
							break;
						}
						if (mv.getDestinationPosition() == middleTile->getPosition()) {
							canCastle = false;
							break;
						}
						if (mv.getDestinationPosition() == rightTile->getPosition()) {
							canCastle = false;
							break;
						}
					}
				}

				// Then we can castle queenside as white
				if (canCastle) {
					Move mv = Move(MoveType::CASTLE_QUEEN, thisPiece, rightTile, middleTile);

					moveCollector.push_back(mv);
				}
			}
		}

	}


	return moveCollector;
}


std::shared_ptr<Piece> King::clone() {
	King pn = King(this->getColorType(), getPosition().first, getPosition().second);
	defaultClone(pn);

	std::shared_ptr<Piece> p = std::make_shared<King>(pn);
	return p;
}

/*
	EmptyTile
*/

EmptyTile::EmptyTile(ColorType ct, int x, int y) {
	init();
	pType = PieceType::EMPTY_TILE;
	cType = ct;
	position = std::pair<int, int>(x, y);
}

std::vector<Move> EmptyTile::getAllMoves(Board& brd) {
	std::vector<Move> moveCollector;

	// EmptyTile do not have moves

	return moveCollector;
}

std::shared_ptr<Piece> EmptyTile::clone() {
	EmptyTile pn = EmptyTile(this->getColorType(), getPosition().first, getPosition().second);
	defaultClone(pn);

	std::shared_ptr<Piece> p = std::make_shared<EmptyTile>(pn);
	return p;
}

//
