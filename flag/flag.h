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
// Created by rodrigo on 5/27/25.
//

#ifndef FLUENT_LIBC_CLI_FLAG_H
#define FLUENT_LIBC_CLI_FLAG_H

#ifndef FLUENT_LIBC_RELEASE
#   include <vector.h> // fluent_libc
#else
#   include <fluent/vector/vector.h> // fluent_libc
#endif
#include "../type/type.h"

/**
 * @struct cli_flag_t
 * @brief Represents a command-line flag.
 *
 * This structure holds information about a command-line flag, including its name,
 * description, type, and value. The value is stored as a pointer and should be
 * interpreted according to the flag's type.
 *
 * Members:
 *   description : Description of the flag.
 *   type        : Type of the flag (see cli_type_t).
 *   value       : Pointer to the value of the flag.
 */
typedef struct
{
    char *description;   /**< Description of the flag */
    cli_type_t type;     /**< Type of the flag (static, string, integer, float, array) */
    void *value;         /**< Pointer to the value of the flag */
} cli_flag_t;

/**
 * @struct cli_i_flag_t
 * @brief Represents the internal value(s) of a parsed command-line flag.
 *
 * This structure is used to store the parsed value(s) of a command-line flag or command.
 * It supports multiple types of values, including string, array, integer, and float.
 *
 * Members:
 *   value      : Pointer to the string value of the flag (may be NULL).
 *   vec_value  : Pointer to a vector of string values for array-type flags/commands (may be NULL).
 *   num_val    : Integer value for integer-type flags/commands (may be 0).
 *   float_val  : Floating-point value for float-type flags/commands (may be 0.0).
 */
typedef struct
{
    char *value;           /**< Pointer to the value of the flag (might be NULL) */
    vector_t *vec_value;   /**< Vector of values for array flags/command (might be NULL) */
    size_t num_val;        /**< Value for integer flags/commands (might be 0) */
    float float_val;       /**< Value for float flags/commands (might be 0.0) */
} cli_i_flag_t;

#endif //FLUENT_LIBC_CLI_FLAG_H
