# Othello / Reversi — A Monte Carlo Tree Search Bot in C++

A command-line implementation of Othello (Reversi) where the computer opponent is a from-scratch **Monte Carlo Tree Search (MCTS)** bot, built on top of a hand-written **bitboard**.

---

## Demo

![Mid-game terminal view, showing the AI's execution time and chosen move](Screenshot%202026-06-20%20100514.png)
*The AI prints its execution time and chosen move after every turn.*

![Another in-progress match, showing the board and the human player's valid moves](Screenshot%202026-06-20%20100620.png)
*Board state and the list of valid moves offered to the human player.*

![End-of-game leaderboard and AI execution-time summary](Screenshot%202026-06-20%20100709.png)
*End-of-game leaderboard, plus the AI execution-time summary (min / max / average across the whole match).*

---

## Goals of This Project

- Build a bot that can play Othello / Reversi.
- Understand and implement a plain Monte Carlo Tree Search algorithm, using the Upper Confidence Bound for Trees (UCT) as the selection rule.
- Write a highly efficient board class in C++ that can represent the 8x8 Othello board, generate all valid moves, print the board, and make a move — all in under O(n²) time, where n is the size of the board (n = 8 here). This kind of representation is known as a **bitboard**.
- Use the project as an excuse to practice object-oriented programming and get a deeper understanding of C++.

## What This Project Is *Not* Trying to Do

- This isn't an attempt to build a very strong bot with complex heuristics. In fact, plain MCTS isn't even the ideal approach for an Othello bot — hand-crafted heuristics already beat it. The point here was to learn Monte Carlo methods, not to maximize playing strength.
- That said, it's far from dumb: the implementation is efficient enough to run a full **one million simulations in roughly 0 to 3 seconds per move**. (See [Simulations & Execution Time](#simulations--execution-time) below for real numbers.)

## How the Board Works

### Representing the Board

To represent a single cell, all we need to know is whether it holds a black pawn, a white pawn, or nothing — two bits would be enough, and there are 64 cells in total. Instead, the board uses two `uint64_t` integers, `black` and `white` (this is the bitboard technique).

- If the *i*-th bit is set in `black`, the cell at `(i / 8, i % 8)` (0-indexed) holds a black pawn — and likewise for `white`.
- The same *i*-th bit can never be set in both `black` and `white` at once.
- If bit *i* is unset in `black | white`, that cell is empty.

### Making a Move

Suppose we want to place a white pawn at cell `(i / 8, i % 8)`, given that the move is valid:

1. Declare `uint64_t mask = (1ULL << i);`
2. Picture the 8x8 board. Moving the mask left is `mask >>= 1`, right is `mask <<= 1`, up is `mask >>= 8`, down is `mask <<= 8`, up-right is `mask >>= 7`, and so on for the remaining diagonals.
3. Slide the mask one step at a time in a chosen direction until it either falls off the board or reaches the end of a line of pawns.
4. From there it's simple: if that line should be flipped, walk back along the same path and flip every pawn in it; otherwise, move on to the next direction.

### Finding All Valid Moves

Suppose we want every valid move for black, given the current board: every empty cell that "brackets" a line of white pawns with a black pawn on the other end.

Checking all 64 cells one at a time would be slow. Instead, the entire board can be processed at once using bitwise logic. Let `me` be black, `yu` be white, and `empty = ~(me | yu)` (every set bit in `empty` is an empty cell).

For a single direction — say, right, using a `<< 1` shift:

1. Shift `me` one step and `AND` it with `yu`. This gives every white pawn sitting immediately to the right of a black pawn.
2. Shift *that* result again and `AND` it with `yu` once more. Repeat this up to **6 times total** — 6 is the longest possible run of pawns that can be flipped on an 8-wide board.
3. Shift the final result one more time and `AND` it with `empty`. Every bit still set is a valid move in that direction.

Repeat this for all 8 directions and `OR` the results together. What's left is a single `uint64_t` in which every set bit is a valid move — computed with **no loops over individual cells at all**.

#### Avoiding Wraparound at the Edges

A subtle bug waiting to happen: a left/right shift can wrap a bit from one edge of the board onto the other. Two masks fix this:

```cpp
uint64_t not0 = 0xfefefefefefefefeULL; // every bit set, except column 0
uint64_t not7 = 0x7f7f7f7f7f7f7f7fULL; // every bit set, except column 7
```

`AND`-ing with `not0` before a leftward shift (and `not7` before a rightward shift) stops pawns from "falling off" one edge of the board and reappearing on the other.

## How Monte Carlo Tree Search Works (Vanilla MCTS)

Suppose there's more than one legal move available — if there's only one, the bot just plays it immediately and skips simulation entirely. Otherwise, the bot calls `getBestMove(board, moves)`. For now, assume every move has already been tried at least once.

Say a total of `N` simulations have been run so far (each simulation explores exactly one candidate move). To decide which move to simulate next, the bot uses the **Upper Confidence Bound for Trees (UCT)**:

For each candidate move *i*, let `n_i` be the number of times it has been picked out of the `N` simulations run so far, and let `w_i` be the number of those simulations that ended in a win for the bot.

```
UCT(i) = (w_i / n_i) + c × √( ln(N) / n_i )
```

The move with the highest UCT score gets simulated next, and its stats — `n_i`, `w_i`, and `N` — are updated afterward. Once all `N` simulations are done, the bot returns the move with the highest UCT score (equivalently, in this implementation, the most-visited move) as its final choice. `c` is a constant, usually `√2`.

**Intuition:** as `n_i` grows toward infinity, `w_i / n_i` (the *exploitation* term) converges to that move's true win probability. The second term is the *exploration* term — when a move hasn't been simulated much, its `n_i` is small, which inflates this term and eventually forces the bot to give that move another look.

So what actually happens *inside* a single simulation? In vanilla MCTS, after making that one chosen move, the rest of the game is played out by picking moves completely at random for both sides until the game ends, and recording who won. This random "rollout" is also exactly why this bot isn't especially clever — but, again, playing strength was never the point.

## Project Structure

| File | Responsibility |
|---|---|
| `Board.h` | Bitboard representation — move generation, move validation, making moves, printing the board |
| `Computer.h` | The MCTS / UCT bot (`Computer::npc`) |
| `Game.h` | Main game loop — turns, input handling, timing every AI move |
| `Leaderboard.h` | Reads and writes the all-time leaderboard |
| `leaderboard.txt` | Persisted leaderboard data |
| `Main.cpp` | Entry point and main menu |

## Building & Running

Compile with a C++20-capable compiler:

```bash
g++ -O3 -march=native -std=c++20 Main.cpp -o Othello.exe
```

Then run the resulting executable and follow the on-screen menu to play a match, view the leaderboard, or exit.

## Simulations & Execution Time

Every AI move runs exactly **1,000,000 (one million) Monte Carlo simulations** before committing to a move — this number is fixed in `Computer.h` (`int simulations = 1000000;`), and it's what the [goals section](#what-this-project-is-not-trying-to-do) above is referring to when it talks about a million simulations per move.

Each game times every single AI move with `std::chrono::high_resolution_clock`, printing a live "Execution time" line after each computer turn, and a full **min / max / average** summary once the match ends. Some real numbers captured during testing (compiled with `-O3 -march=native`):

| Measurement | Time |
|---|---|
| Example move | 1.81923 seconds |
| Example move | 0.616388 seconds |
| Match minimum | 0.00000 seconds *(only one legal move — simulation skipped entirely)* |
| Match maximum | 3.11267 seconds |
| Match average | 1.60574 seconds |

In short: one million simulations per move consistently finish in well under 3 seconds, and often in well under 2, on typical hardware with optimizations enabled. Exact numbers will vary depending on your machine and compiler.
