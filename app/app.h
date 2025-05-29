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
#else
#   include <fluent/hashmap/hashmap.h> // fluent_libc
#   include <fluent/vector/vector.h> // fluent_libc
#endif
#include "../value/value.h"

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
    hashmap_t *flags; /**< Flags */
    hashmap_t *commands; /**< Commands */
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
static inline unsigned int cli_new_app(cli_app_t *app)
{
    app->flags = hashmap_new(15, 1.5, NULL, hash_str_key, 0);
    app->commands = hashmap_new(15, 1.5, NULL, hash_str_key, 0);

    // Handle memory allocation failure
    if (!app->flags || !app->commands)
    {
        return 0; // Memory allocation failed
    }

    return 1; // Successfully created the CLI application
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
static inline int cli_has_value(const cli_app_t *app, const char *name)
{
    if (app->flags && name)
    {
        // Check if the name exists in the flags or commands hashmaps
        return
            hashmap_get(app->flags, (void *)name) != NULL
            || hashmap_get(app->commands, (void *)name) != NULL;
    }

    return 0; // Invalid parameters, return false
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
static inline int cli_insert_flag(const cli_app_t *app, const char *flag_name, cli_value_t *value)
{
    if (app && app->flags && flag_name && value)
    {
        // Check if the name already exists in the hashmaps
        if (cli_has_value(app, flag_name))
        {
            // Handle the case where the flag already exists
            return 0; // Flag already exists, do not insert again
        }

        // Insert the flag into the hashmap
        hashmap_insert(app->flags, (void *)flag_name, value);

        // Insert the alias if it exists
        if (value->alias)
        {
            // Check if the alias already exists in the hashmaps
            if (cli_has_value(app, value->alias))
            {
                // Handle the case where the alias already exists
                return 0; // Alias already exists, do not insert again
            }

            // Insert the alias into the hashmap
            hashmap_insert(app->flags, (void *)value->alias, value);
        }
        return 1; // Successfully inserted the flag
    }

    return 0; // Invalid parameters, insertion failed
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
static inline int cli_insert_command(const cli_app_t *app, const char *command_name, cli_value_t *value)
{
    if (app && app->commands && command_name && value)
    {
        // Check if the name already exists in the hashmaps
        if (cli_has_value(app, command_name))
        {
            // Handle the case where the command already exists
            return 0; // Flag already exists, do not insert again
        }

        // Insert the command into the hashmap
        hashmap_insert(app->commands, (void *)command_name, value);

        // Insert the alias if it exists
        if (value->alias)
        {
            // Check if the alias already exists in the hashmaps
            if (cli_has_value(app, value->alias))
            {
                // Handle the case where the alias already exists
                return 0; // Alias already exists, do not insert again
            }

            // Insert the alias into the hashmap
            hashmap_insert(app->commands, (void *)value->alias, value);
        }

        return 1; // Successfully inserted the command
    }

    return 0; // Invalid parameters, insertion failed
}

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
}
#endif

#endif //FLUENT_LIBC_CLI_APP_H
