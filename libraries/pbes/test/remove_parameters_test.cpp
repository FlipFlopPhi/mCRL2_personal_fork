// Author(s): Wieger Wesselink
// Copyright: see the accompanying file COPYING.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
/// \file remove_parameters.cpp
/// \brief Tests for removing parameters..

#include <iostream>
#include <boost/test/minimal.hpp>
#include "mcrl2/old_data/data.h"
#include "mcrl2/pbes/pbes.h"
#include "mcrl2/pbes/remove_parameters.h"
#include "mcrl2/pbes/detail/test_utility.h"

using namespace mcrl2::core;
using namespace mcrl2::old_data;
using namespace mcrl2::pbes_system;
using namespace mcrl2::pbes_system::detail;

void test_propositional_variable()
{
  data_variable_list d = make_list(nat("n"), pos("p"), bool_("b"), bool_("c"));
  propositional_variable X = propvar("X", d);
  std::vector<int> v;
  v.push_back(1);
  v.push_back(3);
  propositional_variable X1 = remove_parameters(X, v);
  data_variable_list d1 = make_list(nat("n"), bool_("b"));
  BOOST_CHECK(X1 = propvar("X", d1));
}

void test_propositional_variable_instantiation()
{
  data_expression_list d = make_list(nat("n"), pos("p"), bool_("b"), bool_("c"));
  propositional_variable_instantiation X = propvarinst("X", d);
  std::vector<int> v;
  v.push_back(1);
  v.push_back(3);
  propositional_variable_instantiation X1 = remove_parameters(X, v);
  data_expression_list d1 = make_list(nat("n"), bool_("b"));
  BOOST_CHECK(X1 = propvarinst("X", d1));
}

void test_pbes_expression()
{
  using namespace data_expr;

  data_variable_list d1 = atermpp::make_list(sort_expr::nat(), sort_expr::bool_());
  data_variable_list d2 = atermpp::make_list(sort_expr::nat(), sort_expr::bool_(), sort_expr::nat());
  propositional_variable X1 = propvar("X1", d1);
  propositional_variable X2 = propvar("X2", d2);

  data_expression_list e1 = make_list(plus(nat("m"), nat("n")), bool_("b"));
  data_expression_list e2 = make_list(multiplies(nat("m"), nat("n")), bool_("b"), nat("p"));
  propositional_variable_instantiation x1 = propvarinst("X1", e1); 
  propositional_variable_instantiation x2 = propvarinst("X2", e2); 

  pbes_expression p = pbes_expr::and_(X1, X2);

  std::map<identifier_string, std::vector<int> > to_be_removed;
  std::vector<int> v1;
  v1.push_back(1);
  to_be_removed[X1.name()] = v1;
  std::vector<int> v2;
  v2.push_back(0);
  v2.push_back(2);
  to_be_removed[X2.name()] = v2;

  pbes_expression q = remove_parameters(p, to_be_removed);
  
  pbes_expression r;
  {
    data_variable_list d1 = atermpp::make_list(sort_expr::nat());
    data_variable_list d2 = atermpp::make_list(sort_expr::bool_());
    propositional_variable X1 = propvar("X1", d1);
    propositional_variable X2 = propvar("X2", d2);
    
    data_expression_list e1 = make_list(plus(nat("m"), nat("n")));
    data_expression_list e2 = make_list(bool_("b"));
    propositional_variable_instantiation x1 = propvarinst("X1", e1); 
    propositional_variable_instantiation x2 = propvarinst("X2", e2);
    
    r = pbes_expr::and_(X1, X2);
  } 
  BOOST_CHECK(q == r);
}

int test_main(int argc, char** argv)
{
  MCRL2_ATERMPP_INIT(argc, argv)

  test_propositional_variable_instantiation();

  return 0;
}
