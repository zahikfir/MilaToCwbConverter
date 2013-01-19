/*
 * MilaConvereter.h
 *
 *  Created on: Jan 3, 2013
 *      Author: haims
 */

#ifndef MILACONVERETER_H_
#define MILACONVERETER_H_

#include <iostream>
#include <string>
#include <assert.h>
#include <map>

using namespace std;
#define EMPTYATTRIB "|"

typedef map< string, string > MapStringString;

class CMilaConvereter {
public:
	CMilaConvereter();
	virtual ~CMilaConvereter();

	bool Initialize();

	string GetPrefixConvertedString(string attribName, string attribValue);
	string GetBaseConvertedString  (string attribName, string attribValue);
	string GetSuffixConvertedString(string attribName, string attribValue);

private:

	bool InitializePrefixMap();
	bool InitializeBaseMap();
	bool InitializeSuffixMap();

	map<string, MapStringString > m_PrefixMap;
	map<string, MapStringString > m_BaseMap;
	map<string, MapStringString > m_SuffixMap;

};

#endif /* MILACONVERETER_H_ */
