/////////////////////////////////////////////////////////////////////////////////
// Rommel Santor - 8Apr2003 - lucky760@yahoo.com
/////////////////////////////////////////////////////////////////////////////////
// Most people are familiar with reading/writing text to/from a disk file.
// This is a quick little example detailing how to go about doing the same
// with binary data. This will allow you to make better use of files. You will
// be able to save/load structures in files and other non-human-readable data.
/////////////////////////////////////////////////////////////////////////////////
// I have commented this file thoroughly to help beginners understand what the
// code is doing. If you need something explained, feel free to ask.
/////////////////////////////////////////////////////////////////////////////////

#include <fstream> // for ifstream, ofstream, ios_base
#include <iostream>// for cin, cout, endl
// you can just say `using namespace std;` here instead of all these 'using's
using std::ios_base;
using std::cout;
using std::cin;
using std::endl;

// just creating a constant for the name of the database
static const char *DATABASE_FILE = "db.dat";

// the data that will go into the database
struct Person {
  char name[40];
  int age;
  float salary;
};

// this function takes a pointer to a Person struct
// that will be filled with the data in the file
bool LoadDB(Person *per) {
  // first parameter is the name of the file
  // second is the open flags. 
  //    `in` says you will read from the file
  //    `binary` says you will read binary data instead of simple text
  std::ifstream fin(DATABASE_FILE, ios_base::in | ios_base::binary);
  
  // if we couldn't open the file (it may not exist); fail
  if (!fin.is_open())
    return false;
  
  // the file is open; tell the read function to look at `per` as a
  // char pointer instead of a Person pointer so that `fin` can place
  // one byte at a time into the struct
  // the second parameter tells read() how many bytes to read
  // Note: do not use the ">>" operator to read from a binary file 
  fin.read((char*)per, sizeof(Person));
  fin.read((char*)per, sizeof(Person));

  fin.close();// close the file - not necessary, but we should be neat

  // success  
  return true;
}

// this function takes a pointer to a Person struct
// that will be written into the file
bool SaveDB(Person *per) {
  // these flags say:
  //    `out` - we will be writing data into the file
  //    `binary` - we will be writing binary data and not simple text
  //    `trunc` - if the file already exists, truncate (wipe out) the existing data
  std::ofstream fout(DATABASE_FILE, ios_base::out | ios_base::binary | ios_base::app);
  // couldn't open it (disk error?); fail
  if (!fout.is_open())
    return false;

  // notice this time we cast the Person pointer to a const char* because we aren't
  // altering the structure this time; rather we're just looking at it's contents.  
  fout.write((const char*)per, sizeof(Person));
  fout.close();// close the file - not necessary, but we should be neat

  // success  
  return true;
}

int main() {
  // create an empty person struct
  Person per;
  
  // load the data in the db file
  if (LoadDB(&per)) {
    // print what we loaded into our Person struct
    cout << "Loaded Database...\n";
    cout << "Name = " << per.name << endl;
    cout << "Age = " << per.age << endl;
    cout << "Salary = " << per.salary << endl;
    cout << endl;
  }

  // prompt user to enter the new info  
  cout << "Set database info...\n";
  cout << "Name: ";
  cin.getline(per.name, 40);
  
  cout << "Age: ";
  (cin >> per.age).get();// add the .get() to clear the newline character
  
  cout << "Salary: ";
  (cin >> per.salary).get();// note .getline() clears its own newline character

  // save the new user-entered data  
  if (SaveDB(&per))
    cout << "\nDatabase saved!\n";
  else
    cout << "\nUnable to save database!\n";

  // give the user a chance to look things over before quitting
  cout << "\n[hit enter]";  
  cin.get();

  // return 0 on success (main() should never be void!)
  return 0;
}
