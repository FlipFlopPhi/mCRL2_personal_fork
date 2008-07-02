// Author(s): Wieger Wesselink
// Copyright: see the accompanying file COPYING or copy at
// https://svn.win.tue.nl/trac/MCRL2/browser/trunk/COPYING
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
/// \file mcrl2/lps/constelm.h
/// \brief Add your file description here.

#ifndef MCRL2_LPS_CONSTELM_H
#define MCRL2_LPS_CONSTELM_H

#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <vector>
#include "mcrl2/old_data/data.h"
#include "mcrl2/old_data/replace.h"
#include "mcrl2/old_data/rewriter.h"
#include "mcrl2/lps/specification.h"
#include "mcrl2/lps/detail/remove_parameters.h"

namespace mcrl2 {

namespace lps {

/// Returns zero or more constant process parameters of the process p with initial state init.
/// The result is returned as a map m that maps the constant parameters to their
/// constant value.
template <typename Rewriter>
std::map<old_data::data_variable, old_data::data_expression> compute_constant_parameters(const linear_process& p, old_data::data_expression_list init, Rewriter& r)
{
  using namespace old_data::data_expr;
  
  std::map<old_data::data_variable, old_data::data_expression> replacements;
  old_data::data_variable_list::iterator i = p.process_parameters().begin();
  old_data::data_expression_list::iterator j = init.begin();
  for ( ; i != p.process_parameters().end(); ++i, ++j)
  {
    replacements[*i] = r(*j);
  }

  bool has_changed;
  do
  {
    has_changed = false;
    for (summand_list::iterator i = p.summands().begin(); i != p.summands().end(); ++i)
    {
      old_data::data_expression rc = r(old_data::data_variable_map_replace(i->condition(), replacements));
      if (rc == false_())
      {
        continue;
      }

      for (old_data::data_assignment_list::iterator j = i->assignments().begin(); j != i->assignments().end(); ++j)
      {
        std::map<old_data::data_variable, old_data::data_expression>::iterator k = replacements.find(j->lhs());
        if (k != replacements.end())
        {
          old_data::data_expression gj = old_data::data_variable_map_replace(j->rhs(), replacements);
          if (r(or_(not_(rc), not_equal_to(k->second, gj))) == true_())
          {
            replacements.erase(k);
            has_changed = true;
          }
        }
      }
      if (has_changed)
      {
        break;
      }
    }
  } while (has_changed == true);

  return replacements;
}

/// Returns zero or more constant process parameters of the process p with initial state init.
/// The result is returned as a map m that maps the constant parameters to their
/// constant value.
std::map<old_data::data_variable, old_data::data_expression> compute_constant_parameters_subst(const linear_process& p, old_data::data_expression_list init, old_data::rewriter& r)
{
  using namespace old_data::data_expr;
  namespace opt = old_data::data_expr::optimized;
  
  typedef std::map<old_data::data_variable, std::list<old_data::rewriter::substitution>::iterator> index_map;

  // create a mapping from process parameters to initial values
  std::map<old_data::data_variable, old_data::data_expression> replacements;
  old_data::data_expression_list::iterator j = init.begin();
  for (old_data::data_variable_list::iterator i = p.process_parameters().begin(); i != p.process_parameters().end(); ++i, ++j)
  {
    replacements[*i] = *j;
  }

  // put the substitutions in a list, and make an index for it
  std::list<old_data::rewriter::substitution> substitutions;
  index_map index;
  for (std::map<old_data::data_variable, old_data::data_expression>::iterator i = replacements.begin(); i != replacements.end(); ++i)
  {
    substitutions.push_back(old_data::rewriter::substitution(r, i->first, i->second));
    index[i->first] = --substitutions.end();
  }

  bool has_changed;
  do
  {
    has_changed = false;
    for (summand_list::iterator i = p.summands().begin(); i != p.summands().end(); ++i)
    {
      old_data::data_expression rc = r(i->condition(), substitutions);

      if (rc == false_())
      {
        continue;
      }
      for (old_data::data_assignment_list::iterator j = i->assignments().begin(); j != i->assignments().end(); ++j)
      {
        index_map::iterator k = index.find(j->lhs());
        if (k != index.end())
        {
          old_data::data_expression d  = j->lhs();  // process parameter
          old_data::data_expression g  = j->rhs();  // assigned value
          old_data::data_expression x = opt::or_(opt::not_(rc), not_equal_to(d, g));
          if (r(x, substitutions) == true_())
          {
            replacements.erase(d);
            substitutions.erase(index[d]);
            index.erase(k);
            has_changed = true;
          }
        }
      }
      if (has_changed)
      {
        break;
      }
    }
  } while (has_changed == true);

  return replacements;
}

/// Removes zero or more constant parameters from the specification p.
template <typename Rewriter>
specification constelm(const specification& spec, Rewriter& r, bool verbose = false)
{
  using core::pp;

  std::map<old_data::data_variable, old_data::data_expression> replacements = compute_constant_parameters_subst(spec.process(), spec.initial_process().state(), r);
  std::set<old_data::data_variable> constant_parameters;
  for (std::map<old_data::data_variable, old_data::data_expression>::iterator i = replacements.begin(); i != replacements.end(); ++i)
  {
	  constant_parameters.insert(i->first);
  }

  if (verbose)
  {
    std::cout << "Removing the constant process parameters: ";
    for (std::set<old_data::data_variable>::iterator i = constant_parameters.begin(); i != constant_parameters.end(); ++i)
    {
      std::cout << pp(*i) << " ";
    }
    std::cout << std::endl;
  }

  specification result = repair_free_variables(spec);
  result = detail::remove_parameters(result, constant_parameters);

  // N.B. The replacements may only be applied to the process and the initial process!
  linear_process new_process   = old_data::data_variable_map_replace(result.process(), replacements);
  process_initializer new_init = old_data::data_variable_map_replace(result.initial_process(), replacements);
  result = set_lps(result, new_process);
  result = set_initial_process(result, new_init);

  assert(result.is_well_typed());
  return result;
}

} // namespace lps

} // namespace mcrl2

#endif // MCRL2_LPS_CONSTELM_H
