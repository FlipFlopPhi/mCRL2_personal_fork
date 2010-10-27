// Author(s): Muck van Weerdenburg, Jan Friso Groote
// Copyright: see the accompanying file COPYING or copy at
// https://svn.win.tue.nl/trac/MCRL2/browser/trunk/COPYING
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

/** \file
 *
 * \brief This include file contains routines to read and write 
 *        labelled transitions from and to files and from streams.
 * \details This is the LTS library's main header file. It declares all publicly
 * accessible data structures that form the interface of the library.
 * \author Muck van Weerdenburg
 */

#ifndef MCRL2_LTS_LTS_IO_H
#define MCRL2_LTS_LTS_IO_H

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "boost/iterator/transform_iterator.hpp"

#include "aterm2.h"
#include "mcrl2/atermpp/set.h"
#include "mcrl2/atermpp/vector.h"
#include "mcrl2/atermpp/container_utility.h"

#include "mcrl2/lts/transition.h"
#include "mcrl2/exception.h"
#include "mcrl2/lts/lts_fsm.h"


namespace mcrl2
{

  namespace lps
  {
    class specification;
  } 


  namespace lts
  {
    namespace detail
    {
      /** \brief Add an mCRL2 data specification, parameter list and action
       *         specification to a mCRL2 LTS in SVC format.
       * \param[in] filename   The file name of the mCRL2 LTS.
       * \param[in] data_spec  The data specification to add in mCRL2 internal
       *                       format (or NULL for none).
       * \param[in] params     The list of state(/process) parameters in mCRL2
       *                       internal format (or NULL for none).
       * \param[in] act_spec   The action specification to add in mCRL2 internal
       *                       format (or NULL for none).
       * \pre                  The LTS in filename is a mCRL2 SVC without extra
       *                       information. */
      void add_extra_mcrl2_lts_data(std::string const &filename, const ATermAppl data_spec, const ATermList params, const ATermList act_spec);

      lts_type detect_type(std::string const& filename);
      lts_type detect_type(std::istream &is);

      lps::specification const& empty_specification();


      /** \brief Determines the LTS format from a filename by its extension.
       * \param[in] s The name of the file of which the format will be
       * determined.
       * \return The LTS format based on the extension of \a s.
       * If the extension is \p svc then the mCRL2 SVC format is assumed and
       * \a lts_lts is returned.
       * If no supported format can be determined from the extension then \a
       * lts_none is returned.  */
      lts_type guess_format(std::string const& s);

      /** \brief Determines the LTS format from a format specification string.
       * \details This can be any of the following strings:
       * \li "aut" for the Ald&eacute;baran format;
       * \li "mcrl" or "svc+mcrl" for the muCRL SVC format;
       * \li "mcrl2" or "svc+mcrl2" for the mCRL2 SVC format;
       * \li "svc" for the SVC format;
       * \li "fsm" for the FSM format;
       * \li "dot" for the GraphViz format;
       * \li "bcg" for the BCG format (only available if the LTS library is built
       * with BCG support).
       *
       * \param[in] s The format specification string.
       * \return The LTS format based on the value of \a s.
       * If no supported format can be determined then \a lts_none is returned.
       */
      lts_type parse_format(std::string const& s);

      /** \brief Gives a string representation of an LTS format.
       * \details This is the "inverse" of \ref parse_format.
       * \param[in] type The LTS format.
       * \return The name of the LTS format specified by \a type. */
      std::string string_for_type(const lts_type type);

      /** \brief Gives the filename extension associated with an LTS format.
       * \param[in] type The LTS format.
       * \return The filename extension of the LTS format specified by \a type.
       */
      std::string extension_for_type(const lts_type type);

      /** \brief Gives the MIME type associated with an LTS format.
       * \param[in] type The LTS format.
       * \return The MIME type of the LTS format specified by \a type.
       */
      std::string mime_type_for_type(const lts_type type);

      /** \brief Gives the set of all supported LTS formats.
       * \return The set of all supported LTS formats. */
      const std::set<lts_type> &supported_lts_formats();

      /** \brief Gives a textual list describing supported LTS formats.
       * \param[in] default_format The format that should be marked as default
       *                           (or \a lts_none for no default).
       * \param[in] supported      The formats that should be considered
       *                           supported.
       * \return                   A string containing lines of the form
       *                           "  'name' for the ... format". Every line
       *                           except the last is terminated with '\n'. */
      std::string supported_lts_formats_text(lts_type default_format = lts_none, const std::set<lts_type> &supported = supported_lts_formats());

      /** \brief Gives a textual list describing supported LTS formats.
       * \param[in] supported      The formats that should be considered
       *                           supported.
       * \return                   A string containing lines of the form
       *                           "  'name' for the ... format". Every line
       *                           except the last is terminated with '\n'. */
      std::string supported_lts_formats_text(const std::set<lts_type> &supported);

      /** \brief Gives a list of extensions for supported LTS formats.
       * \param[in] sep       The separator to use between each extension.
       * \param[in] supported The formats that should be considered supported.
       * \return              A string containing a list of extensions of the
       *                      formats in \a supported, separated by \a sep.
       *                      E.g. "*.aut,*.lts" */
      std::string lts_extensions_as_string(const std::string &sep = ",", const std::set<lts_type> &supported = supported_lts_formats());

      /** \brief Gives a list of extensions for supported LTS formats.
       * \param[in] supported The formats that should be considered supported.
       * \return              A string containing a list of extensions of the
       *                      formats in \a supported, separated by \a ','.
       *                      E.g. "*.aut,*.lts" */
      std::string lts_extensions_as_string(const std::set<lts_type> &supported);

    }
  }
}

#endif // MCRL2_LTS_LTS_IO_H
