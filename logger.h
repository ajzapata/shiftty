#ifndef LOGGER_INCLUDED
#define LOGGER_INCLUDED

/// For now, logging capabilities are limited to posting messages to stderr

#include <iostream>
#include <cstdlib>
#include <string>

using std::string;

/** Global Constants **/

enum MessageType{ info, warning, error };

/** Global Variables **/

/** Logging Functions **/

/// init_log: Executed on startup (init)
void init_log();

/// log: Write string to main log.
void slog(string text, MessageType t = MessageType::info);

#endif // LOGGER_INCLUDED
