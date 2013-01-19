/*
 * Analysis.h
 *
 *  Created on: Dec 31, 2012
 *      Author: haims
 */

#ifndef ANALYSIS_H_
#define ANALYSIS_H_

#include "MilaElement.h"
#include "BaseType.h"

using namespace std;
using namespace pugi;

#define ANALYSIS "analysis"
#define PREFIX "prefix"
#define BASE "base"
#define SUFFIX "suffix"
#define MAXPREFIX 6
#define BASECELLS 13
#define SUFFIXCELLS 1


// PrefixNode class
class CPrefixNode : public CMilaElement{
public:
	CPrefixNode(xml_node* doc, ofstream* out);
	~CPrefixNode();
	virtual bool ParseAttrib(char_t* attrName, bool required, char* defaultValue, bool print, xml_node* node = NULL);
	virtual bool Parse();
};


// BaseNode class
class CBaseNode : public CMilaElement{
public:
	CBaseNode(xml_node* doc, ofstream* out);
	~CBaseNode();
	virtual bool ParseAttrib(char_t* attrName, bool required, char* defaultValue, bool print, xml_node* node = NULL);
	virtual bool Parse();
	static bool ParseBaseAttrib(char_t* attrName, bool required, char* defaultValue, bool print, xml_node* node = NULL);

private:
	static CBaseType* m_BaseTypeHelper;
	static bool m_Initialize;
};


// SuffixNode class
class CSuffixNode : public CMilaElement{
public:
	CSuffixNode(xml_node* doc, ofstream* out);
	~CSuffixNode();
	virtual bool ParseAttrib(char_t* attrName, bool required, char* defaultValue, bool print, xml_node* node = NULL);
	virtual bool Parse();
};


class CAnalysis: public CMilaElement {
public:
	CAnalysis(xml_node* doc, ofstream* out);
	virtual ~CAnalysis();
	virtual bool Parse();
};

#endif /* ANALYSIS_H_ */
