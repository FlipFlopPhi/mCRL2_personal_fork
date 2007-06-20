// Author(s): Wieger Wesselink
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
/// \file atermpp/list.h
/// \brief Add your file description here.

#ifndef MCRL2_ATERMPP_LIST_H
#define MCRL2_ATERMPP_LIST_H

#include <memory>
#include <list>
#include "atermpp/aterm_traits.h"
#include "protaterm.h"          // IProtectedATerm

namespace atermpp {

template <class T, class Allocator = std::allocator<T> >
class list: public std::list<T, Allocator>, IProtectedATerm
{
  public:
    list()
    {
      ATprotectProtectedATerm(this);
    }

    explicit list(const Allocator& a)
      : std::list<T, Allocator>(a)
    {
      ATprotectProtectedATerm(this);
    }

    explicit list(typename std::list<T, Allocator>::size_type count)
      : std::list<T, Allocator>(count)
    {
      ATprotectProtectedATerm(this);
    }
    
    list(typename std::list<T, Allocator>::size_type count, const T& val)
      : std::list<T, Allocator>(count, val)
    {
      ATprotectProtectedATerm(this);
    }

    list(typename std::list<T, Allocator>::size_type count, const T& val, const Allocator& a)
      : std::list<T, Allocator>(count, val, a)
    {
      ATprotectProtectedATerm(this);
    }

    list(const list& right)
      : std::list<T, Allocator>(right)
    {
      ATprotectProtectedATerm(this);
    }

    template<class InIt>
        list(InIt first, InIt last)
      : std::list<T, Allocator>(first, last)
    {
      ATprotectProtectedATerm(this);
    }

    template<class InIt>
        list(InIt first, InIt last, const Allocator& a)
      : std::list<T, Allocator>(first, last, a)
    {
      ATprotectProtectedATerm(this);
    }

    ~list()
    {
      ATunprotectProtectedATerm(this);
    }

    void ATprotectTerms()
    {
#ifdef ATERM_DEBUG_PROTECTION
std::cout << "atermpp::list.ATprotectTerms() : protecting " << list<T>::size() << " elements" << std::endl;
#endif // ATERM_DEBUG_PROTECTION
      for (typename std::list<T, Allocator>::iterator i = std::list<T, Allocator>::begin(); i != std::list<T, Allocator>::end(); ++i)
      {
        aterm_traits<T>::mark(*i);
      }
    }
};

} // namespace atermpp

#endif // MCRL2_ATERMPP_LIST_H
