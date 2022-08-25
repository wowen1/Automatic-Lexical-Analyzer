#include <iostream>
#include <string>

using namespace std;

struct REG_Node
{
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
int main()
{

    string r1 = "((a)*).((b)*)";
    string t1 = "token1";

    cout << "Hello World" << endl;

    return 0;
}
REG tt(string t1, string r1)
{
    REG r;


    return r;
}
REG createREG(string s)//string would be token
{
    REG n = new REG;
    REG_Node node1 = new REG_Node;
    REG_Node node2 = new REG_Node;

    *node1.firstN = node2;
    node1.secondN = nullptr;
    node1.firstLabel = s;
    node1.secondLabel = "";

    node2.firstN = nullptr;
    node2.firstLabel = "_";
    node2.secondN = nullptr;
    node2.secondLabel = "";
    
    *n.start = node1;
    *n.accept = node2;

    return n;
}

REG dotREG(REG r1, REG r2)
{
    REG r3 = new REG;
    
    //accept state of R1 has epsilon 
    //transition to start state of R2
    r1.accept->firstN = r2.start;
    r1.accept->firstLabel = "_";


    //New REG R3 start state is the start
    //state of R1
    r3.start = r1.start;


    //New REG R3 accept state is the accept
    //state of R2
    r3.accept = r2.accept;

    return r3;
}

REG starREG(REG r)
{
    REG nr = new REG;

    //r accept state second neighbor has
    //epsilon transition to r start state
    r.accept->secondN = r.start;
    r.accept->secondLabel = "_";

    //new start node has epsilon transition
    //TO original start node
    REG_Node newStart = new REG_Node;
    newStart.firstN = r.start;
    newStart.firstLabel = "_";

    //new accept node has epsilon transition
    //FROM original accept node
    REG_Node newAccept = new REG_Node;
    newAccept.firstLabel = "_";
    newAccept.firstN = nullptr;
    newAccept.secondN = nullptr;
    newAccept.secondLabel = "";
    *r.accept->firstN = newAccept;

    //New Start node second neighbor 
    //has epsilon transition
    //to new Accept node
    *newStart.secondN = newAccept;
    newStart.secondLabel = "_";

    //Point to new Start and Accept nodes
    *nr.start = newStart;
    *nr.accept = newAccept;

    return nr;
}

REG orReg(REG r1, REG r2)
{
    REG r3 = new REG;

    //new start state has epsilon transitions
    //to the start states of r1 and r2
    REG_Node newStart = new REG_Node;
    newStart.firstN = r1.start;
    newStart.firstLabel = "_";
    newStart.secondN = r2.start;
    newStart.secondLabel = "_";

    //new accept state has epsilon transition
    //from the accept states of r1 and r2
    REG_Node newAccept = new REG_Node;
    newAccept.firstLabel = "_";
    newAccept.firstN = nullptr;
    newAccept.secondN = nullptr;
    newAccept.secondLabel = "";
    *r1.accept->firstN = newAccept;
    r1.accept->firstLabel = "_";
    *r2.accept->firstN = newAccept;
    r2.accept->firstLabel = "_";

    //r3 point to new Start and Accept states
    *r3.start = newStart;
    *r3.accept = newAccept;
    return r3;
}

/*
REG parse_expr()
{
    Token t;
    REG R1,R2,R3; //new
    t = lexer.peek(1);
    if (t.token_type == LPAREN)
    {
        t = expect(LPAREN);
        R2 = parse_expr(); //new
        t = expect(RPAREN);
        t = lexer.peek(1);
        if(t.token_type == DOT)
        {
            t = expect(DOT);
            t = expect(LPAREN);
            R3 = parse_expr();
            t = expect(RPAREN);
        }
        else if(t.token_type == OR)
        {
            t = expect(OR);
            t = expect(LPAREN);
            parse_expr();
            t = expect(RPAREN);
        }
        else if(t.token_type == STAR)
        {
            t = expect(STAR);
        }
        else
        {
            syntax_error();
        }

    }
    else if(t.token_type == CHAR)
    {
        t = expect(CHAR);
        R1 = createREG(t); //new
    }
    else if(t.token_type == UNDERSCORE)
    {
        t = expect(UNDERSCORE);
        R1 = createREG(t); //new
    }
    else
    {
        syntax_error();
    }

    return R1;
}*/