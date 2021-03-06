#include "../include/leitor.hpp"
#include "../include/field_info.hpp"
#include "../include/attribute_info.hpp"
#include "../include/method_info.hpp"
using namespace std;

const char * versionJVM(u2 major){
	const char * version =(const char *)malloc(4);
	switch (major) {
    case 46:
        version = "1.2";
        break;
    case 47:
        version = "1.3";
        break;
    case 48:
        version = "1.4";
        break;
    case 49:
        version = "1.5";
        break;
    case 50:
        version = "1.6";
        break;
    case 51:
        version = "1.7";
        break;
    case 52:
        version = "1.8";
        break;
    case 53:
        version = "1.9";
        break;
    case 54:
        version = "1.10";
        break;
    case 55:
        version = "1.11";
        break;
    case 56:
        version = "1.12";
        break;
    case 57:
        version = "1.13";
        break;
    }
	return version;
}
u1 Leitor::read1byte(){
	u1 ret;
	ret = *(this->byte_array + this->current_size);
	this->current_size += sizeof(ret);
	return ret;
}

u2 Leitor::read2byte(){
	u2 b[1];
	u1 ret[2];
	ret[1] = read1byte();
	ret[0] = read1byte();
	memcpy(b, &ret, sizeof(u2));
	return *b;
}

u4 Leitor::read4byte(){
	u4 b[1];
	u1 ret[4];
	ret[3] = read1byte();
	ret[2] = read1byte();
	ret[1] = read1byte();
	ret[0] = read1byte();
	memcpy(b, &ret, sizeof(u4));
	return *b;
}

// u2 getUtf8Size()

/* LEITOR */

