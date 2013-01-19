/*
 * Token.cpp
 *
 *  Created on: Dec 31, 2012
 *      Author: haims
 */

#include "Token.h"

CToken::CToken(xml_node* doc, ofstream* out) :CMilaElement(doc, out) {
	m_name = TOKEN;
}


CToken::~CToken() {
}

bool CToken::Parse(){
	//Parse the corpus Element

	//Find only the correct analysis
	xml_node correctAnalysisNode = FindCorrectAnalysis();

	//Parsing only the correct analysis for the token
	if (correctAnalysisNode){

		//output the current token and tab
		*m_out << m_doc->attribute("surface").value() << "\t";

		//Print the correct analysis
		cout << "The correct analysis of token "<< m_doc->attribute("id").value() <<
				" is "<< correctAnalysisNode.attribute("id").value() << endl;

		CAnalysis analysis(&correctAnalysisNode, m_out);
		if (!analysis.Parse())
			return false;

		//endline
		*m_out << endl;

	}


	//Finish parse
	return true;
}

xml_node CToken::FindCorrectAnalysis(){
	return m_doc->find_child_by_attribute("score","1.0");
}
