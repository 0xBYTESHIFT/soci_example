#pragma once
#include <utility>

template <class T> class property {
private:
  T m_val;

public:
  template <class... Args>
  property(Args &&... args) : m_val(std::forward<Args>(args)...) {}

  property()=default;
  ~property()=default;

  const T &get() const { return m_val; }
  T &get() { return m_val; }
  const T &operator*() const { return m_val; }
  T &operator*() { return m_val; }
  template <class Arg> void set(Arg &&val) { m_val = std::forward<Arg>(val); }
};