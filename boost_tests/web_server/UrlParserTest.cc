#include "web_server/UrlParser.h"

#include <boost/test/unit_test.hpp>

namespace {

BOOST_AUTO_TEST_SUITE(UrlParserTest)

BOOST_AUTO_TEST_CASE(testPositive) {
  web_server::UrlParser urlParser;
  BOOST_CHECK_EQUAL("abs.html", urlParser.getPath("http://localhost:80/abs.html"));
  BOOST_CHECK_EQUAL("abs.html", urlParser.getPath("http://localhost:80/abs.html"));
  BOOST_CHECK_EQUAL("abs.html", urlParser.getPath("http://localhost:80/abs.html#olol"));
  BOOST_CHECK_EQUAL("abs.html", urlParser.getPath("http://localhost:80/abs.html?1=2"));
  BOOST_CHECK_EQUAL("abs.html%20aa", urlParser.getPath("http://localhost.asd/abs.html%20aa"));
  BOOST_CHECK_EQUAL("", urlParser.getPath("regex101.com/"));
  BOOST_CHECK_EQUAL("", urlParser.getPath("regex101.com"));
}

BOOST_AUTO_TEST_SUITE_END()

}
