#include "textdisplay.h"
#include "grid.h"
#include "info.h"
#include "player.h"
#include "computerl1.h"
#include "computerl2.h"
#include "computerl3.h"
#include "computerl4.h"
#include "human.h"
#include "util.h"
#include <iostream>
#include <memory>
#include <algorithm>

using namespace std;

// Helper function to validate chess piece characters
bool isValidPiece(char piece) {
    char lowerPiece = tolower(piece);
    return lowerPiece == 'p' || lowerPiece == 'r' || lowerPiece == 'n' || 
           lowerPiece == 'b' || lowerPiece == 'q' || lowerPiece == 'k';
}

// Helper function to validate chess position (algebraic notation)
bool isValidPosition(const string& position) {
    if (position.length() != 2) {
        return false;
    }
    
    char file = position[0]; // column (a-h)
    char rank = position[1]; // row (1-8)
    
    return (file >= 'a' && file <= 'h') && (rank >= '1' && rank <= '8');
}


istream &setup(istream &in, Grid &grid, Colour &currentTurn) {
    // In setup mode
    string command;
    while (in >> command) {
        if (command == "done") {
            // Validate the final setup before exiting setup mode
            grid.updateDecisionTree(currentTurn);
            if (!grid.isSetupValid()) {
                cerr << "Invalid setup: The board configuration is not valid" << endl;
                continue; // Stay in setup mode, don't break
            }
            cout << "Setup completed successfully." << endl;
            break; // Only break if setup is valid
        } else if (command == "+") {
            char piece;
            string position;
            in >> piece >> position;
            
            // Validate piece type
            if (!isValidPiece(piece)) {
                cerr << "Invalid piece: " << piece << endl;
                continue;
            }
            
            // Validate position
            if (!isValidPosition(position)) {
                cerr << "Invalid position: " << position << endl;
                continue;
            }
            
            // Place the piece on the board at the specified position
            grid.add(piece, position);
            cout << grid << endl;
        } else if (command == "-") {
            string position;
            in >> position;
            
            // Validate position
            if (!isValidPosition(position)) {
                cerr << "Invalid position: " << position << endl;
                continue;
            }
            
            // Remove the piece from the specified position
            grid.remove(position);
            cout << grid << endl;
        } else if (command == "=") {
            string colour;
            in >> colour;
            // Set the turn to the specified colour
            if (colour == "white" || colour == "White") {
                currentTurn = Colour::White;
                cout << "Current turn set to white." << endl;
            } else if (colour == "black" || colour == "Black") {
                currentTurn = Colour::Black;
                cout << "Current turn set to black." << endl;
            } else {
                cerr << "Invalid colour: " << colour << ". Use 'white' or 'black'" << endl;
            }
        } else {
            cerr << "Invalid setup command: " << command << endl;
        }
    }
    return in;
}

unique_ptr<Player> make_player(const string &spec) {
    // Function to check the player, throws an error in case of invalid input
    //If it's a human
    if (spec == "human") {
        return make_unique<Human>();
    }
    //If it's a computer - and which one
    if (spec.find("computer") != string::npos && spec.length() == 9) {
        if (spec[8] == '1') {
            return make_unique<ComputerL1>();
        } else if (spec[8] == '2') {
            return make_unique<ComputerL2>();
        } else if (spec[8] == '3') {
            return make_unique<ComputerL3>();
        } else if (spec[8] == '4') {
            return make_unique<ComputerL4>();
        }
    }

    //Throwing an error in case of invalid input
    throw invalid_argument("Invalid Argument " + spec);
}

