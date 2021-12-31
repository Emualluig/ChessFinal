

#include <memory>
#include <vector>
#include <string>

#include "move.h"
#include "board.h"
#include "piece.h"
#include "agent.h"

#include "window.h"

int main() {

	const char defaultBoard[8][8] = {
				{'r','n','b','q','k','b','n','r'},
				{'p','p','p','p','p','p','p','p'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'P','P','P','P','P','P','P','P'},
				{'R','N','B','Q','K','B','N','R'}
	};
	const bool DEBUG_OVERRIDE = false;
	const char customBoard[8][8] = {
				{'r','n','b','q','k','b','n','r'},
				{'p','p','p','p','p','p','p','p'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'P','P','P','P','P','P','P','P'},
				{'R','_','_','_','K','_','_','R'}
	};
	char blankBoard[8][8] = {
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'}
	};
	char setupBoard[8][8] = {
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'},
				{'_','_','_','_','_','_','_','_'}
	};

	int whiteScore = 0;
	int blackScore = 0;

	bool useSetupBoard = false;
	int setupTurn = 0;
	while (true) {
		std::string command;

		std::cin >> command;
		if (std::cin.eof()) {
			//std::cout << "[MAINLOOP] exiting program" << std::endl;
			std::cin.clear();
			break;
		} else if (command == "game") {

			// Determine which to board to use and which player to start
			int turnToUse = 0;
			char boardToUse[8][8];
			if (useSetupBoard == true) {
				for (int row = 0; row < 8; row++) {
					for (int col = 0; col < 8; col++) {
						boardToUse[row][col] = setupBoard[row][col];
					}
				}
				// Set turn to correct turn
				turnToUse = setupTurn;
			} else {
				for (int row = 0; row < 8; row++) {
					for (int col = 0; col < 8; col++) {
						boardToUse[row][col] = defaultBoard[row][col];
					}
				}
				// Set turn to correct turn
				turnToUse = 0;
			}

			if (DEBUG_OVERRIDE) {
				for (int row = 0; row < 8; row++) {
					for (int col = 0; col < 8; col++) {
						boardToUse[row][col] = customBoard[row][col];
					}
				}
			}

			Board board = Board(boardToUse);
			board.setTurnNumber(turnToUse);

			Xwindow xw{board};

			// Read in the agents
			std::string agentStr1;
			std::string agentStr2;

			std::cin >> agentStr1;
			if (std::cin.eof() || std::cin.fail()) {
				std::cout << "[GAME ERROR] failed to read first argument" << std::endl;
				break;
			}

			std::cin >> agentStr2;
			if (std::cin.eof() || std::cin.fail()) {
				std::cout << "[GAME ERROR] failed to read second argument" << std::endl;
				break;
			}

			// Determine the agent type
			std::shared_ptr<Agent> ag1;
			if (agentStr1 == "human") {
				ag1 = std::make_shared<Player>(Player());
			} else if (agentStr1 == "computer1") {
				ag1 = std::make_shared<Bot>(Bot(1));
			} else if (agentStr1 == "computer2") {
				ag1 = std::make_shared<Bot>(Bot(2));
			} else if (agentStr1 == "computer3") {
				ag1 = std::make_shared<Bot>(Bot(3));
			} else if (agentStr1 == "computer4") {
				ag1 = std::make_shared<Bot>(Bot(4));
			} else {
				std::cout << "[GAME ERROR] unknown first player type" << std::endl;
				break;
			}
			std::shared_ptr<Agent> ag2;
			if (agentStr2 == "human") {
				ag2 = std::make_shared<Player>(Player());
			} else if (agentStr2 == "computer1") {
				ag2 = std::make_shared<Bot>(Bot(1));
			} else if (agentStr2 == "computer2") {
				ag2 = std::make_shared<Bot>(Bot(2));
			} else if (agentStr2 == "computer3") {
				ag2 = std::make_shared<Bot>(Bot(3));
			} else if (agentStr2 == "computer4") {
				ag2 = std::make_shared<Bot>(Bot(4));
			} else {
				std::cout << "[GAME ERROR] unknown first player type" << std::endl;
				break;
			}

			std::shared_ptr<Agent> agents[2] = {ag1, ag2};

			while (true) {
				board.lX11Draw(xw);

				int activeAgentId = board.getTurnNumber() % 2;
				std::shared_ptr<Agent> activeAgent = agents[activeAgentId];
				ColorType currentAgentColor = intToColorType(activeAgentId);

				// Detect checkmate
				ColorType checkmateColor = ColorType::NONE;
				if (board.isCheckmate(checkmateColor)) {
					//std::cout << "[GAMELOOP] CHECKMATE DETECTED V1" << std::endl;

					// Add to game score
					if (checkmateColor == ColorType::WHITE) {
						std::cout << "Checkmate! Black wins!" << std::endl;
						blackScore++;
					}
					else if (checkmateColor == ColorType::BLACK) {
						std::cout << "Checkmate! White wins!" << std::endl;
						whiteScore++;
					}

					break;
				}

				// More detect checkmate
				if (activeAgent->isCheckmated(board, currentAgentColor)) {
					//std::cout << "[GAMELOOP] CHECKMATE DETECTED V2" << std::endl;

					// Add to game score
					if (currentAgentColor == ColorType::WHITE) {
						std::cout << "Checkmate! Black wins!" << std::endl;
						blackScore++;
					} else if (currentAgentColor == ColorType::BLACK) {
						std::cout << "Checkmate! White wins!" << std::endl;
						whiteScore++;
					}

					break;
				}

				// Detect stalemate
				ColorType stalemateColor = ColorType::NONE;
				if (board.isStalemate(stalemateColor)) {
					//std::cout << "[GAMELOOP] STALEMATE DETECTED" << std::endl;
					std::cout << "Stalemate!" << std::endl;
					break;
				}

				// Detect insuffiant material
				ColorType noMatColor = ColorType::NONE;
				if (board.isInsuffiantMaterial(noMatColor)) {
					//std::cout << "[GAMELOOP] INSUFFIANT MATERIAL" << std::endl;
					std::cout << "Insufficient Material!" << std::endl;
					break;
				}

				// Print check alerts
				if (board.isColorInCheck(ColorType::WHITE)) {
					std::cout << "White is in check!" << std::endl;
				} else if (board.isColorInCheck(ColorType::BLACK)) {
					std::cout << "Black is in check!" << std::endl;
				}

				std::string gameCommand;
				std::cin >> gameCommand;
				//gameCommand = "move";
				if (std::cin.eof() || std::cin.fail()) {
					//std::cout << "[GAMELOOP] exit" << std::endl;
					std::cin.clear();
					break;
				} else if (gameCommand == "resign") {
					if (currentAgentColor == ColorType::WHITE) {
						std::cout << "Black wins!" << std::endl;
						blackScore++;
					} else if (currentAgentColor == ColorType::BLACK) {
						std::cout << "White wins!" << std::endl;
						whiteScore++;
					}
					break;
				} else if (gameCommand == "move") {

					// The command move is needed for the computer
					if (activeAgent->getType() == AgentType::BOT) {
						Move selectedMove = activeAgent->getMove(board, currentAgentColor);
#if false
						// TMP
						if (selectedMove.getMoveType() == MoveType::EN_PASSANT) {
							std::cout << "[TMP] PLAYED EN PASSANT" << std::endl;
						} else if (selectedMove.getMoveType() == MoveType::CAPTURE && selectedMove.getFromPieceType() == PieceType::PAWN) {
							std::cout << "[TMP] CAPTURED: (" << selectedMove.getCapturePosition().first << ", " << selectedMove.getCapturePosition().second << ") : (" << selectedMove.getDestinationPosition().first << ", " <<  selectedMove.getDestinationPosition().second << ")" << std::endl;
						}
						std::cout << "[TMP] MOVE TYPE" << (int)selectedMove.getMoveType() << std::endl;
#endif

						board.enactMove(selectedMove);
					} else {
						
						// Move this into the player function getMove
						// TODO:

						// If not a valid move, do not advance the game
						// Instead the loop will prompt the player for a new command

						std::vector<Move> possibleMoves = board.getAllValidColorMoves(currentAgentColor, false);

						// Read in human commands
						std::string arg1;
						std::string arg2;

						std::cin >> arg1 >> arg2;

						if (std::cin.eof() || std::cin.fail()) {
							std::cin.clear();
							std::cout << "[MOVE] error reading input" << std::endl;
						} else {
							// Convert the arguments to coordinate system used by the classes
							int xIndexArg1 = arg1[0] - 'a';
							int yIndexArg1 = '8' - arg1[1];
							std::pair<int, int> positionArg1 = std::pair<int, int>(xIndexArg1, yIndexArg1);

							int xIndexArg2 = arg2[0] - 'a';
							int yIndexArg2 = '8' - arg2[1];
							std::pair<int, int> positionArg2 = std::pair<int, int>(xIndexArg2, yIndexArg2);

							// Determine if the position given are valid
							bool validArgs1 = false;
							if ((0 <= xIndexArg1) && (xIndexArg1 < 8) && (0 <= yIndexArg1) && (yIndexArg1 < 8)) {
								validArgs1 = true;
							}
							bool validArgs2 = false;
							if ((0 <= xIndexArg2) && (xIndexArg2 < 8) && (0 <= yIndexArg2) && (yIndexArg2 < 8)) {
								validArgs2 = true;
							}
							bool validArgs = validArgs1 && validArgs2;

							// Only continue if they are valid
							if (validArgs) {

								// Check if positionArg1 and positionArg2 match the from and destination of a move
								// if not, it might be castling
								bool foundMoves = false;
								std::vector<Move> selectedMoves;

								bool hasCastlingMoves = false;
								std::vector<Move> castlingMoves;

								for (Move &mv : possibleMoves) {

									if (mv.getFromPosition() == positionArg1 && mv.getDestinationPosition() == positionArg2) {
										// Get all moves that go from the first argument to the second argument
										selectedMoves.push_back(mv);
										foundMoves = true;
									}
									if (mv.getMoveType() == MoveType::CASTLE_KING || mv.getMoveType() == MoveType::CASTLE_QUEEN) {
										// Get all castling moves
										castlingMoves.push_back(mv);
										hasCastlingMoves = true;
									}
								}

								// Determine if more arguments are needed to specify which move (pawn promotion)
								if (foundMoves) {

									int selectedIndex = 0;

									// There is pawn promotion, get additional information from player
									if (selectedMoves.size() > 1) {
										std::string promotionPiece;
										std::cin >> promotionPiece;

										if (std::cin.eof() || std::cin.fail()) {
											std::cin.clear();
											std::cout << "[MOVE] error reading promotion piece, defaulting to queen" << std::endl;
											promotionPiece = "Q";
										}

										// Get the promotion piece symbol
										char promotionSymbol = promotionPiece[0];

										// Convert to upper case if needed
										if (('a' <= promotionSymbol) && (promotionSymbol <= 'z')) {
											promotionSymbol = promotionSymbol - 'a' + 'A';
										}

										// Convert character to PieceType
										PieceType toPromoted = PieceType::EMPTY_TILE;
										switch (promotionSymbol) {
											case 'N':
												toPromoted = PieceType::KNIGHT;
												break;
											case 'B':
												toPromoted = PieceType::BISHOP;
												break;
											case 'R':
												toPromoted = PieceType::ROOK;
												break;
											case 'Q':
												toPromoted = PieceType::QUEEN;
												break;
											default:
												// Default to promotion to queen
												toPromoted = PieceType::QUEEN;
												break;
										}

										// Change selectedIndex to correct promotion move
										for (int i = 0; i < selectedMoves.size(); i++) {
											Move promotionMove = selectedMoves[i];
											if (promotionMove.getPromoteType() == toPromoted) {
												selectedIndex = i;
												break;
											}
										}
									}

									board.enactMove(selectedMoves[selectedIndex]);
								} else if (hasCastlingMoves) {
								
									bool isCastling = false;

									for (Move& castlingMove : castlingMoves) {
										if (castlingMove.getDestinationPosition() == positionArg1 && castlingMove.getCapturePosition() == positionArg2) {

											board.enactMove(castlingMove);

											isCastling = true;
											break;
										}
									}

									if (!isCastling) {
										std::cout << "[MOVE] the move you gave was not found" << std::endl;
									}

								} else {
									std::cout << "[Move] invalid move" << std::endl;
								}

							} else {
								std::cout << "Invalid positions" << std::endl;
							}

							

						}
					}

				} else if (gameCommand == "undo") {
					std::cout << "[GAMELOOP] UNDO" << std::endl;
					board.undoLastMove();
				} else {
					std::cout << "[GAMELOOP] unknown command" << std::endl;
				}

			}

			std::cout << std::endl;

		} else if (command == "setup") {
			std::cout << "[SETUP] enter setup" << std::endl;

			int startingColor = 0; // 0 is white, 1 is black

			while (true) {
				std::string setupCommand;
				std::cin >> setupCommand;

				if (std::cin.eof() || std::cin.fail()) {
					std::cout << "[SETUP] exit" << std::endl;
					std::cin.clear();
					break;
				} else if (setupCommand == "done") {

					Board stBoard = Board(setupBoard);
					bool isSetupCorrectly = stBoard.validate();


					// Check there is only one of each king
					// No pawns are on the last rows
					// No king is in check

					if (isSetupCorrectly) {
						// Load the board

						std::cout << "[SETUP] custom board has been accepted" << std::endl;
						useSetupBoard = true;
						setupTurn = startingColor;

						break;
					} else {
						std::cout << "[SETUP] invalid board, cannot leave setup mode" << std::endl;
						useSetupBoard = false;
					}

				} else if (setupCommand == "=") {
					std::string colorTurn;
					std::cin >> colorTurn;
					if (colorTurn == "white") {
						startingColor = 0;
					} else if (colorTurn == "black") {
						startingColor = 1;
					} else {
						std::cout << "[SETUP] unknown color" << std::endl;
						std::cin.clear();
					}
				} else if (setupCommand == "+") {
					std::string pieceChar;
					std::string locationString;

					std::cin >> pieceChar >> locationString;

					if (std::cin.eof() || std::cin.fail()) {
						std::cout << "[SETUP] input error" << std::endl;
						std::cin.clear();
					} else {
						char piece = pieceChar[0];

						char xPosition = locationString[0];
						char yPosition = locationString[1];

						int intXPosition = xPosition - 'a'; // Since 'a' is 0 and 'h' is 7
						int intYPosition = '8' - yPosition; // Since '1' is 0 and '8' is 7

						if ((0 <= intXPosition) && (intXPosition < 8) && (0 <= intYPosition) && (intYPosition < 8)) {
							setupBoard[intYPosition][intXPosition] = piece;
						} else {
							std::cout << "Invalid position" << std::endl;
						}

					}
				} else if (setupCommand == "-") {
					std::string locationString;
					std::cin >> locationString;

					if (std::cin.eof() || std::cin.fail()) {
						std::cout << "[SETUP] input error" << std::endl;
						std::cin.clear();
					} else {
						char xPosition = locationString[0];
						char yPosition = locationString[1];

						int intXPosition = xPosition - 'a'; // Since 'a' is 0 and 'h' is 7
						int intYPosition = '8' - yPosition; // Since '1' is 7 and '8' is 0

						if ((0 <= intXPosition) && (intXPosition < 8) && (0 <= intYPosition) && (intYPosition < 8)) {
							setupBoard[intYPosition][intXPosition] = '_';
						} else {
							std::cout << "Invalid position" << std::endl;
						}
					}
				} else if (setupCommand == "reset") {
					std::cout << "[SETUP] reseting to empty board" << std::endl;

					for (int row = 0; row < 8; row++) {
						for (int col = 0; col < 8; col++) {
							setupBoard[row][col] = blankBoard[row][col];
						}
					}

				} else if (setupCommand == "view") {
					// Do nothing
				} else {
					std::cout << "[SETUP] unknown command: " << setupCommand << std::endl;
				}

				// This board is used only for drawing
				Board drawBoard = Board(setupBoard);

				drawBoard.drawChars();
			}

		} else if (command == "quit") {
			std::cout << "[MAINLOOP] exiting program" << std::endl;
			break;
		} else {
			std::cout << "[error] unknown command" << std::endl;
		}
	}

	std::cout << "Final Score:" << std::endl;
	std::cout << "White: " << whiteScore << std::endl;
	std::cout << "Black: " << blackScore << std::endl;

	return 0;
}