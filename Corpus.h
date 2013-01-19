/*
 * Corpus.h
 *
 *  Created on: Dec 31, 2012
 *      Author: haims
 */

#ifndef CORPUS_H_
#define CORPUS_H_

#include "MilaElement.h"
#include "Article.h"
#include <string>

using namespace std;
using namespace pugi;

#define CORPUS "corpus"

class CCorpus:public CMilaElement{
public:
	CCorpus(xml_node* doc ,ofstream* out, string currentFolder);
	virtual ~CCorpus();
	virtual bool Parse();
};

#endif /* CORPUS_H_ */
