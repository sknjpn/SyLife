#include "MoleculeManager.h"
#include <fstream>
#include <iostream>
#include <string>

void MoleculeManager::Init()
{
	std::ifstream ifs("./data/1alk.fa");
	int buf_size = 81;

	char str[buf_size];
	if (ifs.fail()) {
		std::cerr << "Failed to open file." << std::endl;
		return -1;
	}
	while (ifs.getline(str, buf_size)) {
		std::cout << "#" << str << std::endl;
	}
	return 0;
}