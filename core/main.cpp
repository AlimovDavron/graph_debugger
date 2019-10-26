#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    FILE *gdb = popen("gdb --interpreter=mi --silent", "w");
    fprintf(gdb, "shell ls\n");
    fprintf(gdb, "file %s\n", argv[1]);
    pclose(gdb);

    return 0;
}