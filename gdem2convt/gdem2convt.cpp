
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

	const std::string targetPath = path + "\\..\\files";
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
	FILE *ifp = nullptr, *ofp = nullptr;

	if (fopen_s(&ifp, source.c_str(), "rb") == -1) {
		std::cerr << "error: source file \"" << source << "\" open failure!" << std::endl;
		return false;
	}

	if (fopen_s(&ofp, target.c_str(), "wb+") == -1) {
		std::cerr << "error: target file \"" << target << "\" open failure!" << std::endl;
		fclose(ifp);
		return false;
	}

	// check contents
	if (get_file_size(source) < 8) {
		std::cerr << "error: source file \"" << source << "\" is too small!" << std::endl;
		fclose(ifp);
		fclose(ofp);
		return false;
	}

	char buffer[8192] = { 0 };

	// read source header
	fseek(ifp, 8, SEEK_SET);

	// write target header
	const unsigned char header[8] = { 0xAA, 0x55, 0x64, 0x00, 0x01, 0x00, 0x00, 0x00 };
	size_t writed_size = fwrite(header, sizeof(unsigned char), _countof(header), ofp);
	if (writed_size != sizeof(header)) {
		std::cerr << "error: write source file \"" << source << "\" failure!" << std::endl;
		fclose(ifp);
		fclose(ofp);
		return false;
	}

	const size_t TOTAL_SIZE = sizeof(header) + 3601 * 3601 * 2;
	size_t totalSize = sizeof(header);

	while (!feof(ifp)) {
		size_t size = fread(buffer, sizeof(char), _countof(buffer), ifp);
		if (size == 0) {
			break;
		}

		if (totalSize + size > TOTAL_SIZE) {
			size = TOTAL_SIZE - totalSize;
		}

		writed_size = fwrite(buffer, sizeof(char), size, ofp);
		if (writed_size != size) {
			std::cerr << "error: write target file \"" << target << "\" failure!" << std::endl;
			fclose(ifp);
			fclose(ofp);
			return false;
		}

		totalSize += size;

		if (totalSize == TOTAL_SIZE) {
			break;
		}
	}

	fclose(ifp);
	fclose(ofp);

	const size_t targetSize = get_file_size(target);

	std::cout << "Size of target file \"" << string_section(target, "\\", -1, -1) 
		<< "\" is " << targetSize << std::endl;

	return true;
}
