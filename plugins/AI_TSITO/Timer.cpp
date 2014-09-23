#include	"Timer.h"

Timer::Timer()
{
    /* Disable timer by default. */
    this->setupTimer( 0, 0, 0 );
}

void
Timer::setupTimer(int moves, int seconds, int increment)
{
    _moves     = moves;
    _seconds   = seconds;
    _increment = increment;

    resetTimer();
}

bool Timer::haveTimeLeftForMove()
{
    if (_seconds == 0 && _increment == 0) return true; // Timer is shut off.

    int timePerMove = (_secondsLeft - 1) / _movesLeft + _increment;

    //cerr << "Time per move is: " << timePerMove << endl;

    time_t now = time(NULL);
    int used = (int)difftime(now,_startTime);
    //cerr << "Time used is: " << used << endl;
    return (used < timePerMove);
}

void Timer::moveMade()
{
    int timeThisMove = (int)difftime(_endTime, _startTime);
    _secondsLeft -= timeThisMove;
    _secondsLeft += _increment;

    //cerr << "Time left was: " << _secondsLeft << endl;

    if (_movesLeft == 1 && _secondsLeft >= 0) resetTimer();
    else _movesLeft--;
}

bool Timer::surpasedTime()
{
    return (_secondsLeft < 0);
}

void Timer::startTimer()
{
    _startTime = time(NULL);
}

void Timer::stopTimer()
{
    _endTime = time(NULL);
}

void Timer::resetTimer()
{
    _secondsLeft = _seconds;
    _movesLeft   = _moves;
}

int Timer::timeLeft()
{
    int used = (int)difftime(time(NULL), _startTime);
    return _seconds - used;
}

////////////////////////////////// END OF FILE ////////////////////////////
