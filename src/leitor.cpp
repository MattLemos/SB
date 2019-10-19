#include "../include/leitor.hpp"
using namespace std;


/* LEITOR */

void Leitor::loadFile(const string filename) {
	ifstream classfile;
	classfile.open(filename, ios::in|ios::binary);

	classfile.seekg(0, ios::end);
	int32_t filesize = classfile.tellg();

	char buffer;
	unsigned char *fileContent = new unsigned char[filesize];
	classfile.seekg(0, ios::beg);
	
	if (classfile.is_open()) {
		int32_t i = 0;
		while(!classfile.eof()) {
			classfile.read(&buffer, 1);
			fileContent[i] = static_cast<unsigned char>(buffer);
			i++;
		}
	}
	else {
		printf("Unable to open file\n");
	};
	this->byte_array = fileContent;
}

bool Leitor::setMagicNumber() {
	int32_t size = 4;
	int32_t buffer[size];
	unsigned char magic[size];
	int32_t j = size - 1;
	bool result  = false;

	for (int32_t i = 0; i < size; i++) {
		magic[j]= *(this->byte_array + i);
		j--;
	}

	memcpy(buffer, &magic, sizeof(magic));

	if (*buffer == (int32_t)0xcafebabe)  {
		this->magic = *buffer;
		result = true;
	}
	else {
		printf("Invalid class file\n");
	}
	this->current_size = sizeof(this->magic);
	return result;
}


bool Leitor::setMinorVersion() {
	int32_t size = 2;
	int32_t j = size - 1;
	int16_t buffer[size];
	int16_t minor_version[size];

	for (int32_t i = 0; i < size; i++) {
		minor_version[j] = *(this->byte_array + this->current_size + i);
		j--;
	}
	memcpy(buffer, &minor_version, sizeof(minor_version));
	this->minor_version = *buffer;
	this->current_size += sizeof(this->minor_version);
	return true;
}

bool Leitor::setMajorVersion() {
	int32_t size = 2;
	int32_t j = size - 1;
	int16_t buffer[size];
	int16_t major_version[size];

	for (int32_t i = 0; i < size; i++) {
		major_version[j] = *(this->byte_array + this->current_size + i);
		j--;
	}
	memcpy(buffer, &major_version, sizeof(major_version));
	this->major_version = *buffer;
	this->current_size += sizeof(this->major_version);
	return true;
}

bool Leitor::setConstantPoolCount() {
	int32_t size = 2;
	int32_t j = size - 1;
	int16_t buffer[size];
	int16_t constant_pool_count[size];

	for (int32_t i = 0; i < size; i++) {
		constant_pool_count[j] = *(this->byte_array + this->current_size + i);
		j--;
	}
	memcpy(buffer, &constant_pool_count, sizeof(constant_pool_count));
	this->constant_pool_count = *buffer;
	this->current_size += sizeof(this->constant_pool_count);
	return true;
}

bool Leitor::setConstantPool() {

	int32_t size = this->constant_pool_count - 2;
	int8_t tag;
	int32_t ret = 0;
	int32_t pos = 0;
	int32_t utf8_size = 0;

	Cp_info *constant_pool = new Cp_info();

	for (int32_t i = 0; i < this->constant_pool_count; i++) {
		tag = *(this->byte_array + this->current_size + pos);
		utf8_size = *(this->byte_array + this->current_size + pos + 2);
		ret = constant_pool->getConstantPoolTag(tag, utf8_size);
		pos += ret;
		printf("tag: %d\n", tag);
	}
	delete(constant_pool);

	this->current_size += size;
	return true;
}

bool Leitor::setAccessFlags() {
	int32_t size = 2;
	int32_t j = size - 1;
	int16_t buffer[size];
	int16_t access_flags[size];

	for (int32_t i = 0; i < size; i++) {
		access_flags[j] = *(this->byte_array + this->current_size + i);
		j--;
	}
	memcpy(buffer, &access_flags, sizeof(access_flags));
	this->access_flags = *buffer;
	this->current_size += sizeof(this->access_flags);
	return true;
}

bool Leitor::setThisClass() {
	int32_t size = 2;
	int32_t j = size - 1;
	int16_t buffer[size];
	int16_t this_class[size];

	for (int32_t i = 0; i < size; i++) {
		this_class[j] = *(this->byte_array + this->current_size + i);
		j--;
	}
	memcpy(buffer, &this_class, sizeof(this_class));
	this->this_class = *buffer;
	this->current_size += sizeof(this->this_class);
	return true;
}

bool Leitor::setSuperClass() {
	int32_t size = 2;
	int32_t j = size - 1;
	int16_t buffer[size];
	int16_t super_class[size];

	for (int32_t i = 0; i < size; i++) {
		super_class[j] = *(this->byte_array + this->current_size + i);
		j--;
	}
	memcpy(buffer, &super_class, sizeof(super_class));
	this->super_class = *buffer;
	this->current_size += sizeof(this->super_class);
	return true;
}

bool Leitor::setInterfacesCount() {
	int32_t size = 2;
	int32_t j = size - 1;
	int16_t buffer[size];
	int16_t interfaces_count[size];

	for (int32_t i = 0; i < size; i++) {
		interfaces_count[j] = *(this->byte_array + this->current_size + i);
		j--;
	}
	memcpy(buffer, &interfaces_count, sizeof(interfaces_count));
	this->interfaces_count = *buffer;
	this->current_size += sizeof(this->interfaces_count);
	return true;
}

// bool Leitor::setAccessFlagCount() {
// 	int32_t size = 2;
// 	int32_t j = size -1;
// 	int16_t buffer[size];
// 	int16_t access_flags[size];

// 	for (int32_t i = 0; i < size; i++) {
// 		access_flags[j] = *(this->byte_array + this->current_size + i);
// 		j--;
// 	}

// 	memcpy(buffer, &access_flags, sizeof(access_flags));
// 	this->access_flags = *buffer;
// 	this->current_size += sizeof(this->access_flags);
// 	return true;
// }


/* EXIBIDOR */

void Leitor::exibir() {
	printf("Magic number: %x\n", this->magic);
	printf("Minor version: %x\n", this->minor_version);
	printf("Major version: %x\n", this->major_version);
	printf("Constant pool count: %d\n", this->constant_pool_count);
	// printf("Access flags: %x\n", this->access_flags);
}