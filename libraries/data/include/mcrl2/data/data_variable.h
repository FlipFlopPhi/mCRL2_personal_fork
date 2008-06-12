// Author(s): Wieger Wesselink
// Copyright: see the accompanying file COPYING or copy at
// https://svn.win.tue.nl/trac/MCRL2/browser/trunk/COPYING
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
/// \file mcrl2/data/data_variable.h
/// \brief The class data_variable.

#ifndef MCRL2_DATA_DATA_VARIABLE_H
#define MCRL2_DATA_DATA_VARIABLE_H

#include <cassert>
#include <string>
#include "mcrl2/atermpp/aterm_list.h"
#include "mcrl2/atermpp/aterm_appl.h"
#include "mcrl2/atermpp/aterm_access.h"
#include "mcrl2/atermpp/aterm_traits.h"
#include "mcrl2/core/identifier_string.h"
#include "mcrl2/data/data_expression.h"

namespace mcrl2 {

namespace data {

///////////////////////////////////////////////////////////////////////////////
// data_variable
/// \brief data variable
///
// DataVarId(<String>, <SortExpr>)
class data_variable: public data_expression
{
  public:
    /// Constructor.
    ///             
    data_variable()
      : data_expression(core::detail::constructDataVarId())
    {}

    /// Constructor.
    ///             
    data_variable(atermpp::aterm_appl t)
     : data_expression(t)
    {
      assert(core::detail::check_rule_DataVarId(m_term));
    }

    /// Constructor for strings like "d:D".
    /// Only works for constant sorts.
    ///
    data_variable(const std::string& s)
    {
      std::string::size_type idx = s.find(':');
      assert (idx != std::string::npos);
      std::string name = s.substr(0, idx);
      std::string type = s.substr(idx+1);
      m_term = reinterpret_cast<ATerm>(core::detail::gsMakeDataVarId(core::detail::gsString2ATermAppl(name.c_str()), mcrl2::data::sort_expression(type)));
    }

    /// Constructor.
    ///             
    data_variable(core::identifier_string name, const sort_expression& s)
     : data_expression(core::detail::gsMakeDataVarId(name, s))
    {}

    /// Constructor.
    ///             
    data_variable(const std::string& name, const sort_expression& s)
     : data_expression(core::detail::gsMakeDataVarId(core::detail::gsString2ATermAppl(name.c_str()), s))
    {}

    /// Returns the name of the data_variable.
    ///
    core::identifier_string name() const
    {
      return atermpp::arg1(*this);
    }

    /// Returns the sort of the data_variable.
    ///
    data::sort_expression sort() const
    {
      return atermpp::arg2(*this);
    }
  };
                                                            
/// \brief singly linked list of data variables
///
typedef atermpp::term_list<data_variable> data_variable_list;

/// \brief Returns true if the term t is a data variable
inline
bool is_data_variable(atermpp::aterm_appl t)
{
  return core::detail::gsIsDataVarId(t);
}

/// \brief Converts a data_variable_list to a data_expression_list.
inline
data_expression_list make_data_expression_list(data_variable_list l)
{
  return ATermList(l);
}

} // namespace data

} // namespace mcrl2

/// \cond INTERNAL_DOCS
MCRL2_ATERM_TRAITS_SPECIALIZATION(mcrl2::data::data_variable)
/// \endcond

#endif // MCRL2_DATA_DATA_VARIABLE_H
