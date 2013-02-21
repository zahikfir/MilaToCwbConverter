/*
 * MilatoCWBConverter.cpp
 *
 *  Created on: Jan 19, 2013
 *      Author: haims
 */

#include "MilatoCWBConverter.h"

CMilatoCWBConverter::CMilatoCWBConverter() {
	cout << "Creating the CMilaToCWBConverter instance." << endl;

}

CMilatoCWBConverter::~CMilatoCWBConverter() {
	cout << "Destroy the CMilaToCWBConverter instance." << endl;
}

//Initialize the converter class with the data from the configuration file
bool CMilatoCWBConverter::Init(){

	cout << "Initialize the CMilatoCWBConverter with the ConverterConfig.xml data. " << endl;

	//Creating the document
	xml_document configdoc;

	//Load the file
	xml_parse_result result = configdoc.load_file("ConverterConfig.xml");

	//Error Logger file
	string sErrorLogger;

	//Check if the xml file is valid
	if (result){
		try {
			xml_node ConfigurationNode = configdoc.child("configuration");
            if (ConfigurationNode.empty())
            	throw "configuration";
			m_MilaFolderPath = ConfigurationNode.child("MilaCorporaPath").attribute("path").value();
			if (m_MilaFolderPath == "")
				throw "MilaCorporaPath" ;
			m_VrtFolderPath = ConfigurationNode.child("VRTFolderPath").attribute("path").value();
			if (m_VrtFolderPath == "")
				throw "VRTFolderPath" ;
			m_CWBDataFolderPath = ConfigurationNode.child("CWBDataFolderPath").attribute("path").value();
			if (m_CWBDataFolderPath == "")
				throw "CWBDataFolderPath" ;
			m_CWBRegistryFolderPath = ConfigurationNode.child("CWBRegistryFolderPath").attribute("path").value();
			if (m_CWBRegistryFolderPath == "")
				throw "CWBRegistryFolderPath" ;
			sErrorLogger = ConfigurationNode.child("ErrorLogger").attribute("path").value();
			if (sErrorLogger == "")
				throw "ErrorLogger";
		}
		catch( char const* str ){
		    cout << "Problem occurred with the node: " << str << endl;
		    return false;
		}
	}
	else {
		cout << "Can't open configuration file . Closing application" << endl;
		return false;
	}

	//Init the Error Logger
	errorLogger = new CErrorLogger();
	if (!errorLogger->Init(sErrorLogger))
	{
		return false;
	}

	cout << "Initialized CMilaToCwbConverter from the configuration file successfully." << endl;
	return true;
}

//Print the current configuration paths
void CMilatoCWBConverter::PrintConfigurationPaths(){
	cout << "Configuration Paths : " << endl;
	cout << "	MilaInputFolder  : " << m_MilaFolderPath << endl;
	cout << "	VrtOutputFolder  : " << m_VrtFolderPath << endl;
	cout << "	CWBDataFolder    : " << m_CWBDataFolderPath << endl;
	cout << "	CWBRegistryFolder: " << m_CWBRegistryFolderPath << endl;
	cout << "   ErrorLogger      : " << errorLogger->GetErrorFileString() << endl;
}

//Delete all the files in the vrt folder
bool CMilatoCWBConverter::CleanTheVrtFolder(){

	cout << "-------------------------" << endl;
	cout << " Cleaning The Vrt Folder " << endl;
	cout << "-------------------------" << endl << endl;

	//Open a linux folders tree
	char *dot[] = {const_cast<char *>(m_VrtFolderPath.data()), 0};
	FTS *tree = fts_open(dot,FTS_NOCHDIR, 0);
	if (!tree) {
		perror("fts_open");
		return 1;
	}

	//Start working on every xml file in the input directory.
	FTSENT *node;

	while ((node = fts_read(tree)))
	{
		if (node->fts_level > 0 && node->fts_name[0] == '.')
			fts_set(tree, node, FTS_SKIP);
		//If directory node - will delete it
		else if ((node->fts_info & FTS_DP) && (node->fts_level > 0))
		{
			string sRemoveCommand = node->fts_accpath;

			//Replace all occurences of " " with "\ "
			string from = " ";
			string to = "\\~\\";
			while(sRemoveCommand.find(from) != std::string::npos) {
				sRemoveCommand.replace(sRemoveCommand.find(from), from.length(), to);
			}

			from = "~\\";
			to = " ";
			while(sRemoveCommand.find(from) != std::string::npos) {
				sRemoveCommand.replace(sRemoveCommand.find(from), from.length(), to);
			}

			sRemoveCommand.insert(0, "rm -r ");

			cout << "Removing folder : " << sRemoveCommand << endl;

			system(sRemoveCommand.data());
		}
	}

	cout << endl;

	if (fts_close(tree)) {
		perror("fts_close");
		return false;
	}

	return true;
}

