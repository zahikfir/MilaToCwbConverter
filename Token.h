/*
 * Token.h
 *
 *  Created on: Dec 31, 2012
 *      Author: haims
 */

#ifndef TOKEN_H_
#define TOKEN_H_

#include "MilaElement.h"
#include "Analysis.h"

using namespace std;
using namespace pugi;

#define TOKEN "token"

class CToken : public CMilaElement {
public:
	CToken(xml_node* doc, ofstream* out);
	virtual ~CToken();
	virtual bool Parse();

protected:
	xml_node FindCorrectAnalysis();
};

#endif /* TOKEN_H_ */
