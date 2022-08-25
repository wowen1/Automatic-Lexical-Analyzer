//William Owen, CSE 340, Professor Bazzi.
//ASU ID: 1207961816

/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"
#include <vector>

//William Owen, CSE 340, Professor Bazzi.
//ASU ID: 1207961816

using namespace std;

struct REG_Node
{
    int label;

    REG_Node* firstN;
    string firstLabel;

    REG_Node* secondN;
    string secondLabel;
};
struct REG
{
    REG_Node* start;
    REG_Node* accept;
};
struct myTokens
{
    string my_ID;
    REG my_REG;
};
struct tokenMatches
{
    string tokenID;
    string match;
};

class Parser {
  public:
    void ConsumeAllInput();
    string parse_input();
    void parse_tokens_section();
    void parse_token_list();
    void parse_token();
    REG* parse_expr();
    REG* createREG(string s);
    REG* dotREG(REG r1, REG r2);
    REG* starREG(REG r);
    REG* orReg(REG r1, REG r2);
    void checkLabels(REG_Node* x,string s);
    int checkSet(int label);
    void checkforEpsilon();
    tokenMatches match(myTokens t, string s, int p);
    vector<REG_Node> match_one_char(vector<REG_Node> S, string c);
    int checkSet2(int label, vector<REG_Node> set);
    tokenMatches my_getToken(string s, int index);
    void lexAnalysis(string s);
  private:
    LexicalAnalyzer lexer;
    void syntax_error();
    Token expect(TokenType expected_type);
};

#endif

