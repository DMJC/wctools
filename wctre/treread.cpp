#include "treread.h"
#include "xtre.h"
namespace fs = std::filesystem;
using namespace std;

void extract_file(char* trename, string file_name, unsigned int offset, unsigned int size){
	/*Stripping the ..\\..\\ off of file paths. This is for clean output directories*/
	string substring = "..\\..\\";
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

int main(int argv, char* argc[]){
	cout << "Tre reader" << endl;
	if (argv >= 2){
		fstream trefile;
		trefile.open(argc[1], ios::in);
		if (!trefile){cout << "error opening .tre file" << endl;}
			else {
			cout << "opening " << argc[1] << endl << endl;
			char xtre[4];
			trefile.read((char*)&xtre, 4);
			if (strcmp(xtre,"XTRE")==0){
				read_xtre(trefile);
			} else {
				cout << "Wing Commander Privateer/Armada TRE File" << endl;
				cout << "Would you like to Extract Files y/n?" << endl;
				char extract;
				cin >> extract;
				trefile.seekg(0);
				unsigned char buf[4];
				trefile.read((char*)&buf, 4);
				unsigned int files = 0;
				files |= buf[0] << 0; files |= buf[1] << 8; files |= buf[2] << 16; files |= buf[3] << 24;
				cout << "Number of Files in Archive " << dec << files << endl;
				trefile.read((char*)&buf, 4);
				unsigned int data_begin = 0;
				data_begin |= buf[0];data_begin |= buf[1] << 8;data_begin |= buf[2] << 16;data_begin |= buf[3] << 24;
				cout << "Data begins at: " << dec << data_begin << endl;
				for (int i = 0; i != files; i++){
					char ignored;
					trefile.read((char*)&ignored,1);
					char filename[65];
					trefile.read((char*)&filename,65);
					cout << "File: " << filename;
					unsigned char offset_buf[4];
					trefile.read((char*)&offset_buf,4);
					unsigned int offset = 0;
					offset |= offset_buf[0];offset |= offset_buf[1] << 8;offset |= offset_buf[2] << 16;offset |= offset_buf[3] << 24;
					cout << " Offset: " << offset;
					unsigned char size_buf[4];
					trefile.read((char*)&size_buf,4);
					unsigned int size = 0;
					size |= size_buf[0];size |= size_buf[1] << 8;size |= size_buf[2] << 16;size |= size_buf[3] << 24;
					cout << " Size: " << size << endl;
					cout << "Counter Value: " << i << endl;
					if (extract == 'y'){
						cout << "Extracting: " << filename << endl;
						extract_file(argc[1],filename,offset,size);
					}else{

					}
				}
			}
		}
	} else { cout << "No TRE file specified rerun with ./treread file.tre" << endl; }
	return 0;
}
