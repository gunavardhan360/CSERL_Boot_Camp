#include <iostream>
using namespace std;

class complex
{
    public:
    int r = 0;
    int i = 0;

    complex(){
        r = 0; i = 0;}

    complex(int a, int b){
        r = a; i = b;}

    void complexAdd(complex num1, complex num2){
        r = num1.r + num2.r;
        i = num1.i + num2.i;
    }
};

int main(){
    int temp1, temp2;

    cout << "Enter first complex number: ";
    cin >> temp1 >> temp2 ;
    complex num1(temp1, temp2);

    cout << "Enter second complex number: ";
    cin >> temp1 >> temp2 ;
    complex num2(temp1, temp2);

    complex res;
    res.complexAdd(num1, num2);
    cout << "Result: " << res.r << " " << res.i << "\n";

    return 0;
}