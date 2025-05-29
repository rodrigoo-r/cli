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
// CLI Parser API
// ----------------------------------------
// A lightweight command-line interface (CLI) parser for C.
//
// Parses command-line arguments into a structured format
// with support for:
//
// - Static (boolean) flags
// - Integer, float, and string flags
// - Array values (multi-value flags)
// - Single command support with arguments
//
// Designed to work with `cli_app_t` definitions containing expected flags/commands.
//
// Features:
// - parse_argv:      Parses argc/argv and returns structured CLI data
// - argv_t:          Struct to hold parsed flags and command info
//
// Function Signatures:
// ----------------------------------------
// argv_t parse_argv(const int argc, char **argv, cli_app_t *app);
//     Example:
//         cli_app_t app = ...; // Define CLI structure
//         argv_t args = parse_argv(argc, argv, &app);
//
// destroy_argv:     Frees memory associated with an argv_t struct
//     Example:
//         destroy_argv(&args); // Free parsed arguments
//
// Structure Details:
// ----------------------------------------
// typedef struct
// {
//     int success;
//     cli_i_value_t command;
//     hashmap_t *statics;
//     hashmap_t *strings;
//     hashmap_t *integers;
//     hashmap_t *floats;
//     hashmap_t *arrays;
// } argv_t;
//
// Notes:
// - You must define your expected CLI schema using `cli_app_t`
// - Uses internal hashmaps to store parsed data
// - Automatically handles flag type matching and type conversion
//
// Dependencies:
// ----------------------------------------
// - fluent/hashmap
// - fluent/atoi
// - fluent/app (cli_app_t, cli_value_t definitions)
//
// ----------------------------------------
// Initial revision: 2025-05-26
// ----------------------------------------

#ifndef FLUENT_LIBC_CLI_LIBRARY_H
#define FLUENT_LIBC_CLI_LIBRARY_H
#ifndef FLUENT_LIBC_RELEASE
#   include <hashmap.h> // fluent_libc
#   include <atoi.h> // fluent_libc
#   include <std_bool.h> // fluent_libc
#else
#   include <fluent/hashmap/hashmap.h> // fluent_libc
#   include <fluent/atoi/atoi.h> // fluent_libc
#   include <fluent/std_bool/std_bool.h> // fluent_libc
#endif
#include "app/app.h"

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
extern "C"
{
#endif

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
    int success;            /**< Indicates if parsing was successful */
    cli_i_value_t command;  /**< Command value */
    hashmap_t *statics;     /**< Static (boolean) flags */
    hashmap_t *strings;     /**< String flags */
    hashmap_t *integers;    /**< Integer flags */
    hashmap_t *floats;      /**< Float flags */
    hashmap_t *arrays;      /**< Array flags */
    cli_value_t *cmd_ptr;   /**< Pointer to the command value in the CLI schema */
} argv_t;

/**
 * @brief Initializes an array value for a CLI array flag or command.
 *
 * This function allocates and initializes a vector to store multiple values
 * for a CLI flag or command of type array. It updates the parsing state flags
 * and assigns the new vector to the provided pointer.
 *
 * @param parsed_args    Pointer to the argv_t struct being populated.
 * @param command        Pointer to the current command value.
 * @param array_values   Output pointer to the vector_t* that will hold array values.
 * @param parsing_array  Output flag set to 1 if parsing an array.
 * @param waiting_value  Output flag set to 1 to indicate a value is expected.
 * @return int           1 on success, 0 on memory allocation failure.
 */
static int argv_t_process_array(
    argv_t *parsed_args,
    const cli_i_value_t *command,
    vector_t **array_values,
    int *parsing_array,
    int *waiting_value
)
{
    // If the flag is an array, initialize the array values vector
    vector_t *vector = (vector_t *)malloc(sizeof(vector_t));

    // Handle memory allocation failure
    if (vector == NULL)
    {
        parsed_args->success = 0; // Set success to false if vector creation failed
        parsed_args->command = *command;
        return 0;
    }

    vec_init(vector, 10, sizeof(char *), 1.5); // Initialize vector with capacity of 10

    *array_values = vector; // Assign the vector to array_values
    *parsing_array = 1; // Set parsing array flag
    *waiting_value = 1; // Set waiting value flag
    return 1;
}

