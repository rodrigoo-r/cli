/*
 * This code is distributed under the terms of the GNU General Public License.
 * For more information, please refer to the LICENSE file in the root directory.
 * -------------------------------------------------
 * Copyright (C) 2025 Rodrigo R.
 * This program comes with ABSOLUTELY NO WARRANTY; for details type show w'.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions; type show c' for details.
*/

//
// Created by rodrigo on 5/27/25.
//

#ifndef FLUENT_LIBC_CLI_FLAG_H
#define FLUENT_LIBC_CLI_FLAG_H

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
extern "C"
{
#endif

#ifndef FLUENT_LIBC_RELEASE
#   include <vector.h> // fluent_libc
#   include <std_bool.h> // fluent_libc
#else
#   include <fluent/vector/vector.h> // fluent_libc
#   include <fluent/std_bool/std_bool.h> // fluent_libc
#endif
#include "../type/type.h"

/**
 * @struct cli_value_t
 * @brief Represents a command-line flag or command.
 *
 * This structure holds information about a command-line flag or command, including its name,
 * description, type, and value. The value is stored as a pointer and should be
 * interpreted according to the flag's type.
 *
 * Members:
 *   description : Description of the flag/command.
 *   type        : Type of the flag (see cli_type_t).
 *   value       : Pointer to the value of the flag/command.
 *   alias       : Alias for the flag/command (may be NULL).
 *   required    : Indicates if the flag/command is required (default is FALSE).
 */
typedef struct
{
    char *description;   /**< Description of the flag/command */
    cli_type_t type;     /**< Type of the flag/command (static, string, integer, float, array) */
    char *alias;         /**< Alias for the flag/command (may be NULL) */
    bool required;       /**< Indicates if the flag/command is required (default is FALSE) */
} cli_value_t;

/**
 * @struct cli_i_value_t
 * @brief Represents the internal value(s) of a parsed command-line flag or command.
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
    const char *value;     /**< Pointer to the value of the command (might be NULL) */
    vector_t *vec_value;   /**< Vector of values for array flags/command (might be NULL) */
    long num_val;          /**< Value for integer flags/commands (might be 0) */
    float float_val;       /**< Value for float flags/commands (might be 0.0) */
} cli_i_value_t;

/**
 * @brief Creates and initializes a new cli_value_t structure.
 *
 * @param description  The description of the flag/command.
 * @param type         The type of the flag/command (see cli_type_t).
 * @param alias        The alias for the flag/command (may be NULL).
 * @param required     Indicates if the flag/command is required (default is FALSE).
 * @return             An initialized cli_value_t structure.
 */
static inline cli_value_t cli_new_value(const char *description, const cli_type_t type, const char *alias, const bool required)
{
    cli_value_t value;
    value.description = (char *)description; // Copy the description
    value.type = type;                       // Set the type
    value.alias = (char *)alias;             // Copy the alias
    value.required = required;               // Set the required flag

    return value; // Return the initialized value
}

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
}
#endif

#endif //FLUENT_LIBC_CLI_FLAG_H
