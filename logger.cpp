#include "logger.h"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

/// init_log: Executed on startup (init)
void init_log()
{
    /// Nothing to do here yet
}

/// slog: Write string to main log.
void slog(string text, MessageType t)
{
    string prefix;
    switch (t)
    {
		case MessageType::info:
			prefix = "[ INFO ]";
			break;
		case MessageType::warning:
			prefix = "[ WARN ]";
			break;
		case MessageType::error:
			prefix = "[ ERR  ]";
			break;
		default:
			cerr << "ERR: LOG MSG NOT PREFIXED" << endl;
			exit(-1);
    }
    prefix += ' ';

	cerr << prefix + text << endl;
}
