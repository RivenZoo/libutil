#ifndef _FILE_INFO_H__
#define _FILE_INFO_H__

#include "macro.h"
#include <sys/stat.h>
#include <stdio.h>

#define MAX_PATH_LEN    255

typedef enum {
    file_regular,
    file_dir,
    file_link,
    file_unknown,
} FileType;

struct FileInfo {
    char        fpath[MAX_PATH_LEN+1];
    FileType    ftype;
    off_t       fsize;
};

struct FileInfo* NewFileInfo(const char* fpath);
void ReleaseFileInfo(struct FileInfo* fi);

void Description(FileInfo)(struct FileInfo* fi, FILE* output);

int IsDir(struct FileInfo* fi);
int IsLink(struct FileInfo* fi);
int IsRegularFile(struct FileInfo* fi);

void Test(FileInfo);

#endif
