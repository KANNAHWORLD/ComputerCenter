#include <vector>
#include <string>
#include <utility>
#include "stringOps.h"

/**
 * @brief Splits a string into substrings based on a delimiter
 * Modifies result vector in place to contain the substrings, avoiding unnecessary copies/constructors/destructors
 * @param str: The string to split
 * @param result: vector that will be modified to contain the substrings
 * @param delim: The delimiter to split the string on
 * 
 */
void split_string(const std::string& str, std::vector<std::string_view>& result, const char delim = ' ') noexcept{
    if(str.empty())
        return;
    result.resize(0);

    int start = 0;
    int end = 0;
    auto str_data = str.data();
    int val = 0;

    while((end = str.find(delim, start)) != std::string::npos){
        val = end - start;
        if(val > 0) [[likely]]
            result.emplace_back(str_data + start, val);
        start = end + 1;
    }
    result.emplace_back(str_data + start, str.size() - start);
}

/**
 * @brief Splits a string into substrings based on a delimiter
 * @param str: The string to split
 * @param delim: The delimiter to split the string on
 * @return A vector containing the substrings as string_views
 */
[[nodiscard]] std::vector<std::string_view> split_string(const std::string& str, const char delim = ' ') noexcept{
    std::vector<std::string_view> result;
    result.reserve(10);
    split_string(str, result, delim);
    return std::move(result);
}


[[nodiscard]] std::vector<std::string> split_string_inefficient(const std::string& str, const char delim = ' ') noexcept{
    if(str.empty())
        return {};
    std::vector<std::string> result;
    result.reserve(10);
    int start = 0;
    int end = 0;

    while((end = str.find(delim, start)) != std::string::npos){
        if(end-start > 0)
            result.emplace_back(str.data() + start, end - start);
        start = end + 1;
    }
    result.emplace_back(str.data() + start, str.size() - start);
    return std::move(result);
}
