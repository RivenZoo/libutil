#include "macro.h"
#include "file_info.h"
#include "list.h"

void test_run() {
    Test(FileInfo);
    Test(List);
}

int main(int argc, char* argv[]) {
    test_run();
    return 0;
}