#include "computerl2.h"

const int PREFERED_WEIGHT = 10;

MoveRequest ComputerL2::move() {
    Grid *grid = Player::theGrid;
    DecisionTree *dt = grid->getDecisionTree();
    const vector<unique_ptr<DecisionTree>> &availableMoves = dt->getChildren();

    MoveRequest bestMove;
    int bestScore = -1;
    for (const auto &child : availableMoves) {
        MoveRequest mr = child->getMoveRequest();
        int score = 0;

        // Prefer capturing moves
        if (child->getCaptured() != "") {
            score += PREFERED_WEIGHT;
        }

        // Prefer moves that put the opponent in check
        if (child->getInCheck()) {
            score += PREFERED_WEIGHT;
        }

        // Update best move if this one is better
        if (score > bestScore) {
            bestScore = score;
            bestMove = mr;
        }
    }

    // fallback if all moves are neutral
    if (bestScore == 0) {
        int i = randNat(availableMoves.size());
        bestMove = availableMoves[i]->getMoveRequest();
    }

    return bestMove;
}
