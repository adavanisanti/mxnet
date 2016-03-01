/**
 * Copyright (C) 2015 Dato, Inc.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 */
#include <string>
#include <vector>
#include <unity/lib/gl_sarray.hpp>
#include <unity/lib/gl_sframe.hpp>
#include <unity/lib/toolkit_function_macros.hpp>
using namespace graphlab;

void sarray_callback(graphlab::gl_sarray input, size_t callback_addr,
                     size_t begin, size_t end) {
  typedef bool(*callback_type)(const flexible_type*);
  callback_type callback_fun = (callback_type)(callback_addr);
  auto ra = input.range_iterator(begin, end);
  auto iter = ra.begin();
  while (iter != ra.end()) {
    bool success = callback_fun(&(*iter));
    if (!success) log_and_throw("Error applying callback");
    ++iter;
  }
}

void sframe_callback(graphlab::gl_sframe input, size_t callback_addr,
                     size_t begin, size_t end) {
  ASSERT_MSG(input.num_columns() > 0, "SFrame has no column");
  typedef bool(*callback_type)(const flexible_type*, size_t);
  callback_type callback_fun = (callback_type)(callback_addr);
  auto ra = input.range_iterator(begin, end);
  auto iter = ra.begin();
  while (iter != ra.end()) {
    auto& row_vec = (*iter);
    bool success = callback_fun(&(row_vec[0]), row_vec.size());
    if (!success) log_and_throw("Error applying callback");
    ++iter;
  }
}

BEGIN_FUNCTION_REGISTRATION
REGISTER_FUNCTION(sarray_callback, "input", "callback_addr");
REGISTER_FUNCTION(sframe_callback, "input", "callback_addr");
END_FUNCTION_REGISTRATION
