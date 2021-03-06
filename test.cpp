// wxWidgets "Liseuse de BD" Program
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "wx/image.h"
#include "test.hpp"
#include "fenetre.hpp"
#include "fenetre.cpp"
#include "book.hpp"
#include <iostream>
#include <wx/string.h>
#include <wx/aboutdlg.h>
#include <vector>
#include <algorithm>
#include <wx/unichar.h>
#include <wx/listctrl.h>
#include <wx/app.h>
#include <wx/bitmap.h>
#include <wx/dcclient.h>
#include <wx/filedlg.h>
#include <wx/frame.h>
#include <wx/image.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/artprov.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/gdicmn.h>

using namespace std;

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Desarchiver,MyFrame::OnArchiveOuvrir)
    EVT_MENU(ID_Quitter, MyFrame::OnFichierQuitter)
    EVT_MENU(ID_Livres, MyFrame::OnBibliothequeLivres)
    EVT_MENU(ID_Apropos, MyFrame::OnInfoApropos)
    EVT_MENU(ID_Lire, MyFrame::OnLire)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);


bool MyApp::OnInit()
{
    const wxString Titre("Liseuse de BD");
    MyFrame *frame = new MyFrame( Titre, wxPoint(50, 50), wxSize(540, 430) );
    frame->Show( true );
    return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{

    wxMenu *fichierMenu = new wxMenu;
    fichierMenu->Append(ID_Desarchiver, "&Desarchiver\tCtrl-H",
                     "Ouvrir une archive d'une livre");
    fichierMenu->AppendSeparator();
    fichierMenu->Append(ID_Lire, "&Lire");
    fichierMenu->AppendSeparator();
    fichierMenu->Append(ID_Quitter, "&Quitter");

    wxMenu *infoMenu = new wxMenu;
    infoMenu->Append(ID_Apropos, "&A propos");

    wxMenu *bibliMenu = new wxMenu;
    bibliMenu->Append(ID_Livres, "&Livres");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(fichierMenu, "&Fichier");
    menuBar->Append(bibliMenu, "&Bibliotheque");
    menuBar->Append(infoMenu, "&Infos");

    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText( "Bienvenue sur la liseuse!" );

    wxInitAllImageHandlers(); // Permet de gérer tous les formats d'images

    mainWindow = new Fenetre(this);
    // bibWindow = new Fenetre(this);
    this->createToolbar();
}


void MyFrame::CreerBibli(std::vector<wxString> names)
{

        wxPanel* mainPane = new wxPanel(this,wxID_ANY, wxDefaultPosition, wxSize(540,430));
        wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        wxListCtrl *liste;
        liste = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(540,430), wxLC_REPORT, wxDefaultValidator);


        wxListItem col0;
        col0.SetId(0);
        col0.SetText( _("Vos livres") );
        col0.SetWidth(540);
        liste->InsertColumn(0, col0);

        for (int i=0; i<names.size(); i++) {
            wxListItem item;
            item.SetId(i);
            item.SetText("1");
            liste->InsertItem( item );

            liste->SetItem(i, 0, names.at(i)); }

        sizer->Add(liste,1, wxEXPAND);
        mainPane->SetSizer(sizer);
}

void MyFrame::OnFichierQuitter(wxCommandEvent& event)
{
    Close( true );
}

void MyFrame::OnInfoApropos(wxCommandEvent& event)
{
    wxMessageBox( "Il s'agit d'une liseuse de bande dessinees",
                  "A propos", wxOK | wxICON_INFORMATION );
}

void MyFrame::OuvrirArchive(Book archive, const wxString& strTargetDir)
{
    archive.open_book(strTargetDir);
}

std::vector<wxString> names;

void MyFrame::OnArchiveOuvrir(wxCommandEvent &WXUNUSED(event))
{
    wxFileDialog dialog(mainWindow, wxT("Ouvrir une archive"),"", "", "Archives (*.zip;*.cbr;*.cbz)|*.zip;*.cbr;*.cbz;", wxFD_OPEN |wxFD_FILE_MUST_EXIST);
    if(dialog.ShowModal() == wxID_CANCEL)
        return;
    Book archive(dialog.GetPath());
    wxString name = dialog.GetPath().AfterLast('/');
    int id=wxID_ANY;
    int lastId=wxID_ANY;
    if (wxMessageBox("Voulez-vous decompresser cette archive ?","",wxYES_NO ) == wxYES)  {
        this->OuvrirArchive(archive,"../"+name);
        wxString reducted_name = dialog.GetFilename().BeforeLast('.');
// On s'assure que le livre n'est pas deja dans la bibliotheque
        if (!(any_of(names.begin(), names.end(), [&](const wxString& elem) { return elem == reducted_name; })))
            names.insert(names.end(), reducted_name);

        int answer = wxMessageBox( "Archive decompressee avec succes", "", wxOK );


     }
}

