#include "image.hpp"
#include <math.h>

Image::Image(wxPanel *parent) : wxScrolled<wxPanel>(parent, -1)
{
    wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);

    espace_image =
        new wxStaticBitmap(this, -1, wxNullBitmap, wxDefaultPosition, wxSize(-1, -1));

    panelSizer->Add(espace_image, 1);
    this->SetSizer(panelSizer);

    this->FitInside();

    this->SetScrollRate(5, 5);
}


void Image::setImage(std::shared_ptr<wxImage> image)
{
    if (image != nullptr)
    {
        monimage = image;
        wxBitmap bmp(*monimage, wxBITMAP_SCREEN_DEPTH);
        espace_image->SetBitmap(bmp);
        this->SetVirtualSize(image->GetSize());
    }
}


