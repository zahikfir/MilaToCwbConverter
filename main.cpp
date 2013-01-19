#include <iostream>
#include <fstream>
#include "pugixml-1.2/pugixml.hpp"
#include "pugixml-1.2/pugiconfig.hpp"
#include "Corpus.h"
#include <string>
#include <map>
#include <fts.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>

using namespace std;
using namespace pugi;

int main(int argc, char **argv) {

	//Getting default definitions

	//Creating the document
	xml_document configdoc;

	//Save the input and output folders path
	string InputFolderPath ;//= paths[0];
	string OutputFolderPath ;//= paths[1];

	//Load the file
	xml_parse_result result = configdoc.load_file("ConverterConfig.xml");

	//Check if the xml file is valid
	if (result){
		xml_node ConfigurationNode = configdoc.child("configuration");
		xml_node MilaNode = ConfigurationNode.child("MilaCorporaPath");
		if (MilaNode)
		{
			InputFolderPath = MilaNode.attribute("path").value();
		}
		xml_node outputFolderNode = ConfigurationNode.child("OutputFolderPath");
		if (outputFolderNode){
			OutputFolderPath = outputFolderNode.attribute("path").value();
		}
	}
	else {
		cout << "Can't open configuration file . Closing application" << endl;
		return false;
	}

	//Checking for input arguments
	char *dot[] = {(char*)InputFolderPath.c_str(), (char*)OutputFolderPath.c_str(), 0};


	//If there is no arguments run the program with the default folders
	char **paths = argc > 1 ? argv + 1 : dot;

	//Save the input and output folders path
	InputFolderPath = paths[0];
	OutputFolderPath = paths[1];

	//Open a linux folders tree
	FTS *tree = fts_open(paths, FTS_NOCHDIR, 0);
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
			 vrtPath.replace(vrtPath.find(InputFolderPath),InputFolderPath.length(),OutputFolderPath.data());
			 cout << " --- Creating output folder " << vrtPath << endl;
			 mkdir(vrtPath.data() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
		//If corrent node is a file
		else if (node->fts_info & FTS_F)
		{

			//The absoulte path to the mila xml file including the name
			string xmlName = node->fts_accpath;

			//The name of the output vrt file
			string outName = node->fts_name;
			outName.replace(outName.find(".xml"),4,".vrt");

			//The path to the output file
			string vrtPath = xmlName.data();
			vrtPath.replace(vrtPath.find(InputFolderPath),InputFolderPath.length(),OutputFolderPath.data());
			vrtPath.replace(vrtPath.find(node->fts_name),outName.length(),"");

			//Start Parsing this xml file
			cout << "=======================================================" << endl;
			cout << "Start Parsing " << xmlName << " to folder " << sCurrentFolder << " In path: "<< vrtPath << " and file " << outName << endl;

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

				//If enable to open output file
				if (outputFile.is_open())
				{
					xml_node corpusNode= doc.child("corpus");
					if (corpusNode)
					{

						//Start parsing the xml file into the outputfile
						cout << "Start parsing " << corpusNode.attribute("name").value() << endl;

						//Create a corpus object
						CCorpus corpus(&corpusNode,&outputFile, sCurrentFolder);

						//Initialize the mila converter helper class
						corpus.InitializeConverter();

						//Start parsing
						if (!corpus.Parse())
							cout << "Error Parsing " << corpusNode.attribute("name").value() << endl;
						else
							cout << "Parsing completed" << endl;
					}
					//close the output file
					outputFile.close();
				}
				//if Failed to open file
				else {
					cout << "Unable to open output file" << endl;
					if (fts_close(tree)) {
							perror("fts_close");
							return false;
						}
					return false;
				}

			}
			else
			{
				//if Not valid print the description
				cout << "Load result " << result.description() << endl;
				cout << "Error offset: " << result.offset << "(error at [..." << xmlName << result.offset << "]" << endl << endl;
				if (fts_close(tree)) {
						perror("fts_close");
						return false;
					}
				return false;
			}
		}
	}

	if (fts_close(tree)) {
		perror("fts_close");
		return false;
	}

	return true;
}

