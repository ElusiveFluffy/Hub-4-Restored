#pragma once

struct KromeIniLine
{
    char* Comment; // Line comment (denoted by //), begins at first non-whitespace character after "//"
    char* Section; // name of ini section "name sectionName" Only lines that have "name sectionName" have this field
    char* FieldName; // Line field name "pos -1, 0, 3" -> pFieldName = "pos"
    char* Data; // Line data, "pos -1, 0, 3" -> data = "-1, 0, 3"
    int ElementCount; // Number of data elements
};
struct KromeIni
{
    char Name[0x20];
    void* pFileMem;
    bool FileMemAllocated;
    int FileSize;
    KromeIniLine* pLines;
    int LineCount;
    int CurrentLine;
    int SectionCount; // count for "[]" sections
};