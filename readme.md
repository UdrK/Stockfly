# Stockfly
Stockfly aims to be a simple chess ai, maybe capable of beating the average 1000 elo player. The name is a pun that refers to stockfish, one of the best chess AI available today (fish eat flies).

## Bignami of game theory concerning chess
In mathematics, multi-agent competitive environments in which players have conflicting goals are often known as games. Chess has many characteristics that make it interesting for AI development, 
in particular, chess is a zero-sum, deterministic, perfect information game.
- Zero-sum: one's player advantage is the others' disadvantage, which implies that if a player is winning, their adversary is losing. In particular, in zero-sum games, `score(player_a) = -score(player_b)`;
- Deterministic: the position (configuration of pieces on the board) that arises from a move is determined and, given a position, the same move will always lead to the same new position;
- Perfect information: both players fully know the state of the game. No information is hidden.
Such games can often be represented with a game tree. In such a tree, each node represents a different state of the game, and each arc represents a move that given a state leads to another. This 
representation will be useful when implementing the algorithms that try to work out (search in the game tree) the best move available to the player.

Here's an example of a partial game tree for a simplified version of chess.

![Image of a game tree](https://github.com/UdrK/stockfly/blob/master/res/readme/tree_diagram_dark_theme.png?raw=true)

## Implementing Chess

Chess has an average branching factor of 35 (on average players choose between 35 possible moves) while games go on for, on average, 40 moves per player. This means that the search tree has about 80^35 nodes.
Speed, therefore, is paramount and is the reason behind many of the design choices made. 

### Board representation

The board is modeled by the Board class which represents the board as a 64-sized array of pointers to pieces. `NULL` pointers represent empty squares. 
In this representation, there's no distinction between light and dark squares. The board state also includes castling rights and en passant flag. To refer to pieces in a faster manner, the board
also keeps track of them in two `std::vector`, one for each player. Pointers to both kings are also part of the state of the board. This makes generating moves and checking for mate faster.
To initialize the board a FEN string is required. An empty FEN string will result in the starting position being setup.

### Pieces representation

Pieces are modeled by the Piece abstract class which is then implemented by a concrete class for each piece type. Pieces have a position (an integer in [0-63]) and a side (white or black, true or false).
Behavior-wise pieces are distinct in the way they move. These classes are responsible for implementing piece movement and therefore pseudo-legal move generation.

Pseudo-legal moves generation is one of the first challenges to face when implementing the game of chess. Pseudo-legal moves are moves that only obey rules regarding how pieces move, meaning that if the king
is in check, the player can make a move that doesn't resolve the check if pseudo-legal moves are allowed. The pseudocode below shows how moves (represented as an integer in [0-63]) can be generated given the square (an integer in [0-63]) on 
which the piece sits.

```
diagonal = true if piece moves diagonally
orthogonal = true if piece moves orthogonally

ranks_n = ranks north of given position
ranks_s = ranks south of given position
files_e = files east of given position
files_w = files west of given position 

moves = []

foreach rank in ranks_n
	if diagonal && nwe_flag && file in files_w && square north west is empty or enemy piece
		moves.add(square north west)
		if square north west is a piece
			nwe_flag = false
	
	if orthogonal && n_flag && square north is empty or enemy piece
		moves.add(square north)
		if square north is a piece
			n_flag = false
	
	if diagonal && new_flag && file in files_e && square north east is empty or enemy piece
		moves.add(square nort east)
		if square nort east is a piece
			new_flag = false

if orthogonal
	foreach file in files_w
		if square west is empty or enemy piece
			moves.add(square west)
			if square west is piece
				break
	
	foreach file in files_e
		if square east is empty or enemy piece
			moves.add(square east)
			if square east is piece
				break

// north squares generation analog for south squares
```
By wrapping this pseudocode in a function we obtain a function generic enough to use for the movement of King, Queen, Rook, and Bishop.
Knight and Pawn obey very different rules and require specific functions to generate pseudo-legal moves. On top of that castling and promotion need to be considered.

### Making a move

When making a move on the board it's important to keep the state of the board and pieces consistent, many bugs can arise from small errors in how pieces are moved from one square to another or in how pieces
are taken. Furthermore, before allowing a move to happen it's necessary to verify the legality of the move. Generally, a move is not legal if:
- the move is not pseudo-legal (the piece doesn't move according to its rules)
- after the move, the player's king is in check

This last case can happen for two reasons:
- the king was in check before the player moved
- the player's move discovered a check on the king: the piece moved was pinned

Assuming the pseudo-legality of moves isn't an issue, one way to verify the legality of a move is to "play it out" and then verify whether the player's king is in check or not.
In any case, the move has to be undone after the legality check. One notable exception is with castling: by definition, if the move can be done it won't result in an illegal position.

### Undoing a move

The simplest approach to undoing a move is to save the FEN representing the board before the move, and reset the board with it after the move has been made. While this approach is really simple, 
the repeated initialization of pieces and of the entire board is slow, and since undoing a move is necessary to recursively generate lines of possible positions to evaluate, it needs to be fast.
Therefore moves are represented by the Ply class. A ply is simply a move made by one player. The term is used because in chess, a move is a couple of plies: one for white and one for black. E.g.: 1. e4 e5 is a move. e4 is a ply, e5 is a ply.
Ply holds all the information needed to undo a move (which pieces are involved, where they were before the move, etc). With ply, undoing a move is less computationally complex than doing the move in the first place.

## Stockfly

Stockfly will be what is referred to as an expert system. Its capabilities won't derive from fancy machine learning algorithms, reinforcement learning methods or other means of self-learning. Instead, the AI will make use of old-school adversarial search
methods and domain-specific heuristics to consider all possible moves and find the "best" (as long as the model knows) one.

### Game tree generation

For the AI to be able to find the best move, it needs to be able to search the game tree. Move generation can start from pseudo-legal moves generation. Then a legality check can be used to filter these moves. To generate
the game tree it's necessary to generate every possible response to each move generated and recursively keep going until a certain depth is reached.

Below we have a diagram showing the most important function calls that allow an exhaustive game tree generation.

![Game tree generation calls](https://github.com/UdrK/stockfly/blob/master/res/readme/game_tree_generation.png?raw=true)

### The minimax algorithm

### alpha-beta pruning

## Roadmap
