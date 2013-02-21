/*
 * MilaElement.h
 *
 *  Created on: Dec 31, 2012
 *      Author: haims
 */
#ifndef MILAELEMENT_H_
#define MILAELEMENT_H_


#include "MilaConvereter.h"
#include <stdlib.h>



using namespace std;
using namespace pugi;

class CMilaElement {
public:
	CMilaElement(xml_node* doc ,ofstream* out);
	virtual ~CMilaElement();

	//Initialize CMilaConverter
	bool InitializeConverter();

	//The main parse function for each element
	virtual bool Parse() = 0;

	//Parse a Mila attribute
	//Input: attrName - the name of the attribute.
	//		 required - a required flag. if the attribute will not found the program will stop.
	//       defaultValue - default value in the absent of the attribute. (for optional attributes without default the "NULL" string should be inserted
	//       print - is need to print to the output file flag
	//       node - an optional parameter. Parsing a external node and not the m_doc
	virtual bool ParseAttrib(char_t* attrName, bool required, char* defaultValue, bool print, xml_node* node = NULL);

	//String Converter helper class
	static CMilaConvereter milaConverter;

protected:
	//Current xmlNode
	xml_node* m_doc;

	//Reference to the output file handler
	ofstream* m_out;

	//Node specifier
	string m_name;
};

#endif /* MilaElement_H_ */
