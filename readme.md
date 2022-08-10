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

The minimax algorithm searches the game tree in a depth-first fashion for the best move. The algorithm traverses the tree to the leafs or until reaching a depth limit and then returns a static evaluation of the game's state.
This evaluation is used to find the best move: one player will choose the move that yelds the maximum evaluation, and the other player will choose the move that yelds the minimum, alternating each other. To achieve this
the algorithm uses two mutually recursive functions. Forgetting about depth restrictions, the algorithm is the following:

```
Max-player(state)
	if (is_leaf(state))
		return evaluation(state)

	value = - infinity

	foreach move
		new_state = state.move()
		value = max(value, Min-player(new_state))

	return value

Min-player(state)
	if (is_leaf(state))
		return evaluation(state)

	value = + infinity

	foreach move
		new_state = state.move()
		value = min(value, Max-player(new_state))

	return value
```

Let's consider the Max-player: the `foreach` loop tries every possible move available to the player. This yields a new game state (a new position on the board) from which the Min-player has to carry on playing.
The Min-player will choose the best move for himself, the one that minimizes the score and return that score to the Max-player. Max-player's job will be to choose the move that maximizes the score. This keeps happening
for the entire height of the tree until reaching the leafs that can only be statically evaluated. If the game is ended it's easy to assign a good score for a win, a bad score for a loss and 0 to a draw.

Below an example of how minimax works. In chess terms consider that each node of the tree corresponds to a position on the board. Each arc corresponds to a move. The score associated to the leafs of the tree is calculated
by Min-player which performs a static evaluation of the position. When backtracking the recursion selects the main line that is highlighted in green at the end of the animation.

![Minimax animation](https://github.com/UdrK/stockfly/blob/master/res/readme/minimax_optimized.gif?raw=true)


### alpha-beta pruning

Now consider the following tree:

![Alpha-beta example tree](https://github.com/UdrK/stockfly/blob/master/res/readme/alpha_beta_tree.png?raw=true)

Note that nodes (positions) have been given names (letters). By solving the left subtree we find that Max-player will obtain a score of 4 by playing the 'b' position, this is evident since, if Max plays 'b', Min will play 'e' to minimize the final game's score.
Now consider the right subtree, in particular, consider the situation Min encounters when they evaluate the 'f' position and get a score of 1. In this situation it's possible to leverage 2 facts to completely avoid evaluating 'g':

- Max can obtain a score of 4 by playing 'b'
- Min will not play 'g' in the right subtree if that position's score is higher than 1 (because Min will always play the move that minimizes the score)

Therefore, if Max plays 'c' in the first position, Min can at least get a score of 1, if not better (for Min). This implies that Max will not play 'c' regardless of score of the 'g' position. Therefore
it's possible to skip the evaluation of 'g'. Imagine 'g' isn't just a position, but an entire subtree, this can method can avoid a lot of computation.

This kind of optimization in tree searches is called pruning, and in particular this optimization is called alpha-beta pruning after the names of the parameters that are added to minimax to achive this behavior.

The following is the minimax algorithm with alpha-beta pruning:

```
alpha-beta-Max-player(state, alpha, beta)
	if (is_leaf(state))
		return evaluation(state)

	foreach move
		new_state = state.move()
		value = alpha-beta-Min-player(new_state, alpha, beta)
		
		if (value >= beta)
			return beta

		if (value > alpha)
			alpha = value

	return alpha

alpha-beta-Min-player(state, alpha, beta)
	if (is_leaf(state))
		return evaluation(state)

	foreach move
		new_state = state.move()
		value = alpha-beta-Max-player(new_state, alpha, beta)
		
		if (value <= alpha)
			return alpha

		if (value < beta)
			beta = value

	return beta
```

Similarly to minimax we have two mutually recursive functions. Alpha represents the "best" score achievable by Max. Beta, conversely, represents the "best" score achievable by Min.
To get a sense of how the algorithm works, consider the tree above, and the following call stack, obtained from issuing `alpha-beta-Max-player(state=a, α=-∞, β=+∞)` with `a` the root node of the tree above:

```
Max(state=a, α=-∞, β=+∞)
	// First iteration
	score = Min(state=b, α=-∞, β=+∞)
		// First iteration
		score = Max(state=d, α=-∞, β=+∞) = 9
		score = 9 <= α=-∞ ? No
		score = 9 < β=+∞ ? Yes -> β=9
		// Second iteration
		score = Max(state=e, α=-∞, β=9) = 4
		score = 4 <= α=-∞ ? No
		score = 4 < β=9 ? Yes -> β=4
		// Min for left subtree done
		return β=4
	= 4
	score = 4 >= β=+∞ ? No
	score = 4 >= α=-∞ ? Yes -> α=4
	// Second iteration
	score = Min(state=c, α=4, β=+∞)
		// First iteration, here we have the pruning
		score = Max(state=f, α=4, β=+∞) = 1
		score = 1 <= α=4 ? Yes -> return α=4
	= 4
	// notice the last call of min only went through one iteration
	score = 4 >= β=+∞ ? No
	score = 4 > α=4 ? No
	return α=4
```


## Roadmap

### The vision
- [x] A working chessboard with working logic
- [x] A simple, adversarial search based AI
- [ ] A refined version of such an AI capable of giving chess beginners a little headache and capable of beating the previous version consistently
- [ ] A GUI
- [ ] A computer vision based solution to play the AI on a real-life chess board

### How to get there

As of now the AI lacks some optimization which can allow it to search deeper in the tree and in general play better:
- An opening book, so the AI plays decent moves when the search would have to go too deep to actually understand what moves are good
- Search optimizations to search deeper than 4 ply, including but not limited to: an heuristic to order moves from most likely best to least like best, a transposition table to avoid evaluating a position 
already considered, quiescence search to mitigate the horizon problem, heuristics to make a better static evaluation
- Rules aided playing for endgames

With the regards to the GUI i plan on using python's chess library as an interface. I suspect move input would still be command line based but i'm ok with it.

Finally for the computer vision part of the project i plan on using a webcam to gather a dataset and probably train some version of yolo to do the object detection and classification. I'm still unsure on how
to transform this data into a FEN to feed to the engine.
