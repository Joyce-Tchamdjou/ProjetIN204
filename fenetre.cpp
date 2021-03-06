#include "fenetre.hpp"
#include "image.cpp"

Fenetre::Fenetre(wxFrame *parent) : wxScrolledWindow (parent, -1)
{
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    monimage = new Image(this);
    monimage->SetMinSize(wxSize(-1, 300));
    sizer->Add(monimage, 1, wxALL | wxEXPAND, 10);

   
    this->SetScrollRate(20, 20);
    this->SetSizer(sizer);
    this->FitInside();
}

void Fenetre::setImage(std::shared_ptr<wxImage> image)
{
    monimage->setImage(image);
}