/**
 * @brief Parses command-line arguments according to the provided CLI schema.
 *
 * This function processes the given `argc` and `argv` parameters, matching them
 * against the expected flags and commands defined in the `cli_app_t` schema.
 * It supports static (boolean) flags, integer, float, and string flags, as well as
 * array (multi-value) flags and single command support with arguments.
 *
 * The parsed results are stored in an `argv_t` struct, which contains hashmaps
 * for each flag type and the parsed command value.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line argument strings.
 * @param app  Pointer to the CLI schema definition (`cli_app_t`).
 * @return     An `argv_t` struct containing the parsed arguments and status.
 *
 * @note
 *   - The function assumes the CLI schema is properly defined in `app`.
 *   - Returns `success = 0` in the result if parsing fails at any point.
 *   - Memory for values and vectors is dynamically allocated and should be freed by the caller.
 */
static inline argv_t parse_argv(const int argc, const char **argv, cli_app_t *app)
{
    // Define the structure to hold parsed command-line arguments
    argv_t parsed_args;
    cli_i_value_t command;
    command.float_val = 0.0f; // Initialize float value to 0.0
    command.num_val = 0;      // Initialize integer value to 0
    command.value = NULL;     // Initialize string value to NULL
    command.vec_value = NULL; // Initialize vector value to NULL

    parsed_args.success = 1; // Assume success initially
    parsed_args.statics = hashmap_new(15, 1.5, NULL, (hash_function_t)hash_str_key, 0);
    parsed_args.strings = hashmap_new(15, 1.5, NULL, (hash_function_t)hash_str_key, 0);
    parsed_args.integers = hashmap_new(15, 1.5, NULL, (hash_function_t)hash_str_key, 0);
    parsed_args.floats = hashmap_new(15, 1.5, NULL, (hash_function_t)hash_str_key, 0);
    parsed_args.arrays = hashmap_new(15, 1.5, NULL, (hash_function_t)hash_str_key, 0);
    parsed_args.cmd_ptr = NULL; // Initialize command to NULL

    // Check if the hashmaps were created successfully
    if (!parsed_args.statics || !parsed_args.strings || !parsed_args.integers ||
        !parsed_args.floats || !parsed_args.arrays)
    {
        parsed_args.success = 0; // Set success to false if any hashmap creation failed
        parsed_args.command = command;
        return parsed_args;
    }

    // Used to keep track of the parsing status
    int waiting_value = 0;
    int parsing_command = 0;
    int parsing_array = 0;
    vector_t *array_values = NULL;
    cli_type_t last_type = CLI_TYPE_STATIC; // Initialize last type to static
    cli_type_t command_type = CLI_TYPE_STATIC;
    const char *flag_name = NULL;
    const char *command_name = NULL;

    // Iterate through the command-line arguments
    for (int i = 1; i < argc; i++)
    {
        // Get the current argument
        const char *arg = argv[i];

        // Check if the argument is a flag (starts with '-')
        if (arg[0] == '-')
        {
            // Check if we are waiting for a value
            if (waiting_value && !parsing_array)
            {
                // Handle failure
                parsed_args.success = 0; // Set success to false if we were waiting for a value
                parsed_args.command = command;
                return parsed_args;
            }

            // Handle the case where we are parsing an array
            if (waiting_value && parsing_array)
            {
                // Handle failure
                parsed_args.success = 0;
                return parsed_args;
            }

            parsing_array = 0; // Reset parsing array flag
            // Check if we have a long flag
            if (arg[1] == '-')
            {
                // Update the flag name to the long flag
                flag_name = arg + 2; // Skip the '--'

                // Remove the flag name from the required flags
                hashmap_remove(app->required_flags, (void *)flag_name);
            } else if (arg[1] == '\0') // Handle unexpected single dash
            {
                parsed_args.success = 0; // Set success to false if a single dash is found
                parsed_args.command = command;
                return parsed_args;
            } else
            {
                // Update the flag name to the short flag
                flag_name = arg + 1; // Skip the '-'

                // Handle unexpected values
                if (flag_name[0] == '\0')
                {
                    parsed_args.success = 0; // Set success to false if an empty flag is found
                    parsed_args.command = command;
                    return parsed_args;
                }

                // Remove the flag name from the required flags
                hashmap_remove(app->required_flags, (void *)flag_name);
            }

            // Get the flag from the flag map
            const cli_value_t *flag = (cli_value_t *)hashmap_get(app->flags, (void *)flag_name);

            // Check if the flag exists
            if (flag == NULL)
            {
                // If the flag does not exist, set success to false and return
                parsed_args.success = 0;
                parsed_args.command = command;
                return parsed_args;
            }

            // Check the type of the flag
            if (flag->type == CLI_TYPE_ARRAY)
            {
                if (
                    !argv_t_process_array(
                        &parsed_args,
                        &command,
                        &array_values,
                        &parsing_array,
                        &waiting_value
                    )
                )
                {
                    // Handle failure in processing the array
                    parsed_args.success = 0; // Set success to false if array processing failed
                    parsed_args.command = command;
                    return parsed_args;
                }
                continue;
            }

            // Handle static flags
            if (flag->type == CLI_TYPE_STATIC)
            {
                waiting_value = 0; // Reset waiting value flag

                // Insert the static flag into the statics hashmap
                hashmap_insert(parsed_args.statics, (void *)flag_name, (void *)1);
            }

            // Handle other types of flags
            parsing_command = 0; // Reset parsing command flag
            waiting_value = 1; // Set waiting value flag
            last_type = flag->type; // Update the last type to the current flag type

            // Skip subsequent code
            continue;
        }

        // Check if we are waiting for a value
        if (waiting_value)
        {
            // Handle arrays
            if (parsing_array)
            {
                // Consider the arg to be a value
                vec_push(array_values, (void *)arg);
                continue;
            }

            // Add the value to the map depending on the type
            cli_type_t parse_type;
            cli_i_value_t *value;
            if (parsing_command)
            {
                parse_type = command_type; // Use the command type if parsing a command
            } else
            {
                parse_type = last_type; // Use the last type if not parsing a command

                // Allocate memory for the value if needed
                if (parse_type != CLI_TYPE_STATIC)
                {
                    value = malloc(sizeof(cli_i_value_t)); // Allocate memory for the value

                    // Handle memory allocation failure
                    if (value == NULL)
                    {
                        parsed_args.success = 0; // Set success to false if value allocation failed
                        parsed_args.command = command;
                        return parsed_args;
                    }
                }
            }

            // Parse the value based on the type
            switch (parse_type)
            {
                case CLI_TYPE_INTEGER:
                {
                    // Convert the argument to an integer
                    long int_value = atoi_convert(arg);

                    // Parse commands
                    if (parsing_command)
                    {
                        command.num_val = int_value; // Set the integer value for the command
                        parsing_command = 0; // Reset parsing command flag
                    } else
                    {
                        value->num_val = int_value; // Set the integer value for the flag
                        // Insert the integer value into the integers hashmap
                        hashmap_insert(parsed_args.integers, (void *)command_name, (void *)value);
                    }
                    break;
                }

                case CLI_TYPE_STATIC:
                {
                    // Ensure that we are not parsing a command
                    if (parsing_command)
                    {
                        // Handle failure
                        parsed_args.success = 0; // Set success to false if parsing a command with static flag
                        parsed_args.command = command;
                        return parsed_args;
                    }

                    // Insert the static flag into the statics hashmap
                    hashmap_insert(parsed_args.statics, (void *)command_name, (void *)1);
                    break;
                }

                case CLI_TYPE_ARRAY:
                {
                    // Add the value to the array values vector
                    vec_push(array_values, (void *)arg);
                    break;
                }

                case CLI_TYPE_STRING:
                {
                    // Parse commands
                    if (parsing_command)
                    {
                        command.value = arg; // Set the float value for the command
                        parsing_command = 0; // Reset parsing command flag
                    } else
                    {
                        value->value = arg; // Set the float value for the flag
                        // Insert the float value into the floats hashmap
                        hashmap_insert(parsed_args.floats, (void *)command_name, (void *)value);
                    }

                    break;
                }

                case CLI_TYPE_FLOAT:
                {
                    // Convert the argument to a float
                    float float_value = atof(arg);

                    // Parse commands
                    if (parsing_command)
                    {
                        command.float_val = float_value; // Set the float value for the command
                        parsing_command = 0; // Reset parsing command flag
                    } else
                    {
                        value->float_val = float_value; // Set the float value for the flag
                        // Insert the float value into the floats hashmap
                        hashmap_insert(parsed_args.floats, (void *)command_name, (void *)value);
                    }
                    break;
                }
            }

            waiting_value = parse_type == CLI_TYPE_ARRAY;
            continue;
        }

        // Handle commands
        if (command_name == NULL)
        {
            // Make sure we are not parsing a flag
            if (waiting_value)
            {
                // Handle failure
                parsed_args.success = 0;
                return parsed_args;
            }

            command_name = arg; // Set the command name to the current argument

            // Get the command from the commands map
            cli_value_t *command_value = (cli_value_t *)hashmap_get(app->commands, (void *)command_name);

            // Check if the command exists
            if (command_value == NULL)
            {
                // If the command does not exist, set success to false and return
                parsed_args.success = 0; // Set success to false if command not found
                parsed_args.command = command;
                return parsed_args;
            }

            // Update the last type
            command_type = command_value->type;
            waiting_value = 1;
            parsing_array = command_value->type == CLI_TYPE_ARRAY;
            parsing_command = 1;
            parsed_args.cmd_ptr = command_value; // Set the command name in parsed_args

            // Handle arrays
            if (parsing_array)
            {
                // Process the array command
                if (
                    !argv_t_process_array(
                        &parsed_args,
                        &command,
                        &array_values,
                        &parsing_array,
                        &waiting_value
                    )
                )
                {
                    // Handle failure in processing the array
                    parsed_args.success = 0; // Set success to false if array processing failed
                    parsed_args.command = command;
                    return parsed_args;
                }
            }
        } else
        {
            // Handle unexpected additional commands
            parsed_args.success = 0; // Set success to false if multiple commands are found
            parsed_args.command = command;
            return parsed_args;
        }
    }

    // Make sure we are not waiting for a value at the end
    // and have a command
    if ((waiting_value && !parsing_array) || !command_name || app->required_flags->count != 0)
    {
        // Handle failure
        parsed_args.success = 0; // Set success to false if we were waiting for a value at the end
        parsed_args.command = command;
        return parsed_args;
    }

    // Update the command value
    parsed_args.command = command;
    return parsed_args;
}

