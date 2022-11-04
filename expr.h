#pragma once

#include <memory>
#include <string>

class Expr {
public:
  // Pretty-print the expression, surround every binary operation with
  // parenthesis
  virtual std::string prettyPrint() const = 0;

  virtual ~Expr() {}

  class Or;
  class And;
  class Var;
};

class Expr::Or : public Expr {
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;

public:
  Or(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
      : left{std::move(left)}, right{std::move(right)} {}

  std::string prettyPrint() const override {
    return "(" + left->prettyPrint() + "|" + right->prettyPrint() + ")";
  }
};

class Expr::And : public Expr {
  // Add code here for Expr::And
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;

public:
  And(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
      : left{std::move(left)}, right{std::move(right)} {}

  std::string prettyPrint() const override {
    return "(" + left->prettyPrint() + "&" + right->prettyPrint() + ")";
  }
};

class Expr::Var : public Expr {
  // Add code here for Expr::Var
  std::string name;

public:
  Var(std::string name) : name{std::move(name)} {}

  std::string prettyPrint() const override { return name; }
};
