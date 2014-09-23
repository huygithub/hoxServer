#ifndef __OPTIONS_H__
#define __OPTIONS_H__

/*
 * Options.h (c) Noah Roberts
 *
 * The Options class is in change of tracking all definable states within the game.
 * Such states include things such as interface settings, computer color, time limits,
 * max ply, and any other thing we wish to provide settings for.  This class will be
 * passed command line options at the start of the program and will be in charge of
 * parsing those options into settings.  Some commands will alter the settings this
 * class is in charge of, those commands will be passed to this class for processing.
 */
#include	<map>
#include	<list>
#include	<string>
#include	<iostream>

// Observer pattern superclass
class OptionsObserver
{
 public:
  virtual void optionChanged(std::string which)
    { std::cerr << "optionChanged() is a subclass responsibility!!\n"; }
};

class Options
{
 private:
  typedef std::map<std::string, std::string>	OptionMap;
  typedef OptionMap::value_type			OptionPair;
  
  OptionMap	_options;
  
  void loadDefaults();

  static Options	_defaultOptions;
  Options();

  std::list<OptionsObserver*>	observers;
  void dispatchChangeNotice(std::string whatChanged);
  void setOption(std::string optionText);
 public:
  static Options *defaultOptions() { return &_defaultOptions; }
  void decipherCommandArgs(int argc, char **argv);
  std::string getValue(std::string option);
  void setValue(std::string option, std::string value);
  bool isOption(std::string commandText); // if it is an option returns true, also deals with
  // it.

  // Observer pattern
  void addObserver(OptionsObserver *observer);
};

#endif
