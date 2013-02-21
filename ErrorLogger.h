/*
 * ErrorLogger.h
 *
 *  Created on: Feb 21, 2013
 *      Author: nlpuser
 */

#ifndef ERRORLOGGER_H_
#define ERRORLOGGER_H_

#include "CustomDefinitions.h"

using namespace std;
using namespace pugi;

class CErrorLogger {
public:
	CErrorLogger();
	virtual ~CErrorLogger();

	bool Init(string sErrorFile);
	void PrintError(string sError , xml_node* node);

	string GetErrorFileString();

	void SetCurrentFile(string sFile);
	void StartANewText(string sText);

private:
	ofstream* m_ErrorFile;
	string m_sErrorfile;
	string m_sCurrentFile;
};


#endif /* ERRORLOGGER_H_ */
