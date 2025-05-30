#pragma once

#ifndef STRUTILS_H
#define STRUTILS_H

#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>

// Overload this, later needed by other components
// like DynamicArray_Unsortable
bool operator!(const std::string& str);

// Overload this, later needed by other components
// like DynamicArray_Unsortable
bool operator==(const std::string&str, const std::nullptr_t& nptr);

// Overload this, later needed by other components
// like DynamicArray_Unsortable
bool operator==(const std::nullptr_t& nptr, const std::string&str);

// NOTE: Functions labelled as 'STL' need to 
// be replaced later on, as per Assignment guidelines.
// For now, these are used as emergency solutions.

// Erases the leading whitespace from 'str'
// Warning: this will modify 'str'
void trimLeadingInPlace_STL(std::string &str);

// Erases the trailing whitespace from 'str'
// Warning: this will modify 'str'
void trimTrailingInPlace_STL(std::string &str);

/*
// Returns a new string without the leading (front) whitespace
std::string trimLeading(std::string str);

// Returns a new string without the trailing (back) whitespace  
std::string trimTrailing(std::string str);
*/

#endif // STRUTILS_H