#ifndef HELP_H
#define HELP_H

#include <string>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>

void ErrorCheckExit(bool condition, std::string message);
void ErrorCheck(bool condition, std::string message);
std::vector<std::string> splitStr(std::string aString, char *delims);
std::string getEnv(std::string varname);
std::string getPwd();
char **convertVector(std::vector<std::string> &aVector);
std::string findBin(std::string cmd);

#endif