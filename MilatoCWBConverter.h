/*
 * MilatoCWBConverter.h
 *
 *  Created on: Jan 19, 2013
 *      Author: haims
 */

#ifndef MILATOCWBCONVERTER_H_
#define MILATOCWBCONVERTER_H_

#include "Corpus.h"
#include <string>
#include <string.h>
#include <strings.h>
#include <algorithm>
#include <map>
#include <fts.h>
#include <sys/types.h>
#include <sys/stat.h>


using namespace std;
using namespace pugi;

class CMilatoCWBConverter {
public:
	CMilatoCWBConverter();
	virtual ~CMilatoCWBConverter();

	//Initialize the converter class with the data from the configuration file
	bool Init();

	//Print the current configuration paths
	void PrintConfigurationPaths();

	//Delete all the files in the vrt folder
	bool CleanTheVrtFolder();

	//Convert all the mila xml's to vrt files and store them in the vrt folder
	bool ConvertFromMilaToVrt();

	//Run the cwb-encode exec on each corpus in the vrt folder
	bool EncodeVrtToCWB();

	//Run the cwb-make on each corpus in the vrt folder
	bool CompressCorpus();

	//Create a Container Corpus which build by all the corpora in the vrt folder
	bool CreateAContainerCorpus();

private:

	//Create the corpus folders in the CWBData folders
	bool CreateCorpusFolders();

	string m_MilaFolderPath;
	string m_VrtFolderPath;
	string m_CWBDataFolderPath;
	string m_CWBRegistryFolderPath;
};

#endif /* MILATOCWBCONVERTER_H_ */
