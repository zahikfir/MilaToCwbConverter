/*
 * MilaElement.cpp
 *
 *  Created on: Dec 31, 2012
 *      Author: haims
 */

#include "MilaElement.h"

CErrorLogger* errorLogger;

CMilaConvereter CMilaElement::milaConverter;


CMilaElement::CMilaElement(xml_node* doc ,ofstream* out)  : m_doc(doc), m_out(out){
	m_name = MILAELEMENT;
}

CMilaElement::~CMilaElement() {}

bool CMilaElement::InitializeConverter()
{
	return milaConverter.Initialize();
}

bool CMilaElement::ParseAttrib(char_t* attrName, bool required, char* defaultValue, bool print, xml_node* node )
{
	xml_attribute attr;

	if (!node)
	{
		node = m_doc;
	}
	attr = node->attribute(attrName);
	if (attr) // if attribute exist print is value
	{
		if (print){
			*m_out << attr.value() << "\t";
		}
	}
	if (!attr)
	{
		if (required){ // if required return error and exit the program
			return false;
		}
		else // Not required
		{
			if (print){
			*m_out << defaultValue << "\t";
			}
		}
	}

	return true;
}
