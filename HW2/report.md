# Homework 2 Report

## 1. 解題說明

### 1.1 問題描述
本作業要求實現一個 Polynomial 類，其抽象數據類型 (ADT) 和私有數據成員分別參考 `113-3 HomeWork 2 (0801).ppt` 的圖 1 和圖 2，並結合 `資料結構(一)作業二參考資料.pdf` 的規範。任務包括：
- 根據提供的 ADT 實現 Polynomial 類，包含 `Term` 類作為其項次表示。
- 撰寫 C++ 函數以輸入和輸出多項式，需重載 `<<`（輸出）和 `>>`（輸入）運算子。
- 支援多項式的加法 (`Add`)、乘法 (`Mul`) 和評估 (`Eval`) 操作。
- 截止日期為 8月1日。

### 1.2 解題策略
- **類設計**：根據參考資料設計 `Term` 類，包含係數 (`coef`) 和指數 (`exp`)，並作為 `Polynomial` 類的私有成員陣列 (`termArray`) 使用。`Polynomial` 類管理項次數量 (`terms`) 和容量 (`capacity`)。
- **輸入輸出實現**：根據 PDF 第 6 頁的建議，實現 `>>` 運算子支援兩種輸入方式：(1) 輸入項數 `n` 後依次輸入每個項的係數和指數；(2) 連續輸入係數和指數直到 EOF。`<<` 運算子格式化輸出多項式。
- **功能實現**：實現 `Add`、`Mul` 和 `Eval` 方法，分別處理多項式加法、乘法和指定點的評估。
- **數據結構**：使用動態陣列儲存 `Term` 物件，當容量不足時擴展為兩倍。

## 2. 程式實作

```cpp
#include <iostream>

class Polynomial; // 前向宣告

class Term {
    friend class Polynomial;
    friend std::ostream& operator<<(std::ostream& os, const Term& term);
    friend std::istream& operator>>(std::istream& is, Term& term);
private:
    float coef;
    int exp;
public:
    Term(float c = 0, int e = 0) : coef(c), exp(e) {}
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
            if (i >= terms) result.newTerm(other.termArray[j].coef, other.termArray[j++].exp);
            else if (j >= other.terms) result.newTerm(termArray[i].coef, termArray[i++].exp);
            else if (termArray[i].exp > other.termArray[j].exp) result.newTerm(termArray[i].coef, termArray[i++].exp);
            else if (termArray[i].exp < other.termArray[j].exp) result.newTerm(other.termArray[j].coef, other.termArray[j++].exp);
            else {
                float sum = termArray[i].coef + other.termArray[j].coef;
                if (sum != 0) result.newTerm(sum, termArray[i].exp);
                i++; j++;
            }
        }
        return result;
    }
    // 乘法
    Polynomial Mul(const Polynomial& other) const {
        Polynomial result(capacity + other.capacity);
        for (int i = 0; i < terms; i++)
            for (int j = 0; j < other.terms; j++)
                result.newTerm(termArray[i].coef * other.termArray[j].coef, termArray[i].exp + other.termArray[j].exp);
        return result;
    }
    // 評估
    float Eval(float x) const {
        float result = 0;
        for (int i = 0; i < terms; i++)
            result += termArray[i].coef * pow(x, termArray[i].exp);
        return result;
    }
};

std::ostream& operator<<(std::ostream& os, const Term& term) {
    os << term.coef;
    if (term.exp > 0) {
        os << "x";
        if (term.exp > 1) os << "^" << term.exp;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Polynomial& poly) {
    bool first = true;
    for (int i = 0; i < poly.terms; i++) {
        if (poly.termArray[i].coef != 0) {
            if (!first && poly.termArray[i].coef > 0) os << "+";
            os << poly.termArray[i];
            first = false;
        }
    }
    if (first) os << "0";
    os << std::endl;
    return os;
}

std::istream& operator>>(std::istream& is, Term& term) {
    is >> term.coef >> term.exp;
    return is;
}

std::istream& operator>>(std::istream& is, Polynomial& poly) {
    int n;
    if (is >> n) {
        while (n-- && is >> poly.termArray[poly.terms].coef >> poly.termArray[poly.terms].exp) {
            poly.terms++;
            if (poly.terms == poly.capacity) poly.resize();
        }
    } else {
        while (is >> poly.termArray[poly.terms].coef >> poly.termArray[poly.terms].exp) {
            poly.terms++;
            if (poly.terms == poly.capacity) poly.resize();
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
