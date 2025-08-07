NAME_MINISHELL = minishell

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC_MINISHELL = \
	src/minishell.c src/shell_level.c src/tokens.c src/quotes.c src/control.c src/variables.c src/list_manip.c src/token_utils.c src/signals.c src/expansion.c src/variable_utils.c src/local_vars.c src/directories.c src/export.c src/env.c src/utils.c src/echo.c src/list_management.c src/nodes.c src/cmd.c src/piping.c src/redirect.c src/redirect_utils.c src/validation.c src/children.c src/clear.c src/io.c src/error.c

OBJ_MINISHELL = $(SRC_MINISHELL:.c=.o)

CC = cc
CFLAGS = -g -Wall -Wextra -Werror -Iincludes -I$(LIBFT_DIR)
LDFLAGS = -L$(LIBFT_DIR)
LDLIBS = -lft -lreadline -lhistory

.PHONY: all clean fclean re debug check-deps

all: check-deps $(NAME_MINISHELL)

check-deps:
	@echo "Checking dependencies..."
	@if [ ! -f "$(LIBFT)" ]; then \
		echo "libft not found, will build it..."; \
		make -C $(LIBFT_DIR); \
	fi

$(NAME_MINISHELL): $(OBJ_MINISHELL) $(LIBFT)
	@echo "Linking $(NAME_MINISHELL)..."
	$(CC) $(CFLAGS) $(OBJ_MINISHELL) -o $@ $(LDFLAGS) $(LDLIBS)

src/%.o: src/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBFT_DIR) clean
	rm -f $(OBJ_MINISHELL)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME_MINISHELL)

re: fclean all

debug:
	@echo "=== Debug Information ==="
	@echo "Operating system: $(shell uname -s)"
	@echo "LIBFT exists: $(shell test -f $(LIBFT) && echo 'YES' || echo 'NO')"
	@echo "Git available: $(shell which git >/dev/null 2>&1 && echo 'YES' || echo 'NO')"
	@echo "Source files:"
	@ls -la src/ 2>/dev/null || echo "src/ directory not found"
	@echo "========================"
