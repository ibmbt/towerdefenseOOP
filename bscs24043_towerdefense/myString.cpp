#include<iostream>
#include<fstream>
#include<conio.h>
#include "myString.h"

using namespace std;

void intregrow(int*& arr, int& size) {
    int* temp = new int[size + 1] {};
    for (int i = 0; i < size; i++) {
        temp[i] = arr[i];
    }
    size = size + 1;
    delete[] arr;
    arr = temp;
}
String::String() :len{ 0 }, cap{ 1 }, str{ nullptr } {}
int String::StringLength(const char* arr) const {
    int length = 0;
    for (int i = 0; arr[i] != 0; i++) {
        length++;
    }
    return length;
}
String::String(const char* arr) : len{ StringLength(arr) }, cap{ len + 1 } {
    str = new char[cap];
    for (int i = 0; i < len; i++) {
        str[i] = arr[i];
    }
    str[len] = '\0';
}
String::String(int cap, char a) :len{ cap }, cap{ cap }, str{ new char[cap] {} } {
    for (int i = 0; i < this->len; i++) {
        this->str[i] = a;
    }
}
String::String(const String& obj) {
    this->len = obj.len;
    this->cap = obj.cap + 2;
    this->str = new char[this->cap] {};
    for (int i = 0; i < obj.len; i++) {
        this->str[i] = obj.str[i];
    }
}
String& String::operator=(const String& obj) {
    this->len = obj.len;
    this->cap = obj.cap;
    this->str = new char[obj.cap] {};
    for (int i = 0; i < obj.len; i++) {
        this->str[i] = obj.str[i];
    }
    return *this;
}
int String::str_to_int(const String& s) const {
    int num = 0;
    for (int i = 0; i < s.len && s.str[i] != 0; i++) {
        num = (s.str[i] - '0') + (num * 10);
    }
    return num;
}
String String::int_to_str(int num) const {
    String str;
    int temp = num;
    while (temp > 0) {
        temp = temp / 10;
        str.len++;
    }
    str.cap = str.len + 1;
    str.str = new char[str.cap] {};
    for (int i = str.len - 1; i > -1; i--) {
        str.str[i] = num % 10;
        num = num / 10;
    }
    return str;
}
String::String(int num) {
    *this = int_to_str(num);
}
void String::regrow() {
    cap = (cap * 2) + 1;
    char* newStr = new char[cap] {};
    for (int i = 0; i < len; i++) {
        newStr[i] = str[i];
    }
    newStr[len] = '\0';
    delete[] str;
    str = newStr;
}
void String::String_reg(String*& obj, int& size) {
    String* s = new String[size + 1];
    for (int i = 0; i < size; i++) {
        s[i] = obj[i];
    }
    size = size + 1;
    delete[] obj;
    obj = s;
}
void String::ShallowCopy(const String& obj) {
    this->len = obj.len;
    this->cap = obj.cap;
    this->str = obj.str;
}
char String::operator[](int i)const {
    return this->str[i];
}
char& String::operator[](int i) {
    return this->str[i];
}
void String::insert_at(char a, int idx) {
    if (idx > -1) {
        if (this->len >= this->cap - 1 || idx >= this->cap - 1) {
            this->regrow();
        }
        this->str[idx] = a;
    }
}
void String::insert_at(int idx, const String& sub) {
    if (idx > -1 && idx < this->len) {
        String s;
        s.cap = this->cap + sub.cap;
        s.len = this->len + sub.len;
        s.str = new char[s.cap] {};
        int i = 0, j = 0, k = 0;
        while (i < idx) {
            s.insert_at(this->str[i], k);
            i++;
            k++;
        }
        while (j < sub.len) {
            s.insert_at(sub.str[j], k);
            j++;
            k++;
        }
        while (i < this->len) {
            s.insert_at(this->str[i], k);
            i++;
            k++;
        }
        *this = s;
    }
}
void String::rep_first_char(char a) {
    this->str[0] = a;
}
void String::inputString(char chr) {
    this->cap = 1;
    this->len = 0;
    this->str = new char[this->cap] {};
    char ch;
    while (true) {
        ch = _getche();
        if (ch == chr) {
            break;
        }
        else if (ch == '\b') {
            this->len = this->len - 1;
            cout << ' ' << '\b';
        }
        else {
            this->str[this->len] = ch;
            this->len++;
        }
        if (this->len >= this->cap - 1) {
            this->regrow();
        }
    }
    cout << endl;
}
ostream& operator<<(ostream& fout, const  String& str) {
    for (int i = 0; i < str.len; i++) {
        fout << str.str[i];
    }
    return fout;
}
istream& getline(istream& fin, String& str, char ch) {
    str.inputString(ch);
    return fin;
}
void String::shift_left(int idx) {
    for (int i = idx + 1; i < this->len; i++) {
        this->str[i - 1] = this->str[i];
    }
    this->len--;
    this->str[len] = '\0';
}
String String::remove_extra_spaces() {
    String str;
    str.cap = this->cap;
    str.len = this->len;
    str.str = new char[str.cap] {};
    for (int i = 0; i < this->len; i++) {
        if (this->str[i] == '\n' || this->str[i] == '\t') {
            str.str[i] = ' ';
        }
        else {
            str.str[i] = this->str[i];
        }
    }
    int temp = 1;
    bool flag = false;
    for (int i = 0; i < this->len; i++) {
        if (str.str[i] == ' ') {
            if (flag) {
                temp = 0;
                flag = false;
            }
        }
        else {
            flag = true;
        }
        if (temp == 0) {
            temp = 1;
        }
        else if (!flag) {
            flag = false;
            str.shift_left(i);
            i--;
        }
    }
    return str;
}
bool String::operator==(const String& str2)const {
    if (this->len != str2.len) {
        return false;
    }
    for (int i = 0; i < this->len; i++) {
        if (this->str[i] != str2.str[i]) {
            return false;
        }
    }
    return true;
}
bool String::operator!=(const String& str2)const {
    if (*this == str2) {
        return false;
    }
    return true;
}
bool String::operator<(const String& str2)const {
    int i = 0, j = 0;
    if (*this == str2) {
        return false;
    }
    while (i < this->len && j < str2.len) {
        if (this->str[i] > str2.str[j]) {
            return false;
        }
        else if (this->str[i] < str2.str[j]) {
            return true;
        }
        else {
            i++;
            j++;
        }
    }
    if (i < this->len) {
        return false;
    }
    else {
        return true;
    }
}
bool String::operator<=(const String& str2)const {
    if (*this > str2) {
        return false;
    }
    return true;
}
bool String::operator>(const String& str2)const {
    int i = 0, j = 0;
    if (*this == str2) {
        return false;
    }
    while (i < this->len && j < str2.len) {
        if (this->str[i] < str2.str[j]) {
            return false;
        }
        else if (this->str[i] > str2.str[j]) {
            return true;
        }
        else {
            i++;
            j++;
        }
    }
    if (i < this->len) {
        return true;
    }
    return false;
}
bool String::operator>=(const String& str2)const {
    if (*this < str2) {
        return false;
    }
    return true;
}
bool String::is_sub_str(const String& str2)const {
    for (int i = 0; i < this->len; i++) {
        if (this->str[i] <= 'z' && this->str[i] >= 'a') {
            if (this->str[i] != str2[i] && this->str[i] != str2[i] + 32) {
                return false;
            }
        }
        else {
            if (this->str[i] != str2[i] && this->str[i] != str2[i] - 32) {
                return false;
            }
        }
    }
    return true;
}
String operator+(const String& s1, const String& str2) {
    String new_str;
    new_str.cap = s1.cap + str2.cap;
    new_str.str = new char[new_str.cap] {};
    new_str.len = s1.len + str2.len;
    int i = 0, j = 0, k = 0;
    while (i < s1.len) {
        new_str.str[k++] = s1.str[i++];
    }
    while (j < s1.len) {
        new_str.str[k++] = str2.str[j++];
    }
    return new_str;
}
String& String::operator+=(const String& str2) {
    *this = *this + str2;
    return *this;
}
int* String::all_chars(char ch, int& count) const {
    int size = 1, ct = 0;
    int* idx = new int[size] {};
    for (int i = 0; i < this->len; i++) {
        if (this->str[i] == ch) {
            count++;
            if (ct >= size) {
                intregrow(idx, size);
            }
            idx[ct++] = i;
        }
    }
    return idx;
}
String* String::split(const char ch, int& count)const {
    int* idx = this->all_chars(ch, count);
    String* s = new String[count + 1];
    int i = 0, j = 0;
    while (i < count) {
        s[i].str = new char[s[i].cap] {'\0'};
        int k = 0;
        while (j < idx[i]) {
            if (k >= s[i].cap - 1) {
                s[i].regrow();
            }
            s[i].str[k++] = this->str[j];
            s[i].len++;
            j++;
        }
        j++;
        i++;
    }
    int k = 0;
    s[i].str = new char[s[i].cap] {'\0'};
    while (j < this->len) {
        if (k >= s[i].cap - 1) {
            s[i].regrow();
        }
        s[i].str[k++] = this->str[j++];
        s[i].len++;
    }
    count++;
    return s;
}
int* String::all_sub_str(char* arr, int& count) const {
    int length = this->StringLength(arr);
    int size = 1, ct = 0;
    int* idx = new int[size] {};
    for (int i = 0; i < this->len; i++) {
        int j = 0;
        if (this->str[i] == arr[j]) {
            bool flag = true;
            int k = i;
            while (j < length) {
                if (this->str[i] != arr[j]) {
                    flag = false;
                    break;
                }
                if (i < this->len) {
                    i++;
                }
                else {
                    flag = false;
                    break;
                }
                j++;
            }
            if (flag) {
                count++;
                if (ct >= size) {
                    intregrow(idx, size);
                }
                idx[ct++] = k;
            }
        }
    }
    return idx;
}
String* String::tokenize(char* arr, int& count)const {
    int* idx = this->all_sub_str(arr, count);
    int length = this->StringLength(arr);
    String* s = new String[count + 1];
    int i = 0, j = 0;
    while (i < count) {
        s[i].str = new char[s[i].cap] {'\0'};
        int k = 0;
        while (j < idx[i]) {
            if (k >= s[i].cap - 1) {
                s[i].regrow();
            }
            s[i].str[k++] = this->str[j];
            s[i].len++;
            j++;
        }
        j += length;
        i++;
    }
    int k = 0;
    s[i].str = new char[s[i].cap] {'\0'};
    while (j < this->len) {
        if (k >= s[i].cap - 1) {
            s[i].regrow();
        }
        s[i].str[k++] = this->str[j++];
        s[i].len++;
    }
    count++;
    return s;
}
int String::first_idx(const char ch)const {
    int count = 0;
    int* idx = this->all_chars(ch, count);
    return idx[0];
}
int String::last_idx(const char ch)const {
    int count = 0;
    int* idx = this->all_chars(ch, count);
    return idx[count - 1];
}
int String::first_sub_str(char* arr)const {
    int count = 0;
    int* idx = this->all_sub_str(arr, count);
    return idx[0];
}
int String::last_sub_str(char* arr)const {
    int count = 0;
    int* idx = this->all_sub_str(arr, count);
    return idx[count - 1];
}
void String::remove_at(int idx) {
    if (idx > -1 && idx < this->len) {
        char* temp = new char[this->cap] {};
        for (int i = 0, j = 0; i < this->len; i++) {
            if (i != idx) {
                temp[j++] = this->str[i];
            }
        }
        this->len--;
        delete[] this->str;
        this->str = temp;
    }
}
void String::remove_first(char ch) {
    int count = 0;
    int* idx = all_chars(ch, count);
    if (count != 0) {
        this->remove_at(idx[0]);
    }
}
void String::remove_last(char ch) {
    int count = 0;
    int* idx = all_chars(ch, count);
    if (count != 0) {
        this->remove_at(idx[count - 1]);
    }
}
void String::remove_all(char ch) {
    int count = 0;
    int* idx = all_chars(ch, count);
    for (int i = 0; i < count; i++) {
        this->remove_at(idx[i]);
        for (int j = i + 1; j < count; j++) {
            idx[j]--;
        }
    }
}
const char* String::c_str() const {
    if (str == nullptr) return "";
    return str;
}
void String::clear() {
    delete[] this->str;
    this->cap = 1;
    this->len = 0;
    this->str = new char[this->cap] {};
}
String::~String() {
    delete[] this->str;
}
void String::loadMyfile(ifstream& rdr, const char chr) {
    char ch;
    this->len = 0;
    this->cap = 1;
    if (this->str == nullptr) {
        this->str = new char[this->cap] {};
    }
    while (rdr.get(ch)) {
        if (ch == chr) {
            break;
        }
        if (this->len >= this->cap - 1) {
            this->regrow();
        }
        this->str[this->len] = ch;
        this->len++;
    }
}
char* String::begin() { return str; }
char* String::end() { return str + len; }
const char* String::begin() const { return str; }
const char* String::end() const { return str + len; }

ifstream& getline(ifstream& fin, String& str, const char ch) {
    str.loadMyfile(fin, ch);
    return fin;
}
void String::reverse() {
    String temp(*this);
    for (int i = 0, j = this->len - 1; i < this->len; i++, j--) {
        temp.str[i] = this->str[j];
    }
    *this = temp;
}
void String::save(ofstream& wtr) {
    for (int i = 0; i < this->len; i++) {
        wtr << this->str[i];
    }
}