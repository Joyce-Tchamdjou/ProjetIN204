#ifndef FENETRE_H
#define FENETRE_H
#include <memory>
#include <wx/wx.h>
#include"image.hpp"

class Fenetre : public wxScrolledWindow
{
    public:
        Fenetre(wxFrame *parent);
        void setImage(std::shared_ptr<wxImage> image);
    private:
        Image *monimage;
};

#endif
