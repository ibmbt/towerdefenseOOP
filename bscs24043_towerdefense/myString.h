#ifndef MYSTR_H
#define MYSTR_H
#include<fstream>
#include<iostream>
using namespace std;

class String
{
    int len;
    int cap;
    char* str;
public:
    String();
    int StringLength(const char* arr) const;
    String(const char* arr);
    String(int cap, char a);
    String(const String& obj);
    String& operator=(const String& obj);
    int str_to_int(const String& s) const;
    String int_to_str(int num) const;
    String(int num);
    void regrow();
    void String_reg(String*& obj, int& size);
    void ShallowCopy(const String& obj);
    char operator[](int i)const;
    char& operator[](int i);
    void insert_at(char a, int idx);
    void insert_at(int idx, const String& sub);
    void rep_first_char(char a);
    void inputString(char chr);
    friend std::ostream& operator<<(std::ostream& fout, const  String& str);
    friend std::istream& getline(std::istream& fin, String& str, char ch);
    void shift_left(int idx);
    String remove_extra_spaces();
    bool operator==(const String& str2)const;
    bool operator!=(const String& str2)const;
    bool operator<(const String& str2)const;
    bool operator<=(const String& str2)const;
    bool operator>(const String& str2)const;
    bool operator>=(const String& str2)const;
    bool is_sub_str(const String& str2)const;
    friend String operator+(const String& s1, const String& str2);
    String& operator+=(const String& str2);
    int* all_chars(char ch, int& count) const;
    String* split(const char ch, int& count)const;
    int* all_sub_str(char* arr, int& count) const;
    String* tokenize(char* arr, int& count)const;
    int first_idx(const char ch)const;
    int last_idx(const char ch)const;
    int first_sub_str(char* arr)const;
    int last_sub_str(char* arr)const;
    void remove_at(int idx);
    const char* c_str() const;
    void remove_first(char ch);
    void remove_last(char ch);
    void remove_all(char ch);
    void clear();
    void reverse();
    char* begin();
    char* end();
    const char* begin() const;
    const char* end() const;
    void loadMyfile(std::ifstream& rdr, const char chr);
    friend ifstream& getline(std::ifstream& fin, String& str, const char ch);
    void save(std::ofstream& wtr);
    ~String();
};
#endif 