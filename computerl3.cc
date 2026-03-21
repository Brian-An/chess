#include "computerl3.h"

const double NORMAL_WEIGHT = 1;
const double CHECK_WEIGHT = 5;
const double CAPTURE_WEIGHT = 5;
const double AVOID_CAPTURE_ALL_PIECES_WEIGHT = 5;
const double AVOID_CAPTURE_MOVED_PIECE_WEIGHT = 5;

// Helper function to check ratio of captures to total moves for a move made
double percentOpponentCaptures(const unique_ptr<DecisionTree> &dt) {
    int c = 0;
    int tot = 0;
    for (auto &child: dt->getChildren()) {
        if (child->getCaptured() != "") {
            c++;
        }
        tot++;
    }
    if (tot == 0) return 0;
    return static_cast<double>(c) / tot;
}

MoveRequest ComputerL3::move() {
    const vector<unique_ptr<DecisionTree>> &nextMoves = Player::theGrid->getDecisionTree()->getChildren();
    vector<double> weights;
    double weightSum = 0;

    // Assign weights to each move
    for (auto &dt: nextMoves) {
        double w = NORMAL_WEIGHT;
        if (dt->getInCheck()) w += CHECK_WEIGHT; // Prefer checks
        if (dt->getCaptured() != "") w += CAPTURE_WEIGHT; // Prefer captures
        // Prefer moved piece not captureable
        w += AVOID_CAPTURE_MOVED_PIECE_WEIGHT;
        for (auto &child: dt->getChildren()) {
            if (child->getMoveRequest().newLoc == dt->getMoveRequest().newLoc) {
                w -= AVOID_CAPTURE_MOVED_PIECE_WEIGHT;
                break;
            }
        }
        // Prefer any piece not getting captured by opponent
        double p = percentOpponentCaptures(dt);
        w += AVOID_CAPTURE_ALL_PIECES_WEIGHT * (1 - p); 

        weights.emplace_back(w);
        weightSum += w;
    }

    if (!weightSum) {
        cerr << "No available moves!" << endl;
        return {"", "", false, ""};
    }

    // Amplify weights with bigger values, and diminish smaller values.
    double weightAvg = weightSum / weights.size();
    weightSum = 0;
    for (auto &w: weights) {
        w -= weightAvg;
        w = exp2(w);
        weightSum += w;
    }

    // Random logic to select move according to weight
    double rand = randNat(static_cast<int>(weightSum));
    int i = 0;
    while (rand > weights[i]) {
        rand -= weights[i];
        ++i;
    }

    return nextMoves[i]->getMoveRequest();
}   
