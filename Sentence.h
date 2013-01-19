/*
 * Sentence.h
 *
 *  Created on: Dec 31, 2012
 *      Author: haims
 */

#ifndef SENTENCE_H_
#define SENTENCE_H_

#include "MilaElement.h"
#include "Token.h"

using namespace std;
using namespace pugi;

#define SENTENCE "sentence"

class CSentence : public CMilaElement {
public:
	CSentence(xml_node* doc, ofstream* out);
	virtual ~CSentence();
	virtual bool Parse();
};

#endif /* SENTENCE_H_ */
