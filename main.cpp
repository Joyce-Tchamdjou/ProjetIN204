#include <iostream>
#include <wx/string.h>
#include <wx/unichar.h>
#include "book.hpp"

using namespace std;

int main()
{
    wxString fname("/home/joycepascale/Bureau/GOT.zip");
    Book test(fname);
    wxString name = fname.AfterLast('/');
    test.open_book("../"+name);
    std::vector<wxString>::iterator it = test.chap.begin();
    std::cout << "Collection" << std::endl;
    while(it != test.chap.end()){
        std::cout << *it << std::endl;
        it++;
    }
    return 0;
}
