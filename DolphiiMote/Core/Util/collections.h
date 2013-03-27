// Copyright 2013 Max Malmgren

// This file is part of DolphiiMote.

// DolphiiMote is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// DolphiiMote is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with DolphiiMote.  If not, see <http://www.gnu.org/licenses/>.

#include <vector>
#include <algorithm>

template <typename T, typename K>
std::vector<std::pair<T, K>> pair_filter(std::vector<std::pair<T, K>> input, std::function<bool(T)> filter_func)
{
  std::vector<std::pair<T, K>> result;

  for(auto el : input)
  {
    if(filter_func(el.first))
      result.push_back(el);
  }

  return result;
}

template <typename T, typename K>
std::vector<K> take_second(std::vector<std::pair<T, K>> input)
{
  std::vector<K> result;

  for(auto el : input)
    result.push_back(el.second);

  return result;
}

template <typename T, typename K = u32>
class checked_array
{
public:
  checked_array(T *data, K size) : data(data), _size(size), _valid(true)
  { }  

  checked_array sub_array(size_t sub_offset, K sub_size)
  {
    if(sub_offset > 0 && sub_offset + sub_size < _size)
      return checked_array(data + sub_offset, sub_size);

    return checked_array();
  }

  const T operator[] (K index)
  {
    if(_size > index)
      return data[index];
    else return 0;
  }

  K size()
  {
    return _size;
  }

  bool valid()
  {
    return _valid;
  }

private:
  T *data;
  K _size;
  const bool _valid;

  checked_array() : data(nullptr), _size(0), _valid(false)
  { }  
};

template <typename T>
class optional
{
public:
  optional(T value) : value(value), valid(true)
  {  }

  optional() : value(), valid(false)
  { }

  operator bool()
  {
    return valid;
  }

  void set(T value)
  {
    this->value = value;
  }

  void invalidate()
  {
    valid = false;
  }

  T& val()
  {
    return value;
  }

private:
  bool valid;
  T value;
};