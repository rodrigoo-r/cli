/*
 * This code is distributed under the terms of the GNU General Public License.
 * For more information, please refer to the LICENSE file in the root directory.
 * -------------------------------------------------
 * Copyright (C) 2025 Rodrigo R.
 * This program comes with ABSOLUTELY NO WARRANTY; for details type show w'.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions; type show c' for details.
*/

// ============= FLUENT LIB C =============
// TODO: Documentation

#ifndef FLUENT_LIBC_CLI_LIBRARY_H
#define FLUENT_LIBC_CLI_LIBRARY_H
#ifndef FLUENT_LIBC_RELEASE
#   include <hashmap.h> // fluent_libc
#else
#   include <fluent/hashmap/hashmap.h> // fluent_libc
#endif
#include "app/app.h"

/*// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
extern "C"
{
#endif*/

/**
 * @brief Structure to hold parsed command-line arguments.
 *
 * This struct organizes command-line flags and commands into separate hashmaps
 * for efficient access and management.
 *
 * Members:
 *   success   - Indicates if parsing was successful (1 for success, 0 for failure).
 *   statics   - Hashmap of static (boolean) flags.
 *   strings   - Hashmap of string flags or commands.
 *   integers  - Hashmap of integer flags or commands.
 *   floats    - Hashmap of float flags or commands.
 *   arrays    - Hashmap of array commands.
 */
typedef struct
{
    int success;          /**< Indicates if parsing was successful */
    hashmap_t *statics;   /**< Static (boolean) flags */
    hashmap_t *strings;   /**< String flags/commands */
    hashmap_t *integers;  /**< Integer flags/commands */
    hashmap_t *floats;    /**< Float flags/commands */
    hashmap_t *arrays;    /**< Array flags/commands */
} argv_t;

inline argv_t parse_argv(const int argc, char **argv, cli_app_t *app)
{
    // Define the structure to hold parsed command-line arguments
    argv_t parsed_args;
    parsed_args.success = 1; // Assume success initially
    parsed_args.statics = hashmap_new(15, 1.5, NULL, hash_str_key, 0);
    parsed_args.strings = hashmap_new(15, 1.5, NULL, hash_str_key, 0);
    parsed_args.integers = hashmap_new(15, 1.5, NULL, hash_str_key, 0);
    parsed_args.floats = hashmap_new(15, 1.5, NULL, hash_str_key, 0);
    parsed_args.arrays = hashmap_new(15, 1.5, NULL, hash_str_key, 0);

    // Check if the hashmaps were created successfully
    if (!parsed_args.statics || !parsed_args.strings || !parsed_args.integers ||
        !parsed_args.floats || !parsed_args.arrays)
    {
        parsed_args.success = 0; // Set success to false if any hashmap creation failed
        return parsed_args;
    }

    // Used to keep track of the parsing status
    // Iterate through the command-line arguments
    for (int i = 0; i < argc; i++)
    {
        char *arg = argv[i];
    }
}

/*// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
}
#endif*/

#endif //FLUENT_LIBC_CLI_LIBRARY_H