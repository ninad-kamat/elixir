# Loops - A revised iteration model for C++

## Introduction

**_Elixir.Loops_** is a revised model for iteration in C++. In this model, we use the humble
_index_ instead of an _iterator_ for looping over a sequence. A similar model is also
used in some other languages such as python and Rust. In my view, this model is far
easier to implement and provides more flexibility as compared to the _iterator_ model.
**_Elixir.Loops_** is a `ranges`-like library written with this new iteration model and
can be used to write composable algorithms over sequences.

### Terminalogy

Here are some terms that we will use to describe this new model.

1. **Range**: A range is the concrete _sequence_ that we want to process.
2. **Loop**: A loop is a construct that adapts a _range_ into this new model.
3. **Index**: An index is the position of an item in a single _loop_ or multiple _loops_.

### Operations

There are primarily four operations that define this model.

1. **`iter`**: This operation takes in a _loop_ and returns the starting _index_ for the
   _loop_.
2. **`valid`**: This operation takes in a _loop_ and an _index_ and returns if the _index_
   is a valid position in the _loop_'s sequence.
3. **`next`**: This operation takes in a _loop_ and an _index_ and returns the next _index_
   in the _loop_'s sequence.
4. **`deref`**: This operation takes in a _loop_ and an _index_ and dereferences the
   position specified by the _index_ in the _loop_'s sequence.

## Motivation

Ever since Sean Parent famously gave us "No raw loops", a lot of work has been done to make
existing C++ STL algorithms composable. The `ranges` framework introduced in C++20 is a key
step in making this possible. All of this is built on top of _iterators_. But if you have
even sat down to implement an iterator, you know it is not the most fun experience. Well,
if you try and implement one of the _range adaptor_ objects in `ranges` it is even worse in
my opinion. Further, such heavy encapsulation of logic in _iterator_ objects can make it
harder for compilers to look through code and optimize or vectorize loops. _Loops_ started
as an attempt at exploring how a `ranges`-like framework would work with the concept on an
`index` over a sequence. The goal is to compare it with ranges and see if this can yield
better code, especially, when applied to multi-dimensional sequences.

## The for loop

With these operations, our `for` loop over a sequence `l` looks like

```cpp
for (auto i = iter(l), valid(l, i), i = next(l, i)) {
    auto& r = deref(l, i);
}
```

Compare it with a simple index based loop over an array `a` of size `N`.

```cpp
for ( int i = 0 /* iter */; i < N /* valid */; i++ /* next */) {
    auto x = a[i]; /* deref */
}
```

Now, do you start seeing how we are modeling the index here?

Over the last few decades, C++ has shifted heavily in favor of `iterator` as it's model
of iteration. Iterators are modeled on top of a pointer. The index-based `for` loop shown
above would look a lot different using pointers.

```cpp
const auto* iter_end = a + N;
for (auto* iter = a; iter != iter_end; ++iter) {
    auto& x = *iter;
}
```

Can you see the subtle difference in these two? In the _iterator_ model, all the logic of
performing the iteration is largely placed in the _iterator_ (pointer-like) object. I have
intentionally defined the `iter_end` object outside the loop here as with _iterators_, you
only have equality comparison with other _iterators_. C++17 did give us the _sentinel_, but
that is just a workaround to the fundamental problem with _iterators_.

On the other hand, with _Loops_, the logic of iteration lies with the _Loop_ itself. This
means an _index_ does not carry much logic. In fact, in most cases it is a POD type like
`std::size_t`. Further, this _index_ can be used over multiple _loops_. Each _loop_
understands how it should react to a particular position in its sequence. This significantly
reduces code for implementation. Unlike `ranges` where you probably implement and `iterator`
and a `view` object, here you just implement the `loop` object.

Another key difference is that in _Loops_, the _index_ is exactly the same for all components
of the pipeline. On the contrary, in ranges, an _iterator_ typically will contain the underlying
_iterator_ and a pointer or reference to the _view_; and the _view_ will also encapsulate the
underlying _view_ object. _Loops_ are far more efficient with the stack. Each _loop_ encapsulates
the underlying _loop_, but since we use simple _index_ type, we do not need to do anything else.

Let us consider an example of how a simple C array can be conceptually thought of as a _loop_

> \***\*NOTE**:\*\* This is not how an actual _loop_ is implemented on C arrays but more of a
> conceptual discussion.

```cpp
template<typename T, std::size_t N>
constexpr auto iter(T (&x)[N]) noexcept -> std::size_t {
    return 0;
}

template<typename T, std::size_t N>
constexpr auto valid(T (&x)[N], std::size_t i) noexcept -> bool {
    return i < N;
}

template<typename T, std::size_t N>
constexpr auto next(T (&x)[N], std::size_t i) noexcept -> std::size_t {
    return ++i;
}

template<typename T, std::size_t N>
constexpr auto deref(T (&x)[N], std::size_t i) noexcept -> T& {
    return x[i];
}
```

The `iter` method just returns a 0 as that is the first _index_ for the array. The `valid` method
just checks the _index_ against the array bounds `N`. The `next` method is a simple increment on
the _index_. And lastly, the `deref` method just returns the item in the array at the _index_.
