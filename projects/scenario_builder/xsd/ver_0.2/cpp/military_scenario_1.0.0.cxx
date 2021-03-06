// Copyright (c) 2005-2014 Code Synthesis Tools CC
//
// This program was generated by CodeSynthesis XSD, an XML Schema to
// C++ data binding compiler.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
// In addition, as a special exception, Code Synthesis Tools CC gives
// permission to link this program with the Xerces-C++ library (or with
// modified versions of Xerces-C++ that use the same license as Xerces-C++),
// and distribute linked combinations including the two. You must obey
// the GNU General Public License version 2 in all respects for all of
// the code used other than Xerces-C++. If you modify this copy of the
// program, you may extend this exception to your version of the program,
// but you are not obligated to do so. If you do not wish to do so, delete
// this exception statement from your version.
//
// Furthermore, Code Synthesis Tools CC makes a special exception for
// the Free/Libre and Open Source Software (FLOSS) which is described
// in the accompanying FLOSSE file.
//

// Begin prologue.
//
//
// End prologue.

#include <xsd/cxx/pre.hxx>

#include "military_scenario_1.0.0.hxx"

namespace msdl_1
{
}

#include <xsd/cxx/xml/dom/parsing-source.hxx>

#include <xsd/cxx/tree/type-factory-map.hxx>

namespace _xsd
{
  static
  const ::xsd::cxx::tree::type_factory_plate< 0, char >
  type_factory_plate_init;
}

namespace msdl_1
{
}

#include <ostream>

#include <xsd/cxx/tree/std-ostream-map.hxx>

namespace _xsd
{
  static
  const ::xsd::cxx::tree::std_ostream_plate< 0, char >
  std_ostream_plate_init;
}

namespace msdl_1
{
}

#include <istream>
#include <xsd/cxx/xml/sax/std-input-source.hxx>
#include <xsd/cxx/tree/error-handler.hxx>

namespace msdl_1
{
  ::std::unique_ptr< ::msdl_1::MilitaryScenarioType >
  MilitaryScenario (const ::std::string& u,
                    ::xml_schema::flags f,
                    const ::xml_schema::properties& p)
  {
    ::xsd::cxx::xml::auto_initializer i (
      (f & ::xml_schema::flags::dont_initialize) == 0,
      (f & ::xml_schema::flags::keep_dom) == 0);

    ::xsd::cxx::tree::error_handler< char > h;

    ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
      ::xsd::cxx::xml::dom::parse< char > (
        u, h, p, f));

    h.throw_if_failed< ::xsd::cxx::tree::parsing< char > > ();

