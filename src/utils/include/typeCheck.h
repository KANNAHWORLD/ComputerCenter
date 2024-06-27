#ifndef TYPE_CHECK_H
#define TYPE_CHECK_H
#include <type_traits>

template <typename T>
using void_t = void;

template <typename T>
using remove_ref_const_t = std::remove_const_t<std::remove_reference_t<T>>;

//checks if a type has a member function begin
template <typename T, typename = void>
struct has_begin : std::false_type {};

template <typename T>
struct has_begin<T, void_t<decltype(std::declval<T>().begin())>> : std::true_type {};

template <typename T, typename = void>
struct has_end : std::false_type {};

template <typename T>
struct has_end<T, void_t<decltype(std::declval<T>().end())>> : std::true_type {};

/**
 * checks if a container has both begin and end iterators
 */
template <typename T>
struct has_iterators {
    static constexpr bool value = has_begin<remove_ref_const_t<T>>::value && has_end<remove_ref_const_t<T>>::value; 
};

/**
 * Checks if a container holds a specific type
 * 
 */
template <typename C, typename T, typename = void>
struct container_holds_type : std::false_type {};

template <typename C, typename T>
struct container_holds_type<C, T, std::void_t<typename remove_ref_const_t<C>::value_type>> {
    static constexpr bool value = std::is_same<typename remove_ref_const_t<C>::value_type, T>::value;
};

template <typename T, typename = void>
struct string_like : std::false_type {};

template <typename T>
struct string_like{
    static constexpr bool value = std::is_same< remove_ref_const_t<T>, std::string>::value || std::is_same<T, std::string_view>::value;
};
#endif
