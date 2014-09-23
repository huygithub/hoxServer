#ifndef	__TSI_ENGINE_H__
#define	__TSI_ENGINE_H__

/*
 * Engine.h (c) Noah Roberts 2003-02-26
 * The Engine class is the brain of it all, this is the class that ties the rest of
 * the classes together to actually perform the function of playing chess.  This class
 * creates and navigates the search tree and decides what move to make next.  This class
 * is also the one that will notice and respond when the game has ended.
 */

#include <string>
#include <vector>
#include <sys/timeb.h>

#include "Move.h"
#include "Options.h"
#include "Timer.h"

class Board;
class Evaluator;
class Lawyer;
class OpeningBook;
class TranspositionTable;

enum { NO_CUTOFF, HASH_CUTOFF, NULL_CUTOFF, MATE_CUTOFF, MISC_CUTOFF };

struct PVEntry
{
  Move move;
  int cutoff;
  PVEntry(Move m, int c) : move(m), cutoff(c) {}
};

//#define	INFIN	0x0FFFFFFF;
#define		INFIN	3000
#define CHECKMATE (-2000)

class tsiEngine : public OptionsObserver
{
private:
    Board*               board;
    Lawyer*              lawyer;
    Evaluator*           evaluator;
    OpeningBook*         _openingBook;
    TranspositionTable*  _transposTable;

    // search result
    std::vector<PVEntry> _principleVariation;
    short                _searchState;

    // Search information
    int                  _maxPly;
    std::vector<Move>    killer1;
    std::vector<Move>    killer2;
    std::vector<Move>    _priorityTable;
    int                  nullMoveReductionFactor;
    short                _searchAborted;

    // Timers ( !!! Shut off timers by default !!! )
    Timer                _redTimer;
    Timer                _blueTimer;

    Timer*               _myTimer;  // This Engine (AI) 's timer.

    // Search statistics
    int                  nodeCount;
    int                  hashHits;
    int                  nullCutoffs;
    struct timeb         _startTime;

    // User configurable options

    bool        _useOpeningBook;
    bool        _displayThinking;

    bool        _useQuiescence; // perform quiescence search or not
    bool        _useQNull;      // null cutoff in quiescence
    bool        _useQHash;      // use table in quiescence

    int         searchMethod; // Tells system which algorithm to use

    bool        _useMTDF;
                    /* Use MTD(f) (Memory-enhanced Test Driver) algorithm.
                     * See: http://en.wikipedia.org/wiki/MTD-f
                     */

    bool        _useIterDeep;
                    /* If yes, then the search is tried at depths [1 ... _maxPly].
                     * otherwise just _maxPly.
                     * See: http://en.wikipedia.org/wiki/Iterative_deepening_depth-first_search
                     */

    bool        _allowNull;    // If false then null move will never be tried.
    bool        _verifyNull;   // Use verified null-move pruning or standard.

    bool        allowTableWindowAdjustments; // If table contains a depth < our target the
                                             // norm is to adjust the window.  User may
                                             // disable that behaviour.
    bool        _useTable;

private:

    // Search functions
    long quiescence(long alpha, long beta, int ply, int depth, bool nullOk = true);
    void filterOutNonCaptures(std::list<Move> &l);

    // Search algorithms...

    long alphaBeta(std::vector<PVEntry> &pv, std::list<Move> moveList,
                 long alpha, long beta, int ply, int depth,
                 bool legalonly = false, bool nullOk = true, bool verify = true);
    //long pvSearch(std::vector<PVEntry> &pv, long alpha, long beta, int ply, int depth,
    //              bool legalonly = false, bool nullOk = true, bool verify = true);
    long negaScout(std::vector<PVEntry> &pv,  std::list<Move> moveList,
                 long alpha, long beta, int ply, int depth,
                 bool legalonly = false, bool nullOk = true, bool verify = true);

    // Generic search - calls specific search, will add heuristics here...
    long search(std::vector<PVEntry> &pv, long alpha, long beta, int ply, int depth,
              bool legalonly = false, bool nullOk = true, bool verify = true);

    // MTD(f) algorithm...calls search.
    long mtd(std::vector<PVEntry> &pv, long guess, int depth);

    // Support functions...

    // looks for position in table.
    bool tableSearch(int ply, int depth, long &alpha, long &beta, Move &m, long &score, bool &nullok);
    // stores position in table.
    void tableSet(int ply, int depth, long alpha, long beta, Move m, long score);
    // looks for killers and alters priority table
    void setUpKillers(int ply);
    // Adds killer move at ply
    void newKiller(Move& theMove, int ply);

public:
    tsiEngine(Board *brd, Lawyer *law);
    ~tsiEngine();

    // If board is replaced...
    void setBoard(Board *brd) { board = brd; }

    // Tells engine to think...
    long think();

    // Tells engine that something happened such that search must be stopped.
    void endSearch();
    bool doneThinking();
    bool thinking();

    // Search information retrieval...
    Move getMove();
    std::string variationText(const std::vector<PVEntry>& pv) const;

    // OptionObserver requirements
    void optionChanged(const std::string& whatOption);
};

#endif /* __TSI_ENGINE_H__ */
