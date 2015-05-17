/*
 * Copyright (C) 2015 Tommi Maekitalo
 *
 */

#include <hpdf++.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>

#include <cxxtools/regex.h>
#include <cxxtools/directory.h>
#include <cxxtools/log.h>

log_define("hpdf")

namespace hpdf
{

////////////////////////////////////////////////////////////////////////
// Error handling code
//
namespace
{
  const char* errStr(HPDF_STATUS err)
  {
    switch (err)
    {
      case HPDF_ARRAY_COUNT_ERR: return "array count err";
      case HPDF_ARRAY_ITEM_NOT_FOUND: return "array item not found";
      case HPDF_ARRAY_ITEM_UNEXPECTED_TYPE: return "array item unexpected type";
      case HPDF_BINARY_LENGTH_ERR: return "binary length err";
      case HPDF_CANNOT_GET_PALLET: return "cannot get pallet";
      case HPDF_DICT_COUNT_ERR: return "dict count err";
      case HPDF_DICT_ITEM_NOT_FOUND: return "dict item not found";
      case HPDF_DICT_ITEM_UNEXPECTED_TYPE: return "dict item unexpected type";
      case HPDF_DICT_STREAM_LENGTH_NOT_FOUND: return "dict stream length not found";
      case HPDF_DOC_ENCRYPTDICT_NOT_FOUND: return "doc encryptdict not found";
      case HPDF_DOC_INVALID_OBJECT: return "doc invalid object";
      case HPDF_DUPLICATE_REGISTRATION: return "duplicate registration";
      case HPDF_EXCEED_JWW_CODE_NUM_LIMIT: return "exceed jww code num limit";
      case HPDF_ENCRYPT_INVALID_PASSWORD: return "encrypt invalid password";
      case HPDF_ERR_UNKNOWN_CLASS: return "err unknown class";
      case HPDF_EXCEED_GSTATE_LIMIT: return "exceed gstate limit";
      case HPDF_FAILD_TO_ALLOC_MEM: return "faild to alloc mem";
      case HPDF_FILE_IO_ERROR: return "file io error";
      case HPDF_FILE_OPEN_ERROR: return "file open error";
      case HPDF_FONT_EXISTS: return "font exists";
      case HPDF_FONT_INVALID_WIDTHS_TABLE: return "font invalid widths table";
      case HPDF_INVALID_AFM_HEADER: return "invalid afm header";
      case HPDF_INVALID_ANNOTATION: return "invalid annotation";
      case HPDF_INVALID_BIT_PER_COMPONENT: return "invalid bit per component";
      case HPDF_INVALID_CHAR_MATRICS_DATA: return "invalid char matrics data";
      case HPDF_INVALID_COLOR_SPACE: return "invalid color space";
      case HPDF_INVALID_COMPRESSION_MODE: return "invalid compression mode";
      case HPDF_INVALID_DATE_TIME: return "invalid date time";
      case HPDF_INVALID_DESTINATION: return "invalid destination";
      case HPDF_INVALID_DOCUMENT: return "invalid document";
      case HPDF_INVALID_DOCUMENT_STATE: return "invalid document state";
      case HPDF_INVALID_ENCODER: return "invalid encoder";
      case HPDF_INVALID_ENCODER_TYPE: return "invalid encoder type";
      case HPDF_INVALID_ENCODING_NAME: return "invalid encoding name";
      case HPDF_INVALID_ENCRYPT_KEY_LEN: return "invalid encrypt key len";
      case HPDF_INVALID_FONTDEF_DATA: return "invalid fontdef data";
      case HPDF_INVALID_FONTDEF_TYPE: return "invalid fontdef type";
      case HPDF_INVALID_FONT_NAME: return "invalid font name";
      case HPDF_INVALID_IMAGE: return "invalid image";
      case HPDF_INVALID_JPEG_DATA: return "invalid jpeg data";
      case HPDF_INVALID_N_DATA: return "invalid n data";
      case HPDF_INVALID_OBJECT: return "invalid object";
      case HPDF_INVALID_OBJ_ID: return "invalid obj id";
      case HPDF_INVALID_OPERATION: return "invalid operation";
      case HPDF_INVALID_OUTLINE: return "invalid outline";
      case HPDF_INVALID_PAGE: return "invalid page";
      case HPDF_INVALID_PAGES: return "invalid pages";
      case HPDF_INVALID_PARAMETER: return "invalid parameter";
      case HPDF_INVALID_PNG_IMAGE: return "invalid png image";
      case HPDF_INVALID_STREAM: return "invalid stream";
      case HPDF_MISSING_FILE_NAME_ENTRY: return "missing file name entry";
      case HPDF_INVALID_TTC_FILE: return "invalid ttc file";
      case HPDF_INVALID_TTC_INDEX: return "invalid ttc index";
      case HPDF_INVALID_WX_DATA: return "invalid wx data";
      case HPDF_ITEM_NOT_FOUND: return "item not found";
      case HPDF_LIBPNG_ERROR: return "libpng error";
      case HPDF_NAME_INVALID_VALUE: return "name invalid value";
      case HPDF_NAME_OUT_OF_RANGE: return "name out of range";
      case HPDF_PAGE_INVALID_PARAM_COUNT: return "page invalid param count";
      case HPDF_PAGES_MISSING_KIDS_ENTRY: return "pages missing kids entry";
      case HPDF_PAGE_CANNOT_FIND_OBJECT: return "page cannot find object";
      case HPDF_PAGE_CANNOT_GET_ROOT_PAGES: return "page cannot get root pages";
      case HPDF_PAGE_CANNOT_RESTORE_GSTATE: return "page cannot restore gstate";
      case HPDF_PAGE_CANNOT_SET_PARENT: return "page cannot set parent";
      case HPDF_PAGE_FONT_NOT_FOUND: return "page font not found";
      case HPDF_PAGE_INVALID_FONT: return "page invalid font";
      case HPDF_PAGE_INVALID_FONT_SIZE: return "page invalid font size";
      case HPDF_PAGE_INVALID_GMODE: return "page invalid gmode";
      case HPDF_PAGE_INVALID_INDEX: return "page invalid index";
      case HPDF_PAGE_INVALID_ROTATE_VALUE: return "page invalid rotate value";
      case HPDF_PAGE_INVALID_SIZE: return "page invalid size";
      case HPDF_PAGE_INVALID_XOBJECT: return "page invalid xobject";
      case HPDF_PAGE_OUT_OF_RANGE: return "page out of range";
      case HPDF_REAL_OUT_OF_RANGE: return "real out of range";
      case HPDF_STREAM_EOF: return "stream eof";
      case HPDF_STREAM_READLN_CONTINUE: return "stream readln continue";
      case HPDF_STRING_OUT_OF_RANGE: return "string out of range";
      case HPDF_THIS_FUNC_WAS_SKIPPED: return "this func was skipped";
      case HPDF_TTF_CANNOT_EMBEDDING_FONT: return "ttf cannot embedding font";
      case HPDF_TTF_INVALID_CMAP: return "ttf invalid cmap";
      case HPDF_TTF_INVALID_FOMAT: return "ttf invalid fomat";
      case HPDF_TTF_MISSING_TABLE: return "ttf missing table";
      case HPDF_UNSUPPORTED_FONT_TYPE: return "unsupported font type";
      case HPDF_UNSUPPORTED_FUNC: return "unsupported func";
      case HPDF_UNSUPPORTED_JPEG_FORMAT: return "unsupported jpeg format";
      case HPDF_UNSUPPORTED_TYPE1_FONT: return "unsupported type1 font";
      case HPDF_XREF_COUNT_ERR: return "xref count err";
      case HPDF_ZLIB_ERROR: return "zlib error";
      case HPDF_INVALID_PAGE_INDEX: return "invalid page index";
      case HPDF_INVALID_URI: return "invalid uri";
      case HPDF_PAGE_LAYOUT_OUT_OF_RANGE: return "page layout out of range";
      case HPDF_PAGE_MODE_OUT_OF_RANGE: return "page mode out of range";
      case HPDF_PAGE_NUM_STYLE_OUT_OF_RANGE: return "page num style out of range";
      case HPDF_ANNOT_INVALID_ICON: return "annot invalid icon";
      case HPDF_ANNOT_INVALID_BORDER_STYLE: return "annot invalid border style";
      case HPDF_PAGE_INVALID_DIRECTION: return "page invalid direction";
      case HPDF_INVALID_FONT: return "invalid font";
      case HPDF_PAGE_INSUFFICIENT_SPACE: return "page insufficient space";
      case HPDF_PAGE_INVALID_DISPLAY_TIME: return "page invalid display time";
      case HPDF_PAGE_INVALID_TRANSITION_TIME: return "page invalid transition time";
      case HPDF_INVALID_PAGE_SLIDESHOW_TYPE: return "invalid page slideshow type";
      case HPDF_EXT_GSTATE_OUT_OF_RANGE: return "ext gstate out of range";
      case HPDF_INVALID_EXT_GSTATE: return "invalid ext gstate";
      case HPDF_EXT_GSTATE_READ_ONLY: return "ext gstate read only";
      case HPDF_INVALID_U3D_DATA: return "invalid u3d data";
      case HPDF_NAME_CANNOT_GET_NAMES: return "name cannot get names";
      case HPDF_INVALID_ICC_COMPONENT_NUM: return "invalid icc component num";
      default: return "unknown error";
    }
  }

