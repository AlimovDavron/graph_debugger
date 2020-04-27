#include <bits/stdc++.h>
using namespace std;

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

    int** graph = new int*[n];
    int* used = new int[n];

    for(int i = 0; i < n; i++){
        used[i] = 0;
    }

    int buf;
    for(int i = 0; i < 5; i++){
        *(graph + i) = new int[n];
        for(int j = 0; j < 5; j++){
            cin >> buf;
            *(*(graph + i) + j) = buf;
        }
    }

    show(graph, n);
    dfs(graph, used, 0, n);
    cout << "incremented " << endl;
    show(graph, n);

    return 0;
}