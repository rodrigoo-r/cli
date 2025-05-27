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
#ifndef FLUENT_LIBC_RELEASE
#   include <hashmap.h> // fluent_libc
#   include <vector.h> // fluent_libc
#else
#   include <fluent/hashmap/hashmap.h> // fluent_libc
#   include <fluent/vector/vector.h> // fluent_libc
#endif

/**
 * @brief Represents the state and configuration of the CLI application.
 *
 * This structure holds all the necessary data for managing command-line
 * arguments, flags, and suggestions in the CLI.
 *
 * Members:
 *   statics     - Hashmap of static (boolean) flags.
 *   strings     - Hashmap of string flags/commands.
 *   integers    - Hashmap of integer flags/commands.
 *   floats      - Hashmap of float flags/commands.
 *   arrays      - Hashmap of array flags/commands.
 *   tree        - Pointer to the root of the command suggestion tree.
 */
typedef struct
{
    hashmap_t *statics;   /**< Static (boolean) flags */
    hashmap_t *strings;   /**< String flags/commands */
    hashmap_t *integers;  /**< Integer flags/commands */
    hashmap_t *floats;    /**< Float flags/commands */
    hashmap_t *arrays;    /**< Array flags/commands */
} cli_app_t;

inline unsigned int cli_new_app(cli_app_t *app)
{
    app->statics = hashmap_new(15, 1.5, NULL, hash_str_key, 0);
    app->strings = hashmap_new(15, 1.5, NULL, hash_str_key, 0);
    app->integers = hashmap_new(15, 1.5, NULL, hash_str_key, 0);
    app->floats = hashmap_new(15, 1.5, NULL, hash_str_key, 0);
    app->arrays = hashmap_new(15, 1.5, NULL, hash_str_key, 0);

    // Handle memory allocation failure
    if (
        !app->statics ||
        !app->strings || !app->integers ||
        !app->floats || !app->arrays
    )
    {
        return 0; // Memory allocation failed
    }

    return 1; // Successfully created the CLI application
}

#endif //FLUENT_LIBC_CLI_APP_H
