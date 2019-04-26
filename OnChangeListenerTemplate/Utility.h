#include <iostream>
	using std::ostream;
#include <vector>
	using std::vector;

//Write any kind of Vector
template <typename T> 
ostream& operator<<(ostream& os, const vector<T>& v) 
{ 
    os << "["; 
    for (int i = 0; i < v.size(); ++i) { 
        os << v[i]; 
        if (i != v.size() - 1) 
            os << ", "; 
    } 
    os << "]\n"; 
    return os; 
}

std::string getFileExt(const std::string& s) {

    size_t i = s.rfind('.', s.length());
    if (i != string::npos) {
        return(s.substr(i+1, s.length() - i));
    }

    return("");
}
