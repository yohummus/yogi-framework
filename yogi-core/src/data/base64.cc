/*
 * This file is part of the Yogi Framework
 * https://github.com/yohummus/yogi-framework.
 *
 * Copyright (c) 2020 Johannes Bergmann.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <src/data/base64.h>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <boost/archive/iterators/transform_width.hpp>
using namespace boost::archive::iterators;

std::string encode_base64(std::string plain) {
  using Iterator = insert_linebreaks<base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>, 76>;

  size_t padding((3 - plain.size() % 3) % 3);
  plain.append(padding, 0);
  std::string encoded(Iterator(plain.begin()), Iterator(plain.end() - static_cast<Iterator::difference_type>(padding)));
  encoded.append(padding, '=');

  return encoded;
}

std::string decode_base64(std::string encoded) {
  using Iterator = transform_width<binary_from_base64<remove_whitespace<std::string::const_iterator>>, 8, 6>;

  encoded.append((4 - encoded.size() % 4) % 4, '=');
  auto padding = std::count(encoded.begin(), encoded.end(), '=');

  try {
    std::replace(encoded.begin(), encoded.end(), '=', 'A');

    std::string plain;
    plain.assign(Iterator(encoded.begin()), Iterator(encoded.end()));
    plain.erase(plain.end() - padding, plain.end());
    return plain;
  } catch (const std::exception&) {
    return {};
  }
}
