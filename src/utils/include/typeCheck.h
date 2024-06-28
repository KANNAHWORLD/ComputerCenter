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

// Checks if a type of template argument is a string or string_view at compile time
template <typename... T2>
struct string_like : std::true_type {};

template <typename T>
struct string_like<T>{
    static constexpr bool value = (
    std::is_same< remove_ref_const_t<T>, std::string>::value || 
    std::is_same<remove_ref_const_t<T>, std::string_view>::value
    );
};

template <typename T, typename... T2>
struct string_like<T, T2...>{
    static constexpr bool value = string_like<T>::value && string_like<T2...>::value;
};

// Returns the size of an array at compile time
template <typename T, int N>
constexpr int array_size(T (&)[N]) { return N;}

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

/**
 * Checks if a container holds a string-like type
 */
template <typename... T>
struct container_holds_string_like : std::true_type {};

template <typename C>
struct container_holds_string_like<C, string_like<typename remove_ref_const_t<C>::value_type>> 
: string_like<typename remove_ref_const_t<C>::value_type>
{};

template <typename C, typename... T>
struct container_holds_string_like<C, T...>
: std::conjunction<string_like<typename remove_ref_const_t<C>::value_type>, 
                                container_holds_string_like<T...>> {};


#endif