  std::string mkerrstr(HPDF_STATUS errno_no, HPDF_STATUS detail_no)
  {
    std::ostringstream msg;
    msg << "PDF ERROR " << std::hex << errno_no;
    if (detail_no)
      msg << '(' << std::dec << detail_no << ')';
    msg << ": " << errStr(errno_no);
    return msg.str();
  }

  void error_handler(HPDF_STATUS errno_no, HPDF_STATUS detail_no, void *user_data)
  {
    throw Exception(errno_no, detail_no);
  }
}

Exception::Exception(HPDF_STATUS errno_no, HPDF_STATUS detail_no)
  : std::runtime_error(mkerrstr(errno_no, detail_no)),
    _errno_no(errno_no),
    _detail_no(detail_no)
{
}

////////////////////////////////////////////////////////////////////////
// class Pdf
//
Pdf::Pdf()
  : pdf(HPDF_New(error_handler, 0 /* void* user_data */))
{
  if (!pdf)
    throw std::runtime_error("failed to create PDF");
}

Pdf::~Pdf()
{
  if (pdf)
    HPDF_Free(pdf);
}

namespace
{

static cxxtools::Regex reAfm("(.*)\\.afm$");
static cxxtools::Regex reFontName("^FullName (.*)");
static cxxtools::Regex reTtf("(.*)\\.ttf$");

void getType1Fonts(std::vector<std::string>& ret, const std::string& directory)
{
  log_trace("getType1Fonts(" << directory << ')');

  cxxtools::RegexSMatch s;

  cxxtools::Directory d(directory);
  for (auto it = d.begin(); it != d.end(); ++it)
  {
    std::string path = it.path();

    if ((*it)[0] == '.')
    {
    }
    else if (cxxtools::FileInfo(path).isDirectory())
    {
      getType1Fonts(ret, path);
    }
    else if (reAfm.match(path, s)
      && (cxxtools::FileInfo::exists(std::string(s[1]) + ".pfa")
       || cxxtools::FileInfo::exists(std::string(s[1]) + ".pfb")))
    {
      std::ifstream afmfile(path);
      std::string line;
      while (std::getline(afmfile, line))
      {
        if (reFontName.match(line, s))
        {
          ret.push_back(s[1]);
          break;
        }
      }
    }
  }
}

void getTTFonts(std::vector<std::string>& ret, const std::string& directory)
{
  log_trace("getTTFonts(" << directory << ')');

  cxxtools::RegexSMatch s;

  cxxtools::Directory d(directory);
  for (auto it = d.begin(); it != d.end(); ++it)
  {
    std::string path = it.path();

    if ((*it)[0] == '.')
    {
    }
    else if (cxxtools::FileInfo(path).isDirectory())
    {
      getTTFonts(ret, path);
    }
    else if (reTtf.match(path, s))
    {
      ret.push_back(path);
    }
  }
}

}

std::vector<std::string> getType1Fonts(const std::string& directory)
{
  std::vector<std::string> ret;
  getType1Fonts(ret, directory);
  std::sort(ret.begin(), ret.end());
  return ret;
}

std::vector<std::string> getTTFonts(const std::string& directory)
{
  std::vector<std::string> ret;
  getTTFonts(ret, directory);
  std::sort(ret.begin(), ret.end());
  return ret;
}

const char* Pdf::loadType1FontFromName(const std::string& fontname, const std::string& directory)
{
  log_trace("loadType1FontFromname(\"" << fontname << "\", \"" << directory << "\")");

  cxxtools::Directory d(directory);
  for (auto it = d.begin(); it != d.end(); ++it)
  {
    std::string path = it.path();

    cxxtools::RegexSMatch reAfmMatch;

    if ((*it)[0] == '.')
    {
    }
    else if (cxxtools::FileInfo(path).isDirectory())
    {
      const char* font = loadType1FontFromName(fontname, path);
      if (font)
        return font;
    }
    else if (reAfm.match(path, reAfmMatch))
    {
      std::ifstream afmfile(path);
      std::string line;
      while (std::getline(afmfile, line))
      {
        cxxtools::RegexSMatch s;
        if (reFontName.match(line, s))
        {
          if (fontname == s[1])
          {
            std::string pffilename = reAfmMatch[1];
            pffilename += ".pfa";
            if (!cxxtools::FileInfo::exists(pffilename))
              pffilename.back() = 'b';
            log_debug("load type 1 font; afmfilename=\"" << path << "\" pffilename=\"" << pffilename << '"');
            return loadType1FontFromFile(path.c_str(), pffilename.c_str());
          }

          break;
        }
      }
    }
  }

  return 0;
}

std::ostream& operator<< (std::ostream& out, Pdf& pdf)
{
  HPDF_SaveToStream(pdf.handle());
  log_fine("the size of data is " << HPDF_GetStreamSize(pdf.handle()));

  HPDF_ResetStream(pdf.handle());

  while (out)
  {
    HPDF_BYTE buf[4096];
    HPDF_UINT32 siz = 4096;
    HPDF_ReadFromStream(pdf.handle(), buf, &siz);

    if (siz == 0)
      break;

    out.write(reinterpret_cast<const char*>(&buf[0]), siz);
  }

  return out;
}

////////////////////////////////////////////////////////////////////////
// class Page
//
Page::Page(Pdf& pdf)
  : page(HPDF_AddPage(pdf.handle()))
{
}

Text Page::text()
{
  return Text(*this);
}

////////////////////////////////////////////////////////////////////////
// class Text
//

////////////////////////////////////////////////////////////////////////
// class Outline
//
Outline& Outline::setDestination(Page& page)
{
  HPDF_Destination dst = HPDF_Page_CreateDestination(page.handle());
  HPDF_Destination_SetXYZ(dst, 0, page.height(), 1);
  return setDestination(dst);
}

}
