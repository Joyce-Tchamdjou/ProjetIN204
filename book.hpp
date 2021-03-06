#ifndef BOOK_HPP
#define BOOK_HPP

#include <iostream>
#include <vector>
#include <wx/zipstrm.h>
#include <wx/wfstream.h>
#include <wx/log.h>
#include <wx/fs_zip.h>
#include <wx/filesys.h>
#include <wx/chartype.h>
#include <memory>
#include "image.hpp"
#include <unordered_map>
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/sizer.h>

#define TAILLE_STANDARD 100

using namespace std;

class Book{
private:
    wxString nom_book;
    int taille_book;

public:
    std::vector<wxString> chap;

    Book(){};
    explicit Book(wxString nom_book, int taille_book) : nom_book(nom_book), taille_book(taille_book)
    {}
    explicit Book(wxString nom_book) : nom_book(nom_book), taille_book(TAILLE_STANDARD)
    {}

    void set_nom_book(wxString name){
        nom_book = name;
    }

    void open_book(const wxString& strTargetDir){
        //On cree l'objet entry
        shared_ptr<wxZipEntry> entry;

        //Ouvrir l'archive
        wxFFileInputStream in(this->nom_book);
        if (!in.IsOk()){
            wxLogError(wxS("Couldn't open the file '%s'."), this->nom_book);
        }
        wxZipInputStream zip(in);

        //Parcourir l'archive
        while (entry.reset(zip.GetNextEntry()), entry.get() != NULL) {
            //Utiliser les meta-data
            wxString name = entry->GetName();
            wxString strFileName = strTargetDir + wxFileName::GetPathSeparator() + name;
            int nPermBits = entry->GetMode();
            wxFileName fn;

            if (entry->IsDir()){ // This entry is a directory.
                fn.AssignDir(strFileName);
            }else{ // This entry is a regular file.
                fn.Assign(strFileName);
            }

            // Check if the directory exists, and if not, create it recursively.
            if (!wxDirExists(fn.GetPath()))
                wxFileName::Mkdir(fn.GetPath(), 0777, wxPATH_MKDIR_FULL);

            if (entry->IsDir()) // This entry is a directory.
                continue; // Skip the file creation, because this entry is not a regular file, but a directory.

            zip.OpenEntry(*entry.get());
            wxFileOutputStream file(strFileName);
            if (!file) {
                wxLogError(_T("Can not create file '") + strFileName + _T("'."));
                break;
            }

            //Verifier les extensions pour s'assurer de ne prendre que des images
            wxString low = name.MakeLower();
            if(low.Matches("*.png") or low.Matches("*.jpg") or low.Matches("*.jpeg") or low.Matches("*.bmp")){
                //Ranger dans les noms dans le tableau pour la table des matieres
                chap.push_back(strFileName);
                //pour l'extraction
                zip.Read(file);
            }
        }
    }
};

#endif
