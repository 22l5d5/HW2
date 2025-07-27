#include <iostream>
#include <cmath>

class Polynomial; // 前向宣告

class Term {
    friend class Polynomial;
    friend std::ostream& operator<<(std::ostream& os, const Term& term);
private:
    float coef;
    int exp;
public:
    Term(float c = 0, int e = 0) : coef(c), exp(e) {}
    float getCoef() const { return coef; }
    int getExp() const { return exp; }
    void setCoef(float c) { coef = c; }
};

class Polynomial {
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& poly);
    friend std::istream& operator>>(std::istream& is, Polynomial& poly);
private:
    Term* termArray;
    int capacity;
    int terms;
    void copy(Term* target, const Term* source, int count) {
        for (int i = 0; i < count; i++) target[i] = source[i];
    }
    void resize() {
        Term* newArray = new Term[capacity * 2];
        copy(newArray, termArray, terms);
        delete[] termArray;
        termArray = newArray;
        capacity *= 2;
    }
    void newTerm(float coef, int exp) {
        if (terms == capacity) resize();
        termArray[terms++] = Term(coef, exp);
    }
    bool readTerm(std::istream& is, float& coef, int& exp) {
        bool success = (is >> coef) && (is >> exp);
        return success && !is.fail();
    }
    void mergeTerms() {
        if (terms <= 1) return;
        for (int i = 0; i < terms - 1; i++) {
            for (int j = i + 1; j < terms;) {
                if (termArray[i].getExp() == termArray[j].getExp()) {
                    termArray[i].setCoef(termArray[i].getCoef() + termArray[j].getCoef());
                    for (int k = j; k < terms - 1; k++) {
                        termArray[k] = termArray[k + 1];
                    }
                    terms--;
                } else {
                    j++;
                }
            }
        }
    }
public:
    Polynomial(int initialCapacity = 2) : capacity(initialCapacity), terms(0) {
        termArray = new Term[capacity];
    }
    ~Polynomial() {
        delete[] termArray;
    }
    // 加法
    Polynomial Add(const Polynomial& other) const {
        Polynomial result(capacity + other.capacity);
        int i = 0, j = 0;
        while (i < terms || j < other.terms) {
            if (i >= terms) {
                result.newTerm(other.termArray[j].getCoef(), other.termArray[j].getExp());
                j++;
            } else if (j >= other.terms) {
                result.newTerm(termArray[i].getCoef(), termArray[i].getExp());
                i++;
            } else if (termArray[i].getExp() > other.termArray[j].getExp()) {
                result.newTerm(termArray[i].getCoef(), termArray[i].getExp());
                i++;
            } else if (termArray[i].getExp() < other.termArray[j].getExp()) {
                result.newTerm(other.termArray[j].getCoef(), other.termArray[j].getExp());
                j++;
            } else {
                float sum = termArray[i].getCoef() + other.termArray[j].getCoef();
                if (sum != 0) result.newTerm(sum, termArray[i].getExp());
                i++; j++;
            }
        }
        result.mergeTerms();
        return result;
    }
    // 乘法
    Polynomial Mul(const Polynomial& other) const {
        Polynomial result(capacity + other.capacity);
        for (int i = 0; i < terms; i++)
            for (int j = 0; j < other.terms; j++)
                result.newTerm(termArray[i].getCoef() * other.termArray[j].getCoef(), termArray[i].getExp() + other.termArray[j].getExp());
        result.mergeTerms();
        return result;
    }
    // 評估
    float Eval(float x) const {
        float result = 0;
        for (int i = 0; i < terms; i++) {
            if (termArray[i].getExp() >= 0) {
                result += termArray[i].getCoef() * std::pow(x, termArray[i].getExp());
            } else {
                result += termArray[i].getCoef() / std::pow(x, -termArray[i].getExp());
            }
        }
        return result;
    }
};

std::ostream& operator<<(std::ostream& os, const Term& term) {
    if (term.getCoef() == 0) return os;
    if (term.getExp() == 0) {
        os << term.getCoef();
    } else if (term.getExp() > 0) {
        os << term.getCoef() << "x";
        if (term.getExp() > 1) os << "^" << term.getExp();
    } else { // 負指數
        os << term.getCoef() << "/x^" << -term.getExp();
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Polynomial& poly) {
    bool first = true;
    for (int i = 0; i < poly.terms; i++) {
        if (poly.termArray[i].getCoef() != 0) {
            if (!first && poly.termArray[i].getCoef() > 0) os << "+";
            os << poly.termArray[i];
            first = false;
        }
    }
    if (first) os << "0";
    os << std::endl;
    return os;
}

std::istream& operator>>(std::istream& is, Polynomial& poly) {
    int n;
    if (is >> n) {
        float coef;
        int exp;
        while (n-- && poly.readTerm(is, coef, exp)) {
            poly.newTerm(coef, exp);
        }
    } else {
        float coef;
        int exp;
        while (poly.readTerm(is, coef, exp)) {
            poly.newTerm(coef, exp);
        }
    }
    return is;
}

int main() {
    Polynomial p;
    std::cout << "輸入項數 (或直接輸入係數和指數直到 EOF):" << std::endl;
    std::cin >> p;
    std::cout << "多項式: " << p;
    Polynomial q;
    std::cin >> q;
    std::cout << "另一個多項式: " << q;
    Polynomial sum = p.Add(q);
    std::cout << "加法結果: " << sum;
    Polynomial product = p.Mul(q);
    std::cout << "乘法結果: " << product;
    std::cout << "在 x = 2 時的評估值: " << p.Eval(2) << std::endl;
    return 0;
}
