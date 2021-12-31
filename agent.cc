#include "agent.h"

bool Agent::isCheckmated(Board& board, ColorType color) {
	std::vector<Move> possibleMoves = board.getAllValidColorMoves(color, false);

	// Checkmate is when a color that is in check also has no valid moves
	if (board.isColorInCheck(color) && (possibleMoves.size() == 0)) {
		return true;
	} else {
		return false;
	}
}

Player::Player() {
	aType = AgentType::PLAYER;
}

// This method is never used but it must be defined anywas
Move Player::getMove(Board& board, ColorType color) {
	return Move(MoveType::NONE, board.getAt(0,0), board.getAt(0,0), board.getAt(0,0));
}

Bot::Bot(int level) {
	aType = AgentType::BOT;
	botLevel = level;
	HAS_STOCKFISH = false;
}

// Generate a random number on range [lowerBound, upperBound] inclusive
int Bot::getRandomInt(int lowerBound, int upperBound) {

	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_int_distribution<int> distr(lowerBound, upperBound);
	int value = distr(generator);

	return value;
}

Move Bot::getMove(Board& board, ColorType color) {
	// Get all moves that this bot could play
	std::vector<Move> possibleMoves = board.getAllValidColorMoves(color, false);

	// Create move with rankings
	std::vector<std::pair<int, Move>> moveScores;

	// Assign a score to moves
	// The score is generated depending on the level of the bot/computer
	// Higher score is better
	if (level() == 1) {

		// All moves have equal score
		for (Move &mv : possibleMoves) {
			moveScores.push_back(std::pair<int, Move>(1, mv));
		}

	} else if (level() == 2) {
		
		// Have a preference for checking and captures
		for (Move &mv : possibleMoves) {
			int curScore = 1;
			if (mv.isChecking()) {
				curScore += 2;
			}

			if (mv.getMoveType() == MoveType::CAPTURE) {
				curScore += 2;
			}

			moveScores.push_back(std::pair<int, Move>(curScore, mv));
		}

	} else if (level() == 3) {
		
		// Get enemy moves
		ColorType enemyColor = oppositeColor(color);
		std::vector<Move> enemyMoves = board.getAllValidColorMoves(enemyColor);

		// Determine highest value attack, avoid that capture if doesn't have equivalent value capture
		int highestValueUnderAttack = 0;
		for (Move &mv : enemyMoves) {
			int attackedValue = getPiecePoints(mv.getCapturePieceType());
			if (attackedValue > highestValueUnderAttack) {
				highestValueUnderAttack = attackedValue;
			}
		}

		// Loop through bot's possible moves
		for (Move &mv : possibleMoves) {
			int curScore = 1;

			// Prefer checking
			if (mv.isChecking()) {
				curScore += 3;
			}

			// Prefer captures
			if (mv.getMoveType() == MoveType::CAPTURE) {
				curScore += 2;
			}

			// If this position is attacked by enemies, remove score (avoiding capture)
			// This is avoiding capture
			int thisPieceValue = getPiecePoints(mv.getFromPieceType());
			for (Move &enemyMv : enemyMoves) {
				if (enemyMv.getCapturePosition() == mv.getCapturePosition()) {
					curScore -= 1 * thisPieceValue;
					break;
				}
			}

			// Perfer moves near the center
			const float middleTileX = (BOARD_X + 0.0) / 2.0;
			const float middleTileY = (BOARD_Y + 0.0) / 2.0;
			float xDist = std::abs(mv.getCapturePosition().first - middleTileX);
			float yDist = std::abs(mv.getCapturePosition().second - middleTileY);
			float distToCenterRaw = std::pow(xDist, 2.0) + std::pow(yDist, 2.0);
			float distToCenter = std::sqrt(distToCenterRaw);
			if (distToCenter <= 2.0) {
				curScore += 1;

				// Prefer middle pawn big moves
				if (mv.getMoveType() == MoveType::PAWN_BIGMOVE) {
					curScore += 1;
				}
			}

			// Avoid capture of high value pieces
			int pieceValue = getPiecePoints(mv.getFromPieceType());
			if (pieceValue == highestValueUnderAttack) {
				curScore += 2;
			}

			moveScores.push_back(std::pair<int, Move>(curScore, mv));
		}

	} else if (level() == 4) {
		ColorType enemyColor = oppositeColor(color);
		std::vector<Move> enemyMoves = board.getAllValidColorMoves(enemyColor);

		// Calculate simple piece score
		// Pawn: 1
		// Knight: 3
		// Bishop: 3
		// Rook: 5
		// Queen: 9
		// King: 0
		int blackPieceScore = 0;
		int whitePieceScore = 0;

		// TODO: preference for trading when having higher piecescore and aversion when having less piecescore
		for (int row = 0; row < BOARD_Y; row++) {
			for (int col = 0; col < BOARD_X; col++) {
				std::shared_ptr<Piece> pn = board.getAt(col, row);

				// Calculate cumulative black and white piece scores
				if (pn->getColorType() == ColorType::WHITE) {
					whitePieceScore += getPiecePoints(pn->getPieceType());
				} else if (pn->getColorType() == ColorType::BLACK) {
					blackPieceScore += getPiecePoints(pn->getPieceType());
				}
				
			}
		}

		// Determine if current agent has more or less piece score
		bool hasMoreScore = color == ColorType::WHITE ? (whitePieceScore > blackPieceScore) : (blackPieceScore > whitePieceScore);

		// Determine highest value attack, avoid that capture if doesn't have equivalent value capture
		int highestValueUnderAttack = 0;
		for (Move& mv : enemyMoves) {
			int attackedValue = getPiecePoints(mv.getCapturePieceType());
			if (attackedValue > highestValueUnderAttack) {
				highestValueUnderAttack = attackedValue;
			}
		}

		// Loop over possible moves
		for (Move& mv : possibleMoves) {
			int curScore = 1;

			if (mv.isChecking()) {
				curScore += 3;
			}

			if (mv.getMoveType() == MoveType::CAPTURE) {
				curScore += 2;
			}

			// If this position is attacked by enemies, remove score (avoiding capture)
			// This is avoiding capture
			int thisPieceValue = getPiecePoints(mv.getFromPieceType());
			for (Move& enemyMv : enemyMoves) {
				if (enemyMv.getCapturePosition() == mv.getCapturePosition()) {
					curScore -= 1 * thisPieceValue;
					break;
				}
			}

			// Add score if promote, add pereferce for higher value promotes
			if (mv.getMoveType() == MoveType::PROMOTE) {
				curScore += getPiecePoints(mv.getPromoteType()) / 2;
			}

			// Capture pieces with higher values
			curScore += getPiecePoints(mv.getCapturePieceType()) / 3;

			// Avoid moving king
			if (mv.getFromPieceType() == PieceType::KING) {
				curScore -= 2;
			}

			// Avoid capturing on pieces that are supported by another enemy piece
			{
				board.enactMove(mv);

				// Check if this piece can be recaptured
				std::vector<Move> possibleRecaptures = board.getAllValidColorMoves(enemyColor);

				for (Move &potentialReplies : possibleRecaptures) {
					if (potentialReplies.getCapturePosition() == mv.getCapturePosition()) {
						// Remove more points for higher value recaptures
						curScore -= getPiecePoints(mv.getFromPieceType()); // It hurts more to loss
						curScore += getPiecePoints(mv.getCapturePieceType()) / 2; // It gains less to get
						break;
					}
				}

				board.undoLastMove();
			}

			// Prefer not moving the king at start of game
			if (mv.getFromPieceType() == PieceType::KING && (board.getTurnNumber() < 20)) {
				curScore -= 5;
			}

			// Prefer castling over everything
			// TODO: prefer queenside have having a point advantage, prefer kingside elsewise
			if (mv.getMoveType() == MoveType::CASTLE_KING || mv.getMoveType() == MoveType::CASTLE_QUEEN) {
				curScore += 9999;
			}

			// Prefer moves near the center
			const float middleTileX = (BOARD_X + 0.0) / 2.0;
			const float middleTileY = (BOARD_Y + 0.0) / 2.0;
			float xDist = std::abs(mv.getCapturePosition().first - middleTileX);
			float yDist = std::abs(mv.getCapturePosition().second - middleTileY);
			float distToCenterRaw = std::pow(xDist, 2.0) + std::pow(yDist, 2.0);
			float distToCenter = std::sqrt(distToCenterRaw);
			if (distToCenter <= 1.5) {
				curScore += 1;

				// Prefer middle pawn big moves
				if (mv.getMoveType() == MoveType::PAWN_BIGMOVE) {
					curScore += 2;
				}
			}

			// Avoid capture of high value pieces
			int pieceValue = getPiecePoints(mv.getFromPieceType());
			if (pieceValue == highestValueUnderAttack) {
				curScore += 2;
			}

			moveScores.push_back(std::pair<int, Move>(curScore, mv));
		}

	} if (HAS_STOCKFISH) {
		// TODO: implement comminication with stockfish .exe via Boost.process

	} else {
		// ????
	}

	// Determine highest score moves
	int highestScore = moveScores[0].first;
	for (std::pair<int, Move> &scoreMovePair : moveScores) {
		if (scoreMovePair.first > highestScore) {
			highestScore = scoreMovePair.first;
		}
	}

	// Get vector of only the highest scoring moves
	std::vector<Move> highestScoreMoves;
	for (std::pair<int, Move> &scoreMovePair : moveScores) {
		if (scoreMovePair.first == highestScore) {
			highestScoreMoves.push_back(scoreMovePair.second);
		}
	}

	// Choose one of the highest scoring moves at random
	int randomIndex = getRandomInt(0, highestScoreMoves.size() - 1);
	Move mv = highestScoreMoves[randomIndex];

	return mv;
}



