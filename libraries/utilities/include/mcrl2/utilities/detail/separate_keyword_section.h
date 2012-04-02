// Author(s): Wieger Wesselink
// Copyright: see the accompanying file COPYING or copy at
// https://svn.win.tue.nl/trac/MCRL2/browser/trunk/COPYING
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
/// \file mcrl2/utilities/detail/separate_keyword_section.h
/// \brief add your file description here.

#ifndef MCRL2_UTILITIES_DETAIL_SEPARATE_KEYWORD_SECTION_H
#define MCRL2_UTILITIES_DETAIL_SEPARATE_KEYWORD_SECTION_H

#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "mcrl2/utilities/text_utility.h"

namespace mcrl2 {

namespace utilities {

namespace detail {

  // Separates all sections with a keyword from the other keyword sections
  // Returns a pair containing consisiting of the keyword section and the other keyword sections
  inline
  std::pair<std::string, std::string> separate_keyword_section(const std::string& text1, const std::string& keyword, const std::vector<std::string>& all_keywords)
  {
    std::string text = boost::trim_copy(text1);
    std::ostringstream out1; // will contain the keyword sections
    std::ostringstream out2; // will contain the other keyword sections

    std::string regex_keyword = "\\b" + keyword + "\\b";
    // create a regex that looks like this: "(\\beqn\\b)|(\\bcons\\b)|(\\bmap\\b)|(\\bvar\\b)"
    std::vector<std::string> v = all_keywords;
    v.erase(std::remove(v.begin(), v.end(), keyword), v.end()); // erase keyword from v
    for (std::vector<std::string>::iterator i = v.begin(); i != v.end(); ++i)
    {
      *i = "(\\b" + *i + "\\b)";
    }
    std::string regex_other_keywords = boost::algorithm::join(v, "|");

    std::vector<std::string> specs = utilities::regex_split(text, regex_keyword);
    if (text.find(keyword) != 0 && !specs.empty())
    {
      out2 << specs.front() << std::endl;
      specs.erase(specs.begin());
    }
    for (std::vector<std::string>::iterator i = specs.begin(); i != specs.end(); ++i)
    {
      // strip trailing map/cons/var/eqn declarations
      std::vector<std::string> v = utilities::regex_split(*i, regex_other_keywords);
      if (!v.empty())
      {
        out1 << "  " << v.front();
        out2 << i->substr(v.front().size());
      }
    }
    std::string s1 = out1.str();
    if (!s1.empty())
    {
      s1 = keyword + "\n" + s1;
    }
    return std::make_pair(s1 + "\n", out2.str() + "\n");
  }

} // namespace detail

} // namespace utilities

} // namespace mcrl2

#endif // MCRL2_UTILITIES_DETAIL_SEPARATE_KEYWORD_SECTION_H
