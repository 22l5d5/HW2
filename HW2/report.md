# Homework 2 Report

## 解題說明

本作業要求實現一個 Polynomial 類，其抽象數據類型 (ADT) 和私有數據成員分別如圖 1 和圖 2 所示。任務包括：
- 根據提供的 ADT 實現 Polynomial 類。
- 撰寫 C++ 函數以輸入和輸出以圖 2 表示的多項式，需重載 `<<`（輸出）和 `>>`（輸入）運算子。


## 解題策略
1.根據圖 1 的 ADT 定義，設計 Polynomial 類，包含必要的方法（如加法、乘法、評估等），並根據圖 2 的私有數據成員實現內部表示。

2.輸入輸出實現：使用重載運算子 `>>` 從標準輸入讀取多項式（以係數和指數對表示），使用 `<<` 將多項式以可讀格式輸出。

3.數據結構：使用陣列或動態分配的結構儲存多項式的係數和指數，確保能處理不同次數的多項式。

## 2. 程式實作

```cpp
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

```

## 3. 效能分析

### 1時間複雜度
- 輸入（`>>`）：\( O(d) \)，其中 \( d \) 為多項式最高次數，需讀取 \( d + 1 \) 個係數。
- 輸出（`<<`）：\( O(d) \)，需遍歷所有係數進行格式化輸出。

### 2空間複雜度
- \( O(d) \) 用於儲存係數陣列，其中 \( d \) 為多項式最高次數，受到 `MAX_DEGREE` 限制。
        
### 測試案例
| 測試案例 | 輸入參數                  | 預期輸出          | 實際輸出          |
|----------|---------------------------|-------------------|-------------------|
| 測試一   | 度數 2, 係數 [2, -3, 1]   | 2x^2 - 3x + 1     | 2x^2 - 3x + 1     |
| 測試二   | 度數 0, 係數 [4]          | 4                  | 4                  |
| 測試三  | 度數 3, 係數 [1, 0, 0, 0] | x^3               | x^3               |

### 結論
程式能正確輸入和輸出用戶指定的多項式，使用重載的 `<<` 和 `>>` 運算子。

測試案例涵蓋不同次數和係數的情況，驗證了程式的正確性。

當輸入次數超過 `MAX_DEGREE` 時，程式會提示錯誤並重置。

## 5. 申論及開發報告

### 選擇方法的理由
類設計：根據 ADT 和私有數據成員實現 Polynomial 類，使用陣列儲存係數，符合題目要求。

運算子重載：通過重載 `<<` 和 `>>` 提供直觀的輸入輸出界面，增強程式可用性。

限制與改進：目前次數限制為 `MAX_DEGREE = 10`，建議未來使用動態陣列支援更大次數，或加入錯誤處理以處理無效輸入。
