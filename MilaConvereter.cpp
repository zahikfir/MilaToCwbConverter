/*
 * MilaConvereter.cpp
 *
 *  Created on: Jan 3, 2013
 *      Author: haims
 */

#include "MilaConvereter.h"

CMilaConvereter::CMilaConvereter() {
}

CMilaConvereter::~CMilaConvereter() {
	m_PrefixMap.clear();
	m_SuffixMap.clear();
}

bool CMilaConvereter::Initialize()
{
	if (!(InitializePrefixMap() && InitializeBaseMap() && InitializeSuffixMap()))
			return false;
	return true;
}

bool CMilaConvereter::InitializePrefixMap()
{
	//Initialize PrefixMap

	//Surface attribute
	map<string , string> SurfaceMap;
	SurfaceMap.insert(pair<string, string>("מ","a"));
	SurfaceMap.insert(pair<string, string>("ש","b"));
	SurfaceMap.insert(pair<string, string>("ה","c"));
	SurfaceMap.insert(pair<string, string>("ו","d"));
	SurfaceMap.insert(pair<string, string>("כ","e"));
	SurfaceMap.insert(pair<string, string>("ל","f"));
	SurfaceMap.insert(pair<string, string>("ב","g"));
	SurfaceMap.insert(pair<string, string>("כש","h"));
	SurfaceMap.insert(pair<string, string>("מש","i"));
	SurfaceMap.insert(pair<string, string>("מב","j"));
	SurfaceMap.insert(pair<string, string>("מל","k"));
	SurfaceMap.insert(pair<string, string>("בש","l"));
	SurfaceMap.insert(pair<string, string>("לכש","m"));
	SurfaceMap.insert(pair<string, string>("undifined","n"));
	SurfaceMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_PrefixMap.insert(pair < string, map < string, string > >("surface",SurfaceMap));

	//MultiWord
	map<string , string> FunctionMap;
	FunctionMap.insert(pair<string, string>("relativizer","a"));
	FunctionMap.insert(pair<string, string>("conjunction","b"));
	FunctionMap.insert(pair<string, string>("definiteArticle","c"));
	FunctionMap.insert(pair<string, string>("subordinatingConjunction","d"));
	FunctionMap.insert(pair<string, string>("relativizer/subordinatingConjunction","e"));
	FunctionMap.insert(pair<string, string>("temporalSubConj","f"));
	FunctionMap.insert(pair<string, string>("interrogative","g"));
	FunctionMap.insert(pair<string, string>("tenseInversion","h"));
	FunctionMap.insert(pair<string, string>("preposition","i"));
	FunctionMap.insert(pair<string, string>("adverb","j"));
	FunctionMap.insert(pair<string, string>("unspecified","k"));
	FunctionMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_PrefixMap.insert(pair < string, map < string, string > >("function",FunctionMap));


	//MultiWord
	map<string , string> MultiWordMap;
	MultiWordMap.insert(pair<string, string>("true","a"));
	MultiWordMap.insert(pair<string, string>("false","b"));
	MultiWordMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_PrefixMap.insert(pair < string, map < string, string > >("multiWord",MultiWordMap));

	return true;
}

