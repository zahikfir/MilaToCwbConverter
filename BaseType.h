/*
 * BaseType.h
 *
 *  Created on: Jan 5, 2013
 *      Author: haims
 */

#ifndef BASETYPE_H_
#define BASETYPE_H_

#include "MilaElement.h"
#include <map>


typedef bool (*BaseDelegate)(xml_node* node, ofstream* out);
typedef map<string, BaseDelegate> BaseFunctionsMap;
typedef map<string, string> StringToStringMap;

class CBaseType {
public:
	CBaseType(CMilaConvereter* converter);
	~CBaseType();

	static bool Parse(xml_node* node, ofstream* out);

protected:
	bool InitializeBaseFunctions();
	static bool ParseBaseAttrib(char_t* attrName, bool required, char* defaultValue, bool print, xml_node* node, ofstream* out);
	static bool ParseBaseAttrib(string attrName, bool required, char* defaultValue, bool print, xml_node* node, ofstream* out);
	static bool ParseTypeBaseAttrib(char_t* attrName, bool required, char* defaultValue, bool print, xml_node* node, ofstream* out);
	static bool ParseBaseAttribToCell(char_t* attrName, bool required, char* defaultValue, bool print, xml_node* node, ofstream* out);

	//BaseType Functions
	static bool adjective(xml_node* node, ofstream* out);
	static bool adverb(xml_node* node, ofstream* out);
	static bool zevel(xml_node* node, ofstream* out);
	static bool conjunction(xml_node* node, ofstream* out);
	static bool interjection(xml_node* node, ofstream* out);
	static bool interrogative(xml_node* node, ofstream* out);
	static bool negation(xml_node* node, ofstream* out);
	static bool foreign(xml_node* node, ofstream* out);
	static bool url(xml_node* node, ofstream* out);
	static bool noun(xml_node* node, ofstream* out);
	static bool preposition(xml_node* node, ofstream* out);
	static bool pronoun(xml_node* node, ofstream* out);
	static bool properName(xml_node* node, ofstream* out);
	static bool punctuation(xml_node* node, ofstream* out);
	static bool numberExpression(xml_node* node, ofstream* out);
	static bool quantifier(xml_node* node, ofstream* out);
	static bool verb(xml_node* node, ofstream* out);
	static bool participle(xml_node* node, ofstream* out);
	static bool numeral(xml_node* node, ofstream* out);
	static bool existential(xml_node* node, ofstream* out);
	static bool impersonal(xml_node* node, ofstream* out);
	static bool wPrefix(xml_node* node, ofstream* out);
	static bool modal(xml_node* node, ofstream* out);
	static bool copula(xml_node* node, ofstream* out);
	static bool title(xml_node* node, ofstream* out);
	static bool MWE(xml_node* node, ofstream* out);
	static bool unknown(xml_node* node, ofstream* out);

private:
	static BaseFunctionsMap m_BaseFunctionsMap;
	static CMilaConvereter* m_Converter;
	static StringToStringMap m_BaseTypeMap;
};

#endif /* BASETYPE_H_ */
