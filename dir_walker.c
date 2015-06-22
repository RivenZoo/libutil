#include "dir_walker.h"
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

static struct FileNode* readFileNode(const char* fpath, struct ListNode** file_list) {
    struct FileInfo* fi = NewFileInfo(fpath);
    if (fi == NULL || file_list == NULL) return NULL;

    struct ListNode* node = NULL;
    if (*file_list == NULL) {
        node = malloc(sizeof(struct ListNode));
        if (node == NULL) {
            ReleaseFileInfo(fi);
            return NULL;
        }
        ListHeadInit(node);
        *file_list = node;
    }

    struct FileNode* fnode = malloc(sizeof(struct FileNode));
    if (fnode == NULL) {
        ReleaseFileInfo(fi);
        return NULL;
    }
    fnode->finfo = fi;
    ListNodeInit(&fnode->node);
    InsertBack(*file_list, &fnode->node);
    return fnode;
}

static void joinDir(char* buf, const char* dir) {
    int len = strlen(dir);
    if (len > MAX_PATH_LEN) {
        len = MAX_PATH_LEN;
    }

    int i = 0;
    for (i = 0; buf[i] != 0 && i < MAX_PATH_LEN; i++) {}
    if (i >= MAX_PATH_LEN) return;

    if (i == 0) {
        strncpy(buf, dir, len);
    } else {
        if (buf[i-1] != '/' && dir[0] != '/') {
            if (i == MAX_PATH_LEN-1) {
                return;
            }
            buf[i] = '/';
            i++;
        }
        if (MAX_PATH_LEN-i<len) {
            len = MAX_PATH_LEN-i;
        }
        strncpy(&buf[i], dir, len);
    }
}

static void readDir(const char* fpath, struct ListNode** file_list) {
    DIR *dir;
    struct dirent *ent;
    dir = opendir(fpath);
    if (dir != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0
            && strcmp(ent->d_name, "..") != 0) {
                char buf[MAX_PATH_LEN+1] = {0};
                joinDir(buf, fpath);
                joinDir(buf, ent->d_name);
                readFileNode(buf, file_list);
            }
        }
        closedir (dir);
    }
}

struct DirWalker* NewDirWalker(const char* dir, int max_level) {
    struct DirWalker* dir_walker = malloc(sizeof(struct DirWalker));
    if (dir_walker == NULL) return NULL;

    if (max_level <= 0) {
        max_level = DEFAULT_MAX_DOWN_LEVEL;
    }
    if (max_level > MAX_DOWN_LEVEL) {
        max_level = MAX_DOWN_LEVEL;
    }

    dir_walker->file_list = malloc(sizeof(struct ListNode*) * max_level);
    if (dir_walker->file_list == NULL) {
        free(dir_walker);
        return NULL;
    }

    dir_walker->max_level = max_level;
    dir_walker->cur_level = 0;
    for (int i = 0; i < max_level; ++i) {
        dir_walker->file_list[i] = NULL;
    }

    dir_walker->cur_fnode = readFileNode(dir, &(dir_walker->file_list[dir_walker->cur_level]));
    struct ListNode* head = dir_walker->file_list[dir_walker->cur_level];
    if (head != NULL) {
        struct ListNode* pnode = ListNext(head);
        dir_walker->cur_fnode = ListData(struct FileNode, node, pnode);
    }
    return dir_walker;
}

static void nextFileNode(struct DirWalker* dir_walker) {
    struct ListNode* pnode = &(dir_walker->cur_fnode->node);
    struct ListNode* head = dir_walker->file_list[dir_walker->cur_level];

    if (IsLastNode(head, pnode)) {
        if (dir_walker->cur_level < dir_walker->max_level-1) {
            dir_walker->cur_level++;
            head = dir_walker->file_list[dir_walker->cur_level];
            if (head == NULL || ListIsEmpty(head)) {
                dir_walker->cur_fnode = NULL;
            } else {
                pnode = ListNext(head);
                dir_walker->cur_fnode = ListData(struct FileNode, node, pnode);
            }
        } else {
            dir_walker->cur_fnode = NULL;
        }
    } else {
        pnode = ListNext(pnode);
        dir_walker->cur_fnode = ListData(struct FileNode, node, pnode);  
    }
}

struct FileInfo* NextFile(struct DirWalker* dir_walker) {
    if (dir_walker == NULL || dir_walker->cur_fnode == NULL) {
        return NULL;
    }

    struct ListNode* head = NULL;
    head = dir_walker->file_list[0];

    struct FileInfo* fi = dir_walker->cur_fnode->finfo;

    // first is input dir file node
    if (IsFirstNode(head, &(dir_walker->cur_fnode->node))) {
        if (IsDir(fi)) {
            readDir(fi->fpath, &(dir_walker->file_list[dir_walker->cur_level]));
        }
        nextFileNode(dir_walker);
        return fi;
    }

    nextFileNode(dir_walker);

    if (dir_walker->cur_fnode == NULL) {
        return NULL;
    }

    fi = dir_walker->cur_fnode->finfo;
    if (dir_walker->cur_level < dir_walker->max_level-1) {
        int next_level = dir_walker->cur_level + 1;
        if (IsDir(fi)
        && strcmp(fi->fpath, ".") != 0
        && strcmp(fi->fpath, "..") != 0) {
            readDir(fi->fpath, &(dir_walker->file_list[next_level]));
        }
    }
    return fi;
}

void ReleaseDirWalker(struct DirWalker* dir_walker) {
    if (dir_walker == NULL) return;

    for (int i = 0; i <= dir_walker->max_level; ++i) {
        struct ListNode* head = dir_walker->file_list[i];
        if (head == NULL) continue;

        struct ListNode* next = NULL;
        struct ListNode tmp;
        ListTraversal(head, next) {
            struct FileNode* fnode = ListData(struct FileNode, node, next);
            if (fnode != NULL) {
                tmp = *next;
                ReleaseFileInfo(fnode->finfo);
                free(fnode);
                next = &tmp;
            }
        }
        free(head);
    }
    free(dir_walker->file_list);
    free(dir_walker);
}

void Description(DirWalker)(struct DirWalker* dir_walker, FILE* output) {
    if (dir_walker == NULL || output == NULL) return;

    fprintf(output, "max level:%d\n", dir_walker->max_level);
    struct FileInfo* fi = NULL;
    while ((fi = NextFile(dir_walker)) != NULL) {
        fprintf(output, "cur level:%d\n", dir_walker->cur_level);
        Description(FileInfo)(fi, output);
    }
}

void Test(DirWalker) {
    struct DirWalker* dir_walker = NewDirWalker("./", 2);
    Description(DirWalker)(dir_walker, stdout);
}