#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include "Boat.h"
#include "EventLog.h"
#include "ActiveLog.h"

void TestBoat();

Boat* CreateBoat(EventLog *pEventLog);
EventLog* CreateEventLog(ActiveLog *pActiveLog);

void ReadFile(Boat *boat, std::string filePath);
void WriteFile(Boat *boat, std::string filePath);
bool CompareFiles(std::string filePath1, std::string filePath2);

void printHeader(std::string title);
