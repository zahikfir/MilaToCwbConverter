/*
 * ErrorLogger.cpp
 *
 *  Created on: Feb 21, 2013
 *      Author: nlpuser
 */

#include "ErrorLogger.h"



CErrorLogger::CErrorLogger() {
	m_ErrorFile = NULL;
	m_sCurrentFile = "";
}

CErrorLogger::~CErrorLogger() {

	m_ErrorFile->close();
}

bool CErrorLogger::Init(string sErrorFile){
	m_sErrorfile = sErrorFile;

	m_ErrorFile = new ofstream();
	m_ErrorFile->open(m_sErrorfile.data());

	if (!m_ErrorFile->is_open())
	{
		cout << endl << endl << "Error opening the Error Logger, "<< endl <<
				"Please check the configuration file" << endl << "And validate that the path to this file exist" << endl << endl;
		return false;
	}


	*m_ErrorFile << "Error logger For Mila XML converter" << endl;
	*m_ErrorFile << "This file loggs all the errors that found on the converted mila text" << endl << endl;

	return true;
}


string CErrorLogger::GetErrorFileString(){
	return m_sErrorfile;
}

void CErrorLogger::SetCurrentFile(string sFile){
	m_sCurrentFile = sFile;
}

void CErrorLogger::StartANewText(string sText){
	*m_ErrorFile << "---------------------------------------------------------" << endl
				 << "---------------------------------------------------------" << endl
				 << sText << endl << endl;
}

void CErrorLogger::PrintError(string sError ,xml_node* node){

	*m_ErrorFile  << m_sCurrentFile << ":  " << sError << endl;
	node->print(*m_ErrorFile);
	*m_ErrorFile << endl << endl;
}

