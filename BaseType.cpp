/*
 * BaseType.cpp
 *
 *  Created on: Jan 5, 2013
 *      Author: haims
 */

#include "BaseType.h"

BaseFunctionsMap CBaseType::m_BaseFunctionsMap;
StringToStringMap CBaseType::m_BaseTypeMap;
CMilaConvereter* CBaseType::m_Converter = NULL;

CBaseType::CBaseType(CMilaConvereter* converter){

	m_Converter = converter;
	InitializeBaseFunctions();
}

CBaseType::~CBaseType() {
	m_BaseFunctionsMap.clear();
}

bool CBaseType::InitializeBaseFunctions(){

	//BaseTypes
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("adjective", &adjective));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("adverb",&adverb));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("zevel",&zevel));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("conjunction",&conjunction));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("interjection",&interjection));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("interrogative",&interrogative));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("negation",&negation));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("foreign",&foreign));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("url",&url));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("noun",&noun));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("preposition",&preposition));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("pronoun",&pronoun));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("properName",&properName));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("punctuation",&punctuation));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("numberExpression",&numberExpression));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("quantifier",&quantifier));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("verb",&verb));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("participle",&participle));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("numeral",&numeral));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("existential",&existential));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("impersonal",&impersonal));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("wPrefix",&wPrefix));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("modal",&modal));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("copula",&copula));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("title",&title));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("MWE",&MWE));
	m_BaseFunctionsMap.insert(pair<string, BaseDelegate>("unknown",&unknown));

	//TypeAttributesMap
	m_BaseTypeMap.insert(pair<string, string>("conjunction", "ConjunctionType"));
	m_BaseTypeMap.insert(pair<string, string>("interrogative", "InterrogativeType"));
	m_BaseTypeMap.insert(pair<string, string>("pronoun", "PronounType"));
	m_BaseTypeMap.insert(pair<string, string>("properName", "NamedEntityType"));
	m_BaseTypeMap.insert(pair<string, string>("punctuation", "PunctuationType"));
	m_BaseTypeMap.insert(pair<string, string>("numberExpression", "NumberExpressionType"));
	m_BaseTypeMap.insert(pair<string, string>("quantifier", "QuantifierType"));
	m_BaseTypeMap.insert(pair<string, string>("participle", "ParticiplePosType"));
	m_BaseTypeMap.insert(pair<string, string>("numeral", "NumeralType"));
	return true;
}

bool CBaseType::Parse(xml_node* node, ofstream* out , ofstream* containerOut){

	if (m_BaseFunctionsMap[node->name()] != NULL)
		m_BaseFunctionsMap[node->name()](node, out , containerOut);
	else {

		errorLogger->PrintError("Created an empty base type for node  " , node);

		for (int i = 0; i < BASE_CELLS - 1 /* Base Cells -lexiconitem cell*/; i++)
		{
			//if we need to print the base flags cell
			if (i == (BASE_FLAGS_POSITION - 2)) //The first cell is the cell with the flags
			{
				//Print all the empty flags
				for (int j = 0 ; j < BASE_FLAGS ; j++)
				{
					*out << EMPTYATTRIB;
					*containerOut << EMPTYATTRIB;
				}

				//Move to the next cell
				*out << "\t";
				*containerOut << "\t";
			}
			else
			{
				*out << EMPTYCELL << "\t";
				*containerOut << EMPTYCELL << "\t";
			}
		}

		return false;
	}
	return true;
}

bool CBaseType::ParseBaseAttrib(char_t* attrName, bool required, char* defaultValue, bool print, xml_node* node, ofstream* out, ofstream* containerOut){
	xml_attribute attr;

	attr = node->attribute(attrName);
	if (attr) // if attribute exist print is value
	{
		if (print){
			*out << m_Converter->GetBaseConvertedString(attrName, attr.value() , node);
			*containerOut << m_Converter->GetBaseConvertedString(attrName, attr.value() , node);
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
				*out << m_Converter->GetBaseConvertedString(attrName, defaultValue , node);
				*containerOut << m_Converter->GetBaseConvertedString(attrName, defaultValue , node);
			}
		}
	}

	return true;
}

bool CBaseType::ParseBaseAttribToCell(char_t* attrName, bool required, char* defaultValue, bool print, xml_node* node, ofstream* out, ofstream* containerOut){
	xml_attribute attr;

	attr = node->attribute(attrName);
	if (attr) // if attribute exist print is value
	{
		if (print){
			*out << attr.value() << "\t";
			*containerOut << attr.value() << "\t";
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
			*out << defaultValue << "\t";
			*containerOut << defaultValue << "\t";
			}
		}
	}

	return true;
}

bool CBaseType::ParseTypeBaseAttrib(char_t* attrName, bool required, char* defaultValue, bool print, xml_node* node, ofstream* out, ofstream* containerOut){
	string typeName = m_BaseTypeMap[attrName];

	//cout << "The Type Name of base: " << attrName << " is: " << typeName << endl;
	if (typeName ==  "" && print)
	{
		*out << defaultValue;
		*containerOut << m_Converter->GetBaseConvertedString(typeName, defaultValue , node);
	}
	else
	{
		xml_attribute attr;

		attr = node->attribute("type");
		if (attr) // if attribute exist print is value
		{
			if (print){
				*out << m_Converter->GetBaseConvertedString(typeName, attr.value() , node);
				*containerOut << m_Converter->GetBaseConvertedString(typeName, attr.value() , node);
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
					*out << m_Converter->GetBaseConvertedString(typeName, defaultValue , node);
					*containerOut << m_Converter->GetBaseConvertedString(typeName, defaultValue , node);
				}
			}
		}
	}
	return true;
}