bool CMilaConvereter::InitializeBaseMap(){
	//Initialize BaseMap

	//BaseType attribute
	map<string , string> BaseTypeMap;
	BaseTypeMap.insert(pair<string, string>("adjective","a"));
	BaseTypeMap.insert(pair<string, string>("adverb","b"));
	BaseTypeMap.insert(pair<string, string>("zevel","c"));
	BaseTypeMap.insert(pair<string, string>("conjunction","d"));
	BaseTypeMap.insert(pair<string, string>("interjection","e"));
	BaseTypeMap.insert(pair<string, string>("interrogative","f"));
	BaseTypeMap.insert(pair<string, string>("negation","g"));
	BaseTypeMap.insert(pair<string, string>("foreign","h"));
	BaseTypeMap.insert(pair<string, string>("url","i"));
	BaseTypeMap.insert(pair<string, string>("noun","j"));
	BaseTypeMap.insert(pair<string, string>("preposition","k"));
	BaseTypeMap.insert(pair<string, string>("pronoun","l"));
	BaseTypeMap.insert(pair<string, string>("properName","m"));
	BaseTypeMap.insert(pair<string, string>("punctuation","n"));
	BaseTypeMap.insert(pair<string, string>("numberExpression","o"));
	BaseTypeMap.insert(pair<string, string>("quantifier","p"));
	BaseTypeMap.insert(pair<string, string>("verb","q"));
	BaseTypeMap.insert(pair<string, string>("participle","r"));
	BaseTypeMap.insert(pair<string, string>("numeral","s"));
	BaseTypeMap.insert(pair<string, string>("existential","t"));
	BaseTypeMap.insert(pair<string, string>("impersonal","u"));
	BaseTypeMap.insert(pair<string, string>("wPrefix","v"));
	BaseTypeMap.insert(pair<string, string>("modal","w"));
	BaseTypeMap.insert(pair<string, string>("copula","x"));
	BaseTypeMap.insert(pair<string, string>("title","y"));
	BaseTypeMap.insert(pair<string, string>("MWE","z"));
	BaseTypeMap.insert(pair<string, string>("unknown","0"));
	BaseTypeMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("BaseType",BaseTypeMap));

	//gender
	map<string, string> GenderMap;
	GenderMap.insert(pair<string, string>("masculine","a"));
	GenderMap.insert(pair<string, string>("feminine","b"));
	GenderMap.insert(pair<string, string>("masculine and feminine","c"));
	GenderMap.insert(pair<string, string>("unspecified","d"));
	GenderMap.insert(pair<string, string>("irrelevant","e"));
	GenderMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("gender",GenderMap));

	//number
	map<string, string> NumberMap;
	NumberMap.insert(pair<string, string>("singular","a"));
	NumberMap.insert(pair<string, string>("plural","b"));
	NumberMap.insert(pair<string, string>("dual","c"));
	NumberMap.insert(pair<string, string>("dual and plural","d"));
	NumberMap.insert(pair<string, string>("singular and plural","e"));
	NumberMap.insert(pair<string, string>("unspecified","f"));
	NumberMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("number",NumberMap));

	//status
	map<string, string> StatusMap;
	StatusMap.insert(pair<string, string>("absolute","a"));
	StatusMap.insert(pair<string, string>("construct","b"));
	StatusMap.insert(pair<string, string>("absolute and construct","c"));
	StatusMap.insert(pair<string, string>("unspecified","d"));
	StatusMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("status",StatusMap));

	//definiteness
	map<string, string> DefinitenessMap;
	DefinitenessMap.insert(pair<string, string>("true","a"));
	DefinitenessMap.insert(pair<string, string>("false","b"));
	DefinitenessMap.insert(pair<string, string>("unspecified","c"));
	DefinitenessMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("definiteness",DefinitenessMap));

	//interrogative
	map<string, string> InterrogativeMap;
	InterrogativeMap.insert(pair<string, string>("true","a"));
	InterrogativeMap.insert(pair<string, string>("false","b"));
	InterrogativeMap.insert(pair<string, string>("unspecified","c"));
	InterrogativeMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("interrogative",InterrogativeMap));

	//expansion

	//function

	//foreign
	map<string, string> ForeignMap;
	ForeignMap.insert(pair<string, string>("true","a"));
	ForeignMap.insert(pair<string, string>("false","b"));
	ForeignMap.insert(pair<string, string>("unspecified","c"));
	ForeignMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("foreign",ForeignMap));

	//register
	map<string, string> RegisterMap;
	RegisterMap.insert(pair<string, string>("formal","a"));
	RegisterMap.insert(pair<string, string>("archaic","b"));
	RegisterMap.insert(pair<string, string>("informal","c"));
	RegisterMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("register",RegisterMap));

	//spelling
	map<string, string> SpellingMap;
	SpellingMap.insert(pair<string, string>("standard","a"));
	SpellingMap.insert(pair<string, string>("irregular","b"));
	SpellingMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("spelling",SpellingMap));

	//person
	map<string , string> PersonMap;
	PersonMap.insert(pair<string, string>("1","a"));
	PersonMap.insert(pair<string, string>("2","b"));
	PersonMap.insert(pair<string, string>("3","c"));
	PersonMap.insert(pair<string, string>("any","d"));
	PersonMap.insert(pair<string, string>("unspecified","e"));
	PersonMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("person",PersonMap));

	//ConjunctionType
	map<string, string> ConjunctionTypeMap;
	ConjunctionTypeMap.insert(pair<string, string>("coordinating","a"));
	ConjunctionTypeMap.insert(pair<string, string>("subordinating","b"));
	ConjunctionTypeMap.insert(pair<string, string>("relativizing","c"));
	ConjunctionTypeMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("ConjunctionType",ConjunctionTypeMap));

	//InterrogativeType
	map<string, string> InterrogativeTypeMap;
	InterrogativeTypeMap.insert(pair<string, string>("pronoun","a"));
	InterrogativeTypeMap.insert(pair<string, string>("proadverb","b"));
	InterrogativeTypeMap.insert(pair<string, string>("prodet","c"));
	InterrogativeTypeMap.insert(pair<string, string>("yesno","d"));
	InterrogativeTypeMap.insert(pair<string, string>("unspecified","e"));
	InterrogativeTypeMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("InterrogativeType",InterrogativeTypeMap));

	//PronounType
	map<string, string> PronounTypeMap;
	PronounTypeMap.insert(pair<string, string>("interrogative","a"));
	PronounTypeMap.insert(pair<string, string>("personal","b"));
	PronounTypeMap.insert(pair<string, string>("demonstrative","c"));
	PronounTypeMap.insert(pair<string, string>("impersonal","d"));
	PronounTypeMap.insert(pair<string, string>("relativizer","e"));
	PronounTypeMap.insert(pair<string, string>("reflexive","f"));
	PronounTypeMap.insert(pair<string, string>("unspecified","g"));
	PronounTypeMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("PronounType",PronounTypeMap));

	//NamedEntityType
	map<string, string> NamedEntityTypeMap;
	NamedEntityTypeMap.insert(pair<string, string>("person","a"));
	NamedEntityTypeMap.insert(pair<string, string>("location","b"));
	NamedEntityTypeMap.insert(pair<string, string>("organization","c"));
	NamedEntityTypeMap.insert(pair<string, string>("product","d"));
	NamedEntityTypeMap.insert(pair<string, string>("dateTime","e"));
	NamedEntityTypeMap.insert(pair<string, string>("country","f"));
	NamedEntityTypeMap.insert(pair<string, string>("town","g"));
	NamedEntityTypeMap.insert(pair<string, string>("language","h"));
	NamedEntityTypeMap.insert(pair<string, string>("symbol","i"));
	NamedEntityTypeMap.insert(pair<string, string>("art","j"));
	NamedEntityTypeMap.insert(pair<string, string>("other","k"));
	NamedEntityTypeMap.insert(pair<string, string>("unspecified","l"));
	NamedEntityTypeMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("NamedEntityType",NamedEntityTypeMap));

	//PunctuationType
	map<string, string> PunctuationTypeMap;
	PunctuationTypeMap.insert(pair<string, string>("letter","a"));
	PunctuationTypeMap.insert(pair<string, string>("dash","b"));
	PunctuationTypeMap.insert(pair<string, string>("diacritic","c"));
	PunctuationTypeMap.insert(pair<string, string>("apostrophe","d"));
	PunctuationTypeMap.insert(pair<string, string>("whitespace","e"));
	PunctuationTypeMap.insert(pair<string, string>("bullet","f"));
	PunctuationTypeMap.insert(pair<string, string>("connector","g"));
	PunctuationTypeMap.insert(pair<string, string>("open","h"));
	PunctuationTypeMap.insert(pair<string, string>("close","i"));
	PunctuationTypeMap.insert(pair<string, string>("symbol","j"));
	PunctuationTypeMap.insert(pair<string, string>("mathSymbol","k"));
	PunctuationTypeMap.insert(pair<string, string>("currencySymbol","l"));
	PunctuationTypeMap.insert(pair<string, string>("Separator","m"));
	PunctuationTypeMap.insert(pair<string, string>("lineSeparator","n"));
	PunctuationTypeMap.insert(pair<string, string>("unspecified","o"));
	PunctuationTypeMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("PunctuationType",PunctuationTypeMap));

	//NumberExpressionType
	map<string, string> NumberExpressionTypeMap;
	NumberExpressionTypeMap.insert(pair<string, string>("date","a"));
	NumberExpressionTypeMap.insert(pair<string, string>("time","b"));
	NumberExpressionTypeMap.insert(pair<string, string>("gameScore","c"));
	NumberExpressionTypeMap.insert(pair<string, string>("unspecified","d"));
	NumberExpressionTypeMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("NumberExpressionType",NumberExpressionTypeMap));

	//QuantifierType
	map<string, string> QuantifierTypeMap;
	QuantifierTypeMap.insert(pair<string, string>("amount","a"));
	QuantifierTypeMap.insert(pair<string, string>("partitive","b"));
	QuantifierTypeMap.insert(pair<string, string>("determiner","c"));
	QuantifierTypeMap.insert(pair<string, string>("unspecified","d"));
	QuantifierTypeMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("QuantifierType",QuantifierTypeMap));

	//tense
	map<string, string> TenseMap;
	TenseMap.insert(pair<string, string>("past","a"));
	TenseMap.insert(pair<string, string>("present","b"));
	TenseMap.insert(pair<string, string>("beinoni","c"));
	TenseMap.insert(pair<string, string>("future","d"));
	TenseMap.insert(pair<string, string>("imperative","e"));
	TenseMap.insert(pair<string, string>("infinitive","f"));
	TenseMap.insert(pair<string, string>("bareInfinitive","g"));
	TenseMap.insert(pair<string, string>("unspecified","h"));
	TenseMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("tense",TenseMap));

	//root

	//binyan
	map<string, string> BinyanMap;
	BinyanMap.insert(pair<string, string>("Pa'al","a"));
	BinyanMap.insert(pair<string, string>("Nif'al","b"));
	BinyanMap.insert(pair<string, string>("Pi'el","c"));
	BinyanMap.insert(pair<string, string>("Pu'al","d"));
	BinyanMap.insert(pair<string, string>("Hif'il","e"));
	BinyanMap.insert(pair<string, string>("Huf'al","f"));
	BinyanMap.insert(pair<string, string>("Hitpa'el","g"));
	BinyanMap.insert(pair<string, string>("unspecified","h"));
	BinyanMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("binyan",BinyanMap));

	//subcoordinating

	//mood

	//ParticiplePosType
	map<string, string> ParticiplePosTypeMap;
	ParticiplePosTypeMap.insert(pair<string, string>("noun","a"));
	ParticiplePosTypeMap.insert(pair<string, string>("adjective","b"));
	ParticiplePosTypeMap.insert(pair<string, string>("verb","c"));
	ParticiplePosTypeMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("ParticiplePosType",ParticiplePosTypeMap));

	//NumeralType
	map<string, string> NumeralTypeMap;
	NumeralTypeMap.insert(pair<string, string>("numeral ordinal","a"));
	NumeralTypeMap.insert(pair<string, string>("numeral cardinal","b"));
	NumeralTypeMap.insert(pair<string, string>("numeral fractional","c"));
	NumeralTypeMap.insert(pair<string, string>("literal number","d"));
	NumeralTypeMap.insert(pair<string, string>("gematria","e"));
	NumeralTypeMap.insert(pair<string, string>("unspecified","f"));
	NumeralTypeMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("NumeralType",NumeralTypeMap));

	//value

	//polarity
	map<string, string> PolarityMap;
	PolarityMap.insert(pair<string, string>("positive","a"));
	PolarityMap.insert(pair<string, string>("negative","b"));
	PolarityMap.insert(pair<string, string>("unspecified","c"));
	PolarityMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_BaseMap.insert(pair < string, map < string, string > >("polarity",PolarityMap));


	//MWEType Attributes

		//id

		//pos

		//consecutive

		//type(MWE Type)

		//multiWord

		//multiWordPrefixExist
		map <string, string> MultiWordPrexifExistMap;
		MultiWordPrexifExistMap.insert(pair<string, string>("true", "a"));
		MultiWordPrexifExistMap.insert(pair<string, string>("false", "b"));
		MultiWordPrexifExistMap.insert(pair<string, string>(EMPTYATTRIB, EMPTYATTRIB));

		m_BaseMap.insert(pair < string, map < string, string > >("multiWordPrefixExist", MultiWordPrexifExistMap));

	return true;
}

