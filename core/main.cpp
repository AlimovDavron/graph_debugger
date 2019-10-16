#include <iostream>
using namespace std;

int f(){
    cout << "kek" << endl;
    return 1;
}

int main() {
    int n;
    cin >> n;
    int a;
    cin >> a;
    int t = f();

    cout << t << endl;
    return 0;
}