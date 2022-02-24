//
// Created by salim on 24/02/2022.
//

#ifndef FUNCTIONAL_RENDERJSONVALUES_HPP
#define FUNCTIONAL_RENDERJSONVALUES_HPP

#include "functProgPCH.hpp"
#include "jsonDataTypes.hpp"

namespace functfun
{

    //FIXME: implement this!!
   std::string render_JsonValue(const JsonValue& jsv) {return std::string{};}

   std::string render_bool(bool bvalue) { return bvalue ? "true" : "false";}
   std::string render_double(double value) { return fmt::format("{}", value);}

   // FIXME: see if it is better to use std::string value &
   //  std::move inside fmt::format("{}", std::move(svalue)
   std::string render_string(std::string_view svalue)
   {
       return fmt::format("\"{}\"", svalue);
   }

   std::string render_null(std::nullptr_t) { return "null";}

   std::string render_array(const JsonArray& jarray)
   {
       return std::string{"["} +
                  std::accumulate(jarray.cbegin(), jarray.cend(), std::string{","},
                                  [](auto&& init, const JsonValue& jsv)->std::string
                                  {
                                      return init + render_JsonValue(jsv);
                                  })
                                  + "]";

   }







}


#endif//FUNCTIONAL_RENDERJSONVALUES_HPP
