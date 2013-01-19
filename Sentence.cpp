/*
 * Sentence.cpp
 *
 *  Created on: Dec 31, 2012
 *      Author: haims
 */

#include "Sentence.h"

CSentence::CSentence(xml_node* doc, ofstream* out) :CMilaElement(doc, out) {
	m_name = SENTENCE;
	*m_out << "<s>" << endl;
}

CSentence::~CSentence() {
	*m_out << "</s>" << endl;
}

bool CSentence::Parse(){
	//Parse the sentence Element

	cout << "Start Parsing sentence number " << m_doc->attribute("id").value() << endl;

	int countTokens = 0;

	for (xml_node node = m_doc->child(TOKEN);
			node;
			node = node.next_sibling(TOKEN))
	{
		countTokens ++;

		CToken token(&node, m_out);
		if (!token.Parse())
			return false;
	}

	cout << "Finished parsing sentence "<< m_doc->attribute("id").value() << " with " << countTokens << " tokens" << endl;


	//Finish parse
	return true;
}
