#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN  // in only one cpp file

#include "web_server/RequestParser.h"

#include <boost/test/unit_test.hpp>

#include "web_server/ParsingException.h"

namespace {

BOOST_AUTO_TEST_SUITE(RequestParserTest)

BOOST_AUTO_TEST_CASE(testParsePositive) {
  web_server::RequestParser requestParser;
  const web_server::Request& parsedRequest = requestParser.parse("POST http://localhost:80/abs.html HTTP/1.1\n\npost_data");

  BOOST_CHECK_EQUAL(+web_server::RequestMethod::POST, parsedRequest.method);
  BOOST_CHECK_EQUAL("http://localhost:80/abs.html", parsedRequest.url);
  BOOST_CHECK_EQUAL(+web_server::RequestProtocol::HTTP_1_1, parsedRequest.protocol);
  BOOST_CHECK_EQUAL("post_data", parsedRequest.body);
}

BOOST_AUTO_TEST_CASE(testParseNegativeBadProtocol) {
  web_server::RequestParser requestParser;
  BOOST_CHECK_THROW(requestParser.parse("LOST http://localhost:80/abs.html HTTP/1.1\n\npost_data"), web_server::ParsingException);
}

BOOST_AUTO_TEST_SUITE_END()

}