// Lire sans desarchiver
void MyFrame::OnLire(wxCommandEvent &WXUNUSED(event))
{
    wxFileDialog dialog(mainWindow, wxT("Lire une BD"),"", "", "Tous les fichiers(*.*)|*.*", wxFD_OPEN |wxFD_FILE_MUST_EXIST);
    if(dialog.ShowModal() == wxID_CANCEL)
        return;
    wxString filePath = dialog.GetPath();
    this->ChargerFichier(filePath);

    // Recuperer les images du répertoire
    wxString rep = dialog.GetDirectory();
    wxString fileName = dialog.GetFilename();
    this->TrouverImages(rep, fileName);
}

void MyFrame::ChargerFichier(wxString &imagePath)
{
    std::shared_ptr<wxImage> image = std::make_shared<wxImage>();
    bool loadOk = image->LoadFile(imagePath, wxBITMAP_TYPE_ANY);
    // On verifie si le fichier est bien une image
    if (image->IsOk() && loadOk)
    {
        mainWindow->setImage(image);
    }
}

void MyFrame::createToolbar()
{
    wxToolBar *toolbar =
        new wxToolBar(this, -1, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxTB_BOTTOM);


    toolbar->AddStretchableSpace();

    // Icone image precedente
    wxBitmap prevImageBitmap = wxArtProvider::GetBitmap(wxART_GO_BACK);
    wxToolBarToolBase *prevImage = toolbar->AddTool(-1, wxT("Image precedente"), prevImageBitmap);
    Connect(prevImage->GetId(), wxEVT_COMMAND_TOOL_CLICKED,
wxCommandEventHandler(MyFrame::ImagePrec));

    // Icone image suivante
    wxBitmap nextImageBitmap = wxArtProvider::GetBitmap(wxART_GO_FORWARD);
    wxToolBarToolBase *nextImage = toolbar->AddTool(-1, wxT("Image suivante"), nextImageBitmap);
    Connect(nextImage->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::ImageSuiv));

    toolbar->Realize();
    this->SetToolBar(toolbar);
}

void MyFrame::ImagePrec(wxCommandEvent &WXUNUSED(event))
{
    // Cas ou il n'y a qu'une image
    if (m_imageNoms.size() <= 1)
    {
        return;
    }

    // Cas ou c'est la premiere image
    if (m_imageIterateur == 0)
    {
        m_imageIterateur = m_imageNoms.size() - 1;
    }
    else
    {
        m_imageIterateur--;
    }

    wxString &imageName = m_imageNoms.at(m_imageIterateur);
    this->ChargerFichier(imageName);
}

void MyFrame::ImageSuiv(wxCommandEvent &WXUNUSED(event))
{
    // Cas ou il n'y a qu'une image
    if (m_imageNoms.size() <= 1)
    {
        return;
    }

    m_imageIterateur++;
    // Cas ou c'est la derniere image
    if (m_imageIterateur >= m_imageNoms.size())
    {
        m_imageIterateur = 0;
    }

    wxString &imageName = m_imageNoms.at(m_imageIterateur);
    this->ChargerFichier(imageName);
}


void MyFrame::TrouverImages(wxString &directory, wxString &imageName)
{
    wxDir dir(directory);
    if (!dir.IsOpened())
    {
        std::cout << "Repertoire non ouvert" << std::endl;
        return;
    }

    m_imageNoms.clear();
    wxString filename;
    const wxString &currentDir = wxGetCwd();
    bool cont = dir.GetFirst(&filename, wxEmptyString, wxDIR_FILES);

    size_t i = 0;
    while (cont)
    {
        wxFileName fname(currentDir, filename);
        const wxString &ending = fname.GetExt();

        // On verifie si c'est bien une image
        if (ending == wxT("png") || ending == wxT("jpg") || ending == wxT("jpeg") || ending == wxT("bmp"))
        {

m_imageNoms.push_back(dir.GetNameWithSep() + filename);

// On initialise l'iterateur de fichier au fichier courant

        if (!imageName.empty() && imageName == filename)
            {
                m_imageIterateur = i;
            }
        }

        // Prochain fichier
        cont = dir.GetNext(&filename);
        i++;
    }
}

void MyFrame::TrouverImages(wxString &filepath)
{
    wxString imageName = wxT("");
    this->TrouverImages(filepath, imageName);
}

void MyFrame::OnBibliothequeLivres(wxCommandEvent& event)
{
    if (names.size()==0)
        { wxMessageBox("Bibliotheque vide"); }
    if (names.size()!=0)
       {this->CreerBibli(names);}
}


