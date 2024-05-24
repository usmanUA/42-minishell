
NAME 		:=	minishell
ERRTXT		:=	error.txt
OBJSDIR		:=	build
INCSDIR		:=	include
SRCSDIR		:=	src
DEPSDIR		:=	.deps
LIBFTDIR	:=	libft
LIBVECDIR 	:=	vec
LIBFT		:=	$(LIBFTDIR)/libft.a
LIBVEC		:=	$(LIBVECDIR)/libvec.a

RM			:=	rm -rf
AR			:=	ar -rcs
CC			:=	cc
CFLAGS		:=	-Wall -Werror -Wextra
DEBUGFLAGS	=	-g -fsanitize=address
DEPFLAGS	=	-c -MT $$@ -MMD -MP -MF $(DEPSDIR)/$$*.d
SCREENCLR	:=	printf "\033c"
SLEEP		:=	sleep .1

RL_FLG		:=	-lreadline
RL_LIB		:=	-L ~/.brew/Cellar/readline/8.2.10/lib
RL_INC		:=	-I ~/.brew/Cellar/readline/8.2.10/include

MODULES		:=	main \
				builtins \
				parser \
				exec \
				free \
				syntax_checker \
				init \
				error 

SOURCES 	= 	main.c \
			main_helpers.c \
			print_vecs.c \
			signals.c \
			signals_helpers.c \
			init_structs.c \
			make_linked_list_and_utils.c \
			make_2d_envp.c \
			syntax_check.c \
			syntax_utils.c \
			commands.c \
			files_opening.c \
			files_opening_helpers.c \
			get_line.c \
			next_string.c \
			next_string_helpers.c \
			parsing_begins.c \
			quotes.c \
			redirects.c \
			redirects_helpers.c \
			vars_expansion.c \
			update_2d_envp_array.c \
			builtins.c \
			cd_1.c \
			cd_2.c \
			cd_3.c \
			echo.c \
			env.c \
			exit_1.c \
			exit_2.c \
			exit_3.c \
			export_1.c\
			export_2.c \
			export_3.c \
			pwd.c \
			search_linked_list.c \
			unset_1.c\
			unset_2.c\
			utils.c \
			command_validation_helpers.c \
			execute.c \
			redirections.c	\
			exec_helpers.c	\
			validate_and_execute.c \
			validate_commands.c \
			errors_2.c \
			errors.c \
			free_mem.c \
			free_mem2.c \

SOURCEDIR	:=	$(addprefix $(SRCSDIR)/, $(MODULES))
BUILDDIR	:=	$(addprefix $(OBJSDIR)/, $(MODULES))
DEPENDDIR	:=	$(addprefix $(DEPSDIR)/, $(MODULES))
SRCS		:=	$(foreach file, $(SOURCES), $(shell find $(SOURCEDIR) -name $(file)))
OBJS		:=	$(patsubst $(SRCSDIR)/%.c, $(OBJSDIR)/%.o, $(SRCS))
DEPS		:=	$(patsubst $(SRCSDIR)/%.c, $(DEPSDIR)/%.d, $(SRCS))
INCS	 	:=	$(foreach header, $(INCSDIR), -I $(header))
INCS	 	+=	$(foreach header, $(LIBVECDIR)/$(INCSDIR), -I $(header))
INCS	 	+=	$(foreach header, $(LIBFTDIR)/$(INCSDIR), -I $(header))

F			=	=====================================
B			=	\033[1m
T			=	\033[0m
G			=	\033[32m
V			=	\033[35m
C			=	\033[36m
R			=	\033[31m
Y			=	\033[33m

vpath %.c $(SOURCEDIR)

define cc_cmd
$1/%.o: %.c | $(BUILDDIR) $(DEPENDDIR)
	@if ! $(CC) -g $(INCS) $(RL_INC) $(DEPFLAGS) $$< -o $$@ 2> $(ERRTXT); then \
		printf "$(R)$(B)\nERROR!\n$(F)$(T)\n"; \
		printf "$(V)Unable to create object file:$(T)\n\n"; \
		printf "$(R)$(B)$$@$(T)\n"; \
		printf "$(Y)\n"; sed '$$d' $(ERRTXT); \
		printf "$(R)$(B)\n$(F)\nExiting...$(T)\n"; exit 1 ; \
	else \
		printf "$(C)$(B)☑$(T)$(V) $$<$ \n    $(C)⮑\t$(G)$(B)$$@$(T) \t\n"; \
	fi
endef

all: $(LIBFT) $(LIBVEC) $(NAME)

$(LIBFT):
	@make --quiet -C $(LIBFTDIR) all
	@make title

$(LIBVEC):
	@make --quiet -C $(LIBVECDIR) all
	@make title

$(NAME): $(OBJS)
	@$(CC) -g $(INCS) $(RL_INC) $^ $(LIBVEC) $(LIBFT) $(RL_LIB) $(RL_FLG) -o $@
	@make finish

debug: CFLAGS += $(DEBUGFLAGS)
debug: all

clean:
	@make --quiet -C $(LIBFTDIR) clean
	@make --quiet -C $(LIBVECDIR) clean
	@$(RM) $(OBJSDIR) $(DEPSDIR) $(ERRTXT)

fclean: clean
	@make --quiet -C $(LIBFTDIR) fclean
	@make --quiet -C $(LIBVECDIR) fclean
	@$(RM) $(NAME)

re: fclean all

title:
	@$(SCREENCLR) && printf "\n"
	@printf "$(C)╔╦╗╦╔╗╔╦╔═╗╦ ╦╔═╗╦  ╦$(T)\n"
	@printf "$(C)║║║║║║║║╚═╗╠═╣║╣ ║  ║   by mkorpela$(T)\n"
	@printf "$(C)╩ ╩╩╝╚╝╩╚═╝╩ ╩╚═╝╩═╝╩═╝  & uahmed$(T)\n"
	@printf "$(G)$(B)$(F)\n$(T)\n"

finish:
	@printf "\n$(G)$(B)$(F)$(T)\n"
	@printf "$(C)╔═╗╦╔╗╔╦╔═╗╦ ╦╔═╗╔╦╗        $(V)$(B)$(NAME)$(T)\n"
	@printf "$(C)╠╣ ║║║║║╚═╗╠═╣║╣  ║║$(T)\n"
	@printf "$(C)╚  ╩╝╚╝╩╚═╝╩ ╩╚═╝═╩╝$(T)\n\n"

$(BUILDDIR) $(DEPENDDIR):
	@mkdir -p $@

$(DEPS):
	include $(wildcard $(DEPS))

$(foreach build, $(BUILDDIR), $(eval $(call cc_cmd, $(build))))

.PHONY: all debug clean fclean re title finish
