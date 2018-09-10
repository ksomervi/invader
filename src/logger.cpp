#include "logger.h"

using std::endl;

/*
logger::logger() {
  _verbosity = DEBUG;
}
*/
    
const char *logger::_labels[] = {"NOTE", "DEBUG", "ERROR"};

logger::logger(log_level l) {
  _verbosity = l;

  os = &std::cerr;
  /*_labels = new char*[3];
  _labels[NOTE] = "NOTE";
  _labels[DEBUG] = "DEBUG";
  _labels[ERROR] = "ERROR";
  */
}

void logger::_emit(const log_level_e &severity, string &msg) {
  *os << _labels[severity] << ": " << msg << endl;
}

void logger::debug(string msg) {
  if (_verbosity <= DEBUG) {
    _emit(DEBUG, msg);
  }
}

void logger::note(string msg) {
  if (_verbosity == NOTE) {
    _emit(NOTE, msg);
  }
}

void logger::error(string msg) {
  _emit(ERROR, msg);
}

