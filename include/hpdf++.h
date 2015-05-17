/*
 * Copyright (C) 2015 Tommi Maekitalo
 *
 */

#ifndef HPDFPP_H
#define HPDFPP_H

#include <hpdf.h>
#include <stdexcept>
#include <iosfwd>
#include <string>
#include <vector>

namespace hpdf
{
  ////////////////////////////////////////////////////////////////////////
  // class Exception
  //
  class Exception : public std::runtime_error
  {
      HPDF_STATUS _errno_no;
      HPDF_STATUS _detail_no;

    public:
      Exception(HPDF_STATUS errno_no, HPDF_STATUS detail_no);

      HPDF_STATUS errno_no() const    { return _errno_no; }
      HPDF_STATUS detail_no() const   { return _detail_no; }
  };

  ////////////////////////////////////////////////////////////////////////
  // free functions
  //
  std::vector<std::string> getType1Fonts(const std::string& directory = "/usr/share/fonts");
  std::vector<std::string> getTTFonts(const std::string& directory = "/usr/share/fonts");

  ////////////////////////////////////////////////////////////////////////
  // class Pdf
  //
  class Pdf
  {
      friend std::ostream& operator<< (std::ostream& out, Pdf& pdf);

      Pdf(const Pdf&) = delete;
      Pdf& operator=(const Pdf&) = delete;

      HPDF_Doc pdf;

    public:
      Pdf();
      ~Pdf();

      HPDF_Doc handle()  { return pdf; }

      void setPageMode(HPDF_PageMode mode)
        { HPDF_SetPageMode(pdf, mode); }

      const char* loadType1FontFromFile(const char* afmfilename, const char* pfmfilename)
        { return HPDF_LoadType1FontFromFile(pdf, afmfilename, pfmfilename); }
      const char* loadType1FontFromName(const std::string& fontname, const std::string& directory = "/usr/share/fonts");
      const char* loadTTFontFromFile(const char* file_name, HPDF_BOOL embedding = true)
        { return HPDF_LoadTTFontFromFile(pdf, file_name, embedding); }
  };

  std::ostream& operator<< (std::ostream& out, Pdf& pdf);

  ////////////////////////////////////////////////////////////////////////
  // class Page
  //
  class Text;
  class Page
  {
      HPDF_Page page;

    public:
      explicit Page(Pdf& pdf);

      HPDF_Page handle()   { return page; }

      HPDF_REAL height()
        { return HPDF_Page_GetHeight(page); }
      HPDF_REAL width()
        { return HPDF_Page_GetWidth(page); }
      HPDF_REAL textWidth(const char* text)
        { return HPDF_Page_TextWidth(page, text); }

      Page& setLineWidth(HPDF_REAL line_width)
        { HPDF_Page_SetLineWidth(page, line_width); return *this; }
      Page& moveTo(HPDF_REAL x, HPDF_REAL y)
        { HPDF_Page_MoveTo(page, x, y); return *this; }
      Page& lineTo(HPDF_REAL x, HPDF_REAL y)
        { HPDF_Page_LineTo(page, x, y); return *this; }
      Page& curveTo(HPDF_REAL x1, HPDF_REAL y1, HPDF_REAL x2, HPDF_REAL y2, HPDF_REAL x3, HPDF_REAL y3)
        { HPDF_Page_CurveTo(page, x1, y1, x2, y2, x3, y3); return *this; }
      Page& curveTo2(HPDF_REAL x1, HPDF_REAL y1, HPDF_REAL x2, HPDF_REAL y2, HPDF_REAL x3, HPDF_REAL y3)
        { HPDF_Page_CurveTo2(page, x2, y2, x3, y3); return *this; }
      Page& curveTo3(HPDF_REAL x1, HPDF_REAL y1, HPDF_REAL x3, HPDF_REAL y3)
        { HPDF_Page_CurveTo3(page, x1, y1, x3, y3); return *this; }
      Page& closePath()
        { HPDF_Page_ClosePath(page); return *this; }
      Page& rectangle(HPDF_REAL x, HPDF_REAL y, HPDF_REAL width, HPDF_REAL height)
        { HPDF_Page_Rectangle(page, x, y, width, height); return *this; }
      Page& closePathStroke()
        { HPDF_Page_ClosePathStroke(page); return *this; }
      Page& stroke()
        { HPDF_Page_Stroke(page); return *this; }