bool CBaseType::adjective(xml_node* node, ofstream* out, ofstream* containerOut)
{
	if (!ParseBaseAttrib("BaseType", false, "adjective", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, "masculine", true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, "singular", true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, "false", true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("adjective", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
	if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("function", false, "adjective", true, node, out, containerOut) ||
		!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
		!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
		//Type cell only valid in MWE Type
		// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
		)
	{
		return false;
	}
	//Type Cell only valid in MWE type
	*out << EMPTYCELL << "\t";

	return true;
}

bool CBaseType::adverb(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "adverb", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("adverb", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
		if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
			!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
			!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
			!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
			!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
			!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
			!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
			!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
			!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
			!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
			//Type cell only valid in MWE Type
			// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
			)
		{
			return false;
		}
		//Type Cell only valid in MWE type
		*out << EMPTYCELL << "\t";

	return true;
}

bool CBaseType::zevel(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "zevel", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("zevel", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::conjunction(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "conjunction", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("conjunction", false, "coordinating", true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::interjection(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "interjection", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("interjection", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";
	return true;

}

bool CBaseType::interrogative(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "interrogative", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("interrogative", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::negation(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "negation", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, "false", true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("negation", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::foreign(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "foreign", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::url(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "url", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("noun", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	///and now for the other base cells
	if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
		!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
		//Type cell only valid in MWE Type
		// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
		)
	{
		return false;
	}
	//Type Cell only valid in MWE type
	*out << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::noun(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "noun", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, "masculine", true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, "singular", true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, "false", true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("noun", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, "adjective", true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::preposition(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "preposition", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("preposition", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::pronoun(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "pronoun", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, "false", true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("pronoun", false, "unspecified", true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::properName(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "properName", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, "masculine", true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, "singular", true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, "false", true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("properName", false, "unspecified", true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::punctuation(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "punctuation", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("punctuation", false, "unspecified", true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::numberExpression(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "numberExpression", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("numberExpression", false, "unspecified", true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::quantifier(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "quantifier", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, "absolute and construct", true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, "false", true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("quantifier", false, "unspecified", true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;
	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::verb(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "verb", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("verb", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::participle(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "participle", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, "absolute and construct", true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, "false", true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("participle", false, "verb", true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, "false", true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::numeral(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "numeral", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, "masculine", true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, "singular", true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, "false", true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("numeral", false, "unspecified", true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::existential(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "existential", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("interrogative", false, "false", true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("existential", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, "", true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::impersonal(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "impersonal", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("impersonal", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";

	return true;
}

bool CBaseType::wPrefix(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "wPrefix", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, "false", true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("wPrefix", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	///and now for the other base cells
	if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
		!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
		//Type cell only valid in MWE Type
		// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
		)
	{
		return false;
	}
	//Type Cell only valid in MWE type
	*out << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::modal(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "modal", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("modal", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, "false", true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::copula(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "copula", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("copula", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::title(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "title", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, "false", true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("title", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}

bool CBaseType::MWE(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "MWE", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, "unspecified", true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, "false", true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, "formal", true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, "standard", true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, "false", true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("MWE", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
	if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut) ||
		!ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut))
	{
		return false;
	}

	return true;
}

bool CBaseType::unknown(xml_node* node, ofstream* out, ofstream* containerOut){
	if (!ParseBaseAttrib("BaseType", false, "unknown", true, node, out, containerOut) ||
		!ParseBaseAttrib("gender", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("number", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("status", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("definiteness", false, "false", true, node, out, containerOut) ||
		!ParseBaseAttrib("foreign", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("register", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("spelling", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("person", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("tense", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("binyan", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("polarity", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseBaseAttrib("multiWordPrefixExist", false, EMPTYATTRIB, true, node, out, containerOut) ||
		!ParseTypeBaseAttrib("unknown", false, EMPTYATTRIB, true, node, out, containerOut))
		{
			return false;
		}

	//Finished parsing flags
	*out << "\t" ;
	*containerOut << "\t" ;

	//and now for the other base cells
			if (!ParseBaseAttribToCell("expansion", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("function", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("root", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("subcoordinating", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("mood", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("value", false, EMPTYCELL, true, node, out, containerOut)||
				!ParseBaseAttribToCell("id", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("pos", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("consecutive", false, EMPTYCELL, true, node, out, containerOut) ||
				!ParseBaseAttribToCell("multiWord", false, EMPTYCELL, true, node, out, containerOut)
				//Type cell only valid in MWE Type
				// || !ParseBaseAttribToCell("type", false, EMPTYCELL, true, node, out, containerOut)
				)
			{
				return false;
			}
			//Type Cell only valid in MWE type
			*out << EMPTYCELL << "\t";
			*containerOut << EMPTYCELL << "\t";


	return true;
}


