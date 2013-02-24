/*
 * Sentence.cpp
 *
 *  Created on: Dec 31, 2012
 *      Author: haims
 */

#include "Sentence.h"

CSentence::CSentence(xml_node* doc, ofstream* out , ofstream* containerOut) :CMilaElement(doc, out, containerOut) {
	m_name = SENTENCE;
	*m_out << "<s>" << endl;
	*m_ContainerOut << "<s>" << endl;
}

CSentence::~CSentence() {
	*m_out << "</s>" << endl;
	*m_ContainerOut << "</s>" << endl;
}

bool CSentence::Parse(){
	//Parse the sentence Element

	for (xml_node node = m_doc->child(TOKEN);
			node;
			node = node.next_sibling(TOKEN))
	{

		CToken token(&node, m_out, m_ContainerOut);
		if (!token.Parse())
			return false;
	}


	//Finish parse
	return true;
}
