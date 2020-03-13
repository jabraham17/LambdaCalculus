
#include "preprocessor.h"


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

    applyAbstraction(line);
    applyApplication(line, 0);

    //add the line to the buffer in reverse order
    for(int i = line.size() - 1; i >= 0; i--) {
        tokens.push_back(line[i]);
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
        if(foundLambda && t.token_type == LPAREN) {
            parenCount++;
        }
        //already found lambda and rparen, decrease count
        if(foundLambda && t.token_type == RPAREN) {
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

        //calc position
        line.insert(line.begin() + right, t);
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

//finds a term and then searches for the term next to it
//once its found two terms, puts it in brackets
void Preprocessor::applyApplication(std::vector<Token>& line, int start) {

    bool foundA = false;
    int startA = -1;
    bool foundB = false;
    int endB = -1;
    for(int i = start; i < line.size(); i++) {
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
                i = endT;
            }
            //found A, havent found B yet, place here
            else if(foundA && !foundB) {
                foundB = true;
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

    //if we found A and B, search everything again
    if(foundA && foundB) {
        applyApplication(line, 0);
    }
    //if we found A and not B, search excluding the first of A
    if(foundA && !foundB) {
        applyApplication(line, startA+2);
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
}