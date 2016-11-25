#ifndef JSON_INCLUDED
#define JSON_INCLUDED

#include <string>
#include <vector>

class JSON_item
{
public:
	JSON_item();
	JSON_item(const JSON_item &other); /// <----------------------- THIS
	JSON_item(std::string name, std::string value);

	JSON_item operator=(const JSON_item &src);
	bool operator==(const JSON_item &i) {
		return (this->name() == i.name() && this->value() == i.value());
	}
	bool operator!=(const JSON_item &i) {
		return (this->name() != i.name() || this->value() != i.value());
	}

	std::string name() const;
	std::string value() const;
	bool isEmpty() const;

	bool setName(std::string name);
	bool setValue(std::string value);

	// std::string rawText() const;

private:
	std::string m_name;
	std::string m_value;
	// std::string m_raw;
};

const JSON_item JSON_ITEM_EMPTY = JSON_item("", "");

class JSON
{
public:
	JSON();
	JSON(std::vector<JSON_item> items);
	// JSON(string raw_text);

	const std::vector<JSON_item> items() const;

	JSON_item getItem(std::string name) const;

	// std::string exportRaw() const;
	// bool exportFile(std::string filename, FILE* filePtr = nullptr);
	bool importRaw(std::string rawText);

	// bool insertItem(JSON_item item);

private:
	int m_size;
	std::vector<JSON_item> m_items;
	// std::string m_raw;
};

#endif // JSON_INCLUDED
