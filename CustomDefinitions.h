/*
 * Custom Definitions.h
 *
 *  Edited on: Feb, 2013
 *      Author: haims
 */

#ifndef CUSTOMDEFINITIONS_H_
#define CUSTOMDEFINITIONS_H_

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "pugixml-1.2/pugixml.hpp"
#include "pugixml-1.2/pugiconfig.hpp"

//Global Definitions File

#define CCONATINER_CORPUS_NAME "all"

#define EMPTYCELL "N"
#define EMPTYATTRIB "N"

#define DUMMYSENTENCE "סוףטקסט\t9\t9\t9\t9\t9\t9\t9\t9\t9\t9\t9\t9\t9\t9\t9\t9\t9\t9\t9\t9"

#define MILAELEMENT "milaElement"

#define CORPUS "corpus"
#define ARTICLE "article"
#define PARAGRAPH "paragraph"
#define SENTENCE "sentence"
#define TOKEN "token"


//Analyisis defines
#define ANALYSIS "analysis"
#define PREFIX "prefix"
#define BASE "base"
#define SUFFIX "suffix"
#define MAX_PREFIX 6
#define BASE_CELLS 13
#define SUFFIX_CELLS 1

#define PREFIX_FLAGS 3
#define BASE_FLAGS 14
#define BASE_FLAGS_POSITION 2 //The Second cell in the base cells is the one with the flags
#define SUFFIX_FLAGS 5

#endif
