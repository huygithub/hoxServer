#ifndef	__EVALUATOR_H__
#define	__EVALUATOR_H__

/*
 * Evaluator.h (c) Noah Roberts 2003-02-26
 *
 * Evaluator evaluates positions by looking at the board and judging what the score should
 * be.  It is a class because future versions will probably have more than one way to
 * evaluate a position; there could even be several ways the engine might evaluate the
 * position depending on the state of the game - ie mate problems.
 */

class Board;
class Lawyer;
class Move;	

class Evaluator 
{
 protected:
  static Evaluator theEvaluator;
  Evaluator() {};
 public:
  static Evaluator *defaultEvaluator() { return &theEvaluator; }
  long evaluatePosition(Board &theBoard, Lawyer &lawyer); // score the position on the board.
  long evaluateMaterial(Board &theBoard);
  int pieceValue(int piece); // absolute piece values, mostly used for move ordering.
};

#endif /* __EVALUATOR_H__ */
