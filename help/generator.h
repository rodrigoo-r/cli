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
// Created by rodrigo on 5/29/25.
//

#ifndef FLUENT_LIBC_CLI_GENERATOR_H
#define FLUENT_LIBC_CLI_GENERATOR_H

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
extern "C"
}
#endif

#ifndef FLUENT_LIBC_RELEASE
#   include <hashmap.h> // fluent_libc
#   include <std_bool.h> // fluent_libc
#   include "string_builder.h" // fluent_libc
#   include "str_pad.h" // fluent_libc
#else
#   include <fluent/hashmap/hashmap.h> // fluent_libc
#   include <fluent/std_bool/std_bool.h> // fluent_libc
#   include <fluent/string_builder/string_builder.h> // fluent_libc
#   include <fluent/str_pad/str_pad.h> // fluent_libc
#endif
#include "../app/app.h"

/**
 * Writes the flags or commands from a hashmap to a string builder, formatting them with padding and descriptions.
 *
 * @param builder       Pointer to the string builder where the output will be written.
 * @param map           Pointer to the hashmap containing the flags or commands.
 * @param is_flag       Boolean indicating whether the values are flags (TRUE) or commands (FALSE).
 * @param padding_size  Number of characters to pad the flag/command names for alignment.
 *
 * The function iterates over the hashmap, formats each entry (including aliases and padding),
 * and appends the formatted string and its description to the builder.
 */
static void write_app_values(
    string_builder_t *builder,
    hashmap_t *map,
    const bool is_flag,
    const size_t padding_size
)
{
    // Iterate over the hashmap
    hashmap_iter_t iter = hashmap_iter_begin(map);
    hash_entry_t *entry;
    while ((entry = hashmap_iter_next(&iter)) != NULL)
    {
        const cli_value_t *value = (cli_value_t *)entry->value;
        if (value)
        {
            if (is_flag)
            {
                write_string_builder(builder, "  --"); // All flags start with "--"
            }
            // Create a new builder for each flag in order to add padding
            string_builder_t flag_builder;
            init_string_builder(&flag_builder, 50, 1.5); // Initial capacity for each flag
            write_string_builder(&flag_builder, entry->key);

            // Write aliases if they exist
            if (value->alias)
            {
                if (is_flag)
                {
                    write_string_builder(&flag_builder, ", -");
                } else
                {
                    write_string_builder(&flag_builder, ", ");
                }

                write_string_builder(&flag_builder, value->alias);
            }

            // Add padding to the flag description
            const char *padded = str_pad(padding_size, collect_string_builder_no_copy(&flag_builder));

            // Write the padded flag to the main builder
            write_string_builder(builder, padded);

            // Write the description of the flag
            write_string_builder(builder, value->description);
            write_string_builder(builder, "\n");

            // Since the string has been copied to the buffer
            // it's now safe to free the flag builder
            destroy_string_builder(&flag_builder);
        }
    }
}

/**
 * Generates a help message for a CLI application.
 *
 * @param app           Pointer to the CLI application structure containing flags and commands.
 * @param name          Name of the application.
 * @param desc          Description of the application.
 * @param padding_size  Number of characters to pad flag/command names for alignment.
 * @return              Pointer to a dynamically allocated string containing the help message,
 *                      or NULL if parameters are invalid. The caller is responsible for freeing
 *                      the returned string.
 */
static inline char *cli_generate_help(
    const cli_app_t *app,
    const char *name,
    const char *desc,
    const size_t padding_size
)
{
    if (!app || !app->flags || !app->commands || !name || !desc)
    {
        return NULL; // Invalid parameters
    }

    // Create a string builder to hold the help message
    string_builder_t builder;
    init_string_builder(&builder, 1024, 1.5); // Initial capacity of 1024 characters

    // Append the application name and description
    write_string_builder(&builder, name);
    write_string_builder(&builder, " - ");
    write_string_builder(&builder, desc);
    write_string_builder(&builder, "\n\nUsage: ");
    write_string_builder(&builder, name);
    write_string_builder(&builder, " [flags...] <command> [flags...] <value> [flags...]\n\n");
    write_string_builder(&builder, "AVAILABLE FLAGS:\n");

    // Write the flags to the builder
    write_app_values(
        &builder,
        app->flags,
        TRUE, // Flags
        padding_size
    );

    // Write the commands to the builder
    write_string_builder(&builder, "\nAVAILABLE COMMANDS:\n");
    write_app_values(
        &builder,
        app->commands,
        FALSE, // Commands
        padding_size
    );

    // Return the generated help message without copying
    return collect_string_builder_no_copy(&builder);
}

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
}
#endif

#endif //FLUENT_LIBC_CLI_GENERATOR_H
