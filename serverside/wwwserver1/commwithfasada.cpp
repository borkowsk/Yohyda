// FASADA is a simple interface for communicating any user side application
// thru WWW . Mostly based on boost examples.
//
// Copyright (c) 2019, Wojciech Borkowski
// (wborkowsk MAŁPA gmail KROPA com , wborkowski MAŁPA uw KROPA edu KROPA pl)
//
#include "URLparser.hpp" //facies compatible URL parser
#include "request.hpp"
#include "reply.hpp"
#include <iostream>
#include <string>

namespace http {
namespace server {

bool communicate_with_fasada(const request& req, reply& rep)//TMP IMPLEMENTATION
{
    return false;//TOTALLY FAILED
}

} // namespace server
} // namespace http