      Text text();
  };

  ////////////////////////////////////////////////////////////////////////
  // class Font
  //
  class Font
  {
      HPDF_Font font;

    public:
      explicit Font(Pdf& pdf, const char* font_name, const char* encoding_name = 0)
        : font(HPDF_GetFont(pdf.handle(), font_name, encoding_name))
        { }

      HPDF_Font handle()   { return font; }

  };

  ////////////////////////////////////////////////////////////////////////
  // class Text
  //
  class Text
  {
      Page& _page;

      Text(const Text&) = delete;
      Text& operator=(const Text&) = delete;

    public:
      explicit Text(Page& page)
        : _page(page)
      {
        HPDF_Page_BeginText(_page.handle());
      }

      Text(Text&&) = default;

      ~Text()
      {
        HPDF_Page_EndText(_page.handle());
      }

      Text& setFontAndSize(Font& font, HPDF_REAL size)
        { HPDF_Page_SetFontAndSize(_page.handle(), font.handle(), size); return *this; }
      Text& moveTextPos(HPDF_REAL x, HPDF_REAL y)
        { HPDF_Page_MoveTextPos(_page.handle(), x, y); return *this; }
      Text& moveTextPos2(HPDF_REAL x, HPDF_REAL y)
        { HPDF_Page_MoveTextPos2(_page.handle(), x, y); return *this; }
      Text& moveToNextLine()
        { HPDF_Page_MoveToNextLine(_page.handle()); return *this; }
      Text& showText(const char* text)
        { HPDF_Page_ShowText(_page.handle(), text); return *this; }
      Text& showTextNextLine(const char* text)
        { HPDF_Page_ShowTextNextLine(_page.handle(), text); return *this; }
      Text& textOut(HPDF_REAL xpos, HPDF_REAL ypos, const char* text)
        { HPDF_Page_TextOut(_page.handle(), xpos, ypos, text); return *this; }
      Text& textRect(HPDF_REAL left, HPDF_REAL top, HPDF_REAL right, HPDF_REAL bottom, const char* text, HPDF_TextAlignment align, HPDF_UINT* len = 0)
        { HPDF_Page_TextRect(_page.handle(), left, top, right, bottom, text, align, len); return *this; }

      Text& textCenter(HPDF_REAL xpos, HPDF_REAL ypos, const char* text)
        { return textOut(xpos - _page.textWidth(text) / 2, ypos, text); }
      Text& textRight(HPDF_REAL xpos, HPDF_REAL ypos, const char* text)
        { return textOut(xpos - _page.textWidth(text), ypos, text); }
  };

  ////////////////////////////////////////////////////////////////////////
  // class Outline
  //
  class Outline
  {
      Pdf& _pdf;
      HPDF_Outline _outline;

    public:
      Outline(Pdf& pdf, const char* title, HPDF_Encoder encoder = 0)
        : _pdf(pdf),
          _outline(HPDF_CreateOutline(pdf.handle(), 0, title, encoder))
      { }

      Outline(Outline& parent, const char* title, HPDF_Encoder encoder = 0)
        : _pdf(parent._pdf),
          _outline(HPDF_CreateOutline(_pdf.handle(), parent._outline, title, encoder))
      { }

      Outline& setOpened(HPDF_BOOL opened = true)
        { HPDF_Outline_SetOpened(_outline, opened); return *this; }
      Outline& setDestination(HPDF_Destination dst)
        { HPDF_Outline_SetDestination(_outline, dst); return *this; }
      Outline& setDestination(Page& page);
  };

}

#endif // HPDFPP_H

