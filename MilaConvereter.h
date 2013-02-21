/*
 * MilaConvereter.h
 *
 *  Created on: Jan 3, 2013
 *      Author: haims
 */

#ifndef MILACONVERETER_H_
#define MILACONVERETER_H_

#include "ErrorLogger.h"
#include <assert.h>
#include <map>



using namespace std;
using namespace pugi;

typedef map< string, string > MapStringString;
extern CErrorLogger* errorLogger;

class CMilaConvereter {
public:
	CMilaConvereter();
	virtual ~CMilaConvereter();

	bool Initialize();

	string GetPrefixConvertedString(string attribName, string attribValue, xml_node* node);
	string GetBaseConvertedString  (string attribName, string attribValue, xml_node* node);
	string GetSuffixConvertedString(string attribName, string attribValue, xml_node* node);

private:

	bool InitializePrefixMap();
	bool InitializeBaseMap();
	bool InitializeSuffixMap();

	map<string, MapStringString > m_PrefixMap;
	map<string, MapStringString > m_BaseMap;
	map<string, MapStringString > m_SuffixMap;

};

#endif /* MILACONVERETER_H_ */
