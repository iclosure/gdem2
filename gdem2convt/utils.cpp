#include "stdafx.h"
#include "utils.h"
#include <io.h>
#include <direct.h>
#include <fstream>
#include <iostream>
#include <assert.h>

//#pragma warning(disable:4996)

bool folder_exists(const std::string &path)
{
	struct _finddata_t fileinfo;
	memset(&fileinfo, 0, sizeof(fileinfo));
	intptr_t hfile = _findfirst(path.c_str(), &fileinfo);
	if (hfile != -1) {
		_findclose(hfile);
		hfile = NULL;
		if ((fileinfo.attrib & _A_SUBDIR)) {
			return true;
		}
	}

	return false;
}

void get_files(const std::string &path, std::vector<std::string> &files, const std::string &prefix)
{
	std::string dir = path;
	if (_access(dir.c_str(), 04) == -1) {
		std::cerr << "error: directory does not exist." << std::endl;
		exit(-1);
	}

	if (dir.at(dir.length() - 1) != '\\') {
		dir += '\\';
	}

	if (_chdir(dir.c_str()) != 0) {
		std::cerr << "error: function _chdir() failed.";
		exit(-1);
	}

	struct _finddata_t fileinfo;
	memset(&fileinfo, 0, sizeof(fileinfo));
	intptr_t hfile = _findfirst("*", &fileinfo);
	if (hfile != -1) {
		do {
			const std::string name = (prefix.empty() ? "" : prefix + "\\") + fileinfo.name;
			if (fileinfo.attrib & _A_SUBDIR) {
				if ((strcmp(fileinfo.name, ".") != 0) && (strcmp(fileinfo.name, "..") != 0)) {
					get_files(dir + fileinfo.name, files, name);
				}
			}
			else {
				files.push_back(name);
			}
		} while (_findnext(hfile, &fileinfo) == 0);

		_findclose(hfile);
		hfile = NULL;
	}
}

_fsize_t get_file_size(const std::string &file)
{
#if 0
	FILE *fp = nullptr;
	errno_t errpr = fopen_s(&fp, file.c_str(), "rb");
	if (errpr != 0) {
		std::cerr << "error: open file \"" << file << "\" failure!" << std::endl;
		return 0;
	}
	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);
	fclose(fp);
	return size;
#else
	struct _finddata_t fileinfo;
	memset(&fileinfo, 0, sizeof(fileinfo));
	intptr_t hfile = _findfirst(file.c_str(), &fileinfo);
	if (hfile == -1) {
		assert(false);	//TODO
		std::cerr << "error: open file \"" << file << "\" failure!" << std::endl;
		return 0;
	}
	return fileinfo.size;
#endif
}

bool create_path(const std::string &path)
{
	std::string dir = path;
	if (_access(dir.c_str(), 02) != -1) {
		return true;
	}

	std::string _path = path;
	string_replace(_path, "/", "\\");

	std::string::size_type firstPos = _path.rfind("\\");
	if (_path.size() != (firstPos + 1)) {
		std::string strTemp = _path.substr(firstPos, _path.size());
		if (std::string::npos != strTemp.find(".")) {
			_path = _path.substr(0, firstPos + 1);
		}
		else {
			_path += "\\";
		}
	}
	else {
		_path += "\\";
	}

	std::string::size_type startpos = std::string::npos;
	std::string::size_type endpos = std::string::npos;

	while (true) {
		if ((endpos = _path.find("\\", startpos)) != std::string::npos) {
			std::string folder = _path.substr(0, endpos);
			startpos = endpos + 1;

			if (folder.rfind(":") == (folder.size() - 1)) {
				continue;
			}

			struct _stat filestat;
			memset(&filestat, 0, sizeof(filestat));
			if (_stat(folder.c_str(), &filestat) == 0) {
				if (!(filestat.st_mode & _S_IFDIR)) {
					return false;
				}
			}
			else {
				if (_mkdir(folder.c_str()) == -1) {
					return false;
				}
			}

			continue;
		}

		break;
	}

	return true;
}

bool string_startswith(const std::string &str, const std::string &header)
{
	return (str.compare(0, header.size(), header) == 0);
}

bool string_endswith(const std::string &str, const std::string &tail)
{
	return (str.compare(str.size() - tail.size(), tail.size(), tail) == 0);
}

std::string string_section(const char *ch, char sep, int start, int end)
{
	return string_section(std::string(ch), std::string(&sep, 1), start, end);
}

std::string string_section(const std::string &str, char sep, int start, int end)
{
	return string_section(str, std::string(&sep, 1), start, end);
}

std::string string_section(const char *ch, const std::string &sep, int start, int end)
{
	return string_section(std::string(ch), sep, start, end);
}

std::string string_section(const std::string &str, const std::string &sep, int start, int end)
{
	if (str.empty()) {
		return std::string();
	}

	if (start == 0 && end == -1) {
		return str;
	}

	std::vector<std::string> sections;
	string_split(str, sep, sections, false);

	const int sectionsSize = int(sections.size());

	if (start < 0) {
		start += sectionsSize;
	}
	if (end < 0) {
		end += sectionsSize;
	}

	if (start >= sectionsSize || end < 0 || start > end) {
		return std::string();
	}

	std::string ret;
	int first_i = start, last_i = end;
	for (int x = 0, i = 0; x <= end && i < sectionsSize; ++i) {
		const std::string &section = sections.at(std::vector<std::string>::size_type(i));
		const bool empty = section.empty();
		if (x >= start) {
			if (x == start) {
				first_i = i;
			}
			if (x == end) {
				last_i = i;
			}
			if (x > start && i > 0) {
				ret += sep;
			}
			ret += section;
		}
		if (!empty) {
			x++;
		}
	}

	return ret;
}

void string_split(const std::string &str, const std::string &delim,
	std::vector<std::string> &ret, bool keepEmptyParts)
{
	ret.clear();
	std::string::size_type last = std::string::npos;
	std::string::size_type index = str.find_first_of(delim, last);
	std::string sTemp;
	while (index != std::string::npos) {
		sTemp = str.substr(last, index - last);
		if (!keepEmptyParts) {
			sTemp = string_trim(sTemp);
		}
		ret.push_back(sTemp);
		last = index + 1;
		index = str.find_first_of(delim, last);
	}

	if (index - last > 0) {
		sTemp = str.substr(last, index - last);
		if (!keepEmptyParts) {
			sTemp = string_trim(sTemp);
		}
		ret.push_back(sTemp);
	}
}

std::string string_trim(const std::string &str)
{
	if (str.empty()) {
		return std::string();
	}

	std::string::size_type start = str.find_first_not_of(' ');
	if (start == std::string::npos) {
		return str;
	}

	std::string::size_type end = str.find_last_not_of(' ');
	if (end != std::string::npos) {
		return str.substr(start, end - start + 1);
	}

	return str.substr(start);
}

std::string &string_replace(std::string &str, const std::string &old_str,
	const std::string &new_str)
{
	std::string::size_type pos = std::string::npos;
	const std::string::size_type old_length = old_str.size();
	const std::string::size_type new_length = new_str.size();
	while ((pos = str.find(old_str, pos)) != std::string::npos) {
		str.replace(pos, old_length, new_str);
		pos += new_length;
	}

	return str;
}
