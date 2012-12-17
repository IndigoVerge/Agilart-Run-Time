#ifndef PROPERTY_H_
#define PROPERTY_H_

#include <string>

struct Property {

	string Name;
	string DataType;
	string Value;

	Property()
	{

	}

	Property(string name, string dataType, string value) {
		this->Name = name;
		this->DataType = dataType;
		this->Value = value;
	}

	~Property() {
	}
};

#endif // PROPERTY_H_
