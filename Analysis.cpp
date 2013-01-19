/*
 * Analysis.cpp
 *
 *  Created on: Dec 31, 2012
 *      Author: haims
 */

#include "Analysis.h"

///////////////////////////////////////////////////////////////////////////////////////////
// CPrefixNode implementation

CPrefixNode::CPrefixNode(xml_node* doc, ofstream* out) :CMilaElement(doc, out) {
	m_name = PREFIX;
}

CPrefixNode::~CPrefixNode(){
}

bool CPrefixNode::ParseAttrib(char_t* attrName, bool required, char* defaultValue, bool print, xml_node* node )
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
			*m_out << milaConverter.GetPrefixConvertedString(attrName, attr.value());
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
				*m_out << milaConverter.GetPrefixConvertedString(attrName, defaultValue);
			}
		}
	}

	return true;
}

bool CPrefixNode::Parse(){

	if (!ParseAttrib("id", true, EMPTYATTRIB, false) ||
		!ParseAttrib("surface", false, EMPTYATTRIB, true) ||
	    !ParseAttrib("function", false, EMPTYATTRIB, true)||
	    !ParseAttrib("multiWord", false, "false", true))
	{
		return false;
	}

	*m_out << "\t" ;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
// CBaseNode implementation

bool CBaseNode::m_Initialize = false;
CBaseType* CBaseNode::m_BaseTypeHelper = NULL;

CBaseNode::CBaseNode(xml_node* doc, ofstream* out) :CMilaElement(doc, out) {
	m_name = BASE;
	if (!m_Initialize)
	{
		m_BaseTypeHelper = new CBaseType(&milaConverter);
		m_Initialize = true;
	}
}

CBaseNode::~CBaseNode(){
}

bool CBaseNode::ParseAttrib(char_t* attrName, bool required, char* defaultValue, bool print, xml_node* node){
	xml_attribute attr;

	if (!node)
	{
		node = m_doc;
	}
	attr = node->attribute(attrName);
	if (attr) // if attribute exist print is value
	{
		if (print){
			*m_out << milaConverter.GetBaseConvertedString(attrName, attr.value());
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
				*m_out << milaConverter.GetBaseConvertedString(attrName, defaultValue);
			}
		}
	}

	return true;
}

bool CBaseNode::Parse(){

	//Priniting the lexicon

	if (m_doc->attribute("lexiconItem"))
		*m_out << m_doc->attribute("lexiconItem").value() << "\t";
	else
		*m_out << EMPTYCELL << "\t";

	//Updating the m_doc node
	xml_node node = m_doc->first_child();

	if (!m_BaseTypeHelper->Parse(&node, m_out))
	{
		cout << " lexiconItem : " << m_doc->attribute("lexiconItem").value();
		return false;
	}

	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////
// CSuffixNode implementation

CSuffixNode::CSuffixNode(xml_node* doc, ofstream* out) :CMilaElement(doc, out) {
	m_name = SUFFIX;
}

CSuffixNode::~CSuffixNode(){
}

bool CSuffixNode::ParseAttrib(char_t* attrName, bool required, char* defaultValue, bool print, xml_node* node){
	xml_attribute attr;

	if (!node)
	{
		node = m_doc;
	}
	attr = node->attribute(attrName);
	if (attr) // if attribute exist print is value
	{
		if (print){
			*m_out << milaConverter.GetSuffixConvertedString(attrName, attr.value());
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
				*m_out << milaConverter.GetSuffixConvertedString(attrName, defaultValue);
			}
		}
	}

	return true;
}

bool CSuffixNode::Parse(){

	if (!ParseAttrib("surface", false, "unspecified", true) ||
		!ParseAttrib("function", false, "unspecified", true) ||
		!ParseAttrib("person", false, "unspecified", true) ||
		!ParseAttrib("gender", false, "unspecified", true) ||
		!ParseAttrib("number", false, "unspecified", true))
	{
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// CAnalysis implementation

CAnalysis::CAnalysis(xml_node* doc, ofstream* out) :CMilaElement(doc, out) {
	m_name = ANALYSIS;
}

CAnalysis::~CAnalysis() {
}

bool CAnalysis::Parse(){
	//Parse all prefixes
	int nPrefixCounter = 0;

	for (xml_node prefixNode = m_doc->child("prefix");
			prefixNode && (nPrefixCounter < MAXPREFIX);
			prefixNode = prefixNode.next_sibling("prefix") , nPrefixCounter++)
	{
		CPrefixNode prefix(&prefixNode, m_out);
		if (!prefix.Parse())
			return false;
	}

	for (;nPrefixCounter < MAXPREFIX; nPrefixCounter++)
	{
		*m_out << EMPTYCELL << "\t";
	}



	//Parse the base node
	xml_node baseNode = m_doc->child("base");
	if (baseNode)
	{
		CBaseNode base(&baseNode, m_out);
		if (!base.Parse())
			cout << "in analysis node : " << m_doc->attribute("id").value() << endl;
	}
	else {
		for (int i = 0; i < BASECELLS; i++)
			*m_out << EMPTYCELL << "\t";
	}

	//Parse the suffix node
	xml_node suffixNode = m_doc->child("suffix");
	if (suffixNode)
	{
		CSuffixNode suffix(&suffixNode, m_out);
		suffix.Parse();
	}
	else {
		for (int i = 0; i < SUFFIXCELLS; i++)
			*m_out << EMPTYCELL;
	}

	return true;
}
