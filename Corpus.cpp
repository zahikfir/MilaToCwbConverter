/*
 * Corpus.cpp
 *
 *  Created on: Dec 31, 2012
 *      Author: haims
 */

#include "Corpus.h"

CCorpus::CCorpus(xml_node* doc, ofstream* out, ofstream* containerOut,string currentFolder ,string containerTextId)
:CMilaElement(doc, out, containerOut) {
	m_name = CORPUS;
    *m_out << "<text id=\"" << currentFolder << "\">" << endl;
	*m_ContainerOut << "<text id=\"" << containerTextId << "\">" << endl;
}

CCorpus::~CCorpus() {

}

bool CCorpus::Parse(){
	//Parse the corpus Element
	for (xml_node node = m_doc->child(ARTICLE);
			node;
			node = node.next_sibling(ARTICLE))
	{
		CArticle article(&node, m_out , m_ContainerOut);

		if (!article.Parse())
			return false;
	}

	//Adding dummy sentence for the compression to succeed.
	//The cwb-make compression faild an attribute have only one value all over the file
	//We fix it by adding a dummy sentence to the end of every file

	//Structural attributes
	*m_out << "<a>" << endl << "<p>" << endl << "<s>" << endl;
	*m_ContainerOut << "<a>" << endl << "<p>" << endl << "<s>" << endl;

	//The dummy sentence
	*m_out << DUMMYSENTENCE << endl;
	*m_ContainerOut << DUMMYSENTENCE << endl;

	//Closing structural attributes
	*m_out << "</s>" << endl << "</p>" << endl << "</a>" << endl;
	*m_ContainerOut << "</s>" << endl << "</p>" << endl << "</a>" << endl;

	//Closing the text file
	*m_out << "</text>" << endl;
	*m_ContainerOut << "</text>" << endl;

	//Finish parse
	return true;
}

