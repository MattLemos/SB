#include "../include/field_info.hpp"


using namespace std;

bool Field_info::setAcessFlags(u2 access_flags){
    this->access_flags = access_flags;
    return true;
}
bool Field_info::setNameIndex(u2 nameIndex){
    this->name_index = name_index;
    return true;
}
bool Field_info::setAttributesCount(u2 attribute_count){
    this->attributes_count = attributes_count;
    return true;
}
bool Field_info::setDescriptorIndex(u2 descriptorIndex){
    this->descriptor_index = descriptor_index;
    return true;
}
bool Field_info::setAttributes(Attribute_info * attributes){
    for(int32_t i=0;i<this->attributes_count;i++){
        this->attributes[i] = attributes[i];
    }
    return true;
}

u2 Field_info::getAcessFlags(){return this->access_flags;}
u2 Field_info::getAttributesCount(){return this->attributes_count;}
u2 Field_info::getDescriptorIndex(){return this->descriptor_index;}
u2 Field_info::getNameIndex(){return this->name_index;}
Attribute_info * Field_info::getAttributes(){return this->attributes;}