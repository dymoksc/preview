#ifndef PREVIEW_PARSINGEXCEPTION_H
#define PREVIEW_PARSINGEXCEPTION_H

#include <exception>

namespace web_server {

class ParsingException : public std::invalid_argument {
  using std::invalid_argument::invalid_argument;
};

}

#endif //PREVIEW_PARSINGEXCEPTION_H
