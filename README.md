*This project has been created as part of the 42 curriculum by abignals, trambure.*

## Description

ft_irc is an IRC server written in C++98. It implements the core IRC protocol over TCP/IP, allowing multiple clients to connect simultaneously, authenticate, join channels, and exchange messages in real time.
The server is built around a single poll() call for non-blocking I/O multiplexing. No forking is used.

Supported commands:
- Authentication: PASS, NICK, USER
- Messaging: PRIVMSG, NOTICE
- Channels: JOIN, PART, QUIT
- Operator commands: KICK, INVITE, TOPIC, MODE (modes: i, t, k, o, l)

Commands on IRSSI:
- Authentication: /nick (PASS is required when connecting and User is automatically set by IRSSI)
- Messaging: /msg, /notice
- Channels: /join, /part, /quit
- Operator commands: /invite, /invite, /topic, /mode

## Instructions

Compilation:
make

Usage:
./ircserv <port> <password>

Example:
./ircserv 6667 mypassword


Connecting with netcat:
nc -C 127.0.0.1 6667
PASS mypassword
NICK mynick
USER myuser 0 * :My Real Name
JOIN #general

The -C flag is required with netcat to send proper \r\n line endings as specified by the IRC protocol.

Connecting with irssi:
/connect 127.0.0.1 6667 password

Cleanup:
make fclean

## Resources

- [RFC 1459 — Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
- [RFC 2812 — IRC Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [IRC Wikipedia](https://en.wikipedia.org/wiki/IRC)
- [irssi IRC client](https://irssi.org/)

AI usage:

AI was used during this project for the following tasks:
- Reviewing IRC protocol compliance (numeric reply codes, message format)
