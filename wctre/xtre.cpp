#include "xtre.h"

void read_section1(fstream &trefile, int sec1_address, int sec2_address, int sec3_address){
	int i = (sec2_address - sec1_address) / 8;
	cout << "there are " << i << " files" << endl;
	for (int k = 0; k != i; k++ ){
		unsigned char crc_buffer[4];
		unsigned char offset_buffer[4];
		trefile.read((char*)&crc_buffer,4);
		trefile.read((char*)&offset_buffer,4);
		unsigned int crc_address = 0;
		crc_address |= crc_buffer[0];crc_address |= crc_buffer[1] << 8;crc_address |= crc_buffer[2] << 16;crc_address |= crc_buffer[3] << 24;
		unsigned int offset_address = 0;
		offset_address |= offset_buffer[0];offset_address |= offset_buffer[1] << 8;offset_address |= offset_buffer[2] << 16;offset_address |= offset_buffer[3] << 24;
		if (crc_address == 0){}else{
			cout << "CRC: " << crc_address << " Off Addr: " << offset_address;
			if (offset_address < sec3_address){
				read_section2(trefile, offset_address);
			}else{
				read_section3(trefile, offset_address);
			}
		}
	}
}

void read_section2(fstream &trefile, unsigned int offset_address){
	trefile.seekg(offset_address);
	cout << " Sect2 Addr: " << offset_address << endl;
        unsigned char string_size_buf;
        trefile.read((char*)&string_size_buf,1);
        unsigned int string_size = 0;
	string_size = string_size_buf;
	cout << " Name Size: " << string_size;
/*        char * string_buf[string_size];
        string_buf = new char[string_size];*/
	char filename[string_size];
        trefile.read((char*)&filename,string_size);
	cout << " File: " << filename;
	unsigned char offset_buffer[4];
	trefile.read((char*)&offset_buffer,4);
	unsigned int offset_address2 = 0;
	offset_address2 |= offset_buffer[0];offset_address2 |= offset_buffer[1] << 8;offset_address2 |= offset_buffer[2] << 16;offset_address2 |= offset_buffer[3] << 24;
	cout << " Offset 2 Address: " << offset_address2 << endl;
}

void read_section3(fstream &trefile, unsigned int offset_address){
	unsigned char offset_buf[4];
	trefile.read((char*)&offset_buf,4);
	unsigned int offset = 0;
	offset |= offset_buf[0];offset |= offset_buf[1] << 8;offset |= offset_buf[2] << 16;offset |= offset_buf[3] << 24;
	cout << " Offset: " << offset;
	unsigned char size_buf[4];
	trefile.read((char*)&size_buf,4);
	unsigned int size = 0;
	size |= size_buf[0];size |= size_buf[1] << 8;size |= size_buf[2] << 16;size |= size_buf[3] << 24;
	cout << " Uncompressed Size: " << size << endl;
}

void read_xtre(fstream &trefile){
	unsigned char ignored[4];
	unsigned char sec1_buffer[4];
	unsigned char sec2_buffer[4];
	unsigned char sec3_buffer[4];
	unsigned char data_buffer[4];
	trefile.read((char*)&ignored,4);
	trefile.read((char*)&sec1_buffer,4);
	trefile.read((char*)&sec2_buffer,4);
	trefile.read((char*)&sec3_buffer,4);
	trefile.read((char*)&data_buffer,4);
	unsigned int sec1_address = 0;
	sec1_address |= sec1_buffer[0];sec1_address |= sec1_buffer[1] << 8;sec1_address |= sec1_buffer[2] << 16;sec1_address |= sec1_buffer[3] << 24;
	unsigned int sec2_address = 0;
	sec2_address |= sec2_buffer[0];sec2_address |= sec2_buffer[1] << 8;sec2_address |= sec2_buffer[2] << 16;sec2_address |= sec2_buffer[3] << 24;
	unsigned int sec3_address = 0;
	sec3_address |= sec3_buffer[0];sec3_address |= sec3_buffer[1] << 8;sec3_address |= sec3_buffer[2] << 16;sec3_address |= sec3_buffer[3] << 24;
	unsigned int data_address = 0;
	data_address |= data_buffer[0];data_address |= data_buffer[1] << 8;data_address |= data_buffer[2] << 16;data_address |= data_buffer[3] << 24;
	cout << "Section 1 Address: " << sec1_address << endl;
	cout << "Section 2 Address: " << sec2_address << endl;
	cout << "Section 3 Address: " << sec3_address << endl;
	cout << "Data Address: " << data_address << endl;
	cout << "Wing Commander 3/4 TRE File Detected" << endl;
	int num_files = (sec2_address - sec1_address) / 8;
	cout << "there are " << num_files << " files" << endl;
	cout << "Would you like to Extract Files y/n?" << endl;
	char extract;
	cin >> extract;
	read_section1(trefile, sec1_address, sec2_address, sec3_address);
}