bool CMilaConvereter::InitializeSuffixMap()
{
	//Initialize SuffixMap

	//Surface attribute
	map<string , string> SurfaceMap;
	SurfaceMap.insert(pair<string, string>("י","a"));
	SurfaceMap.insert(pair<string, string>("כ","b"));
	SurfaceMap.insert(pair<string, string>("ו","c"));
	SurfaceMap.insert(pair<string, string>("ה","d"));
	SurfaceMap.insert(pair<string, string>("נו","e"));
	SurfaceMap.insert(pair<string, string>("כם","f"));
	SurfaceMap.insert(pair<string, string>("כן","g"));
	SurfaceMap.insert(pair<string, string>("ם","h"));
	SurfaceMap.insert(pair<string, string>("ן","i"));
	SurfaceMap.insert(pair<string, string>("יך","j"));
	SurfaceMap.insert(pair<string, string>("יו","k"));
	SurfaceMap.insert(pair<string, string>("יה","l"));
	SurfaceMap.insert(pair<string, string>("ינו","m"));
	SurfaceMap.insert(pair<string, string>("יכם","n"));
	SurfaceMap.insert(pair<string, string>("יכן","o"));
	SurfaceMap.insert(pair<string, string>("יהם","p"));
	SurfaceMap.insert(pair<string, string>("יהן","q"));
	SurfaceMap.insert(pair<string, string>("unspecified","r"));
	SurfaceMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_SuffixMap.insert(pair < string, map < string, string > >("surface",SurfaceMap));

	//Function
	map<string , string> FunctionMap;
	FunctionMap.insert(pair<string, string>("possessive","a"));
	FunctionMap.insert(pair<string, string>("accusative","b"));
	FunctionMap.insert(pair<string, string>("nominative","c"));
	FunctionMap.insert(pair<string, string>("accusative or nominative","d"));
	FunctionMap.insert(pair<string, string>("pronomial","e"));
	FunctionMap.insert(pair<string, string>("unspecified","f"));
	FunctionMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_SuffixMap.insert(pair < string, map < string, string > >("function",FunctionMap));


	//person
	map<string , string> PersonMap;
	PersonMap.insert(pair<string, string>("1","a"));
	PersonMap.insert(pair<string, string>("2","b"));
	PersonMap.insert(pair<string, string>("3","c"));
	PersonMap.insert(pair<string, string>("any","d"));
	PersonMap.insert(pair<string, string>("unspecified","e"));
	PersonMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_SuffixMap.insert(pair < string, map < string, string > >("person",PersonMap));

	//gender
	map<string, string> GenderMap;
	GenderMap.insert(pair<string, string>("masculine","a"));
	GenderMap.insert(pair<string, string>("feminine","b"));
	GenderMap.insert(pair<string, string>("masculine and feminine","c"));
	GenderMap.insert(pair<string, string>("unspecified","d"));
	GenderMap.insert(pair<string, string>("irrelevant","e"));
	GenderMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_SuffixMap.insert(pair < string, map < string, string > >("gender",GenderMap));

	//number
	map<string, string> NumberMap;
	NumberMap.insert(pair<string, string>("singular","a"));
	NumberMap.insert(pair<string, string>("plural","b"));
	NumberMap.insert(pair<string, string>("dual","c"));
	NumberMap.insert(pair<string, string>("dual and plural","d"));
	NumberMap.insert(pair<string, string>("singular and plural","e"));
	NumberMap.insert(pair<string, string>("unspecified","f"));
	NumberMap.insert(pair<string, string>(EMPTYATTRIB,EMPTYATTRIB));

	m_SuffixMap.insert(pair < string, map < string, string > >("number",NumberMap));


	return true;
}

