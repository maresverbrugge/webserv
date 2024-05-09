# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: fkoolhov <fkoolhov@student.42.fr>            +#+                      #
#                                                    +#+                       #
#    Created: 2023/11/02 16:07:01 by mverbrug      #+#    #+#                  #
#    Updated: 2024/05/09 12:17:44 by fhuisman      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

#========================================#
#========== STANDARD VARIABLES ==========#
#========================================#

SRC_DIR			:=	src
OBJ_DIR			:=	obj
HEAD_DIR		:=	headers

#========================================#
#============= COMPILATION ==============#
#========================================#

CC				=	c++
FLAGS			=	-Wall -Wextra -Werror
CPP_V_FLAG		=	-std=c++17 #! of -std=c++11
COMPILE			=	$(CC) $(FLAGS) $(CPP_V_FLAG)

#========================================#
#=========  GENERAL VARIABLES  ==========#
#========================================#

NAME			=	webserv
SRC 			=	$(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*/*.cpp)
OBJ				=	$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEP				=	$(OBJ:.o=.d)

#========================================#
#=============== INCLUDES ===============#
#========================================#

HEADERS			:= -I $(HEAD_DIR)

#========================================#
#============== RECIPIES  ===============#
#========================================#

all:			$(NAME)

$(NAME):		$(OBJ)
				@$(COMPILE) $(HEADERS) $(OBJ) -o $(NAME)
				@echo "$(P)$(BOLD)======================== DONE COMPILING =========================$(RESET)"
				@echo "\n$(W)~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~$(RESET)\n"
				@echo "$(BOLD)            $(Y)$@ $(G)$@ $(B)$@ $(P)$@ $(R)$@$(RESET)"
				@echo "\n$(W)~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~$(RESET)\n"

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.cpp
				@mkdir -p $(@D)
				@$(COMPILE) $(HEADERS) -c $< -o $@
				@echo "$(B)$(BOLD)COMPILING:  $(RESET)$(B)$(notdir $<)$(RESET)"

clean:
				@rm -rf $(OBJ_DIR)
				@echo "$(C)$(BOLD)CLEANING:   $(RESET)$(C)obj$(RESET)"

fclean:			clean
				@rm -f $(NAME)
				@echo "$(C)$(BOLD)FCLEANING:  $(RESET)$(C)./$(NAME)$(RESET)"

re: 			clean fclean all

gclean:			clean fclean
				@rm -f *.DS_Store ./.DS_Store *.out
				@rm -rf *.dSYM
				@echo "$(C)$(BOLD)DELETED:    $(RESET)$(C)*._DS_Store and *.dSYM files"
				@echo "$(G)$(BOLD)======================== READY TO COMMIT ========================$(RESET)"

.PHONY:			all clean fclean re gclean

-include		$(DEP)

#========================================#
#=============== COLOURS ================#
#========================================#

BOLD      := \033[1m
RESET     := \033[0m
C         := \033[36m
P         := \033[35m
B         := \033[34m
Y         := \033[33m
G         := \033[32m
R         := \033[31m
W         := \033[0m

