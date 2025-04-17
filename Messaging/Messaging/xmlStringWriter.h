#pragma once
//https://stackoverflow.com/questions/60335343/how-to-get-inner-xml-of-a-node-in-pugixml

#include "pugixml.hpp"

struct xml_string_writer : pugi::xml_writer
{
    std::string result;

    virtual void write(const void* data, size_t size)
    {
        result.append(static_cast<const char*>(data), size);
    }
};