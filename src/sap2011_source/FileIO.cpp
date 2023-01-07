//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FileIO.h"
#include <fstream> // for ifstream, ofstream, ios_base
#include <iostream>// for cin, cout, endl
//---------------------------------------------------------------------------

#pragma package(smart_init)



// you can just say `using namespace std;` here instead of all these 'using's
using std::ios_base;
using std::cout;
using std::cin;
using std::endl;

// just creating a constant for the name of the database
//static const char *fileName = "c:\\db.dat";

// the data that will go into the database




void SaveRec(Record *rec, AnsiString file)
{
        const char *fileName = file.c_str();
        std::ofstream fout(fileName, ios_base::out | ios_base::binary | ios_base::app);
        fout.write((const char*)rec, sizeof(Record));

}

void ReadRec(Record *rec, int j, AnsiString file)
{
        const char *fileName = file.c_str();
        std::ifstream fin(fileName, ios_base::in | ios_base::binary);  // open the file
        for(int i=0;i<j;i++)fin.read((char*)rec, sizeof(Record));  // read one record
}
