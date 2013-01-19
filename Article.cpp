/*
 * Article.cpp
 *
 *  Created on: Dec 31, 2012
 *      Author: haims
 */

#include "Article.h"

CArticle::CArticle(xml_node* doc, ofstream* out) :CMilaElement(doc, out) {
	m_name = ARTICLE;
	*m_out << "<a>" << endl;
}

CArticle::~CArticle() {
	*m_out << "</a>" << endl;
}

bool CArticle::Parse(){
	//Parse the corpus Element
	for (xml_node node = m_doc->child(PARAGRAPH);
			node;
			node = node.next_sibling(PARAGRAPH))
	{
		CParagraph paragraph(&node, m_out);

		if (!paragraph.Parse())
			return false;
	}

	//Finish parse
	return true;
}
