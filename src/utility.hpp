#pragma once
#include <type_traits>
#include <utility>

template <typename T, bool = std::is_move_constructible_v<T>>
struct delete_move_constructor {
  delete_move_constructor() = default;
  delete_move_constructor(delete_move_constructor&& other) = default;
  delete_move_constructor(const delete_move_constructor& other) = default;
  delete_move_constructor& operator=(delete_move_constructor&& other) = default;
  delete_move_constructor& operator=(const delete_move_constructor& other) = default;
};

template <typename T>
struct delete_move_constructor<T, false> {
  delete_move_constructor() = default;
  delete_move_constructor(delete_move_constructor&& other) = delete;
  delete_move_constructor(const delete_move_constructor& other) = default;
  delete_move_constructor& operator=(delete_move_constructor&& other) = default;
  delete_move_constructor& operator=(const delete_move_constructor& other) = default;
};

template <typename T, bool = std::is_copy_constructible_v<T>>
struct delete_copy_constructor {
  delete_copy_constructor() = default;
  delete_copy_constructor(delete_copy_constructor&& other) = default;
  delete_copy_constructor(const delete_copy_constructor& other) = default;
  delete_copy_constructor& operator=(delete_copy_constructor&& other) = default;
  delete_copy_constructor& operator=(const delete_copy_constructor& other) = default;
};

template <typename T>
struct delete_copy_constructor<T, false> {
  delete_copy_constructor() = default;
  delete_copy_constructor(delete_copy_constructor&& other) = default;
  delete_copy_constructor(const delete_copy_constructor& other) = delete;
  delete_copy_constructor& operator=(delete_copy_constructor&& other) = default;
  delete_copy_constructor& operator=(const delete_copy_constructor& other) = default;
};

template <typename T, bool = std::is_move_assignable_v<T>>
struct delete_move_assignment {
  delete_move_assignment() = default;
  delete_move_assignment(delete_move_assignment&& other) = default;
  delete_move_assignment(const delete_move_assignment& other) = default;
  delete_move_assignment& operator=(delete_move_assignment&& other) = default;
  delete_move_assignment& operator=(const delete_move_assignment& other) = default;
};

template <typename T>
struct delete_move_assignment<T, false> {
  delete_move_assignment() = default;
  delete_move_assignment(delete_move_assignment&& other) = default;
  delete_move_assignment(const delete_move_assignment& other) = default;
  delete_move_assignment& operator=(delete_move_assignment&& other) = delete;
  delete_move_assignment& operator=(const delete_move_assignment& other) = default;
};

template <typename T, bool = std::is_copy_assignable_v<T>>
struct delete_copy_assignment {
  delete_copy_assignment() = default;
  delete_copy_assignment(delete_copy_assignment&& other) = default;
  delete_copy_assignment(const delete_copy_assignment& other) = default;
  delete_copy_assignment& operator=(delete_copy_assignment&& other) = default;
  delete_copy_assignment& operator=(const delete_copy_assignment& other) = default;
};

template <typename T>
struct delete_copy_assignment<T, false> {
  delete_copy_assignment() = default;
  delete_copy_assignment(delete_copy_assignment&& other) = default;
  delete_copy_assignment(const delete_copy_assignment& other) = default;
  delete_copy_assignment& operator=(delete_copy_assignment&& other) = default;
  delete_copy_assignment& operator=(const delete_copy_assignment& other) = delete;
};
