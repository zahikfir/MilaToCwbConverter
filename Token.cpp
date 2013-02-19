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
		//cout << "The correct analysis of token "<< m_doc->attribute("id").value() << " is " << correctAnalysisNode.attribute("id").value() << endl;

		CAnalysis analysis(&correctAnalysisNode, m_out);
		if (!analysis.Parse())
			return false;

		//endline
		*m_out << endl;

	}
	else
	{
		cout<< endl << "There is no analysis chosen" << endl;
	}


	//Finish parse
	return true;
}

xml_node CToken::FindCorrectAnalysis(){

	//Check if there is a correct analysis
	xml_node correctAnalysisNode =  m_doc->find_child_by_attribute("score","1.0");

	//If there is one correct analysis - return it
	if (correctAnalysisNode)
	{
		return correctAnalysisNode;
	}

	//Finding a secondery analysis

	//Initialize the max score to be zero
	double  maxScore = -1;

	//get all the analysis nodes
	xml_object_range<xml_node_iterator> AnalysisNodes = m_doc->children();

	//iterate over the analysis nodes and save the analysis with the best score
	//in case there is several analysis with the same max score - take the first analysis with this score
	for (xml_node_iterator analysisIter = AnalysisNodes.begin(); analysisIter != AnalysisNodes.end(); analysisIter++)
	{
		//Get the current analysis score
		double currentScore = atof(analysisIter->attribute("score").value());

		//if the current score is greater then the maxScore
		if (currentScore > maxScore)
		{
			//Update the correctAnalysisNode to be the current analysis node
			correctAnalysisNode = *analysisIter;
			maxScore = currentScore;
		}
	}

	//return the chosen analysis
	return correctAnalysisNode;

}
