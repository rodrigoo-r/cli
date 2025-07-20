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

#ifndef FLUENT_LIBC_CLI_TYPE_H
#define FLUENT_LIBC_CLI_TYPE_H

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @enum cli_type_t
 * @brief Enumerates the types of command-line arguments supported.
 *
 * This enum defines the possible types of flags and commands that can be parsed:
 * - CLI_TYPE_STATIC:   Static (boolean) flag.
 * - CLI_TYPE_STRING:   String flag or command.
 * - CLI_TYPE_INTEGER:  Integer flag or command.
 * - CLI_TYPE_FLOAT:    Float flag or command.
 * - CLI_TYPE_ARRAY:    Array flag or command.
 */
typedef enum
{
    CLI_TYPE_STATIC,   /**< Static (boolean) flag */
    CLI_TYPE_STRING,   /**< String flag/command */
    CLI_TYPE_INTEGER,  /**< Integer flag/command */
    CLI_TYPE_FLOAT,    /**< Float flag/command */
    CLI_TYPE_ARRAY,    /**< Array flag/command */
} cli_type_t;

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
}
#endif

#endif //FLUENT_LIBC_CLI_TYPE_H
