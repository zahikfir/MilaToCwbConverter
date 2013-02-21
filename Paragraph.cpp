/*
 * Paragraph.cpp
 *
 *  Created on: Dec 31, 2012
 *      Author: haims
 */

#include "Paragraph.h"

CParagraph::CParagraph(xml_node* doc, ofstream* out) :CMilaElement(doc, out) {
	m_name = PARAGRAPH;
	*m_out << "<p>" << endl;
}

CParagraph::~CParagraph() {
	*m_out << "</p>" << endl;
}

bool CParagraph::Parse(){
	//Parse the corpus Element
	for (xml_node node = m_doc->child(SENTENCE);
			node;
			node = node.next_sibling(SENTENCE))
	{
		CSentence sentence(&node, m_out);

		if (!sentence.Parse())
			return false;
	}

	//Finish parse
	return true;
}
