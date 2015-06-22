#ifndef _DIR_WALKER_H__
#define _DIR_WALKER_H__

#define DEFAULT_MAX_DOWN_LEVEL  5
#define MAX_DOWN_LEVEL          10

struct DirLevel {
    struct DirNode;
    struct FileNode;
};

struct DirWalker {
    int max_level;
    int follow_link;

    struct DirLevel* levels;
};

// max_level: max go down child dir level, if <= 0 use default level 5, up boundary is 10
// follow_link: if == 0, just return link file; other return real file
struct DirWalker* NewDirWalker(const char* dir, int max_level, int follow_link);
void ReleaseDirWalker(struct DirWalker* dir_walker);
void Description(DirWalker)(struct DirWalker* dir_walker, FILE* output);

void Test(DirWalker);

#endif