    return ::std::unique_ptr< ::msdl_1::MilitaryScenarioType > (
      ::msdl_1::MilitaryScenario (
        std::move (d), f | ::xml_schema::flags::own_dom, p));
  }

  ::std::unique_ptr< ::msdl_1::MilitaryScenarioType >
  MilitaryScenario (const ::std::string& u,
                    ::xml_schema::error_handler& h,
                    ::xml_schema::flags f,
                    const ::xml_schema::properties& p)
  {
    ::xsd::cxx::xml::auto_initializer i (
      (f & ::xml_schema::flags::dont_initialize) == 0,
      (f & ::xml_schema::flags::keep_dom) == 0);

    ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
      ::xsd::cxx::xml::dom::parse< char > (
        u, h, p, f));

    if (!d.get ())
      throw ::xsd::cxx::tree::parsing< char > ();

    return ::std::unique_ptr< ::msdl_1::MilitaryScenarioType > (
      ::msdl_1::MilitaryScenario (
        std::move (d), f | ::xml_schema::flags::own_dom, p));
  }

  ::std::unique_ptr< ::msdl_1::MilitaryScenarioType >
  MilitaryScenario (const ::std::string& u,
                    ::xercesc::DOMErrorHandler& h,
                    ::xml_schema::flags f,
                    const ::xml_schema::properties& p)
  {
    ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
      ::xsd::cxx::xml::dom::parse< char > (
        u, h, p, f));

    if (!d.get ())
      throw ::xsd::cxx::tree::parsing< char > ();

    return ::std::unique_ptr< ::msdl_1::MilitaryScenarioType > (
      ::msdl_1::MilitaryScenario (
        std::move (d), f | ::xml_schema::flags::own_dom, p));
  }

  ::std::unique_ptr< ::msdl_1::MilitaryScenarioType >
  MilitaryScenario (::std::istream& is,
                    ::xml_schema::flags f,
                    const ::xml_schema::properties& p)
  {
    ::xsd::cxx::xml::auto_initializer i (
      (f & ::xml_schema::flags::dont_initialize) == 0,
      (f & ::xml_schema::flags::keep_dom) == 0);

    ::xsd::cxx::xml::sax::std_input_source isrc (is);
    return ::msdl_1::MilitaryScenario (isrc, f, p);
  }

  ::std::unique_ptr< ::msdl_1::MilitaryScenarioType >
  MilitaryScenario (::std::istream& is,
                    ::xml_schema::error_handler& h,
                    ::xml_schema::flags f,
                    const ::xml_schema::properties& p)
  {
    ::xsd::cxx::xml::auto_initializer i (
      (f & ::xml_schema::flags::dont_initialize) == 0,
      (f & ::xml_schema::flags::keep_dom) == 0);

    ::xsd::cxx::xml::sax::std_input_source isrc (is);
    return ::msdl_1::MilitaryScenario (isrc, h, f, p);
  }

  ::std::unique_ptr< ::msdl_1::MilitaryScenarioType >
  MilitaryScenario (::std::istream& is,
                    ::xercesc::DOMErrorHandler& h,
                    ::xml_schema::flags f,
                    const ::xml_schema::properties& p)
  {
    ::xsd::cxx::xml::sax::std_input_source isrc (is);
    return ::msdl_1::MilitaryScenario (isrc, h, f, p);
  }

  ::std::unique_ptr< ::msdl_1::MilitaryScenarioType >
  MilitaryScenario (::std::istream& is,
                    const ::std::string& sid,
                    ::xml_schema::flags f,
                    const ::xml_schema::properties& p)
  {
    ::xsd::cxx::xml::auto_initializer i (
      (f & ::xml_schema::flags::dont_initialize) == 0,
      (f & ::xml_schema::flags::keep_dom) == 0);

    ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
    return ::msdl_1::MilitaryScenario (isrc, f, p);
  }

  ::std::unique_ptr< ::msdl_1::MilitaryScenarioType >
  MilitaryScenario (::std::istream& is,
                    const ::std::string& sid,
                    ::xml_schema::error_handler& h,
                    ::xml_schema::flags f,
                    const ::xml_schema::properties& p)
  {
    ::xsd::cxx::xml::auto_initializer i (
      (f & ::xml_schema::flags::dont_initialize) == 0,
      (f & ::xml_schema::flags::keep_dom) == 0);

    ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
    return ::msdl_1::MilitaryScenario (isrc, h, f, p);
  }

  ::std::unique_ptr< ::msdl_1::MilitaryScenarioType >
  MilitaryScenario (::std::istream& is,
                    const ::std::string& sid,
                    ::xercesc::DOMErrorHandler& h,
                    ::xml_schema::flags f,
                    const ::xml_schema::properties& p)
  {
    ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
    return ::msdl_1::MilitaryScenario (isrc, h, f, p);
  }

  ::std::unique_ptr< ::msdl_1::MilitaryScenarioType >
  MilitaryScenario (::xercesc::InputSource& i,
                    ::xml_schema::flags f,
                    const ::xml_schema::properties& p)
  {
    ::xsd::cxx::tree::error_handler< char > h;

    ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
      ::xsd::cxx::xml::dom::parse< char > (
        i, h, p, f));

    h.throw_if_failed< ::xsd::cxx::tree::parsing< char > > ();

    return ::std::unique_ptr< ::msdl_1::MilitaryScenarioType > (
      ::msdl_1::MilitaryScenario (
        std::move (d), f | ::xml_schema::flags::own_dom, p));
  }

  ::std::unique_ptr< ::msdl_1::MilitaryScenarioType >
  MilitaryScenario (::xercesc::InputSource& i,
                    ::xml_schema::error_handler& h,
                    ::xml_schema::flags f,
                    const ::xml_schema::properties& p)
  {
    ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
      ::xsd::cxx::xml::dom::parse< char > (
        i, h, p, f));

    if (!d.get ())
      throw ::xsd::cxx::tree::parsing< char > ();

    return ::std::unique_ptr< ::msdl_1::MilitaryScenarioType > (
      ::msdl_1::MilitaryScenario (
        std::move (d), f | ::xml_schema::flags::own_dom, p));
  }

  ::std::unique_ptr< ::msdl_1::MilitaryScenarioType >
  MilitaryScenario (::xercesc::InputSource& i,
                    ::xercesc::DOMErrorHandler& h,
                    ::xml_schema::flags f,
                    const ::xml_schema::properties& p)
  {
    ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
      ::xsd::cxx::xml::dom::parse< char > (
        i, h, p, f));

    if (!d.get ())
      throw ::xsd::cxx::tree::parsing< char > ();

    return ::std::unique_ptr< ::msdl_1::MilitaryScenarioType > (
      ::msdl_1::MilitaryScenario (
        std::move (d), f | ::xml_schema::flags::own_dom, p));
  }

  ::std::unique_ptr< ::msdl_1::MilitaryScenarioType >
  MilitaryScenario (const ::xercesc::DOMDocument& doc,
                    ::xml_schema::flags f,
                    const ::xml_schema::properties& p)
  {
    if (f & ::xml_schema::flags::keep_dom)
    {
      ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
        static_cast< ::xercesc::DOMDocument* > (doc.cloneNode (true)));

      return ::std::unique_ptr< ::msdl_1::MilitaryScenarioType > (
        ::msdl_1::MilitaryScenario (
          std::move (d), f | ::xml_schema::flags::own_dom, p));
    }

    const ::xercesc::DOMElement& e (*doc.getDocumentElement ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (e));

    ::std::unique_ptr< ::xsd::cxx::tree::type > tmp (
      ::xsd::cxx::tree::type_factory_map_instance< 0, char > ().create (
        "MilitaryScenario",
        "urn:sisostds:scenario:military:data:draft:msdl:1",
        &::xsd::cxx::tree::factory_impl< ::msdl_1::MilitaryScenarioType >,
        true, true, e, n, f, 0));

    if (tmp.get () != 0)
    {
      ::std::unique_ptr< ::msdl_1::MilitaryScenarioType > r (
        dynamic_cast< ::msdl_1::MilitaryScenarioType* > (tmp.get ()));

      if (r.get ())
        tmp.release ();
      else
        throw ::xsd::cxx::tree::not_derived< char > ();

      return r;
    }

    throw ::xsd::cxx::tree::unexpected_element < char > (
      n.name (),
      n.namespace_ (),
      "MilitaryScenario",
      "urn:sisostds:scenario:military:data:draft:msdl:1");
  }

  ::std::unique_ptr< ::msdl_1::MilitaryScenarioType >
  MilitaryScenario (::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d,
                    ::xml_schema::flags f,
                    const ::xml_schema::properties&)
  {
    ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > c (
      ((f & ::xml_schema::flags::keep_dom) &&
       !(f & ::xml_schema::flags::own_dom))
      ? static_cast< ::xercesc::DOMDocument* > (d->cloneNode (true))
      : 0);

    ::xercesc::DOMDocument& doc (c.get () ? *c : *d);
    const ::xercesc::DOMElement& e (*doc.getDocumentElement ());

    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (e));

    if (f & ::xml_schema::flags::keep_dom)
      doc.setUserData (::xml_schema::dom::tree_node_key,
                       (c.get () ? &c : &d),
                       0);

    ::std::unique_ptr< ::xsd::cxx::tree::type > tmp (
      ::xsd::cxx::tree::type_factory_map_instance< 0, char > ().create (
        "MilitaryScenario",
        "urn:sisostds:scenario:military:data:draft:msdl:1",
        &::xsd::cxx::tree::factory_impl< ::msdl_1::MilitaryScenarioType >,
        true, true, e, n, f, 0));

    if (tmp.get () != 0)
    {

      ::std::unique_ptr< ::msdl_1::MilitaryScenarioType > r (
        dynamic_cast< ::msdl_1::MilitaryScenarioType* > (tmp.get ()));

      if (r.get ())
        tmp.release ();
      else
        throw ::xsd::cxx::tree::not_derived< char > ();

      return r;
    }

    throw ::xsd::cxx::tree::unexpected_element < char > (
      n.name (),
      n.namespace_ (),
      "MilitaryScenario",
      "urn:sisostds:scenario:military:data:draft:msdl:1");
  }
}

