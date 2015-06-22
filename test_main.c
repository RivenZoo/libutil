#include "macro.h"
#include "file_info.h"
#include "list.h"
#include "dir_walker.h"

void test_run() {
    // Test(FileInfo);
    // Test(List);
    Test(DirWalker);
}

int main(int argc, char* argv[]) {
    test_run();
    return 0;
}