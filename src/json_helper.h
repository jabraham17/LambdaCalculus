

#ifndef __JSON_HELPER_H_
#define __JSON_HELPER_H_

#include <string>
#include <sstream>

namespace JSON {

    //repeat string x num of times
    std::string repeat(std::string str, int x) {
        std::stringstream out;
        for(int i = 0; i < x; i++) {
            out << str;
        }
        return out.str();
    }


    //make json pretty
    //every time we see '[', ']', '{', or '}' we add a newline
    //every time we see '[' or '{', increase indent
    //every time we see ']' or '}', decrease indent
    //if comma, add new line
    //if colon add space
    //if in quotes do none of these things
    std::string pretty(std::string str) {
        std::stringstream out;
        int indent = 0;
        bool inQuote = false;
        std::string delim = "  ";
        for(int i = 0; i < str.size(); i++) {

            //if in quote and quote isnt escaped, swap inQuote state
            if(str[i] == '"' && i-1 >=0 && str[i] != '\\') {
                inQuote = !inQuote;
                out << str[i];
            }
            //in open brace
            else if(!inQuote && (str[i] == '[' || str[i] == '{')) {
                out << str[i];
                out << '\n' << repeat(delim,++indent);
            }
            //in close brace
            else if(!inQuote && (str[i] == ']' || str[i] == '}')) {
                out << '\n' << repeat(delim,--indent);
                out << str[i];
            }
            //comma
            else if(!inQuote && str[i] == ',') {
                out << str[i];
                out << '\n' << repeat(delim,indent);
            }
            //colon
            else if(!inQuote && str[i] == ':') {
                out << str[i];
                out << " ";
            }
            else {
                out << str[i];
            }
        }
        return out.str();
    }
}



#endif
