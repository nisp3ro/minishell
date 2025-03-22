# 📚 Minishell  100/100

The aim of this project is to build a minimal shell that replicates some basic functionalities of a bash.  
This project not only implements standard shell operations but also includes a fun hidden Easter Egg that activates under certain conditions.

## Installation & Compilation

To compile Minishell and generate the executable, run:

```sh
make
```

This will compile the source files and generate the `minishell` executable, which you can run from your terminal.

### Cleaning Up

- `make clean` – Removes object files.
- `make fclean` – Removes object files, the `minishell` executable, and the secret folder (if cloned).
- `make re` – Fully recompiles the project.

## Usage

To run Minishell, simply execute the generated binary:

```sh
./minishell
```

Once running, you can interact with your shell much like bash.

## Built-ins

Minishell includes several built-in commands that allow you to interact with the shell environment without launching separate processes. These built-ins are implemented directly within the shell and include:

- **cd**: Change the current working directory.
- **echo**: Display a line of text.
- **env**: Print the current environment.
- **exit**: Exit the shell.
- **export**: Set environment variables.
- **unset**: Remove environment variables.
- **pwd**: Print the current working directory.
- **history**: Displays the history.

Each built-in command is carefully implemented to mimic the behavior of its Unix counterpart while handling edge cases and errors gracefully. This integration allows for faster execution and more control over the shell environment.

## Secret Easter Egg

Minishell includes a hidden Easter Egg feature designed to intercept dangerous commands. In the Makefile, a special "Secret" section automatically clones a private repository containing extra configurations and scripts. This repository is used to trigger a playful response if a user attempts to execute a fork bomb (`:(){ :|:& };:`).

### How It Works

The Makefile contains the following logic:

```makefile
# Secret
SECRET_URL	=		"https://github.com/..."
SECRET		=		.secret

@if [ ! -d "$(SECRET)" ]; then \
	git clone $(SECRET_URL) $(SECRET) > /dev/null 2>&1; \
fi
```

- **Checks if the `.secret` folder exists:**  
  If the folder is missing, it indicates that the secret repository has not been cloned yet.

- **Clones the secret repository:**  
  Runs `git clone` to fetch the repository from `$(SECRET_URL)` and stores it in the `.secret` directory. The output is suppressed with `> /dev/null 2>&1`.

- **Deletion on Cleanup:**  
  Running `make fclean` removes the `.secret` folder along with other generated files, ensuring the secret is re-cloned on the next build.

### The Hidden Easter Egg 🐣

This secret repository contains a hidden surprise that is triggered when a user attempts to execute a fork bomb (`:(){ :|:& };:`) inside Minishell. Instead of allowing the dangerous command to crash the system, Minishell intercepts the attempt and displays a custom message or behavior as a playful Easter Egg.  
The secret repository may include additional scripts, assets, or configurations that enhance this feature—adding a fun twist for users who try to exploit the shell with dangerous commands! 😈🚀

## Acknowledgements
I would like to express my sincere gratitude to my colleague [jvidal-t](https://github.com/Flingocho)
 for his invaluable help on this project. His support and guidance were essential to the success of Minishell.

## License

This project is part of the 42 School curriculum.
