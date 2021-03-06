//
// Created by salim on 24/02/2022.
//

#ifndef FUNCTIONAL_RENDERJSONVALUES_HPP
#define FUNCTIONAL_RENDERJSONVALUES_HPP

#include "functProgPCH.hpp"
#include "jsonDataTypes.hpp"
#include "fold_variant.hpp"
#include "join.hpp"

#include <fmt/format.h>


namespace functfun
{
   std::string render_JsonValue(const JsonValue& jsv);

   std::string render_bool(bool bvalue) { return bvalue ? "true" : "false";}
   std::string render_double(double value) { return fmt::format("{}", value);}

   // FIXME: see if it is better to use std::string value &
   //  std::move inside fmt::format("{}", std::move(svalue)
   std::string render_string(std::string_view svalue)
   {
       return fmt::format("{}", svalue);
   }

   std::string render_null(std::nullptr_t) { return "null";}

   std::string render_array(const JsonArray& jarray)
   {
       // FIXME: add a join() algorithm with output_iterator and a delimiter
       //  make sure the delimiter is not added to last element
       return std::string{"["} +
                  std::accumulate(jarray.cbegin(), jarray.cend(), std::string{""},
                                  [](auto&& init, const JsonValue& jsv)->std::string
                                  {
                                      return std::forward<std::string>(init)  + render_JsonValue(jsv) + std::string{","};
                                  })
                                  + "]";

   }

   std::string render_object(const JsonObject& jobject)
   {
       return std::string{"{"} +
            std::accumulate(jobject.cbegin(), jobject.cend(), std::string{""},
            [](auto&& init, const JsonObject::value_type& elem)->std::string
            {

                 return std::forward<std::string>(init) + elem.first + std::string{":"} + render_JsonValue(elem.second) + std::string{","};
            })
             + "}";

   }

   std::string render_JsonValue(const JsonValue& jsv)
   {
          return fold_variant(jsv, render_bool, render_double, render_string, render_null, render_array, render_object);
   }


} // end of namespace functfun


#endif//FUNCTIONAL_RENDERJSONVALUES_HPP
