#ifndef ELIXIR_BASE_STD_FUNCTIONAL
#define ELIXIR_BASE_STD_FUNCTIONAL

#include "elixir/base/config.h"

#include <functional>
#include <type_traits>

namespace elixir {
namespace compat {

#if ELIXIR_CXX17_ENABLED
template <typename F, typename... Args>
using invoke_result = std::invoke_result<F, Args...>;
#else
template <typename F, typename... Args>
using invoke_result = std::result_of<F(Args...)>;
#endif

template <typename F, typename... Args>
using invoke_result_t = typename invoke_result<F, Args...>::type;

}  // namespace compat
}  // namespace elixir

#endif
