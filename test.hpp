#ifndef TEST_H
#define TEST_H
#include <vector>
#include <wx/wx.h>
#include "fenetre.hpp"
#include "book.hpp"

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size); //Constructeur
    wxTextCtrl *my_FileName;
private:
    Fenetre *mainWindow; //Fenêtre principale
// Evènement permettant d'ouvrir un fichier
    void OuvrirArchive( Book archive, const wxString& strTargetDir);
    void CreerBibli(std::vector<wxString> names);
    std::vector<wxString> names;
    void OnArchiveOuvrir(wxCommandEvent& event);
    void OnLire(wxCommandEvent &WXUNUSED(event));
// Fonction permettant de charger une image
    void ChargerFichier(wxString &imagepath);
// Evènement permettant de voir l'image précédent l'image actuelle. Si on regarde la première image, affiche la dernière image du répertoire courant
    void ImagePrec(wxCommandEvent &event);
// Fonction creeant une barre d'outils
    void createToolbar();
// Evènement permettant de voir l'image suivant l'image actuelle. Si on regarde la dernière image, affiche la première image du répertoire courant
    void ImageSuiv(wxCommandEvent &event);
//Fonction permettant de trouver toutes les images du répertoire courant et de fixer l'itérateur d'image
    void TrouverImages(wxString &directory, wxString &imagename);
//Fonction permettant de trouver toutes les images du répertoire courant sans fixer l'itérateur d'image
    void TrouverImages(wxString &filepath);
// Evènement permettant de fermer la liseuse
    void OnFichierQuitter(wxCommandEvent& event);
// Evènement affichant la bibliothèque avec les différents livres
    void OnBibliothequeLivres(wxCommandEvent& event);
// Evènement affichant des informations sur la liseuse
    void OnInfoApropos(wxCommandEvent& event);
// Tableau contenant les chemins des fichiers
    std::vector<wxString> m_imageNoms;
// Position de l'image courante
    size_t m_imageIterateur = 0;
    wxDECLARE_EVENT_TABLE();

};
 enum
    {
    ID_Lire = 1,
    ID_Quitter=100,
    ID_Apropos=200,
    ID_Livres=300,
    ID_Desarchiver = 400
    };

#endif

