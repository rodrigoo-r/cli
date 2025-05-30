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

#ifndef FLUENT_LIBC_CLI_APP_H
#define FLUENT_LIBC_CLI_APP_H

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
extern "C"
{
#endif

#ifndef FLUENT_LIBC_RELEASE
#   include <hashmap.h> // fluent_libc
#   include <vector.h> // fluent_libc
#   include <std_bool.h> // fluent_libc
#else
#   include <fluent/hashmap/hashmap.h> // fluent_libc
#   include <fluent/vector/vector.h> // fluent_libc
#   include <fluent/std_bool/std_bool.h> // fluent_libc
#endif
#include "../value/value.h"
#include "../shared/str_cmp_fn.h"

#ifndef FLUENT_LIBC_CLI_HASHMAP_VALUE
    DEFINE_HASHMAP(char *, cli_value_t *, cli_value)
#   define FLUENT_LIBC_CLI_HASHMAP_VALUE 1
#endif

/**
 * @brief Represents the state and configuration of the CLI application.
 *
 * This structure holds all the necessary data for managing command-line
 * arguments, flags, and suggestions in the CLI.
 *
 * Members:
 *   flags      - A hashmap containing static (boolean) flags.
 *   commands   - A hashmap containing commands and their associated data.
 */
typedef struct
{
    hashmap_cli_value_t *flags; /**< Flags */
    hashmap_cli_value_t *commands; /**< Commands */

    // INTERNALS
    hashmap_cli_value_t *required_flags; /**< Flags */
} cli_app_t;

/**
 * @brief Initializes a new CLI application state.
 *
 * Allocates and initializes the internal hashmaps for flags and commands
 * in the provided `cli_app_t` structure.
 *
 * @param app Pointer to a `cli_app_t` structure to initialize.
 * @return 1 on success, 0 if memory allocation fails.
 */
static inline bool cli_new_app(cli_app_t *app)
{
    app->flags = hashmap_cli_value_new(
        15,
        1.5,
        NULL,
        (hash_cli_value_function_t)hash_str_key,
        (hash_cli_value_cmp_t)cmp_str
    );

    app->commands = hashmap_cli_value_new(
        15,
        1.5,
        NULL,
        (hash_cli_value_function_t)hash_str_key,
        (hash_cli_value_cmp_t)cmp_str
    );

    app->required_flags = hashmap_cli_value_new(
        15,
        1.5,
        NULL,
        (hash_cli_value_function_t)hash_str_key,
        (hash_cli_value_cmp_t)cmp_str
    );

    // Handle memory allocation failure
    if (!app->flags || !app->commands)
    {
        return FALSE; // Memory allocation failed
    }

    return TRUE; // Successfully created the CLI application
}

/**
 * @brief Checks if a given name exists in the CLI application's flags or commands.
 *
 * This function searches for the specified name in both the flags and commands
 * hashmaps of the provided CLI application state. It returns non-zero if the
 * name is found in either hashmap, and zero otherwise.
 *
 * @param app Pointer to the CLI application state.
 * @param name The name to search for in the flags and commands.
 * @return Non-zero if the name exists, zero otherwise.
 */
static inline bool cli_has_value(const cli_app_t *app, char *name)
{
    if (app->flags && name)
    {
        cli_value_t **value = hashmap_cli_value_get(app->flags, name);
        cli_value_t **value2 = hashmap_cli_value_get(app->commands, name);
        // Check if the name exists in the flags or commands hashmaps
        return
            (value != NULL && *value != NULL)
            || (value2 != NULL && *value2 != NULL);
    }

    return FALSE; // Invalid parameters, return false
}

/**
 * @brief Inserts a flag into the CLI application's flags hashmap.
 *
 * Adds a new flag and its associated value to the application's flags.
 *
 * @param app Pointer to the CLI application state.
 * @param flag_name Name of the flag to insert.
 * @param value Pointer to the value associated with the flag.
 */
static inline bool cli_insert_flag(const cli_app_t *app, char *flag_name, cli_value_t *value)
{
    if (app && app->flags && flag_name && value)
    {
        // Check if the name already exists in the hashmaps
        if (cli_has_value(app, flag_name))
        {
            // Handle the case where the flag already exists
            return FALSE; // Flag already exists, do not insert again
        }

        // Insert the flag into the hashmap
        hashmap_cli_value_insert(app->flags, flag_name, value);

        // Insert the alias if it exists
        if (value->alias)
        {
            // Check if the alias already exists in the hashmaps
            if (cli_has_value(app, value->alias))
            {
                // Handle the case where the alias already exists
                return FALSE; // Alias already exists, do not insert again
            }

            // Insert the alias into the hashmap
            hashmap_cli_value_insert(app->flags, value->alias, value);
        }

        // Insert to the required map if needed
        if (value->required)
        {
            hashmap_cli_value_insert(app->required_flags, flag_name, value);
        }

        return TRUE; // Successfully inserted the flag
    }

    return FALSE; // Invalid parameters, insertion failed
}

/**
 * @brief Inserts a command into the CLI application's commands hashmap.
 *
 * Adds a new command and its associated value to the application's commands.
 *
 * @param app Pointer to the CLI application state.
 * @param command_name Name of the command to insert.
 * @param value Pointer to the value associated with the command.
 */
static inline bool cli_insert_command(const cli_app_t *app, char *command_name, cli_value_t *value)
{
    if (app && app->commands && command_name && value)
    {
        // Check if the name already exists in the hashmaps
        if (cli_has_value(app, command_name))
        {
            // Handle the case where the command already exists
            return FALSE; // Flag already exists, do not insert again
        }

        // Insert the command into the hashmap
        hashmap_cli_value_insert(app->commands, command_name, value);

        // Insert the alias if it exists
        if (value->alias)
        {
            // Check if the alias already exists in the hashmaps
            if (cli_has_value(app, value->alias))
            {
                // Handle the case where the alias already exists
                return FALSE; // Alias already exists, do not insert again
            }

            // Insert the alias into the hashmap
            hashmap_cli_value_insert(app->commands, value->alias, value);
        }

        return TRUE; // Successfully inserted the command
    }

    return FALSE; // Invalid parameters, insertion failed
}

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
}
#endif

#endif //FLUENT_LIBC_CLI_APP_H
