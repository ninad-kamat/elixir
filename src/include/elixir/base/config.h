#ifndef ELIXIR_BASE_CONFIG
#define ELIXIR_BASE_CONFIG

#include <limits>

#ifndef __cplusplus
#error "C++ is required"
#else
#include <cstddef>
#include <utility>
#endif

#if defined(_MSVC_LANG)
#define ELIXIR_CXX_VERSION _MSVC_LANG
#else
#define ELIXIR_CXX_VERSION __cplusplus
#endif

#define ELIXIR_CXX11_ENABLED (ELIXIR_CXX_VERSION >= 201103L)
#define ELIXIR_CXX14_ENABLED (ELIXIR_CXX_VERSION >= 201402L)
#define ELIXIR_CXX14_ENABLED (ELIXIR_CXX_VERSION >= 201402L)
#define ELIXIR_CXX17_ENABLED (ELIXIR_CXX_VERSION >= 201703L)
#define ELIXIR_CXX20_ENABLED (ELIXIR_CXX_VERSION >= 202002L)

#if !ELIXIR_CXX11_ENABLED
#error "At least C++11 is needed"
#endif

#if defined(__has_cpp_attribute)
#define ELIXIR_HAS_CXX_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
#define ELIXIR_HAS_CXX_ATTRIBUTE(x) 0
#endif

#if defined(__has_builtin)
#define ELIXIR_HAS_BUILTIN(x) __has_builtin(x)
#else
#define ELIXIR_HAS_BUILTIN(x) 0
#endif

#if defined(__has_feature)
#define ELIXIR_HAS_FEATURE(x) __has_feature(x)
#else
#define ELIXIR_HAS_FEATURE(x) 0
#endif

#endif
