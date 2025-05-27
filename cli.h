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

/*// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
extern "C"
{
#endif*/

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

/**
 * @brief Structure to hold parsed command-line arguments.
 *
 * This struct organizes command-line flags and commands into separate hashmaps
 * for efficient access and management.
 *
 * Members:
 *   statics   - Hashmap of static (boolean) flags.
 *   strings   - Hashmap of string flags or commands.
 *   integers  - Hashmap of integer flags or commands.
 *   floats    - Hashmap of float flags or commands.
 *   arrays    - Hashmap of array commands.
 */
typedef struct
{
    hashmap_t *statics;   /**< Static (boolean) flags */
    hashmap_t *strings;   /**< String flags/commands */
    hashmap_t *integers;  /**< Integer flags/commands */
    hashmap_t *floats;    /**< Float flags/commands */
    hashmap_t *arrays;    /**< Array flags/commands */
} argv_t;

/*// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
}
#endif*/

#endif //FLUENT_LIBC_CLI_LIBRARY_H