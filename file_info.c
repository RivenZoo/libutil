#include "file_info.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static const char* s_file_type_str[] = {
    "Regular",
    "Dir",
    "Link",
    "Unknown",
};
static int s_file_type_num = 4;

static void getFileInfo(struct FileInfo* fi);

struct FileInfo* NewFileInfo(const char* fpath) {
    struct FileInfo* fi = (struct FileInfo*)calloc(1, sizeof(struct FileInfo));
    if (fi == NULL) return NULL;

    int path_len = strlen(fpath);
    path_len = (path_len > MAX_PATH_LEN ? MAX_PATH_LEN : path_len);

    memcpy(fi->fpath, fpath, path_len);
    fi->ftype = file_unknown;
    getFileInfo(fi);
    return fi;
}

void getFileInfo(struct FileInfo* fi) {
    struct stat st;
    int ret = lstat(fi->fpath, &st);
    if (ret != 0) {
        return;
    }
    
    if (S_IFREG & st.st_mode) {
        fi->ftype = file_regular;
    } else if (S_IFDIR & st.st_mode) {
        fi->ftype = file_dir;
    } else if (S_IFLNK & st.st_mode) {
        fi->ftype = file_link;
    } else {
        fi->ftype = file_unknown;
    }
    fi->fsize = st.st_size;
}

void Description(FileInfo)(struct FileInfo* fi, FILE* output) {
    assert(fi);
    assert(output);
    fprintf(output, "path: %s\ntype:%s\nsize:%lld\n", fi->fpath,
        s_file_type_str[fi->ftype%s_file_type_num], fi->fsize);
}

void ReleaseFileInfo(struct FileInfo* fi) {
    free(fi);
}

inline int IsDir(struct FileInfo* fi) {
    if (fi == NULL) return 0;

    return fi->ftype == file_dir;
}

inline int IsLink(struct FileInfo* fi) {
    if (fi == NULL) return 0;

    return fi->ftype == file_link;
}

inline int IsRegularFile(struct FileInfo* fi) {
    if (fi == NULL) return 0;

    return fi->ftype == file_regular;
}

void Test(FileInfo) {
    struct FileInfo* fi = NewFileInfo("./macro.h");
    printf("FileInfo test\n");
    Description(FileInfo)(fi, stdout);
    ReleaseFileInfo(fi);
}

