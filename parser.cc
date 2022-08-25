//William Owen, CSE 340, Professor Bazzi.
//ASU ID: 1207961816

/*
 * Copyright (C) Rida Bazzi, 2020
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>
#include "parser.h"

using namespace std;

//William Owen, CSE 340, Professor Bazzi.
//ASU ID: 1207961816

//globals
vector<myTokens> ntokens;
vector<REG_Node> set_of_nodes;
int NodeLabels;


void Parser::syntax_error()
{
    cout << "SYNTAX ERROR\n";
    exit(1);
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// Parsing

// This function is simply to illustrate the GetToken() function
// you will not need it for your project and you can delete it
// the function also illustrates the use of peek()
void Parser::ConsumeAllInput()
{
    Token token;
    int i = 1;



    token = lexer.peek(i);
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        i = i+1;

        token = lexer.peek(i);
        token.Print();
    }


    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {

        token = lexer.GetToken();
        token.Print();
    }

}

int main()
{

    Parser parser;

    string s;

	s = parser.parse_input();

	parser.checkforEpsilon();


	parser.lexAnalysis(s);


	
}

//This function begins parsing the input.
string Parser::parse_input()
{
    Token t;

    string s;

    parse_tokens_section();

    t = expect(INPUT_TEXT);

    s = t.lexeme;

    t = expect(END_OF_FILE);

    return s;
}

//Parses the token section of the input.
void Parser::parse_tokens_section()
{
    Token t;

    parse_token_list();

    t = expect(HASH);
}

//Parses the list of tokens
void Parser::parse_token_list()
{
    Token t;



    parse_token();


    t = lexer.peek(1);
    if(t.token_type == COMMA)
    {
        t = expect(COMMA);
        parse_token_list();
    }
    else if(t.token_type == HASH)
    {
        return;
    }
}

//Parses a token from the input
void Parser::parse_token()
{
    Token t;
    string _id;
    REG* graph = new REG;
    myTokens* newToken = new myTokens;
    t = expect(ID);

    _id = t.lexeme;

    NodeLabels = 0;
    graph = parse_expr();

    newToken->my_ID = _id;
    newToken->my_REG = *graph;

    ntokens.push_back(*newToken);
}

//Parses the expression part of the token.
//Also creates the REG's for each token.
REG* Parser::parse_expr()
{
    Token t;
    REG* R1;
    REG* R2;
    REG* R3; // new
    t = lexer.peek(1);
    if (t.token_type == LPAREN)
    {
        t = expect(LPAREN);
        R1 = parse_expr();
        t = expect(RPAREN);
        t = lexer.peek(1);
        if(t.token_type == DOT)
        {
            t = expect(DOT);
            t = expect(LPAREN);
            R2 = parse_expr();
            t = expect(RPAREN);
            R3 = dotREG(*R1,*R2);
            return R3;
        }
        else if(t.token_type == OR)
        {
            t = expect(OR);
            t = expect(LPAREN);
            R2 = parse_expr();
            t = expect(RPAREN);
            R3 = orReg(*R1,*R2);
            return R3;
        }
        else if(t.token_type == STAR)
        {
            t = expect(STAR);
            R1 = starREG(*R1);
            return R1;
        }
        else
        {
            syntax_error();
        }

    }
    else if(t.token_type == CHAR)
    {
        t = expect(CHAR);
        R1 = createREG(t.lexeme);
    }
    else if(t.token_type == UNDERSCORE)
    {
        t = expect(UNDERSCORE);
        R1 = createREG("_");
    }
    else
    {
        syntax_error();
    }
    return R1;
}

//This function performs the Lexical Analysis of the INPUT_TEXT
//It first removes the quotation marks
//Then splits the string into multiple strings if there is spaces in the input.
//This function handles all the output after checking for epsilons and parsing syntax errors.
void Parser::lexAnalysis(string s)
{

    vector<string> inputt;
    int start;
    int end,finder;
    int i,index;
    tokenMatches imatch;

    string del2;
    del2 = '"';
    int first,second;
    first = s.find(del2);

    s = s.substr(first+1,s.length());

    second = s.find(del2);

    s = s.substr(0,second);


    string del;
    del = " ";
    start = 0;
    end = s.find(del);
    if(end == -1)
    {
        inputt.push_back(s);
    }
    else
    {
        while (end != -1)
        {
            inputt.push_back(s.substr(start, end - start));
            start = end + del.size();
            end = s.find(del, start);
        }
        inputt.push_back(s.substr(start, end - start));
    }

    index = 0;


    for(i = 0 ; i < inputt.size(); i++)
    {

        index = 0;
        while(index < inputt[i].length())
        {

            imatch = my_getToken(inputt[i], index);
            if(imatch.match == "")
            {
                cout<<"ERROR"<<endl;
                exit(0);
            }

            cout << imatch.tokenID << " , " << "\"" << imatch.match << "\"" << endl;
            index = index + imatch.match.length();


        }
    }

}

//New get_Token function for the INPUT_TEXT.
//Correctly implements breaking ties and longest matching prefix rule.
//calls the match function.
tokenMatches Parser::my_getToken(string s, int index)
{
    vector<tokenMatches> matchedTokens;
    tokenMatches max;
    int i,w;
    for(i = 0; i<ntokens.size();i++)
    {
        matchedTokens.push_back(match(ntokens[i],s,index));
    }
    max = matchedTokens[0];
    for(w = 1; w<matchedTokens.size();w++)
    {
        if(max.match.length() == matchedTokens[w].match.length())
        {
            //do nothing
        }
        else if(max.match.length() < matchedTokens[w].match.length())
        {
            max = matchedTokens[w];
        }
    }
    return max;
}

//This function matches part of a string to a given token
//If a token doesn't exist it tags the return value to output an error.
tokenMatches Parser::match(myTokens t, string s, int p)
{
    vector<REG_Node> setOfNodes1;
    string letter,matchingString,matchingTokenID;
    int nMatching,currIndex,booly;
    tokenMatches m;
    REG r;
    r = t.my_REG;

    nMatching = -1;


    currIndex = p;
    set_of_nodes.clear();
    checkLabels(r.start,"_");
    setOfNodes1 = set_of_nodes;
    booly = checkSet2(r.start->label,setOfNodes1);
    if(booly == 0)
    {
        setOfNodes1.push_back(*r.start);
    }

    while(currIndex < s.length() && !setOfNodes1.empty())
    {
        letter = s[currIndex];
        setOfNodes1 = match_one_char(setOfNodes1,letter);
        booly = checkSet2(r.accept->label,setOfNodes1);
        if(booly == 1)
        {
            nMatching= currIndex;
        }

        currIndex++;

    }


    if(nMatching == -1)
    {
        matchingString = "";
    }
    else
    {
         matchingString = s.substr(p, (nMatching-p)+1);
    }
    matchingTokenID = t.my_ID;

    m.match = matchingString;
    m.tokenID = matchingTokenID;

    return m;

}

//This function returns all the possible nodes that can be reached by consuming a character of the string.
vector<REG_Node> Parser:: match_one_char(vector<REG_Node> S, string c)
{
    vector<REG_Node> son2;
    vector<REG_Node> son3;

    int i,ix,iy,booly,changed;
    son2.clear();
    for(i = 0; i < S.size(); i++)
    {
        if(S[i].firstLabel == c)
        {
            booly = checkSet2(S[i].firstN->label, son2);
            if(booly == 0)
            {
                son2.push_back(*S[i].firstN);
            }
        }
        if(S[i].secondLabel == c)
        {
            booly = checkSet2(S[i].secondN->label, son2);
            if(booly == 0)
            {
                son2.push_back(*S[i].secondN);
            }
        }
    }

    if(son2.empty())
    {
        return son2;
    }
    changed = 1;
    while(changed == 1)
    {
        changed = 0;
        for(ix = 0; ix < son2.size(); ix++)
        {
            booly = checkSet2(son2[ix].label,son3);
            if(booly == 0)
            {
                son3.push_back(son2[ix]);
            }


            if(son2[ix].firstLabel == "_")
            {
                booly = checkSet2(son2[ix].firstN->label, son3);
                if(booly == 0)
                {
                    son3.push_back(*son2[ix].firstN);
                }
            }
            if(son2[ix].secondLabel == "_")
            {
                booly = checkSet2(son2[ix].secondN->label,son3);
                if(booly == 0)
                {
                    son3.push_back(*son2[ix].secondN);
                }
            }
        }

        int wx;
        int same;
        same = 1; // 1 = same , 0 = notsame

        if(son2.size() == son3.size())
        {
            for(wx = 0; wx < son2.size(); wx++)
            {
                if(son2[wx].label != son3[wx].label)
                {
                    same = 0;
                }
            }
        }
        else
        {
            same = 0;
        }

        if(same == 0)
        {
            changed = 1;
            son2 = son3;
            son3.clear();
        }
    }

    return son2;

}
//This function is similar to checkSet but is passed a vector of REG_Nodes instead of comparing to the global vector.
//This function returns a 0 if the vector doesn't contain a node with the given label.
//This function returns a 1 if the vector DOES contain a node with the given label.
int Parser::checkSet2(int label, vector<REG_Node> set)
{
    int booly, i;
    booly = 0; // doesn't contain node
    for (i = 0; i < set.size(); i++)
    {
        if(set[i].label == label)
        {
            booly = 1; //contains node;
        }
    }
    return booly;
}

//This function checks if a given token can accept no input or epsilon.
void Parser::checkforEpsilon()
{
    int i,w,z, hasEpsilon;
    vector<string> tokensWithEpsilon;

    hasEpsilon = 0;
    for(i=0;i<ntokens.size();i++)
    {
        set_of_nodes.clear();
        checkLabels(ntokens[i].my_REG.start,"_");
        for(w = 0; w < set_of_nodes.size(); w++)
        {
            if(ntokens[i].my_REG.accept->label == set_of_nodes[w].label)
            {
                tokensWithEpsilon.push_back(ntokens[i].my_ID);
                hasEpsilon = 1;
            }
        }
    }
    if(hasEpsilon == 1)
    {
        cout<<"EPSILON IS NOOOOOT A TOKEN !!!";
        for(z = 0; z<tokensWithEpsilon.size(); z++)
        {
            cout<<" "<<tokensWithEpsilon[z];
        }
        cout<<endl;
        exit(0);
    }
}

//This function is similar to checkset2 except it compares with the global vector
//This function returns a 0 if the vector doesn't contain a node with the given label.
//This function returns a 1 if the vector DOES contain a node with the given label.
int Parser::checkSet(int label)
{
    int booly,i;

    booly = 0; //doesn't contain node
    for(i = 0; i < set_of_nodes.size(); i++)
    {
        if(set_of_nodes[i].label == label)
        {
            booly = 1; //contains node
        }
    }
    return booly;
}

//This function traverses a REG for a given path or label s
void Parser::checkLabels(REG_Node* x,string s)
{
    int booly;
    if(x->firstLabel == s)
    {
        booly = checkSet(x->firstN->label);
        if(booly == 0)
        {
            set_of_nodes.push_back(*x->firstN);
        }
        if(x->firstN->firstN != nullptr)
        {
            if(x->firstN != nullptr)
            {
                checkLabels(x->firstN, s);
            }
        }
    }
    if(x->secondLabel == s)
    {
        booly = checkSet(x->secondN->label);
        if(booly == 0)
        {
            set_of_nodes.push_back(*x->secondN);

            if (x->secondN->firstN != nullptr)
            {
                if (x->secondN != nullptr)
                {
                    checkLabels(x->secondN, s);
                }
            }
        }
    }
}

//This function creates a basic 2 node REG.
REG* Parser::createREG(string s)
{
    REG* n = new REG;
    REG_Node* node1 = new REG_Node;
    REG_Node* node2 = new REG_Node;

    node1->firstN = node2;
    node1->secondN = nullptr;
    node1->firstLabel = s;

    node1->secondLabel = "";
    node1->label = NodeLabels;


    NodeLabels++;

    node2->firstN = nullptr;
    node2->firstLabel = "";
    node2->secondN = nullptr;
    node2->secondLabel = "";
    node2->label=NodeLabels;

    NodeLabels++;

    n->start = node1;
    n->accept = node2;

    return n;
}

//This function takes 2 basic REG's and concatenates them together.
REG* Parser::dotREG(REG r1, REG r2)
{
    REG* r3 = new REG;

    //accept state of R1 has epsilon
    //transition to start state of R2
    r1.accept->firstN = r2.start;
    r1.accept->firstLabel = "_";


    //New REG R3 start state is the start
    //state of R1
    r3->start = r1.start;


    //New REG R3 accept state is the accept
    //state of R2
    r3->accept = r2.accept;

    return r3;
}

//This function takes a singular REG and applies the kleene star to it.
REG* Parser::starREG(REG r)
{
    REG* nr = new REG;

    //r accept state second neighbor has
    //epsilon transition to r start state
    r.accept->secondN = r.start;
    r.accept->secondLabel = "_";

    //new start node has epsilon transition
    //TO original start node
    REG_Node* newStart = new REG_Node;
    newStart->firstN = r.start;
    newStart->firstLabel = "_";
    newStart->label = NodeLabels;
    NodeLabels++;

    //new accept node has epsilon transition
    //FROM original accept node
    REG_Node* newAccept = new REG_Node;
    newAccept->firstLabel = "";
    newAccept->firstN = nullptr;
    newAccept->secondN = nullptr;
    newAccept->secondLabel = "";
    newAccept->label = NodeLabels;
    NodeLabels++;

    r.accept->firstN = newAccept;
    r.accept->firstLabel = "_";

    //New Start node second neighbor
    //has epsilon transition
    //to new Accept node
    newStart->secondN = newAccept;
    newStart->secondLabel = "_";

    //Point to new Start and Accept nodes
    nr->start = newStart;
    nr->accept = newAccept;

    return nr;
}

//This function takes 2 REG's and "OR"'s them.
REG* Parser::orReg(REG r1, REG r2)
{
    REG* r3 = new REG;

    //new start state has epsilon transitions
    //to the start states of r1 and r2
    REG_Node* newStart = new REG_Node;
    newStart->firstN = r1.start;
    newStart->firstLabel = "_";
    newStart->secondN = r2.start;
    newStart->secondLabel = "_";
    newStart->label = NodeLabels;
    NodeLabels++;

    //new accept state has epsilon transition
    //from the accept states of r1 and r2
    REG_Node* newAccept = new REG_Node;
    newAccept->firstLabel = "";
    newAccept->firstN = nullptr;
    newAccept->secondN = nullptr;
    newAccept->secondLabel = "";
    newAccept->label = NodeLabels;
    NodeLabels++;

    r1.accept->firstN = newAccept;
    r1.accept->firstLabel = "_";
    r2.accept->firstN = newAccept;
    r2.accept->firstLabel = "_";

    //r3 point to new Start and Accept states
    r3->start = newStart;
    r3->accept = newAccept;
    return r3;
}