/**
 * @brief Frees all memory associated with a hashmap of CLI values.
 *
 * Iterates through all entries in the given hashmap, freeing any dynamically allocated
 * `cli_i_value_t` values, including their associated vectors if present. Finally,
 * frees the hashmap itself.
 *
 * @param map Pointer to the hashmap to be destroyed. If NULL, the function does nothing.
 * @param free_map_values If true, the function will free the values in the hashmap.
 * @param is_impl If true, the function will use cli_i_value_t instead of cli_value_t.
 */
static void destroy_argv_map(hashmap_t *map, const bool free_map_values, const bool is_impl)
{
    if (!map) return;

    // Get an iterator for the hashmap
    hashmap_iter_t iter = hashmap_iter_begin(map);
    hash_entry_t* entry;

    // Iterate through the hashmap entries
    while ((entry = hashmap_iter_next(&iter)) != NULL)
    {
        // Get the value from the entry
        void *value = (cli_i_value_t *)entry->value;
        if (value == NULL)
        {
            continue; // Skip if the value is NULL
        }

        // Handle impl cases
        if (is_impl)
        {
            const cli_i_value_t *cli_value = (cli_i_value_t *)value;
            // Check if we have a vector
            if (cli_value->vec_value)
            {
                // Destroy the vector and free its memory
                vec_destroy(cli_value->vec_value, NULL);
                free(cli_value->vec_value);
            }
        }

        // Free the value itself
        if (free_map_values)
        {
            free(value); // Free the cli_i_value_t structure
        }
    }

    hashmap_free(map); // Free the hashmap itself
}

