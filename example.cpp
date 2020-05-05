#include <bits/stdc++.h>
using namespace std;

struct Test{
    int a, b;
    Test(){};
    Test(int a, int b): a(a), b(b){};
};

int dfs(int** graph, int* used, int v, int n){
    used[v] = 1;
    for(int i = 0; i < n; i++){
        if(!used[i]){
            if(*(*(graph+v)+i) != 0){
                *(*(graph+v)+i) = *(*(graph+v)+i) + 1;
                *(*(graph+i)+v) = *(*(graph+i)+v) + 1;
                dfs(graph, used, i, n);
            }
        }
    }
}

void show(int **graph, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << *(*(graph + i) + j) << " ";
        } cout << endl;
    }
}

int main() {
    int n;
    freopen("graph", "r", stdin);
    cin >> n;

    int* toWatch = new int[5];
    for(int i = 0; i < 5; i++){
        toWatch[i] = i;
    }

    int** graph = new int*[n];
    int** loads = new int*[n];
    int* used = new int[n];
    int* permutation = new int[n];
    permutation[0] = 2;
    permutation[1] = 4;
    permutation[2] = 3;
    permutation[3] = 1;
    permutation[4] = 5;


    toWatch[2] = 5;

    Test* tests = new Test[n];
    for(int i = 0; i < n; i++){
        tests[i] = {i, i+1};
    }


    for(int i = 0; i < n; i++){
        used[i] = 0;
    }

    for(int i = 0; i < n; i++){
        cout << tests[i].a << " " << tests[i].b << endl;
    }

    int buf;
    for(int i = 0; i < 5; i++){
        *(graph + i) = new int[n];
        *(loads + i) = new int[n];
        for(int j = 0; j < 5; j++){
            cin >> buf;
            *(*(graph + i) + j) = buf;
            *(*(loads + i) + j) = (buf*2+3)%(i+1);
        }
    }

    dfs(graph, used, 0, n);

    loads[1][2] = 78;

    tests[2] = {55, 55};

    loads[1][2] = 121;


    show(loads, n);
    *(tests+2) = {11, 11};

    loads[1][2] = 100;


    cout << "incremented " << endl;

    loads[1][2] = 100;

    show(graph, n);

    return 0;
}