string CMilaConvereter::GetPrefixConvertedString(string attribName, string attribValue)
{
	//Checking the prefix maps
	// TODO : For debug purposes - Remove it when done
	if (m_PrefixMap[attribName][attribValue]  == "")
	{
		cout << "Error finding Prefix attrib: " << attribName << " value: " << attribValue << endl;
		assert(0);
	}

	//cout << "Prefix attrib:" << attribName << " value:" << attribValue << " Translated: " << m_PrefixMap[attribName][attribValue] << endl;
	//Return the converted string
	return m_PrefixMap[attribName][attribValue];
}

string CMilaConvereter::GetBaseConvertedString(string attribName, string attribValue){
	//Checking the Base maps
	// TODO : For debug purposes - Remove it when done
	if (m_BaseMap[attribName][attribValue]  == "")
	{
		cout << "Error finding Base attrib: " << attribName << " value: " << attribValue << endl;
		assert(0);
	}

	//cout << "Base attrib:" << attribName << " value:" << attribValue << " Translated: " << m_BaseMap[attribName][attribValue] << endl;
	//Return the converted string
	return m_BaseMap[attribName][attribValue];
}

string CMilaConvereter::GetSuffixConvertedString(string attribName, string attribValue)
{
	//Checking the Suffix maps
	// TODO : For debug purposes - Remove it when done
	if (m_SuffixMap[attribName][attribValue]  == "")
	{
		cout << "Error finding Suffix attrib: " << attribName << " value: " << attribValue << endl;
		assert(0);
	}

	//cout << "Suffix attrib:" << attribName << " value:" << attribValue << " Translated: " << m_SuffixMap[attribName][attribValue] << endl;
	//Return the converted string
	return m_SuffixMap[attribName][attribValue];
}
