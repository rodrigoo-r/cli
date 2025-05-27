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

#ifndef FLUENT_LIBC_CLI_TYPE_H
#define FLUENT_LIBC_CLI_TYPE_H

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

#endif //FLUENT_LIBC_CLI_TYPE_H
