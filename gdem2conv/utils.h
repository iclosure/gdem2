#ifndef UTILS_H_
#define UTILS_H_

#include <vector>
#include <string>

// for file/folder
bool folder_exists(const std::string &path);
void get_files(const std::string &path, std::vector<std::string> &files, 
	const std::string &prefix = std::string());
_fsize_t get_file_size(const std::string &file);
bool create_path(const std::string &path);
// for std::string
bool string_startswith(const std::string &str, const std::string &header);
bool string_endswith(const std::string &str, const std::string &tail);
std::string string_section(const char *ch, char sep, int start = 0, int end = -1);
std::string string_section(const std::string &str, char sep, int start = 0, int end = -1);
std::string string_section(const char *ch, const std::string &sep,
	int start = 0, int end = -1);
std::string string_section(const std::string &str, const std::string &sep,
	int start = 0, int end = -1);
void string_split(const std::string &str, const std::string &delim,
	std::vector<std::string> &ret, bool keepEmptyParts = false);
std::string string_trim(const std::string &str);
std::string &string_replace(std::string &str, const std::string &old_str,
	const std::string &new_str);

#endif // UTILS_H_
