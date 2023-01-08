//
// Created by michele on 09.01.23.
//

#include <algorithm>
#include <vector>
#include <string>
#include <sstream>

std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  std::istringstream iss(s);
  std::string item;
  while (std::getline(iss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}