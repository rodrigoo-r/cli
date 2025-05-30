/*
    The Fluent Programming Language
    -----------------------------------------------------
    This code is released under the GNU GPL v3 license.
    For more information, please visit:
    https://www.gnu.org/licenses/gpl-3.0.html
    -----------------------------------------------------
    Copyright (c) 2025 Rodrigo R. & All Fluent Contributors
    This program comes with ABSOLUTELY NO WARRANTY.
    For details type `fluent l`. This is free software,
    and you are welcome to redistribute it under certain
    conditions; type `fluent l -f` for details.
*/

//
// Created by rodrigo on 5/30/25.
//

#ifndef FLUENT_LIBC_CLI_STR_CMP_FN_H
#define FLUENT_LIBC_CLI_STR_CMP_FN_H

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
extern "C"
{
#endif
#include <string.h>

/**
 * Compares two C strings for equality.
 *
 * @param a Pointer to the first null-terminated string.
 * @param b Pointer to the second null-terminated string.
 * @return Non-zero if the strings are equal, 0 otherwise.
 */
static inline int cmp_str(const char *a, const char *b)
{
    // Use strcmp to compare the strings
    return strcmp(a, b) == 0;
}

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
}
#endif

#endif //FLUENT_LIBC_CLI_STR_CMP_FN_H
