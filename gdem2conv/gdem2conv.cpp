// gdem2conv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "utils.h"
#include <io.h>
#include <iostream>
#include <fstream>

bool gdem2_convert(const std::string &source, const std::string &target);

int main()
{
	std::cout << "====== gdem2 converter v1.0 ======" << std::endl;
	
	std::cout << "please input gdem2 path:" << std::endl;
	
	std::string path;
	std::cin >> path;

	if (!folder_exists(path)) {
		std::cerr << "error: directory \"" << path << "\" is not exists!" << std::endl;
		system("pause");
		return -1;
	}

	//
	std::vector<std::string> files;
	get_files(path, files);
	if (files.empty()) {
		std::cerr << "error: has no file in path \"" << path << "\"!" << std::endl;
		system("pause");
		return -1;
	}

	const std::string targetPath = path + "\\files";
	if (!create_path(targetPath)) {
		std::cerr << "error: create path \"" << path << "\" failure!" << std::endl;
		system("pause");
		return -1;
	}
	
	//
	for (std::vector<std::string>::const_iterator citer = files.cbegin();
		citer != files.cend(); ++citer) {
		const std::string &file = *citer;
		const std::string filename = string_section(file, '_', -2, -2);
		const std::string filesuffix = '.' + string_section(file, '.', -1, -1);
		const std::string source(path + '\\' + file);
		const std::string target(targetPath + '\\' + filename + ".dat");
		if (!gdem2_convert(source, target)) {
			std::cerr << "error: convert gdem2 file \"" << file << "\" failure!" << std::endl;
			continue;
		}
	}

	std::cout << "convert finished!" << std::endl;

	system("pause");

    return 0;
}

bool gdem2_convert(const std::string &source, const std::string &target)
{
	std::ifstream ifs(source, std::ios::in | std::ios::binary);
	std::ofstream ofs(target, std::ios::out | std::ios::binary | std::ios::trunc);

	if (!ifs.is_open()) {
		std::cout << "source file \"" << source << "\" open failure!" << std::endl;
		return false;
	}

	if (!ofs.is_open()) {
		std::cout << "target file \"" << target << "\" open failure!" << std::endl;
		return false;
	}

	// check contents
	if (get_file_size(source) < 8) {
		std::cout << "source file \"" << source << "\" is too small!" << std::endl;
		return false;
	}

	char buffer[4096] = { 0 };

	// read source header
	ifs.seekg(8);

	// write target header
	const unsigned char header[8] = { 0xAA, 0x55, 0x64, 0x00, 0x01, 0x00, 0x00, 0x00 };
	const int size = sizeof(header);
	ofs.write(static_cast<const char*>(buffer), sizeof(header));

	while (ifs.read(buffer, 4096)) {
		//
	}

	ifs.close();
	ofs.close();

	return true;
}
