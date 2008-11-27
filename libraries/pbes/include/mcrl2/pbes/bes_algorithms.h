// Author(s): Wieger Wesselink
// Copyright: see the accompanying file COPYING or copy at
// https://svn.win.tue.nl/trac/MCRL2/browser/trunk/COPYING
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
/// \file mcrl2/pbes/bes_algorithms.h
/// \brief Algorithms for boolean equation systems.

#ifndef MCRL2_PBES_ALGORITHMS_H
#define MCRL2_PBES_ALGORITHMS_H

#include "mcrl2/pbes/pbes_expression_with_variables.h"
#include "mcrl2/data/identifier_generator.h"
#include "mcrl2/data/enumerator.h"
#include "mcrl2/data/rewriter.h"
#include "mcrl2/pbes/rewriter.h"
#include "mcrl2/pbes/pbes2bes.h"
#include "mcrl2/pbes/gauss_elimination.h"

namespace mcrl2 {

namespace pbes_system {

/// Function to solve a bes equation.
template <typename PbesRewriter>
struct bes_equation_solver
{
  PbesRewriter& m_rewriter;
  
  bes_equation_solver(PbesRewriter& rewriter)
    : m_rewriter(rewriter)
  {}
   
  pbes_equation operator()(pbes_equation e)
  {
    pbes_equation result = gauss::substitute(e, e.variable(), gauss::sigma(e));
    pbes_expression t = m_rewriter(result.formula());
    result = pbes_equation(result.symbol(), result.variable(), t);
    return result;
  }
};

/// Solves a boolean equation system using Gauss elimination.
/// \precondition The pbes p is a bes.
/// \return 0 if false, 1 if true, 2 if unknown
template <typename Container>
int bes_gauss_elimination(pbes<Container>& p)
{
  typedef data::data_enumerator<number_postfix_generator> my_enumerator;
  typedef enumerate_quantifiers_rewriter<pbes_expression_with_variables, data::rewriter, my_enumerator> my_rewriter;
  typedef bes_equation_solver<my_rewriter> bes_solver;
  typedef typename core::term_traits<pbes_expression> tr;
    
  data::rewriter datar(p.data());
  number_postfix_generator name_generator;
  my_enumerator datae(p.data(), datar, name_generator);
  my_rewriter pbesr(datar, datae);    
  bes_solver solver(pbesr);

  gauss_elimination_algorithm<my_rewriter, bes_solver> algorithm(pbesr, solver);
  algorithm.run(p.equations().begin(), p.equations().end());

  if (tr::is_false(p.equations().front().formula()))
  {
    return 0;
  }
  else if (tr::is_true(p.equations().front().formula()))
  {
    return 1;
  }
  else
  {
    return 2;
  }
}

pbes<> pbes2bes(const pbes<>& pbes_spec, bool lazy = false)
{
  typedef data::data_enumerator<number_postfix_generator> my_enumerator;
  typedef enumerate_quantifiers_rewriter<pbes_expression_with_variables, data::rewriter, my_enumerator> my_rewriter;
  data::rewriter datar(pbes_spec.data());
  number_postfix_generator name_generator;
  my_enumerator datae(pbes_spec.data(), datar, name_generator);
  my_rewriter pbesr(datar, datae);    
  if (lazy)
  {
    return do_lazy_algorithm(pbes_spec, pbesr);
  }
  else
  {
    return do_finite_algorithm(pbes_spec, pbesr);
  }
}

} // namespace pbes_system

} // namespace mcrl2

#endif // MCRL2_PBES_ALGORITHMS_H
