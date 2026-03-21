# cs246chess
## Summary
A chess project developed by Amin, Brian, and Philip for CS 246.

## Getting Started
```bash
git clone https://github.com/Brian-An/cs246chess.git
make
./chess
# or (for bonus features)
./chess -bonus
```

## 🕹️ Command Reference

### `game <white-player> <black-player>`
- **Arguments:** `<white-player>`, `<black-player>` — Either `human` or `computer[1-4]`
- **Pre-requisite:** None  
- **Functionality:** Starts the game.

---

### `resign`
- **Pre-requisite:** Game must be started  
- **Functionality:** Resigns from the current game.

---

### `move <optional:start> <optional:end>`
- **Arguments:** `start`, `end` — Starting and ending squares of the move  
- **Pre-requisite:** Game must be started  
- **Functionality:**  
  - For **human** players: Both `start` and `end` are required.  
  - For **computer** players: Just use `move` with no arguments.

---

### `setup`
- **Pre-requisite:** Game must **not** be started  
- **Functionality:** Enters setup mode for a custom game. Use `game` to begin play once setup is complete.

---

### `+ <type> <loc>`
- **Pre-requisite:** Setup mode must be active  
- **Functionality:** Adds a piece of type `<type>` at location `<loc>`. Replaces any existing piece.

---

### `- <loc>`
- **Pre-requisite:** Setup mode must be active  
- **Functionality:** Removes the piece at `<loc>`. Does nothing if the square is already empty.

---

### `= <col>`
- **Arguments:** `<col>` — Either `white` or `black`  
- **Pre-requisite:** Setup mode must be active  
- **Functionality:** Sets which player starts. Defaults to white if unspecified.

---

### `done`
- **Pre-requisite:** Setup mode must be active  
- **Functionality:** Finalizes the setup. Will not proceed if the board is invalid.

---

## 🌟 Bonus Features

### `bonus-check`
- **Pre-requisite:** None  
- **Functionality:** Displays whether bonus mode is active. Enable it by running the executable with the `-bonus` flag.

---

### `options <loc>`
- **Pre-requisite:** Bonus mode must be active and game must be started  
- **Functionality:** Displays all valid moves from `<loc>`, only if it's the current player's turn.

---

### `opening`
- **Pre-requisite:** Bonus mode must be active and game must be started  
- **Functionality:** Identifies the current opening based on move history.

---

### `undo`
- **Pre-requisite:** Bonus mode must be active and game must be started  
- **Functionality:** Reverts the last move. Can be used repeatedly to undo multiple moves.

## Copyright
- `opening-moves.tsv` uses opening data gathered from https://github.com/lichess-org/chess-openings/() under CC license.
- All piece images (under `piecesImgBits.h`) are from https://commons.wikimedia.org/wiki/Category:PNG_chess_pieces/Standard_transparent under CC license.
