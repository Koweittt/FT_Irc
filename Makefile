NAME        = ircserv
CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -I inc

INC_DIR     = inc
SRC_DIR     = src
OBJ_DIR     = obj

SRCS        = $(SRC_DIR)/main.cpp \
              $(SRC_DIR)/Server.cpp \
              $(SRC_DIR)/Client.cpp \
              $(SRC_DIR)/Channel.cpp \
              $(SRC_DIR)/commands/join.cpp \
              $(SRC_DIR)/commands/privmsg.cpp \
              $(SRC_DIR)/commands/kick.cpp \
              $(SRC_DIR)/commands/invite.cpp \
              $(SRC_DIR)/commands/topic.cpp \
              $(SRC_DIR)/commands/mode.cpp

OBJS        = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