/**
 * @brief Frees all memory associated with an argv_t struct.
 *
 * This function releases all dynamically allocated memory used by the hashmaps
 * in the given argv_t struct, as well as any vector associated with the command value.
 * After freeing, it resets the success flag to 0.
 *
 * @param args Pointer to the argv_t struct to be destroyed. If NULL, the function does nothing.
 */
static inline void destroy_argv(argv_t *args)
{
    if (args)
    {
        // Free the hashmaps
        destroy_argv_map(args->statics, TRUE, TRUE);
        destroy_argv_map(args->strings, TRUE, TRUE);
        destroy_argv_map(args->integers, TRUE, TRUE);
        destroy_argv_map(args->floats, TRUE, TRUE);
        destroy_argv_map(args->arrays, TRUE, TRUE);

        // Free the command value if it has a vector
        if (args->command.vec_value)
        {
            vec_destroy(args->command.vec_value, NULL);
            free(args->command.vec_value);
        }

        // Reset the success flag
        args->success = 0;
    }
}

/**
 * @brief Frees all memory associated with a cli_app_t struct.
 *
 * This function releases all dynamically allocated memory used by the hashmaps
 * in the given cli_app_t struct, including both commands and flags.
 * If free_map_values is true, the function will also free the values stored in the hashmaps.
 *
 * @param app Pointer to the cli_app_t struct to be destroyed. If NULL, the function does nothing.
 * @param free_map_values If true, the function will free the values in the hashmaps.
 */
static inline void cli_destroy_app(const cli_app_t *app, const bool free_map_values)
{
    if (app)
    {
        // Free the hashmaps
        destroy_argv_map(app->commands, free_map_values, FALSE);
        destroy_argv_map(app->flags, free_map_values, FALSE);
        destroy_argv_map(app->required_flags, FALSE, FALSE);
    }
}

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
}
#endif

#endif //FLUENT_LIBC_CLI_LIBRARY_H