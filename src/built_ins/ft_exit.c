#include "../include/minishell.h"

/**
 * @brief Validates that the provided exit argument is numeric.
 *
 * Iterates through the characters of the exit argument (command->args[1]) and 
 * verifies that each character is a digit or a valid sign ('-' or '+'). If an 
 * invalid character is found, it prints an error message, sets the global exit 
 * code to 2, and exits the program.
 *
 * @param data Pointer to the minishell data structure.
 * @param command Pointer to the command structure containing the arguments.
 * @param i Initial index for iteration (usually 0).
 */
static void	validate_numeric_argument(t_data *data, t_command *command, int i)
{
	while (command->args[1][i])
	{
		if (!ft_isdigit(command->args[1][i]) && command->args[1][i] != '-'
			&& command->args[1][i] != '+')
		{
			write(STDERR_FILENO, " numeric argument required\n", 27);
			data->g_exit_code = 2;
			exit(data->g_exit_code);
		}
		i++;
	}
}

/**
 * @brief Checks if the numeric exit argument exceeds allowed limits.
 *
 * This function verifies that the numeric argument provided for exit does not 
 * exceed the limits of a 64-bit signed integer. If it does, an error message is 
 * printed, the global exit code is set to 2, and the program exits.
 *
 * @param data Pointer to the minishell data structure.
 * @param command Pointer to the command structure containing the arguments.
 */
static void	check_numeric_limits(t_data *data, t_command *command)
{
	if ((ft_strlen(command->args[1]) == 19 && ft_strncmp(command->args[1],
				"9223372036854775807", 19) > 0)
		|| (ft_strlen(command->args[1]) == 20 && ft_strncmp(command->args[1],
				"-9223372036854775808", 20) > 0)
		|| ft_strlen(command->args[1]) > 21)
	{
		write(STDERR_FILENO, " numeric argument required\n", 27);
		data->g_exit_code = 2;
		exit(data->g_exit_code);
	}
}

/**
 * @brief Sets the exit code based on the provided numeric argument.
 *
 * Converts the argument string to an integer using ft_atoi. If the number is 
 * negative, it is adjusted to fit into the 0-255 range. The resulting number is 
 * then assigned to the global exit code.
 *
 * @param data Pointer to the minishell data structure.
 * @param command Pointer to the command structure containing the arguments.
 */
static void	set_exit_code(t_data *data, t_command *command)
{
	int	num;

	num = ft_atoi(command->args[1]);
	if (num < 0)
		num = 256 + num;
	data->g_exit_code = num;
}

/**
 * @brief Implements the exit built-in command.
 *
 * Prints "exit" to the standard output and processes the provided exit argument 
 * if available. It validates the numeric argument, checks for extra arguments, 
 * sets the exit code, clears the command history, cleans up command and data 
 * structures, and then terminates the program.
 *
 * @param data Pointer to the minishell data structure.
 * @param command Pointer to the command structure containing the arguments.
 */
void	ft_exit(t_data *data, t_command *command)
{
	int	i;

	i = 0;
	write(STDOUT_FILENO, "exit\n", 5);
	if (command->args[1])
	{
		validate_numeric_argument(data, command, i);
		check_numeric_limits(data, command);
		if (command->args[2])
		{
			write(STDERR_FILENO, " too many arguments\n", 20);
			data->g_exit_code = 1;
			return ;
		}
		set_exit_code(data, command);
	}
	rl_clear_history();
	clean_cmd(command);
	clean_data(data);
	exit(data->g_exit_code);
}
