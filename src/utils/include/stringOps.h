#include <vector>
#include <string>
#include <utility>

/**
 * @brief Splits a string into substrings based on a delimiter
 * Modifies result vector in place to contain the substrings, avoiding unnecessary copies/constructors/destructors
 * @param str: The string to split
 * @param result: vector that will be modified to contain the substrings
 * @param delim: The delimiter to split the string on
 * 
 */
void split_string(const std::string&, std::vector<std::string_view>&, const char);

/**
 * @brief Splits a string into substrings based on a delimiter
 * @param str: The string to split
 * @param delim: The delimiter to split the string on
 * @return A vector containing the substrings as string_views
 */
std::vector<std::string_view> split_string(const std::string& , const char);


std::vector<std::string> split_string_inefficient(const std::string&&, const char);