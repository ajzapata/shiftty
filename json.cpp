#include "json.h"

/* C++ Standard Library */

#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cassert>

/* OS-specific Libraries */

#ifndef WIN32
#include <unistd.h>
#endif

/* CURL Libraries (from package "libcurl-devel") */

#include <curl/curl.h>

using namespace std;

/* JSON_item Class Definitions */

JSON_item::JSON_item()
: m_name(""), m_value("")
{}

JSON_item::JSON_item(const JSON_item &other)
: m_name(other.name()), m_value(other.value())
{}

JSON_item::JSON_item(std::string name, std::string value)
: m_name(name), m_value(value)
{}

JSON_item JSON_item::operator=(const JSON_item &src)
{
	/// "operator=" functions >>sets its own value<< before returning itself
	m_name = src.name();
	m_value = src.value();
	return JSON_item(src.name(), src.value());
}

/// name: Returns name of item.
std::string JSON_item::name() const
{
	return m_name;
}

/// value: Returns value of item.
std::string JSON_item::value() const
{
	return m_value;
}

/// setName: Set or change name of item.
/// Returns true if new name is different from the old name (i.e. success).
bool JSON_item::setName(std::string name)
{
	bool diff = (m_name != name) ? true : false;
	m_name = name;

	return diff;
}


/// setValue: Set or change value of item.
/// Returns true if new value is different from the old value (i.e. success)
bool JSON_item::setValue(std::string value)
{
	bool diff = (m_value != value) ? true : false;
	m_value = value;

	return diff;
}

JSON::JSON()
: m_size(0)
{}

JSON::JSON(std::vector<JSON_item> items)
: m_size(items.size()), m_items(items)
{}

const std::vector<JSON_item> JSON::items() const
{
    return m_items; // (const std::vector<JSON_item>*)&
}

JSON_item JSON::getItem(std::string name) const
{
	for (size_t i = 0; i < m_items.size(); i++) {
		string mm_name = m_items[i].name();
		if (!strcmp(m_items[i].name().c_str(), name.c_str()))
			return m_items[i];
	}

	/// Item not found
	return JSON_ITEM_EMPTY;
}

bool JSON::importRaw(std::string rawText) /// NSFL
{
	/// JSON format: enclosed by [nested] brackets; name-value pairs separated
	/// by a colon character ':'; pairs delimited by comma ','; string
	/// values enclosed by quotation-marks (ASCII, i.e. non-fancy).

	/// Assume JSON is correct...
	/// Does not distinguish between groups/dictionaries/etc. (i.e. duplicates)

	char c;
	string buffer_name = "";
	string buffer_value = "";
	vector<JSON_item> buffer;

	bool side_switch = false;
	string c_ignore = "[{\"\'}]\377";

	for (size_t i = 0; i < rawText.length(); i++)
	{
		c = rawText[i];

		/// If colon, switch between buffers and continue
		if (c == ':') {
			side_switch = side_switch ? false : true;
			continue;
		}

		/// if comma, save buffers and reset
		if (c == ',') {
			side_switch = false;
			buffer.insert(buffer.end(), JSON_item(buffer_name, buffer_value));
			buffer_name = "";
			buffer_value = "";
			continue;
		}

		/// Ignore these characters
		if (c_ignore.find(c) != string::npos) continue;

		if (!side_switch)	/// Write to name buffer
			buffer_name += c;
		else				/// Write to value buffer
			buffer_value += c;
	}

	/// Save last item to buffer (since there isn't a comma to reset at)
	buffer.insert(buffer.end(), JSON_item(buffer_name, buffer_value));

	/// Save buffer
	for (size_t i = 0; i < buffer.size(); i++)
		m_items.insert(m_items.end(), buffer[i]);

	return true;
}

#if 0

	/// Mod 4: each increment is first-open/first-closed/sec-open/sec-closed/..
	int qmark_state = 0;

        c = rawText[i];

        if (c == '\"') qmark_state++;

        if (qmark_state == 1) /// first quote open
        {
			if (c != '\"') buffer_name += c;
        }
        else if (qmark_state == 3) /// second quote open (after first-q closed)
        {
			if (c != '\"') buffer_value += c;
        }
        else if (qmark_state == 4) /// second quote closed -> reset
        {
			buffer.insert(buffer.end(), JSON_item(buffer_name, buffer_value));
			buffer_name = "";
			buffer_value = "";
			qmark_state = 0;
        }

#endif // 0
