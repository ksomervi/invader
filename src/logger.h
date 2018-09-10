#ifndef LOGGER_H
#define LOGGER_H

#include <string>
using std::string;

#include <iostream>
using std::ostream;

class logger {
 
  private:
    ostream *os;
    static const char *_labels[3];

  public:
    typedef enum log_level_e { NOTE, DEBUG, ERROR } log_level;

  private:
    log_level_e _verbosity;

  protected:
    void _emit(const log_level_e&, string&);

  public:
    //logger();
    logger(log_level =DEBUG);

    void note(string);
    void debug(string);
    void error(string);
};

#endif //!defined(LOGGER_H)
