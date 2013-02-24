/*
 * Paragraph.h
 *
 *  Created on: Dec 31, 2012
 *      Author: haims
 */

#ifndef PARAGRAPH_H_
#define PARAGRAPH_H_

#include "MilaElement.h"
#include "Sentence.h"

using namespace std;
using namespace pugi;

class CParagraph: public CMilaElement {
public:
	CParagraph(xml_node* doc, ofstream* out  , ofstream* containerOut);
	virtual ~CParagraph();
	virtual bool Parse();
};

#endif /* PARAGRAPH_H_ */
