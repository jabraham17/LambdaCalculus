
#include "preprocessor.h"
#include <utility>

Token Preprocessor::GetToken() {
    //preprocessing so we get some tokens
    if(tokens.empty()) {
        preprocess();
    }
    //return a token
    Token t = tokens.back();
    tokens.pop_back();
    return t;
}
Token Preprocessor::Peek() {
    Token t = GetToken();
    UngetToken(t);
    return t;
}
TokenType Preprocessor::UngetToken(Token t) {
    tokens.push_back(t);;
    return t.token_type;
}

void Preprocessor::readline(std::vector<Token>& line) {
    //hold the next token
    Token tmp;
    tmp = lexer.GetToken();
    //while not the end of the line, the end of the file, or an error
    while(tmp.token_type != SEMICOLON &&
          tmp.token_type != END_OF_FILE &&
          tmp.token_type != ERROR) {
        //add it and get a new one
        line.push_back(tmp);
        tmp = lexer.GetToken();
    }
    //add the last gotten token
    line.push_back(tmp);
}

void Preprocessor::preprocess() {
    //read a line of input
    std::vector<Token> line;
    readline(line);

    std::vector<Token> name;
    std::vector<Token> expr = line;

    //if this is a named expr, we want to remove the name before we process
    //we do this by finding equals and splitting around it
    for(int i = 0; i < line.size(); i++) {
        if(line[i].token_type == EQUALS) {
            //this is named, split
            name = sliceVector(line, 0, i);
            expr = sliceVector(line, i+1, line.size()-1);
            break;
        }
    }



    applyAbstraction(expr);
    applyApplication(expr);


    //combine name and expr back together
    std::vector<Token> processed;
    for(auto t: name) processed.push_back(t);
    for(auto t: expr) processed.push_back(t);

    //add the line to the buffer in reverse order
    for(int i = processed.size() - 1; i >= 0; i--) {
        tokens.push_back(processed[i]);
    }

}


//this function is recursive
//once it sees a lambda it adds an open curly just before it
//then goes through the line counting parenthesis
// LPAREN is +1, RPAREN is -1
//if counter becomes negative or end of line is reached, put closing curly
void Preprocessor::applyAbstraction(std::vector<Token>& line) {
    bool foundLambda = false;
    int parenCount = 0;
    int left = -1;
    int right = -1;
    for(int i = 0; i < line.size(); i++) {
        Token t = line[i];
        //we haven't found a lambda, searching for it
        if(!foundLambda && t.token_type == LAMBDA) {
            //need to make sure haven't already marked this, check the previous token
            int prev = i - 1;
            if(prev < 0 || line[prev].token_type != LCURLY) {
                //havent seen it before
                //found it, set left location
                foundLambda = true;
                parenCount = 1;
                left = i;
            }
        }
        //already found lambda and lparen, increase count
        if(foundLambda && (t.token_type == LPAREN || t.token_type == LBRACK)) {
            parenCount++;
        }
        //already found lambda and rparen, decrease count
        if(foundLambda && (t.token_type == RPAREN || t.token_type == RBRACK)) {
            parenCount--;
        }

        //already found lambda and the paren count has gone negative
        //OR
        //already found lambda and end of the line
        //set right location and break, we are done
        if(foundLambda && (parenCount <= 0 ||
                           t.token_type == SEMICOLON ||
                           t.token_type == END_OF_FILE ||
                           t.token_type == ERROR)) {
            right = i;
            break;
        }
    }
    //if right is non negative, add it
    if(right >= 0) {

        //make new token
        Token t;
        t.token_type = RCURLY;
        t.lexeme = "";
        //not a real token, so no real line number
        t.line_no = -1;

        auto position = line.begin() + right;
        //calc position
        line.insert(position, t);
    }
    //if left is non negative, add it
    if(left >= 0) {

        //make new token
        Token t;
        t.token_type = LCURLY;
        t.lexeme = "";
        //not a real token, so no real line number
        t.line_no = -1;

        //calc position
        line.insert(line.begin() + left, t);
    }

    //if we found a lambda, we need to search again
    if(foundLambda) {
        applyAbstraction(line);
    }
}

void Preprocessor::applyApplication(std::vector<Token>& line) {
    line = applyApplicationToTerm(line);
}

//a term that is a sequence of tokens
//apply to abstraction bodies
//apply to parenthized terms
//once the whole thing has been applied to, group the outer most stuff
std::vector<Token> Preprocessor::applyApplicationToTerm(std::vector<Token> term) {


    //apply to abstraction body
    for(int i = 0; i < term.size(); i++) {
        //find the end index of the paren
        int j = identifyAbstractionBody(term, i);
        //if its valid
        if(j >= 0) {
            //i is the dot and j is the rcurly
            int start = i+1;
            int end = j-1;
            //get the abstraction
            std::vector<Token> abs = sliceVector(term, start,end);
            //apply application
            abs = applyApplicationToTerm(abs);
            //replace term
            replaceRange(term, start, end, abs);
        }
    }

    //apply to parenthized expressions
    for(int i = 0; i < term.size(); i++) {
        //find the end index of the paren
        int j = identifyParen(term, i);
        //if its valid
        if(j >= 0) {
            //i is the lparen and j is the rparen
            int start = i+1;
            int end = j-1;
            //get the term
            std::vector<Token> subTerm = sliceVector(term, start,end);
            //apply application
            subTerm = applyApplicationToTerm(subTerm);
            //replace term
            replaceRange(term, start, end, subTerm);
        }
    }

    identifyAndApply(term);

    return term;
}

