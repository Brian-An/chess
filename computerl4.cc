#include "computerl4.h"

// creating a scoring map for each move
const std::map<char, int> SCORING_MAP {
    {'p', 1},
    {'n', 3},
    {'b', 3},
    {'r', 5},
    {'q', 9}
};

// Score of a check
const int CHECK_SCORE = 7; // we lowered the score so it doesn't kill itself pretty often
const int MATE_SCORE = 55;
// flag to check for each layer which score do we actually care about
// e.g. do we want to minimize our opponents minscore or it's average score or ...
// options are: 0 2 4
const int WHICH_SCORE_MATTERS = 4; 

struct scoring {
    int minScore;
    size_t minIndex;
    int maxScore;
    size_t maxIndex;
    int avgScore;
};

// helper function to return the 
//         minScore, the index of minScore, maxScore, the index of maxScore, and average score
// of the layer in *this exact order*
scoring getLevelScore(const vector<unique_ptr<DecisionTree>> &availableMoves, int level = 0) {
    int minScore = 0;
    vector<size_t> minIndexs;
    int maxScore = 0;
    vector<size_t> maxIndexs;
    int totalScore = 0;
    int total = 0;
    
    for (size_t i = 0; i < availableMoves.size(); ++i) {
        const unique_ptr<DecisionTree> &move = availableMoves[i];
        // we start every move with a score of 0
        int score = 0;

        // if were taking a piece we add it's score:
        char captures = move->getCaptured()[0];
        if (SCORING_MAP.count(captures) != 0) score += SCORING_MAP.at(captures);
        // if the move puts opponent in check
        if (move->getInCheck()) score+=CHECK_SCORE;
        
        // if were promoting to a valuable character
        if (move->getMoveRequest().promoting) score += SCORING_MAP.at(move->getMoveRequest().promoteTo[0]);
        // getting the next level score
        vector<unique_ptr<DecisionTree>> &children = move->getChildren();
        if (children.size() > 0) {
            scoring nextLayerResult = getLevelScore(children, level + 1);
            if (level % 2 == 1) {
                score-=nextLayerResult.maxScore; //we take best case
            } else {
                score-=nextLayerResult.avgScore; // oponent takes average case
            }
        }
        // if it's a mate:
        if (children.size() == 0 && level != MAX_LEVEL - 1 && move->getInCheck()) {
            score += MATE_SCORE;
            if (level == 0) return { 0, 0, score, i, 0 }; // if it's mate in one move just do it. 
        }
        // if it's the first element we set the variables, else we take the min and max of stuff
        if (i == 0) {
            minScore = maxScore = score;
            minIndexs.emplace_back(i);
            maxIndexs.emplace_back(i);
        } else {
            // if it's the same score as the smallest one we add it to the vector:
            if (minScore == score) {
                minIndexs.emplace_back(i);
            } else if (minScore > score) {
                // if it's less, we rewrite the list
                minScore = score;
                minIndexs.clear();
                minIndexs.emplace_back(i);
            }

            // doing the same for maxScore
            if (maxScore == score) {
                maxIndexs.emplace_back(i);
            } else if (maxScore < score) {
                // if it's less, we rewrite the list
                maxScore = score;
                maxIndexs.clear();
                maxIndexs.emplace_back(i);
            }
        }

        // only adding the score to total if it's not 0
        if (score != 0) {
            totalScore+=score;
            ++total;
        }
    }
    int avgScore = total != 0 ? totalScore / total : 0;
    // if there are multiple elements with the same value, we randomly select one
    size_t minIndex = minIndexs[randNat(minIndexs.size())];
    size_t maxIndex = maxIndexs[randNat(maxIndexs.size())];
    return { minScore, minIndex, maxScore, maxIndex, avgScore };
}

MoveRequest ComputerL4::move() {
    // getting all the available moves:
    Grid *grid = Player::theGrid;
    DecisionTree *dt = grid->getDecisionTree();
    const vector<unique_ptr<DecisionTree>> &availableMoves = dt->getChildren();

    scoring finalResults = getLevelScore(availableMoves);
    // getting the moveRequest at the maxScore and returning it
    MoveRequest mr = availableMoves[finalResults.maxIndex]->getMoveRequest();
    return mr;
}

