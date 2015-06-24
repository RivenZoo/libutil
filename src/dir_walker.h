#ifndef _DIR_WALKER_H__
#define _DIR_WALKER_H__

#include "list.h"
#include "file_info.h"

#define DEFAULT_MAX_DOWN_LEVEL  5
#define MAX_DOWN_LEVEL          10

struct FileNode {
    struct ListNode  node;
    struct FileInfo* finfo;
};

inline void releaseFileNode(struct FileNode* fnode) {
    if (fnode == NULL) return;
    ReleaseFileInfo(fnode->finfo);
}

struct DirWalker {
    int     max_level;
    int     cur_level;

    // file list head array, length max_level+1
    struct ListNode**   file_list;
    struct FileNode*    cur_fnode;
};

// max_level: max go down child dir level, if <= 0 use default level 5, up boundary is 10
// this will not follow link
struct DirWalker* NewDirWalker(const char* dir, int max_level);
struct FileInfo* NextFile(struct DirWalker* dir_walker);
void ReleaseDirWalker(struct DirWalker* dir_walker);
void Description(DirWalker)(struct DirWalker* dir_walker, FILE* output);

void Test(DirWalker);

#endif
