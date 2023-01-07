//---------------------------------------------------------------------------

#ifndef FileIOH
#define FileIOH
//---------------------------------------------------------------------------
#endif
struct Record{
  float pitch;
  float entropy;
  float fm;
  float pgood;
};
void SaveRec(Record *rec, AnsiString file);
void ReadRec(Record *rec, int j, AnsiString file);
