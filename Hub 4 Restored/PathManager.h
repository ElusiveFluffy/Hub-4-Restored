#pragma once

struct PathSegment
{
    unsigned char numPoints;
    char pathID;
    char field_2;
    char field_3;
    struct PathSegment* field_4;
    struct PathSegment* field_8;
    unsigned char field_c;
    unsigned char field_d;
    char _e[2];
};

struct PathManager
{
    PathSegment* pCurrentSegment;
    int field_4;
    int field_8;
    char PathID;
    bool field_d;
    char field_e;
    char padding;
};
