#pragma once

#include "expr.h"
#include <memory>
#include <string>
#include <vector>

class no_matching_expression {}; // exception class

namespace {
typedef std::vector<std::unique_ptr<Expr> (*)(const std::string &, size_t &)>
    Functions;
std::unique_ptr<Expr> oneOf(const std::string &input, size_t &curr,
                            const Functions &functions) {
  std::unique_ptr<Expr> ret = nullptr;
  size_t idx;
  for (auto f : functions) {
    idx = curr; // reset

    try {
      ret = f(input, idx); // try
    } catch (no_matching_expression) {
      continue;
    }

    curr = idx;
    return ret;
  }

  throw no_matching_expression{};
}
char matchOne(const std::string &input, size_t &curr,
              const std::string &targets) {
  size_t idx = targets.find(input[curr]);
  if (idx == std::string::npos) {
    throw no_matching_expression{};
  }
  curr += 1;
  return targets[idx];
}

std::unique_ptr<Expr> Or(const std::string &input, size_t &curr);
std::unique_ptr<Expr> And(const std::string &input, size_t &curr);
std::unique_ptr<Expr> Var(const std::string &input, size_t &curr);

std::unique_ptr<Expr> Or(const std::string &input, size_t &curr) {
  auto left = oneOf(input, curr, Functions{And, Var});
  matchOne(input, curr, "|");
  auto right = oneOf(input, curr, Functions{Or, And, Var});

  return std::make_unique<Expr::Or>(std::move(left), std::move(right));
}

std::unique_ptr<Expr> And(const std::string &input, size_t &curr) {
  auto left = oneOf(input, curr, Functions{Var});
  matchOne(input, curr, "&");
  auto right = oneOf(input, curr, Functions{Or, And, Var});

  return std::make_unique<Expr::And>(std::move(left), std::move(right));
}

std::unique_ptr<Expr> Var(const std::string &input, size_t &curr) {
  std::string name;
  while (curr < input.size() && isalpha(input[curr])) {
    name += input[curr];
    curr += 1;
  }

  return std::make_unique<Expr::Var>(std::move(name));
}
}; // namespace

std::unique_ptr<Expr> parse(std::string input) {
  // strip whitepace
  std::string clean;
  clean.reserve(input.size());
  std::istringstream iss{input};
  char c;
  while (iss >> c) {
    clean += c;
  }

  size_t curr = 0;
  auto ret = oneOf(clean, curr, Functions{Or, And, Var});
  return ret;
}
