#include <iostream>
using namespace std;

class Polynomial {
private:
    int* coefficients; // 儲存係數的陣列
    int degree;       // 多項式最高次數
    static const int MAX_DEGREE = 10; // 最大次數限制

public:
    // 建構子
    Polynomial() : degree(0) {
        coefficients = new int[MAX_DEGREE + 1]();
    }

    // 解構子
    ~Polynomial() {
        delete[] coefficients;
    }

    // 重載輸入運算子
    friend istream& operator>>(istream& in, Polynomial& p) {
        cout << "請輸入多項式最高次數 (0-" << p.MAX_DEGREE << "): ";
        in >> p.degree;
        if (p.degree > p.MAX_DEGREE || p.degree < 0) {
            cout << "次數超出範圍！" << endl;
            p.degree = 0;
            return in;
        }
        cout << "請輸入每個次數的係數 (從最高次開始):" << endl;
        for (int i = p.degree; i >= 0; i--) {
            cout << "x^" << i << " 的係數: ";
            in >> p.coefficients[i];
        }
        return in;
    }

    // 重載輸出運算子
    friend ostream& operator<<(ostream& out, const Polynomial& p) {
        bool first = true;
        for (int i = p.degree; i >= 0; i--) {
            if (p.coefficients[i] != 0) {
                if (!first && p.coefficients[i] >= 0) out << "+";
                if (i == 0 || p.coefficients[i] != 1 || i != 1) out << p.coefficients[i];
                if (i > 0) {
                    if (i == 1) out << "x";
                    else out << "x^" << i;
                }
                first = false;
            }
        }
        if (first) out << "0"; // 若所有係數為 0
        out << endl;
        return out;
    }
};

int main() {
    Polynomial p;
    cin >> p;
    cout << "輸入的多項式為: " << p;
    return 0;
}

