// Author(s): Jan Friso Groote
// Copyright: see the accompanying file COPYING or copy at
// https://svn.win.tue.nl/trac/MCRL2/browser/trunk/COPYING
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

/** \file
 *
 * \brief This file contains a class that contains labelled transition systems in aut format.
 * \details A labelled transition system in aut format is a transition system
 * with as state labels vectors of strings, and as transition labels strings.
 * \author Jan Friso Groote
 */


#ifndef MCRL2_LTS_ACTION_LABEL_STRING_H
#define MCRL2_LTS_ACTION_LABEL_STRING_H

#include <algorithm>
#include <string>
#include <vector>
#include <iterator>
#include "mcrl2/utilities/exception.h"
#include "mcrl2/utilities/text_utility.h"

namespace mcrl2
{
namespace lts
{

/** \brief This class contains strings to be used as values for action labels in lts's.
 *  \details These action labels are used in the aut, fsm, and dot format to represent
 *           values for the action labels in transitions.
 */
class action_label_string: public std::string
{
  public:

    /* \brief Default constructor. The label will contain the default string.
     */
    action_label_string()
    {}

    /** \brief Copy constructor. */
    action_label_string(const action_label_string& )=default;

    /** \brief Copy assignment. */
    action_label_string& operator=(const action_label_string& )=default;

    /* \brief A constructor, where the string s is taken to become the action label.
     */
    explicit action_label_string(const std::string& s):std::string(s)
    {}

    /* \brief An auxiliary function to hide actions. Makes a best-effort attempt at
              parsing the string as an mCRL2 multi-action and hiding the actions
              with a name in string_vector. If all actions are hidden, the resulting
              action name will be "tau". The effects of this operation on action 
              labels that are not generated by the pretty printer are undefined. */
    void hide_actions(const std::vector<std::string>& string_vector)
    {
      std::string ns;                    // New label after hiding.
      std::string::iterator b = begin(); // Start position of current action.
      std::string::iterator c = begin(); // End position of current action.
      while (c != end())
      {
        std::string a;
        // Parse action name into a
        while (c != end() && *c != '(' && *c != '|')
        {
          a.append(1, *c++);
        }
        // Skip over parameters of a, if any
        if (c != end() && *c == '(')
        {
          std::size_t nd = 0;
          do 
          {
            switch (*c++)
            {
              case '(': ++nd; break;
              case ')': --nd; break;
            }
          }
          while (nd > 0 && c != end());
        }
        // Add the appropriate action to the new action label string
        if (std::find(string_vector.begin(), string_vector.end(), a) == string_vector.end())
        {
          if (!ns.empty())
          {
            ns.append("|");
          }
          ns.append(b, c);
        }
        // Skip the multi-action operator, if any
        if (c != end() && *c == '|')
        {
          ++c;
        }
        // The start of the next action is pointed to by c.
        b = c;
      }
      if (ns.empty())
      {
        assign(tau_action());
      }
      else
      {
        assign(ns);
      }
    }

    /* \brief A comparison operator comparing the action_label_strings in the same way as strings.
    */
    bool operator<(const action_label_string& l) const
    {
      return std::string(*this)<std::string(l);
    }

    /* \brief The action label string that represents the internal action.
    */
    static const action_label_string& tau_action()
    {
      static action_label_string tau_action("tau");
      return tau_action;
    }
};

/* \brief A pretty print operator on action labels, returning it as a string.
*/
inline std::string pp(const action_label_string& l)
{
  return l;
}

} // namespace lts
} // namespace mcrl2

#endif


