/*
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>
#include <vector>

// Macro for index to buffer pointer conversion
#define BUFFER_OFFSET(i) (reinterpret_cast<char*>(0 + (i)))

// Vector and matrix operations
struct Vec2 {
    float values[2];
public:
    Vec2 (const float x, const float y){ this->values[0] = x; this->values[1] = y;}
};

// *************
// Debug macros

/*
 * Usage:
 * string s = "foo";
 * DEBUG( "Value of s is '" << s << "'");
 * -> DBG: Value of s is 'foo'
 */
#define DEBUG( msg ) \
    std::cerr << "DBG " << __FILE__ << "(" << __LINE__ << "): " << msg << std::endl

/*
 * Usage:
 * string s = "foo";
 * DEBUGVAR( s );
 * -> DBG: testfile.cpp(20) s = foo
 */
#define DEBUGVAR( var )\
    std::cerr << "DBG " << __FILE__ << "(" << __LINE__ << "): " \
    << #var << " = " << (var) << std::endl

// *************





