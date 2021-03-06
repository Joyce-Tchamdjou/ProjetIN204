#ifndef IMAGE_H
#define IMAGE_H

#include <memory>
#include <wx/wx.h>


class Image : public wxScrolled<wxPanel>
{
    public:
        Image(wxPanel *parent); //Constructeur
        void setImage(std::shared_ptr<wxImage> image);
    private:
        wxStaticBitmap *espace_image; //espace reserve a l'image
        std::shared_ptr<wxImage> monimage; //pointeur sur l'image
};

#endif