int main(int argc, char *argv[]) {
    // call with '-bonus' to see the bonus options
    bool in_bonus = false;
    // getting command line arguments for bonus
    if (argc >= 2) {
        string arg1 = argv[1];
        if (arg1 == "-bonus") in_bonus = true;
    }
    randNatSeed(); // Default to process ID seed for randNat()
    Grid grid;
    Player::setGrid(&grid); // add the grid to the Player
    Colour currentTurn = Colour::White; // Default to white's turn
    vector<unique_ptr<Player>> players(2);
    players[0] = make_unique<Human>();
    players[1] = make_unique<Human>();
    string command;
    string line;
    double whitescore = 0.0;
    double blackscore = 0.0;
    bool gamestart = false; //field to allow other parts to run if the game is on or off
    bool setupDone = false; // to check if were starting from setup or from scratch

    // looping until we reach eof
    while (cin >> command) {
        if (command == "game") {
            // Initial setup of the game
            string first, second;
            // Gettinginput and making sure they are valid
            if (!(cin >> first >> second)) {
                cerr << "Invalid Command, please try again" << endl;
                continue;
            }
            //checking what each player is:
            try {
                players[0] = make_player(first);
                players[1] = make_player(second);
            } catch (invalid_argument &ia) {
                cerr << ia.what() << ". Please try again" << endl;
                continue;
            }
            getline(cin, line); // getting rid of the rest of the line.
            //were doing this at the end so that in case of an error, the user doesn't continue playing
            gamestart = true;

            //Initializing the game state to the start of the game 
            //     - in case the user presses game in the middle of an ongoing game
            if (!setupDone) {
                currentTurn = Colour::White;
                grid.init();
            }
            grid.saveBoard();
            setupDone = false;
            cout << grid << endl;
        } else if (command == "resign" && gamestart) {
            // When one player resigns
            getline(cin, line); // getting rid of the rest of the line.
            if (currentTurn == Colour::White) ++blackscore;
            else ++whitescore;
            cout << colourTranslator(negateColour(currentTurn)) << " wins!" << endl;
            gamestart = false;
        } else if (command == "move" && gamestart) {
            // When a move is to be performed
            Player *targetPlayer = (currentTurn == Colour::White) ? players[0].get() : players[1].get();
            MoveRequest mr = targetPlayer->move();
            if (!grid.move(mr, currentTurn)) {
                cerr << "Invalid Command, please try again" << endl;
                continue;
            }
            currentTurn = negateColour(currentTurn);
            cout << grid << endl;
        } else if (command == "setup" && !gamestart) {
            // When setup mode is to be performed
            currentTurn = Colour::White;
            grid.init(true); // Initialize the grid in setup mode
            setup(cin, grid, currentTurn);
            setupDone = true;
        // Bonus features!
        } else if (command == "options" && gamestart && in_bonus) {
            // command to get the available moves for a square: options <loc>
            string loc;
            cin >> loc;
            getline(cin, line); // getting the rest of the line. 
            // getting all the moves
            const vector<unique_ptr<DecisionTree>> &children = grid.getDecisionTree()->getChildren();
            cout << "Options: " << endl;
            // looping through them and if it matches where they want to start, then printing it
            for (const auto &child : children) {
                const MoveRequest &mr = child->getMoveRequest();
                if (mr.prevLoc == loc) {
                    cout << mr.newLoc << (mr.promoting ? " "+mr.promoteTo : "") << "-";
                }
            }
            cout << endl;
        } else if (command == "opening" && gamestart && in_bonus) {
            // command to get the openning moves
            auto list = grid.getOpeningExplorer()->getOpeningInfo();
            cout << "Name: " << list["name"] << endl;
            cout << "Moves: " << list["pgn"] << endl;
            cout << "ECO: " << list["eco"] << endl << endl;
        } else if (command == "undo" && gamestart && in_bonus) {
            // undo feature: undo
            getline(cin, line); // getting the rest of the line. 
            // undoing the turn
            if (!grid.undo(currentTurn)) {
                currentTurn = Colour::White; // making sure white starts
                cerr << "You have to make a move first" << endl;
                continue;
            }
            // negating the coloour
            currentTurn = negateColour(currentTurn);
            cout << grid << endl;
        } else if (command == "bonus-check") {
            // helper function to check if bonus is on or off
            cout << "You are " << (!in_bonus ? "not " : "") << "in Bonus" << endl;
        } else {
            // if it's invalid input
            getline(cin, line); // getting rid of the rest of the line
            cerr << "Invalid Command, please try again" << endl;
            continue;
        }

        if (gamestart) { // checking if the game is ending or not if the game has started
            // Game in Draw?
            if (grid.isInsufficientMaterial() && gamestart && in_bonus) {
                cout << "Draw due to insufficient material!" << endl;
                whitescore += 0.5;
                blackscore += 0.5;
                gamestart = false;
            }
            
            Colour inCheck = grid.isCheck();
            Colour inCheckmate = grid.isCheckmate();
            // Checkmate?
            if (inCheckmate != Colour::Neither) {
                cout << "Checkmate! " << colourTranslator(negateColour(inCheckmate)) << " wins!" << endl;
                if (inCheckmate == Colour::White) ++blackscore;
                else ++whitescore;
                gamestart = false;
            // stalemate?
            } else if (grid.isStalemate() != Colour::Neither) {
                cout << "Stalemate!" << endl;
                whitescore += 0.5;
                blackscore += 0.5;
                gamestart = false;
            // Check?
            } else if (inCheck != Colour::Neither) {
                cout << colourTranslator(inCheck) << " is in check!" << endl;
            }
        }
    }

    //printing out final scores at the end of the game
    cout << "Final Score:" << endl << "White: " << whitescore << endl << "Black: " << blackscore << endl;
    return 0;
}
