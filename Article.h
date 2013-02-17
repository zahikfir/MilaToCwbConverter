/*
 * Article.h
 *
 *  Created on: Dec 31, 2012
 *      Author: haims
 */

#ifndef ARTICLE_H_
#define ARTICLE_H_

#include "MilaElement.h"
#include "Paragraph.h"

using namespace std;
using namespace pugi;

class CArticle :public CMilaElement {
public:
	CArticle(xml_node* doc, ofstream* out);
	virtual ~CArticle();
	virtual bool Parse();
};

#endif /* ARTICLE_H_ */
