#ifndef	__TIMER_H__
#define	__TIMER_H__

/*
 * Timer.h (c) Noah Roberts 2003-04-12
 * Timer classes keep track of game clocks including move timers and total game clocks.
 */

#include <ctime>

class Timer
{
private:
    int	    _moves;
    int	    _movesLeft;

    int	    _seconds;
    int	    _secondsLeft;
    int     _increment;

    time_t	_startTime;
    time_t	_endTime;

public:
    Timer();
    void    setupTimer(int moves, int seconds, int increment);

    bool	haveTimeLeftForMove();
    void 	moveMade();
    void	startTimer();
    void	stopTimer();
    void	resetTimer();

    bool	surpasedTime();

    int movesLeft() { return _movesLeft;   }
    int timeLeft();
    bool active() { return !(_moves == 0 && _seconds == 0); }
};

#endif /* __TIMER_H__ */
