#include "treread.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <string>
#include <filesystem>
using namespace std;
void read_xtre(fstream &trefile);
void read_section1(fstream &trefile);
void read_section2(fstream &trefile, unsigned int offset_address);
void read_section3(fstream &trefile, unsigned int offset_address);