void Leitor::loadFile(const string filename) {
	ifstream classfile;
	classfile.open(filename, ios::in|ios::binary);

	classfile.seekg(0, ios::end);
	int32_t filesize = classfile.tellg();

	char buffer;
	unsigned char *fileContent = (unsigned char *)malloc(filesize);
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

u4 Leitor::getMagicNumber(){return this->magic;}

bool Leitor::setMinorVersion() {
	this->minor_version = read2byte();
	return true;
}

u2 Leitor::getMinorVersion(){return this->minor_version;}

bool Leitor::setMajorVersion() {
	this->major_version = read2byte();
	return true;
}

u2 Leitor::getMajorVersion(){return this->major_version;}

bool Leitor::setConstantPoolCount() {
	this->constant_pool_count = read2byte();
	return true;
}

u2 Leitor::getConstantPoolCount(){return this->constant_pool_count;}


bool Leitor::setConstantPool() {

	int32_t size = this->constant_pool_count - 1;
	int32_t count = 0;
	int8_t tag;
	int32_t ret = 0;
	int32_t pos = 0;
	int32_t utf8_size = 0;
	Cp_info *cp_info = new Cp_info();

	// printf("size: %d\n", size);
	for (int32_t i = 0; i < size; i++) {
		tag = *(this->byte_array + this->current_size + pos);
		utf8_size = *(this->byte_array + this->current_size + pos + 2);
		ret = cp_info->getConstantPoolTag(tag, utf8_size);
		// printf("%d %d\n", i+1, tag);
		cp_info->addElement(tag, ret, pos, this->current_size,  this->byte_array);
		if (tag == 6 || tag == 5) {
			cp_info->addElement(66, 0, 0, 0, 0);
			i++;
		}
		pos += ret;
		count += ret;
	}
	this->constant_pool = cp_info;
	this->current_size += count;
	// printf("here\n");
	return true;
}
Cp_info* Leitor::getConstantPool(){
	return this->constant_pool;
}
u2 Leitor::getAccessFlags(){
	return this->access_flags;
}

bool Leitor::setAccessFlags() {
	this->access_flags = read2byte();
	return true;
}

u2 Leitor::getThisClass(){return this->this_class;}

bool Leitor::setThisClass(){
	this->this_class= read2byte();
	return true;
}

u2 Leitor::getSuperClass(){return this->super_class;}

bool Leitor::setSuperClass() {
	this->super_class = read2byte();
	return true;
}

u2 Leitor::getInterfacesCount(){return interfaces_count;}

bool Leitor::setInterfacesCount(){
	this->interfaces_count = read2byte();
	return true;
}

u2 * Leitor::getInterfaces(){return this->interfaces;} 

// bool Leitor::setInterfaces(){
// 	int32_t size = this->interfaces_count;
// 	u2  buffer;
// 	u2 interfaces[size];
// 	for(int i=0;i < size; i++){
// 		buffer = read2byte(); 
// 		interfaces[i] = buffer;
// 	}
// 	this->interfaces = (u2*)malloc(sizeof(interfaces));
// 	for(int i=0;i<size;i++){
// 		this->interfaces[i] = interfaces[i];
// 	}
// 	return true;
// }

u2 Leitor::getFieldsCount(){return this->fields_count;}

bool Leitor::setFieldsCount(){
	this->fields_count = read2byte();
	return true;
}
bool Leitor::setFields(){ 
	Field_info* field = new Field_info();
	for (int i = 0; i < this->fields_count; i++) {
		u2 access_flags = read2byte();
		u2 name_index = read2byte();
		u2 descriptor_index = read2byte();
		u2 attributes_count = read2byte();
		field->setFields(access_flags, name_index, descriptor_index, attributes_count);
	}
	this->fields = field;
	return true;	
	// printf("%d")

// 	Field_info  buffer;
// 	Attribute_info attributeAux;
// 	cp_info cpInfoAux;
// 	u2 lengthNameIndex;
// 	char nameIndexAttribute[50];
// 	for(int i=0;i<this->fields_count;i++){
// 		buffer.setAcessFlags(read2byte());
// 		buffer.setNameIndex(read2byte());
// 		buffer.setDescriptorIndex(read2byte());
// 		buffer.setAttributesCount(read2byte());
// 		for(int j=0;j< buffer.getAttributesCount();j++){ // read attributes
// 			attributeAux.setCP(this->constant_pool);
// 			attributeAux.setAttributeNameIndex(read2byte());
// 			attributeAux.setAttributeLength(read4byte());
// 			// set info
// 			cpInfoAux = this->constant_pool->getCpInfoElement(read2byte());
// 			lengthNameIndex = cpInfoAux.constant_element.c11->length;
// 			strcpy(nameIndexAttribute,(const char *)cpInfoAux.constant_element.c11->bytes);
// 			nameIndexAttribute[lengthNameIndex]= '\0';
// 			this->current_size+=attributeAux.setInfo(nameIndexAttribute,lengthNameIndex,this->byte_array + this->constant_pool_count);
// 		}
// 	}
	// return true;
}
// Exibidor
void Leitor::printAccessFlags(){
	uint16_t flags = (uint16_t)this->getAccessFlags();
	uint8_t bits1 = flags & 0x000F;
	uint8_t bits2 = flags & 0x00F0;
	uint8_t bits3 = flags & 0x0F00;
	uint8_t bits4 = flags & 0xF000;
	printf("Access Flags: ");
	if(bits1 == ACC_PUBLIC){
		printf("[ACC_PUBLIC] ");
	}else printf("[ACC_UNKNOWN] ");

	if(bits2!=0){
		if(bits2==ACC_FINAL) printf("[ACC_FINAL] ");
		else if(bits2==ACC_SUPER) printf("[ACC_SUPER] ");
		else printf("[ACC_UNKNOWN] ");
	}
	if(bits3!=0){
		if(bits3==ACC_INTERFACE) printf("[ACC_INTERFACE] ");
		else if(bits3==ACC_ABSTRACT) printf("[ACC_ABSTRACT] ");
		else printf("[ACC_UNKNOWN] ");
	}
	if(bits4!=0){
		if(bits4==ACC_SYNTHETIC) printf("[ACC_SYNTHETIC] ");
		else if(bits4==ACC_ANNOTATION) printf("[ACC_ANNOTATION] ");
		else if(bits4==ACC_ENUM) printf("[ACC_ENUM] ");
		else printf("[ACC_UNKNOWN] ");
	}
	printf("\n");
}
void Leitor::printThisClass(){ // completar depois:fazer  buscar dos nomes no CP
	printf("ThisClass: %x\n", this->getThisClass());
}
void Leitor::printSuperClass(){ // completar depois:fazer  buscar dos nomes no CP
	printf("SuperClass: %x\n", this->getSuperClass());
}
void Leitor::printInterfaceCont(){
	printf("Interfaces Count : %x\n", this->getInterfacesCount());
}
void Leitor::printInterfaces(){
	if(this->interfaces_count!=0){
		printf("Interfaces[0..%d]: ",this->interfaces_count-1);
		for(int i=0;i<this->interfaces_count;i++){
			printf("%d ",this->interfaces[i]);
		}
		printf("\n");
	}
}
void Leitor::printFieldsCount(){
	printf("FieldsCount: %x\n",this->fields_count);
}
Field_info* Leitor::getFields(){
	return this->fields;
	//vector<Field_info> ret;

	//for (u2 i = 0; i < this->fields_count; ++i){
	//	ret.push_back(this->fields[i]);
	//}
	//return ret;
}

// u2 Leitor::getAttributesCount(){return attributes_count;}

u2 Leitor::getMethodsCount(){
	return methods_count;
}

bool Leitor::setMethodsCount() {
	this->methods_count = read2byte();
	return true;
}


bool Leitor::setMethods(){
	// printf("methods_count: %d\n", this->methods_count);
	uint16_t access_flags;
	uint16_t name_index;
	uint16_t descriptor_index;
	uint16_t attributes_count;
	uint16_t attribute_name_index;
	int32_t size;
	// printf("")

	Method_info *a = new Method_info();
	for (int i = 0; i < this->methods_count; ++i){
		method_info_element *e = (method_info_element*)malloc(sizeof(method_info_element));
		// Relativo ao método
		access_flags = read2byte();		
		name_index = read2byte();
		descriptor_index = read2byte();
		attributes_count = read2byte();
		
		attribute_name_index = read2byte();
		size = read4byte();

		Attribute_info* att = this->setAttributes(attribute_name_index, size);		
		
		e->access_flags = access_flags;
		e->name_index = name_index;
		e->descriptor_index = descriptor_index;
		e->attributes_count = attributes_count;
		e->attributes = att;
		a->methods.push_back(e);
		this->current_size += size;
	}
	this->methods = a;
	// free(e);
	return true;
}

bool Leitor::setAttributesCount() {
	this->attributes_count = read2byte();
	return true;
}


Attribute_info* Leitor::setAttributes(u2 attribute_name_index, u4 attribute_length) {
	// int32_t size = 2;
	char string[20] = "";
	Attribute_info *attribute = new Attribute_info();
	info_element *element = (info_element*)malloc(sizeof(info_element));
	attribute->setAttributeNameIndex(attribute_name_index);
	attribute->setAttributeLength(attribute_length);

	// Pega o tipo de attribute
	cp_info attribute_type = this->constant_pool->getCpInfoElement(attribute_name_index);
	CONSTANT_Utf8_info* utf8 = attribute_type.constant_element.c11;
	// Converte a string do utf8 pra um char normal para usar no if
	for (int i = 0; i < utf8->length; i++) {
		string[i] = utf8->bytes[i];
	}

	if (!strcmp(string, "Code")) {
		Code_attribute *code = new Code_attribute();
		code->set(attribute->getAttributeLength(), this->byte_array + this->current_size, attribute_name_index);
		element->codeAttr = code;
		attribute->setInfoElement(string, element);
	}
	return attribute;
}

bool Leitor::setAttributesLeitor(){
	Attribute_info* attribute = new Attribute_info();
	for (int i = 0; i < this->attributes_count; i++) {
		u2 attribute_name_index = read2byte();
		u2 attribute_length = read4byte();
		u1* info = (u1*)malloc(attribute_length * sizeof(u1));
		for (int i = 0; i < attribute_length; i++) {
			info[i] = read1byte();
		}

		attribute->setAttributeNameIndex(attribute_name_index);
		attribute->setAttributeLength(attribute_length);
		attribute->setInfo(info);

		this->attributes.push_back(attribute);
	}
	
	return true;
}

Method_info* Leitor::getMethods(){
	return this->methods;
	// vector<Method_info> ret;

	// for (u2 i = 0; i < this->methods_count; ++i){
	// 	ret.push_back(this->methods[i]);
	// }

	// return ret;
}

// vector<Attribute_info> Leitor::getAttributes(){
// 	vector<Attribute_info> ret;

// 	for (u2 i = 0; i < this->attributes_count; ++i){
// 		ret.push_back(this->attributes[i]);
// 	}

// 	return ret;
// }

void Leitor::printAttributes() {
	vector<Attribute_info*>::iterator i;
	int j = 0;
	for (i = this->attributes.begin(); i != this->attributes.end(); ++i) {
		printf("========== [%d] ===========\n", j);
		printf("Attribute name index: %d\n", (*i)->getAttributeNameIndex());
		printf("Attribute length: %d\n", (*i)->getAttributeLength());
		printf("Specific info:\n");
		u1* info = (*i)->getInfo();
		for (int k = 0; k < (*i)->getAttributeLength(); k+=2) {
			printf("  Source file name index :%d\n", info[k] + info[k+1]);
		}
	}
}

/*Set e Get*/

bool Leitor::set(int key){
	switch(key)
	{
		case MAGIC_NUMBER:
		  return setMagicNumber();
		case MINOR_VERSION:
		  return setMinorVersion();
		case MAJOR_VERSION:
		  return setMajorVersion();
		case CONSTANT_POOL_COUNT:
			return setConstantPoolCount();
		case CONSTANT_POOL:
			return setConstantPool();
		case ACCESS_FLAGS:
			return setAccessFlags();
		case THIS_CLASS:
			return setThisClass();
		case SUPER_CLASS:
			return setSuperClass();
		case INTERFACES_COUNT:
			return setInterfacesCount();
		case INTERFACES:
			// return setInterfaces();
			break;
		case FIELDS:
			return setFields();
		case FIELDS_COUNT:
			return setFieldsCount();
		case METHODS_COUNT:
			return setMethodsCount(); //metodos comentados
		case METHODS:
			return setMethods();
		case ATTRIBUTES_COUNT:
			return setAttributesCount();
		case ATTRIBUTES:
			return setAttributesLeitor();
			// break;
	}
	return false;
}

u2 Leitor::get(int key){
	switch(key)
	{
		case INTERFACES_COUNT:
			return getInterfacesCount();
		case MINOR_VERSION:
			return getMinorVersion();
		case MAJOR_VERSION:
			return getMajorVersion();
		case CONSTANT_POOL_COUNT:
			return getConstantPoolCount();
		case FIELDS_COUNT:
			return getFieldsCount();
		case METHODS_COUNT:
			return getMethodsCount();
		// case METHODS:
			// return getMethods();
		case ATTRIBUTES_COUNT:
			break;
			//return getAttributesCount(); metodo comentado
		case ACCESS_FLAGS:
			return getAccessFlags();
		case SUPER_CLASS:
			return getSuperClass();
		case THIS_CLASS:
			return getThisClass();
	}
	return 0;
}

std::string Leitor::getUTF8(u2 name_index){
	cp_info cp_entry;
    u2 length;
    char* name;
    std::string typeName;
 	// printf("free\n");
    // Busca a referência na CP
    cp_entry = constant_pool->getCpInfoElement(name_index);
   
    // Verifica o tamanho da string UTF-8
    length = cp_entry.constant_element.c11->length;
    // Aloca de acordo com o tamanho 'length' retornado
    name = (char*)malloc(sizeof(char)*length);
 
    // Recupera os bytes UTF-8
    for(u2 i = 0; i < length; i++) {
        name[i] = cp_entry.constant_element.c11->bytes[i];
 	}
    // Passa os bytes de char* para string
    for(u2 i = 0; i < length; i++) {
        typeName += name[i];
    }
    // Desaloca os bytes char*
    free(name);
 
    return typeName;
}

/* EXIBIDOR */

void Leitor::exibir() {
	printf("Magic number: %x\n", this->magic);
	printf("Minor version: %d\n", this->minor_version);
	printf("Major version: %d [%s]\n", this->major_version,versionJVM(this->major_version));
	printf("Constant pool count: %d\n", this->constant_pool_count);
	printf("Access flags: 0x00%x\n", this->access_flags);
	printf("This class: %d\n", this->this_class);
	printf("Super class: %d\n", this->super_class);
	printf("Interfaces count: %d\n", this->interfaces_count);
	printf("Fields count: %d\n", this->fields_count);
	printf("Methods count: %d\n", this->methods_count);
	printf("Attributes count: %d\n", this->attributes_count);
	printf("\nConstant pool:\n");
	this->constant_pool->printConstantPool();
	printf("\nFields:\n");
	this->fields->printFieldsInfo();
	printf("\nMethods:\n");
	this->methods->getMethodInfo();
	printf("\nAttributes:\n");
	this->printAttributes();

}

bool Leitor::isMethodAccessFlagSet(u2 method_index , u2 search_flag){
	u2 access_value = methods->methods[method_index]->access_flags;
    switch(search_flag){
        case ACC_PUBLIC:
            return (access_value & 0x0001);
        default: break;
    }

    return false;
}