//Convert all the mila xml's to vrt files and store them in the vrt folder
bool CMilatoCWBConverter::ConvertFromMilaToVrt(){

	cout << "-------------------------" << endl;
	cout << "     Start Converting    " << endl;
	cout << "-------------------------" << endl << endl;

	//Open a linux folders tree
	char *dot[] = {const_cast<char *>(m_MilaFolderPath.data()), 0};
	FTS *tree = fts_open(dot,FTS_NOCHDIR, 0);
	if (!tree) {
		perror("fts_open");
		return 1;
	}

	//Start working on every xml file in the input directory.
	FTSENT *node;

	//A place holder for the currnet folder
	//Every vrt file will have is current folder string as the value of the id attribute of the text node
	string sCurrentFolder;

	while ((node = fts_read(tree)))
	{
		if (node->fts_level > 0 && node->fts_name[0] == '.')
			fts_set(tree, node, FTS_SKIP);

		//If directory node - We will create this directory in the vrtfolder
		else if (node->fts_info & FTS_D)
		{
			//Saves the current folder
			sCurrentFolder = node->fts_name;

			//Check if the name is valid (a valid c variable name)
			//Add a prefix of t(text) to the value
			sCurrentFolder.insert(0,"t");

			//Replace all occurences of "-" with "_"
			string from = "-";
			string to = "_";
			while(sCurrentFolder.find(from) != std::string::npos) {
				sCurrentFolder.replace(sCurrentFolder.find(from), from.length(), to);
			}


			//Create the output folder
			 string vrtPath = node->fts_accpath;

			 //Update the Error Logger
			 errorLogger->StartANewText(node->fts_accpath);

			 vrtPath.replace(vrtPath.find(m_MilaFolderPath),m_MilaFolderPath.length(),m_VrtFolderPath.data());

			 cout << endl << "-----------------------------------------------------------------------------------------" << endl;

			 cout << " --Start converting the output folder :" << vrtPath << endl << endl;

			 mkdir(vrtPath.data() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

		}
		//If corrent node is a file
		else if (node->fts_info & FTS_F)
		{

			//The absoulte path to the mila xml file including the name
			string xmlName = node->fts_accpath;

			//Updating the ErrorLoger
			errorLogger->SetCurrentFile(xmlName);

			//The name of the output vrt file
			string outName = node->fts_name;
			outName.replace(outName.find(".xml"),4,".vrt");

			//The path to the output file
			string vrtPath = xmlName.data();
			vrtPath.replace(vrtPath.find(m_MilaFolderPath),m_MilaFolderPath.length(),m_VrtFolderPath.data());
			vrtPath.replace(vrtPath.find(node->fts_name),outName.length(),"");


			//////////////////////////////////////////////////
			//Loading document

			//Creating the document
			xml_document doc;

			//Load the file
			xml_parse_result result = doc.load_file(xmlName.data());

			//Check if the xml file is valid
			if (result)
			{
				//If valid

				//Create writing file stream
				outName.insert(0, vrtPath.data());
				ofstream outputFile(outName.data());


				xml_node corpusNode= doc.child("corpus");
				if (corpusNode)
				{
					//If enable to open output file
					if (outputFile.is_open())
					{

						//Start parsing the xml file into the outputfile

						//Create a corpus object
						CCorpus corpus(&corpusNode,&outputFile, sCurrentFolder);

						//Initialize the mila converter helper class
						corpus.InitializeConverter();

						//Start parsing
						if (!corpus.Parse())
							cout << endl << "Error Parsing " << corpusNode.attribute("name").value() << endl;

						outputFile.close();
					}
					else //if Failed to open file
					{
						cout << endl << "Unable to open output file : " << outName << endl;
						if (fts_close(tree)) {
								perror("fts_close");
								return false;
							}
						return false;
					}
				}
				else
				{
					cout << endl << "Error with file: " << xmlName << endl;
				}
			}
			else
			{
				//if Not valid print the description
				cout << endl << "Load result " << result.description() << endl;
				cout << endl << "Error offset: " << result.offset << "(error at [..." << xmlName << result.offset << "]" << endl << endl;
			}
		}
	}

	cout << endl;

	//Close the errorLogger and the error file
	delete errorLogger;

	if (fts_close(tree)) {
		perror("fts_close");
		return false;
	}
	return true;
}

//Run the cwb-encode exec on each corpus in the vrt folder
bool CMilatoCWBConverter::EncodeVrtToCWB(){
	cout << endl;
	cout << "-------------------------" << endl;
	cout << "Encoding the VRT corpora" << endl;
	cout << "-------------------------" << endl << endl;

	CreateCorpusFolders();

	//Open a linux folders tree
	char *dot[] = {const_cast<char *>(m_VrtFolderPath.data()), 0};
	FTS *tree = fts_open(dot,FTS_NOCHDIR, 0);
	if (!tree) {
		perror("fts_open");
		return 1;
	}

	//Start working on every xml file in the input directory.
	FTSENT *node;

	string subCorpusPaths;
	while ((node = fts_read(tree)))
	{
		if (node->fts_level > 0 && node->fts_name[0] == '.')
			fts_set(tree, node, FTS_SKIP);
		//If directory node - will delete it
		else if ((node->fts_info & FTS_DP) && (node->fts_level > 0))
		{
			//The corpus name
			if (node->fts_level == 1){
				string sEncodeCommand = "cwb-encode -c utf8 -d ";
				sEncodeCommand.insert(sEncodeCommand.length(),m_CWBDataFolderPath);
				sEncodeCommand.insert(sEncodeCommand.length(),node->fts_name);
				sEncodeCommand.insert(sEncodeCommand.length(),subCorpusPaths);
				sEncodeCommand.insert(sEncodeCommand.length()," -R ");
				sEncodeCommand.insert(sEncodeCommand.length(),m_CWBRegistryFolderPath);
				sEncodeCommand.insert(sEncodeCommand.length(),node->fts_name);
				sEncodeCommand.insert(sEncodeCommand.length(),
				" -P prefix1 -P prefix2 -P prefix3 -P prefix4 -P prefix5 -P prefix6 -P lexiconitem -P base -P expansion -P function -P root -P subcoordinating -P mood -P value -P id -P pos -P consecutive -P multiword -P type -P suffix -S a -S p -S s -S text:0+id 2>&1");

				cout << "Executing encode command : " << endl << endl;
				cout << sEncodeCommand << endl;

				system(sEncodeCommand.data());

				subCorpusPaths = "";
			}
			// -F paths
			else{
				string sCurrentPath = node->fts_accpath;

				//Replace all occurences of " " with "\ "
				string from = " ";
				string to = "\\~\\";
				while(sCurrentPath.find(from) != std::string::npos) {
					sCurrentPath.replace(sCurrentPath.find(from), from.length(), to);
				}

				from = "~\\";
				to = " ";
				while(sCurrentPath.find(from) != std::string::npos) {
					sCurrentPath.replace(sCurrentPath.find(from), from.length(), to);
				}

				sCurrentPath.insert(0," -F ");

				subCorpusPaths.insert(0, sCurrentPath.data());

			}
		}
	}

	cout << endl << endl;

	if (fts_close(tree)) {
		perror("fts_close");
		return false;
	}

	return true;
}

//Run the cwb-make on each corpus in the vrt folder
bool CMilatoCWBConverter::CompressCorpus(){
	cout << "-------------------------" << endl;
	cout << "Compress the encoded corpora" << endl;
	cout << "-------------------------" << endl << endl;

	//Open a linux folders tree
	char *dot[] = {const_cast<char *>(m_VrtFolderPath.data()), 0};
	FTS *tree = fts_open(dot,FTS_NOCHDIR, 0);
	if (!tree) {
		perror("fts_open");
		return 1;
	}

	//Start working on every xml file in the input directory.
	FTSENT *node;

	string subCorpusPaths;
	while ((node = fts_read(tree)))
	{
		if (node->fts_level > 0 && node->fts_name[0] == '.')
			fts_set(tree, node, FTS_SKIP);
		//If directory node - will delete it
		else if ((node->fts_info & FTS_DP) && (node->fts_level > 0))
		{
			//The corpus name
			if (node->fts_level == 1){
				string sMakeCommand = "cwb-make -r ";
				sMakeCommand.insert(sMakeCommand.length(),m_CWBRegistryFolderPath);
				sMakeCommand.insert(sMakeCommand.length()," -V ");
				string sCurrentCorpus = node->fts_name;
				transform(sCurrentCorpus.begin(),sCurrentCorpus.end(),sCurrentCorpus.begin(),::toupper);
				sMakeCommand.insert(sMakeCommand.length(),sCurrentCorpus);

				cout << "Executing make command : " << endl << endl;
				cout << sMakeCommand << endl << endl;

				system(sMakeCommand.data());

				subCorpusPaths = "";
			}
		}
	}

	cout << endl;

	if (fts_close(tree)) {
		perror("fts_close");
		return false;
	}

	return true;
}

//Create the corpus folders in the CWBData folders
bool CMilatoCWBConverter::CreateCorpusFolders(){

	cout << "Creating corpus folders  : " << endl << endl;


	//Open a linux folders tree
	char *dot[] = {const_cast<char *>(m_VrtFolderPath.data()), 0};
	FTS *tree = fts_open(dot,FTS_NOCHDIR, 0);
	if (!tree) {
		perror("fts_open");
		return 1;
	}

	//Start working on every xml file in the input directory.
	FTSENT *node;

	string subCorpusPaths;
	while ((node = fts_read(tree)))
	{
		if (node->fts_level > 0 && node->fts_name[0] == '.')
			fts_set(tree, node, FTS_SKIP);
		//If directory node - will delete it
		else if ((node->fts_info & FTS_DP) && (node->fts_level == 1))
		{
			string sCorpusPath = m_CWBDataFolderPath + node->fts_name;

			cout << "corpus folder : " << sCorpusPath << endl;
			mkdir(sCorpusPath.data() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
	}

	cout << endl;

	if (fts_close(tree)) {
		perror("fts_close");
		return false;
	}

	return true;
}
