#include "unpak.h"
namespace fs = std::filesystem;
using namespace std;
/*
void extract_file(char* trename, string file_name, unsigned int offset, unsigned int size){
	/*Stripping the ..\\..\\ off of file paths. This is for clean output directories*/
/*	string substring = "..\\..\\";
    	size_t ind = file_name.find(substring);
    	if(ind !=std::string::npos){
        	file_name.erase(ind,substring.length());
	}
#ifdef __linux__
  	replace( file_name.begin(), file_name.end(), '\\', '/');
#endif
	fs::path file_path = file_name;
	if (file_path.has_parent_path()){
#ifdef __linux__
		string fppath = file_path.parent_path();
#else
		wstring fppath = file_path.parent_path();
#endif
		fs::create_directories(fppath);
	}
#ifdef SUPER_VERBOSE
	cout << "Trename: " << trename << endl;
	cout << "File to Extract: " << file_name << endl;
	cout << " Offset: " << offset;
	cout << " Size: " << size << endl;
#endif
	fstream file (trename, ios::in | ios::binary);
	file.seekp(offset);
	unsigned char * file_buf;
	file_buf = new unsigned char[size];
	file.read((char*)file_buf,size);
	ofstream outfile;
	outfile.open(file_name, ios::out | ios::binary);
	outfile.write((char*)file_buf,size);
	outfile.close();
	file.close();
}
*/
int main(int argv, char* argc[]){
	cout << "PAK reader" << endl;
	if (argv >= 2){
		fstream trefile;
		trefile.open(argc[1], ios::in);
		if (!trefile){cout << "error opening .tre file" << endl;}
			else {
			cout << "opening " << argc[1] << endl << endl;
			unsigned char size_buf[4];
			trefile.read((char*)&size_buf, 4);
			unsigned int size = 0;
			size |= size_buf[0] << 0; size |= size_buf[1] << 8; size |= size_buf[2] << 16; size |= size_buf[3] << 24;

			trefile.seekg (0);
			const auto begin = trefile.tellg();
			trefile.seekg (0, ios::end);
			const auto end = trefile.tellg();
			const auto fsize = (end-begin);
			trefile.seekg(4);
			if (size == fsize){
				cout << "Wing Commander Privateer/Strike Commander/Armada PAK File" << endl;
				cout << "Would you like to Extract Files y/n?" << endl;
				char extract;
				cin >> extract;
				unsigned char buf[4];
				trefile.read((char*)&buf, 4);
				unsigned int files = 0;
				files |= buf[0] << 0; files |= buf[1] << 8; files |= buf[2] << 16;
				cout << "Number of Files in Archive " << dec << files << endl;
				trefile.read((char*)&buf, 4);
				unsigned int data_begin = 0;
				data_begin |= buf[0];data_begin |= buf[1] << 8;data_begin |= buf[2] << 16;
				cout << "Data begins at: " << dec << data_begin << endl;
				trefile.seekg(data_begin);
/*				int current_position = trefile.tellg();
				cout << "Current Position:" << current_position << endl;*/
                                trefile.read((char*)&buf, 4);
				unsigned int length = 0;
				length |= buf[0];length |= buf[1] << 8;length |= buf[2] << 16;
				cout << "Length: " << length << endl;
                                trefile.read((char*)&buf, 4);
				unsigned int bit_depth = 0;
				bit_depth |= buf[0];
				cout << "VOC File is " << bit_depth << "-bit" << endl;

				unsigned char * file_buf;
				file_buf = new unsigned char[length];
				trefile.read((char*)file_buf,length);
				ofstream vocfile;
        			vocfile.open("out.voc", ios::out | ios::binary);
        			vocfile.write((char*)file_buf,length);
		        	vocfile.close();
			}
		}
	} else { cout << "No PAK file specified rerun with ./unpak file.pak" << endl; }
	return 0;
}