#include <ostream>
#include <xsd/cxx/tree/error-handler.hxx>
#include <xsd/cxx/xml/dom/serialization-source.hxx>

#include <xsd/cxx/tree/type-serializer-map.hxx>

namespace _xsd
{
  static
  const ::xsd::cxx::tree::type_serializer_plate< 0, char >
  type_serializer_plate_init;
}

namespace msdl_1
{
  void
  MilitaryScenario (::std::ostream& o,
                    const ::msdl_1::MilitaryScenarioType& s,
                    const ::xml_schema::namespace_infomap& m,
                    const ::std::string& e,
                    ::xml_schema::flags f)
  {
    ::xsd::cxx::xml::auto_initializer i (
      (f & ::xml_schema::flags::dont_initialize) == 0);

    ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
      ::msdl_1::MilitaryScenario (s, m, f));

    ::xsd::cxx::tree::error_handler< char > h;

    ::xsd::cxx::xml::dom::ostream_format_target t (o);
    if (!::xsd::cxx::xml::dom::serialize (t, *d, e, h, f))
    {
      h.throw_if_failed< ::xsd::cxx::tree::serialization< char > > ();
    }
  }

  void
  MilitaryScenario (::std::ostream& o,
                    const ::msdl_1::MilitaryScenarioType& s,
                    ::xml_schema::error_handler& h,
                    const ::xml_schema::namespace_infomap& m,
                    const ::std::string& e,
                    ::xml_schema::flags f)
  {
    ::xsd::cxx::xml::auto_initializer i (
      (f & ::xml_schema::flags::dont_initialize) == 0);

    ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
      ::msdl_1::MilitaryScenario (s, m, f));
    ::xsd::cxx::xml::dom::ostream_format_target t (o);
    if (!::xsd::cxx::xml::dom::serialize (t, *d, e, h, f))
    {
      throw ::xsd::cxx::tree::serialization< char > ();
    }
  }

  void
  MilitaryScenario (::std::ostream& o,
                    const ::msdl_1::MilitaryScenarioType& s,
                    ::xercesc::DOMErrorHandler& h,
                    const ::xml_schema::namespace_infomap& m,
                    const ::std::string& e,
                    ::xml_schema::flags f)
  {
    ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
      ::msdl_1::MilitaryScenario (s, m, f));
    ::xsd::cxx::xml::dom::ostream_format_target t (o);
    if (!::xsd::cxx::xml::dom::serialize (t, *d, e, h, f))
    {
      throw ::xsd::cxx::tree::serialization< char > ();
    }
  }

  void
  MilitaryScenario (::xercesc::XMLFormatTarget& t,
                    const ::msdl_1::MilitaryScenarioType& s,
                    const ::xml_schema::namespace_infomap& m,
                    const ::std::string& e,
                    ::xml_schema::flags f)
  {
    ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
      ::msdl_1::MilitaryScenario (s, m, f));

    ::xsd::cxx::tree::error_handler< char > h;

    if (!::xsd::cxx::xml::dom::serialize (t, *d, e, h, f))
    {
      h.throw_if_failed< ::xsd::cxx::tree::serialization< char > > ();
    }
  }

  void
  MilitaryScenario (::xercesc::XMLFormatTarget& t,
                    const ::msdl_1::MilitaryScenarioType& s,
                    ::xml_schema::error_handler& h,
                    const ::xml_schema::namespace_infomap& m,
                    const ::std::string& e,
                    ::xml_schema::flags f)
  {
    ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
      ::msdl_1::MilitaryScenario (s, m, f));
    if (!::xsd::cxx::xml::dom::serialize (t, *d, e, h, f))
    {
      throw ::xsd::cxx::tree::serialization< char > ();
    }
  }

  void
  MilitaryScenario (::xercesc::XMLFormatTarget& t,
                    const ::msdl_1::MilitaryScenarioType& s,
                    ::xercesc::DOMErrorHandler& h,
                    const ::xml_schema::namespace_infomap& m,
                    const ::std::string& e,
                    ::xml_schema::flags f)
  {
    ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d (
      ::msdl_1::MilitaryScenario (s, m, f));
    if (!::xsd::cxx::xml::dom::serialize (t, *d, e, h, f))
    {
      throw ::xsd::cxx::tree::serialization< char > ();
    }
  }

  void
  MilitaryScenario (::xercesc::DOMDocument& d,
                    const ::msdl_1::MilitaryScenarioType& s,
                    ::xml_schema::flags)
  {
    ::xercesc::DOMElement& e (*d.getDocumentElement ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (e));

    if (typeid (::msdl_1::MilitaryScenarioType) == typeid (s))
    {
      if (n.name () == "MilitaryScenario" &&
          n.namespace_ () == "urn:sisostds:scenario:military:data:draft:msdl:1")
      {
        e << s;
      }
      else
      {
        throw ::xsd::cxx::tree::unexpected_element < char > (
          n.name (),
          n.namespace_ (),
          "MilitaryScenario",
          "urn:sisostds:scenario:military:data:draft:msdl:1");
      }
    }
    else
    {
      ::xsd::cxx::tree::type_serializer_map_instance< 0, char > ().serialize (
        "MilitaryScenario",
        "urn:sisostds:scenario:military:data:draft:msdl:1",
        e, n, s);
    }
  }

  ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument >
  MilitaryScenario (const ::msdl_1::MilitaryScenarioType& s,
                    const ::xml_schema::namespace_infomap& m,
                    ::xml_schema::flags f)
  {
    ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d;

    if (typeid (::msdl_1::MilitaryScenarioType) == typeid (s))
    {
      d = ::xsd::cxx::xml::dom::serialize< char > (
        "MilitaryScenario",
        "urn:sisostds:scenario:military:data:draft:msdl:1",
        m, f);
    }
    else
    {
      d = ::xsd::cxx::tree::type_serializer_map_instance< 0, char > ().serialize (
        "MilitaryScenario",
        "urn:sisostds:scenario:military:data:draft:msdl:1",
        m, s, f);
    }

    ::msdl_1::MilitaryScenario (*d, s, f);
    return d;
  }
}

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

