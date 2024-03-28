#pragma once
#include "Common/CommonStandard.hpp"

namespace Zero
{
#define ASSERT(Expression, ...) ErrorIf(!(Expression), ##__VA_ARGS__)

template <typename T>
struct Span
{
  Span() : m_begin(nullptr), m_end(nullptr)
  {
  }
  Span(T* begin, u32 len) : m_begin(begin), m_end(begin + len)
  {
  }
  Span(T* begin, T* end) : m_begin(begin), m_end(end)
  {
  }
  template <int N>
  Span(T (&value)[N]) : m_begin(value), m_end(value + N)
  {
  }
  T& operator[](u32 idx) const
  {
    ASSERT(m_begin + idx < m_end);
    return m_begin[idx];
  }
  operator Span<const T>() const
  {
    return Span<const T>(m_begin, m_end);
  }
  void removePrefix(u32 count)
  {
    ASSERT(count <= length());
    m_begin += count;
  }
  void removeSuffix(u32 count)
  {
    ASSERT(count <= length());
    m_end -= count;
  }
  [[nodiscard]] Span fromLeft(u32 count) const
  {
    ASSERT(count <= length());
    return Span(m_begin + count, m_end);
  }
  [[nodiscard]] Span fromRight(u32 count) const
  {
    ASSERT(count <= length());
    return Span(m_begin, m_end - count);
  }
  T& back()
  {
    ASSERT(length() > 0);
    return *(m_end - 1);
  }
  const T& back() const
  {
    ASSERT(length() > 0);
    return *(m_end - 1);
  }
  bool equals(const Span<T>& rhs)
  {
    bool res = true;
    if (length() != rhs.length())
      return false;
    for (const T& v : *this)
    {
      u32 i = u32(&v - m_begin);
      if (v != rhs.m_begin[i])
        return false;
    }
    return true;
  }

  template <typename F>
  s32 find(const F& f) const
  {
    for (u32 i = 0, c = length(); i < c; ++i)
    {
      if (f(m_begin[i]))
        return i;
    }
    return -1;
  }

  u32 length() const
  {
    return u32(m_end - m_begin);
  }

  T* begin() const
  {
    return m_begin;
  }
  T* end() const
  {
    return m_end;
  }

  T* m_begin;
  T* m_end;
};

struct NewPlaceholder
{
};

template <typename T>
struct Local
{
  ~Local()
  {
    if (obj)
      obj->~T();
  }

  void operator=(const Local&) = delete;

  template <typename... Args>
  void create(Args&&... args)
  {
    ASSERT(!obj);
    obj = new (NewPlaceholder(), mem) T(static_cast<Args&&>(args)...);
  }

  void destroy()
  {
    ASSERT(obj);
    obj->~T();
    obj = nullptr;
  }

  T& operator*()
  {
    ASSERT(obj);
    return *obj;
  }
  T* operator->() const
  {
    ASSERT(obj);
    return obj;
  }
  T* get() const
  {
    return obj;
  }

private:
  alignas(T) u8 mem[sizeof(T)];
  T* obj = nullptr;
};
}

inline void* operator new(size_t, Zero::NewPlaceholder, void* where)
{
  return where;
}
inline void operator delete(void*, Zero::NewPlaceholder, void*)
{
} 

namespace Zero::Gpu
{
template <typename T, u32 count>
constexpr u32 lengthOf(const T (&)[count])
{
  return count;
};
}



#include "Gpu.hpp"