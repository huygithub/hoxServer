/*
 * Options.cpp (c) Noah Roberts 2003-03-01
 */

#include "Options.h"
#include "Timer.h"

#include <cstring>
#include <sstream>  // ... istringstream

using namespace std;

Options Options::_defaultOptions;

Options::Options()
{
    // loadDefaults(); classes themselves will deal with defaults!
}

void Options::loadDefaults()
{
  _options.insert(OptionPair("interface", "user"));
  _options.insert(OptionPair("computerColor", "b"));
  _options.insert(OptionPair("searchPly", "4"));
}

void Options::decipherCommandArgs(int argc, char **argv)
{
  for (int i = 1; i < argc; i++)
    {
      if (!strcmp(argv[i], "-cxboard"))
        setValue("interface", "cxboard");
      else if (!strcmp(argv[i], "-red"))
        setValue("computerColor", "r");
      else if (!strcmp(argv[i], "-blue") || !strcmp(argv[i], "-black"))
        setValue("computerColor", "b");
      else if (!strcmp(argv[i], "-nobook"))
        setValue("useOpeningBook", "n");
      else if (!strcmp(argv[i], "-depth"))
        {
          if (argc < i+1); // do an error report here
          else
            {
              bool valid = true;
              i++;
              
              for (size_t j = 0; j < strlen(argv[i]) && valid == true; j++)
                if (!isdigit(argv[i][j]))
                    valid = false;
              if (valid == true) setValue("searchPly", argv[i]);
            }
        }
      else if (!strcmp(argv[i], "-ts"))
        /*
         * Transposition table size.  Set size of transposition table to
         * 2^argv[i+1].  IE supplied argument is the number of bits of the
         * key to use for index to table.
         */
        {
          if (argc < i+1);
          else
            {
              bool valid = true;
              i++;
              for (size_t j = 0; j < strlen(argv[i]) && valid == true; j++)
                if (!isdigit(argv[i][j]))
                    valid = false;
              if (valid == true) setValue("tableSize", argv[i]);
            }
        }
    }
}

string Options::getValue(string option)
{
  OptionMap::iterator it = _options.find(option);
  if (it == _options.end()) return "";
  else return it->second;
}

void Options::setValue(string option, string value)
{
  OptionMap::iterator it = _options.find(option);
  if (it == _options.end())
    _options.insert(OptionPair(option,value));
  else
    it->second = value;
  dispatchChangeNotice(option);
}

void Options::dispatchChangeNotice(string whatOption)
{
  for (list<OptionsObserver*>::iterator it = observers.begin();
       it != observers.end(); it++)
    (*it)->optionChanged(whatOption); // observers will now load option if they want.
}

void Options::addObserver(OptionsObserver *observer)
{
  observers.push_back(observer);
}

bool Options::isOption(string commandText)
{
  std::istringstream commandInputStream( commandText );

  string command;
  commandInputStream >> command;

  if (command == "cxboard")
    {
      setValue("interface", "cxboard");
    }
  else if (command == "console")
    {
      setValue("interface", "user");
    }
  else if (command == "red")
    {
      setValue("computerColor", "b");
    }
  else if (command == "blue" || command == "black")
    {
      setValue("computerColor", "r");
    }
  else if (command == "force")
    {
      setValue("computerColor", "n"); // NOCOLOR
    }
  else if (command == "sd")
    {
      string searchDepth;
      commandInputStream >> searchDepth;
      setValue("searchPly", searchDepth);
    }
  else if (command == "post") setValue("post", "on");
  else if (command == "nopost") setValue("post", "off");
  else if (command == "st")
    {
      string x;
      commandInputStream >> x;
      if (commandInputStream)
        {
          //char b[8];
          //sprintf(b, "%d", x);
          //setValue("st", b);
          setValue("st", x);
        }
    }
  else if (command == "level")
    {
    }
  /*  else if (command == "quiescence")
    {
      string onOff;
      commandInputStream >> onOff;
      setValue("quiesc", onOff);
      }*/
  else if (command == "set")
    setOption(commandText.substr(4));
  else return false;
  return true;
}

void Options::setOption(string optionText)
{
  std::istringstream optionReader( optionText );

  string option;
  string value;

  optionReader >> option;
  if (!optionReader.eof())
    optionReader >> value;

  setValue(option, value);
}
