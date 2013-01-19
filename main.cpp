#include <iostream>
#include "MilatoCWBConverter.h"

using namespace std;

int main(int argc, char **argv) {

	//Start of Execution

	cout << "----------------------------------------------" << endl;
	cout << " Welcome to the MilaToCwbConverter and Encoder" << endl;
	cout << "----------------------------------------------" << endl;
	cout << endl << endl;

	//Create and initialize the converter class with the data from the configuration file

	cout << "-----------------------" << endl;
	cout << "    Initialization     " << endl;
	cout << "-----------------------" << endl;

	CMilatoCWBConverter mainConverter;
	if (!mainConverter.Init()){
		cout << "Error Initializing the converter. Please check the ConverterConfig.xml file." << endl;
		return false;
	}

	cout << endl << endl;

	//Printing the menu

	cout << "-----------------------" << endl;
	cout << "       MainMenu        " << endl;
	cout << "-----------------------" << endl;

	cout << "Choose the required option. If one of the folder paths is incorrect please exit the program and change the ConverterConfig.xml file. " << endl << endl;

	//Print the Configuration paths
	mainConverter.PrintConfigurationPaths();
	cout << endl << endl;

	//Print the options
	cout << "1. Clean the vrt folder , Convert the data in the mila folder to vrt's and store it in the vrt folder" << endl;
	cout << "2. Encode and compress the data located in the vrt folder to CWB" << endl;
	cout << "3. Encode and compress the data located in the vrt folder to CWB and cleanup the vrt folder" << endl;
	cout << "4. Execute options 1 and 2" << endl;
	cout << "5. Execute options 1 and 3" << endl;
	cout << "9. Exit the program" << endl;

	//Get the user's choice
	cout << "Choice : ";
	int choice;
	cin >> choice;

	//Execute the user's choice
	switch (choice)
	{
	case 1:
		mainConverter.CleanTheVrtFolder();
		mainConverter.ConvertFromMilaToVrt();
		break;
	case 2:
		mainConverter.EncodeVrtToCWB();
		mainConverter.CompressCorpus();
		break;
	case 3:
		mainConverter.EncodeVrtToCWB();
		mainConverter.CompressCorpus();
		mainConverter.CleanTheVrtFolder();
		break;
	case 4:
		mainConverter.CleanTheVrtFolder();
		mainConverter.ConvertFromMilaToVrt();
		mainConverter.EncodeVrtToCWB();
		mainConverter.CompressCorpus();
		break;
	case 5:
		mainConverter.CleanTheVrtFolder();
		mainConverter.ConvertFromMilaToVrt();
		mainConverter.EncodeVrtToCWB();
		mainConverter.CompressCorpus();
		mainConverter.CleanTheVrtFolder();
		break;
	case 9:
		break;
	default:
		cout << "Wrong choice. The program will stop" << endl;
		break;
	}

	//Closing Program

	cout << endl << endl;
	cout << "-----------------------" << endl;
	cout << "     End of Program    " << endl;
	cout << "-----------------------" << endl;
	cout << "Thanks For using the MilaToCWB Converter and Encoder" << endl;

	return true;
}