void Preprocessor::identifyAndApply(std::vector<Token>& term) {
    //now we identify and apply
    bool foundA = false;
    int startA = -1;
    int endA = -1;
    bool foundB = false;
    int startB = -1;
    int endB = -1;
    for(int i = 0; i < term.size(); i++) {
        //temporary
        bool foundT = false;
        int startT = -1;
        int endT = -1;

        //if its an AT followed by an ID, get the start and end
        if(term[i].token_type == AT && i+1 < term.size() && term[i+1].token_type == ID) {
            startT = i;
            endT = i + 1;
            foundT = true;
        }
            //if its an ID not preceded by AT, get the start and end
        else if(term[i].token_type == ID && (i == 0 || (i-1 >= 0 && term[i-1].token_type != AT))) {
            startT = i;
            endT = i;
            foundT = true;
        }
            //if its a kind of lbrace, determine its size
        else if(term[i].token_type == LPAREN ||
                term[i].token_type == LBRACK ||
                term[i].token_type == LCURLY) {
            int j = identifyTerm(term, i);
            startT = i;
            endT = j;
            foundT = true;
        }

        //if we found something, determine where to put it
        if(foundT) {
            //havent found A yet, place here
            if(!foundA) {
                foundA = true;
                startA = startT;
                endA = endT;
                i = endT;
            }
                //found A, havent found B yet, place here
            else if(foundA && !foundB) {
                foundB = true;
                startB = startT;
                endB = endT;
            }
        }

        //if we have found A and B, break
        if(foundA && foundB) break;
    }

    //if we found A and B, place braces at startA and endB
    if(foundA && foundB) {
        //make new token
        Token t;
        t.token_type = RBRACK;
        t.lexeme = "";
        //not a real token, so no real line number
        t.line_no = -1;

        //do right before left so that indexes arent screwed up
        //calc position
        term.insert(term.begin() + endB + 1, t);

        t.token_type = LBRACK;
        //calc position
        term.insert(term.begin() + startA, t);

        // we found at least two terms, need to call again
        identifyAndApply(term);
    }
}

//get a slice of a vector
//start and end are inclusive
std::vector<Token> Preprocessor::sliceVector(std::vector<Token> original, int start, int end) {
    auto startIT = original.begin() + start;
    auto endIT = original.begin() + end;

    //we want end to be inclusive
    //this constructor is [start, end)
    //so we add one it endIT
    std::vector<Token> slice(startIT, endIT+1);
    return slice;
}

//replace a range in vector with another vector
//start and end are inclusive
void Preprocessor::replaceRange(std::vector<Token>& original, int start, int end, std::vector<Token> replacement) {
    auto startIT = original.begin() + start;
    auto endIT = original.begin() + end;

    //now we have three vectors to link up
    std::vector<Token> modified;
    //append front slice
    for(auto it = original.begin(); it != startIT; it++) {
        modified.push_back(*it);
    }
    //append replacement slice
    for(auto it = replacement.begin(); it != replacement.end(); it++) {
        modified.push_back(*it);
    }
    //append back slice
    //endIT is inclusive, so we need to add 1
    for(auto it = endIT + 1; it != original.end(); it++) {
        modified.push_back(*it);
    }

    //modify the origal
    original = modified;
}

int Preprocessor::identifyTerm(std::vector<Token> line, int start) {
    int end = -1;
    if(line[start].token_type == LPAREN) {
        end = identifyParen(line, start);
    }
    else if(line[start].token_type == LBRACK) {
        end = identifyBody(line, start, LBRACK, RBRACK);
    }
    else if(line[start].token_type == LCURLY) {
        end = identifyBody(line, start, LCURLY, RCURLY);
    }
    return end;
}

int Preprocessor::identifyBody(std::vector<Token> line, int start, TokenType left, TokenType right) {
    int end = -1;
    if(line[start].token_type == left) {
        int count = 1;
        //count++ if left, count-- if right, when count is 0 we have a term
        int j = start + 1;
        while(line[j].token_type != SEMICOLON &&
              line[j].token_type != END_OF_FILE &&
              line[j].token_type != ERROR) {

            if(line[j].token_type == left) count++;
            if(line[j].token_type == right) count--;
            if(count == 0) break;
            j++;
        }
        if(line[j].token_type == right) {
            //j now points to closing paren
            end = j;
        }
    }
    return end;
}


