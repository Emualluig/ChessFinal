#ifndef _HEADER_AGENT_H_
#define _HEADER_AGENT_H_

#include <random>

#include "move.h"
#include "board.h"

class Board;
enum class ColorType;

enum class AgentType { PLAYER, BOT };

class Agent {
	

	protected:
		AgentType aType;
		int botLevel = -1; // -1 is human
		bool HAS_STOCKFISH = false;

	public:
		int level() {
			return botLevel;
		}
		AgentType getType() {
			return aType;
		}
		bool isBot() {
			return aType == AgentType::BOT;
		}
		// Determine the move to play
		virtual Move getMove(Board &board, ColorType color) = 0;
		// Determine if the current color is in checkmate
		bool isCheckmated(Board& board, ColorType color);
};

// The player class is essentially used, if I had more time I would move a lot of logic from main.cc to this class
class Player : public Agent {
	public:
		Player();
		Move getMove(Board& board, ColorType color);
};

class Bot : public Agent {

	int getRandomInt(int lowerBound, int upperBound);
	public:
		Bot(int level);
		Move getMove(Board& board, ColorType color);
};


#endif // !_HEADER_AGENT_H_
