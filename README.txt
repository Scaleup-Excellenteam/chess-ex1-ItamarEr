I implemented 2 Exceptions:
- InvalidMoveException: This exception is raised when a move is invalid with the same messages and codeResponse
  as the one in the original code.
- PriorityQueueException: This exception is raised when the priority queue is empty when trying to pull or <<.

The move scoring algorithm works as follows:
getAllMovesScores generates all valid moves for the current player,
simulates each move, and uses evaluateMove to assign a score to each move.
It then uses minimax to estimate the opponent's best reply and subtracts this value from the move's score,
returning a list of moves with their adjusted scores.
The minimax function recursively makes all possible moves and replies for both players up to a certain depth,
always choosing the best score for the current player.
The evaluateMove function assigns a higher score for moves that capture pieces, threaten stronger enemy pieces,
put the opponent in check, or move to central board positions.
It reduces the score if the moved piece is threatened by a weaker enemy piece.
checkmate or stalemate are handled with extreme or zero scores.

The overall time complexity of the algorithm is O(n*b^d),
where n is the number of initial valid moves,
b is the average number of moves per position (valid moves inside minimax) and d is the depth.