//start is the open parent
//the return is the closing curly
int Preprocessor::identifyParen(std::vector<Token> line, int start) {
    return identifyBody(line, start, LPAREN, RPAREN);
}
//start is the dot preceding an abstraction
//the return is the curly closing the abstraction
int Preprocessor::identifyAbstractionBody(std::vector<Token> line, int start) {
    int end = -1;
    if(line[start].token_type == DOT) {

        int count = 1;
        //count++ if lcurly, count-- if rcurly, when count is 0 we have a term
        int j = start + 1;
        while(line[j].token_type != SEMICOLON &&
              line[j].token_type != END_OF_FILE &&
              line[j].token_type != ERROR) {

            if(line[j].token_type == LCURLY) count++;
            if(line[j].token_type == RCURLY) count--;
            if(count == 0) break;
            j++;
        }
        if(line[j].token_type == RCURLY) {
            //j now points to closing curly
            end = j;
        }
    }
    return end;
}

//finds a term and then searches for the term next to it
//once its found two terms, puts it in brackets
/*
void Preprocessor::applyApplication(std::vector<Token>& line, int start, int end) {

    bool foundA = false;
    int startA = -1;
    int endA = -1;
    bool foundB = false;
    int startB = -1;
    int endB = -1;
    for(int i = start; i <= end; i++) {
        //temporary
        bool foundT = false;
        int startT = -1;
        int endT = -1;

        //if its an AT followed by an ID, get the start and end
        if(line[i].token_type == AT && i+1 < line.size() && line[i+1].token_type == ID) {
            startT = i;
            endT = i + 1;
            foundT = true;
        }
        //if its an ID not preceded by AT, get the start and end
        else if(line[i].token_type == ID && (i == 0 || (i-1 >= 0 && line[i-1].token_type != AT))) {
            startT = i;
            endT = i;
            foundT = true;
        }
        //if its a kind of lbrace, determine its size
        else if(line[i].token_type == LPAREN ||
                line[i].token_type == LBRACK ||
                line[i].token_type == LCURLY) {
            identifyTerm(line, i, startT, endT);
            foundT = true;
        }

        //if we found something, determine where to put it
        if(foundT) {
            //havent found A yet, place here
            if(!foundA) {
                foundA = true;
                startA = startT;
                endA = endT;
                i = endT;
            }
            //found A, havent found B yet, place here
            else if(foundA && !foundB) {
                foundB = true;
                startB = startT;
                endB = endT;
            }
        }

        //if we have found A and B, break
        if(foundA && foundB) break;
    }

    //if we found A and B, place braces at startA and endB
    if(foundA && foundB) {
        //make new token
        Token t;
        t.token_type = RBRACK;
        t.lexeme = "";
        //not a real token, so no real line number
        t.line_no = -1;

        //di right before left so that indexes arent screwed up
        //calc position
        line.insert(line.begin() + endB + 1, t);

        t.token_type = LBRACK;
        //calc position
        line.insert(line.begin() + startA, t);
    }

}

//identify the start and end index of a term
//term -> ( x ) | [ x ] | { x }
//x is anything
//start index and end index are first and last of something inclusive
// they include the bracing for CURLY and PAREN, not for BRACK
//i is start of where to begin searching
void Preprocessor::identifyTerm(std::vector<Token> line, int i, int& start, int& end) {
    //if i is lparen, find the matching rparen and set start and end
    if(line[i].token_type == LPAREN) {
        start = i;
        int count = 1;
        //count++ if lparen, count-- if rparen, when count is 0 we have a term
        int j = i + 1;
        while(line[j].token_type != SEMICOLON &&
              line[j].token_type != END_OF_FILE &&
              line[j].token_type != ERROR) {

            if(line[j].token_type == LPAREN) count++;
            if(line[j].token_type == RPAREN) count--;
            if(count == 0) break;
            j++;
        }
        //j now points to closing paren
        end = j;
    }
    //if i is lbrack, find the matching rbrack and set start and end
    else if(line[i].token_type == LBRACK) {
        start = i + 1;
        int count = 1;
        //count++ if lbrack, count-- if rbrack, when count is 0 we have a term
        int j = i + 1;
        while(line[j].token_type != SEMICOLON &&
              line[j].token_type != END_OF_FILE &&
              line[j].token_type != ERROR) {

            if(line[j].token_type == LBRACK) count++;
            if(line[j].token_type == RBRACK) count--;
            if(count == 0) break;
            j++;
        }
        //j now points to closing brack
        end = j - 1;
    }
    //if i is lcurly, find the matching rcurly and set start and end
    else if(line[i].token_type == LCURLY) {
        start = i;
        int count = 1;
        //count++ if lcurly, count-- if rcurly, when count is 0 we have a term
        int j = i + 1;
        while(line[j].token_type != SEMICOLON &&
              line[j].token_type != END_OF_FILE &&
              line[j].token_type != ERROR) {

            if(line[j].token_type == LCURLY) count++;
            if(line[j].token_type == RCURLY) count--;
            if(count == 0) break;
            j++;
        }
        //j now points to closing curly
        end = j;
    }
    //couldnt find anything
    else {
        start = end = -1;
    }
}*/