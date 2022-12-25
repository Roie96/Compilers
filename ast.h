/**************************************
 *
 * University of Haifa
 *
 * Theory of compilation
 *
 * P machine compiler - code generation project
 *
 * Yosi Ben Asher
 * Email: yosibenasher@gmail.com
 *
 * Mariah Akree
 * Email: mariah.uni.haifa@gmail.com
 *
 **************************************/
 /*   PROGRAM = 258,
      BBEGIN = 259,
      END = 260,
      DECLARE = 261,
      PROCEDURE = 262,
      FUNCTION = 263,
      LABEL = 264,
      INTEGER = 265,
      REAL = 266,
      RECORD = 267,
      BOOLEAN = 268,
      ARRAY = 269,
      OF = 270,
      ASSIGN = 271,
      LC = 272,
      RC = 273,
      IF = 274,
      THEN = 275,
      ELSE = 276,
      WHILE = 277,
      REPEAT = 278,
      FI = 279,
      DO = 280,
      OD = 281,
      READ = 282,
      WRITE = 283,
      TRUE = 284,
      FALSE = 285,
      ADD = 286,
      MIN = 287,
      MUL = 288,
      DIV = 289,
      GOTO = 290,
      MOD = 291,
      LES = 292,
      LEQ = 293,
      EQU = 294,
      NEQ = 295,
      GRE = 296,
      GEQ = 297,
      AND = 298,
      OR = 299,
      NOT = 300,
      CASE = 301,
      FOR = 302,
      FIN = 303,
      IDENTICAL = 304,
      FROM = 305,
      BY = 306,
      TO = 307,
      NEW = 308,
      INTCONST = 309,
      IDE = 310,
      REALCONST = 311,
      STRING = 312,
      DUMMY = 313*/

#ifndef AST_H 
#define AST_H

#include <iostream>
#include <assert.h>
#include <string>
#include <unordered_map>
#include <stdio.h>
#include <typeinfo>
#include <iomanip>
#include <vector>

//added
using namespace std;
/**
 * classes 
 */
// base class for tree nodes

class Object {
public:
  virtual void print (ostream& os) = 0;
  virtual void pcodegen(ostream& os) = 0;
  virtual Object * clone () const {return NULL;}
  virtual bool codel(ostream& os, int op){return false;}
  virtual void coder(ostream& os){}
  virtual void switch_index(int x){}
  virtual ~Object () {}
  virtual int GetAddress(){return -1;};
  virtual int GetSize() const{return size;};
  virtual void SetSize(int _size){size = _size;};
  virtual void GetVer(string& temp){};
  virtual int GetOverollSize() const{return this->size;};
  virtual int GetSubpart() const{return this->subpart;};
  virtual void SetSubpart(int subpart_){this->subpart = subpart_;};

private:
  int size = 0;
  int subpart = 0;
};

//added
class Variable{
private:
  int size;
  int address;
  Object* type;
  string id;
  int rAddress;
  int subpart;
  vector<int> d;


public:
  Variable(){}
  Variable(int _size, int _address, Object* _type, string _id, int _subpart, vector<int>& _d){        
    this->size = _size;
    this->address = _address;
    this->type = _type;
    this->id = _id;
    this->subpart = _subpart;
    this->d = _d;
    this->rAddress = -1;
  }
  int GetAddress(){return this->address;}
  int GetSize() const {return this->size;}
  Object* GetType() const{return type;}
  int GetSubpart() const{return subpart;}
  int GetrAddress() const{return rAddress;} //also use as a flag
  void SetrAddress(int a) {this->rAddress = a;}
  vector<int>& GetD() {return this->d;}
  virtual ~Variable() {}
};

static int loopCounter = 1;
static int caseNum = 1;
static int switchNum = 1;
static int elseCounter = 1;
static int indexDim;

class SymbolTable{
public:
int addressCounter = 5;
string currentVarName;
unordered_map<string, Variable> symT;
};

extern SymbolTable* table;


class Type : public Object {
    public:
    vector<int> d;
    virtual void Build_Dim_Vec(vector<int>& d) {
      d.push_back(1);
    }
};

class Expr : public Object {
public :
  // Unary operations
  Expr (int op, Object * atom) : op_(op), atom_(atom), left_(NULL), right_(NULL), unary_(true) {}
  // Binary operations
  Expr (int op, Object * left, Object * right) : op_(op), left_(left), right_(right), atom_(NULL), unary_(false) {}

  Expr(const Expr & exp){
    unary_= exp.unary_; 
    op_ = exp.op_;
    left_ = exp.left_->clone();
    right_ = exp.right_->clone();
    atom_ = exp.atom_->clone();
  }

  virtual ~Expr(){
    if (left_) delete left_;
    if (right_) delete right_;
    if (atom_) delete atom_;
  }

  void print (ostream& os) {
    os<<"Node name : Expr"<<endl;
    assert(op_);
    if (unary_){
      os<<"Unary op is : "<<op_;
      assert(atom_);
      atom_->print(os);
    }else{
      os<<"Binary op is : "<<op_;
      assert(left_ && right_);
      left_->print(os);
      right_->print(os);
    }
  }
  void pcodegen(ostream& os) {
      assert(op_);
      if (unary_) { 
        assert(atom_);
        atom_->codel(os,0);
        atom_->pcodegen(os); 
        atom_->coder(os);
        if (op_ == 287)
          os<<"neg"<<endl;
        else if(op_ == 300) 
          os << "not" << endl;    
      }
      else {         
        assert(left_ && right_);
        right_->codel(os, 0);
        right_->pcodegen(os);
        right_->coder(os);

        bool flag = left_->codel(os, op_);
        left_->pcodegen(os);
        left_->coder(os);
        if(!flag){
            switch (op_)
            {
            case 286:
              os << "add" << endl;
              break;
            case 287:
              os << "sub" << endl;
              break;
             case 288:
              os << "mul" << endl;
              break;
            case 289:
              os << "div" << endl;
              break;
             case 292:
              os << "les" << endl;
              break;
            case 293:
              os << "leq" << endl;
              break;
             case 294:
              os << "equ" << endl;
              break;
            case 295:
              os << "neq" << endl;
              break;
               case 296:
              os << "grt" << endl;
              break;
            case 297:
              os << "geq" << endl;
              break;
             case 298:
              os << "and" << endl;
              break;
            case 299:
              os << "or" << endl;
              break;
            }
         }
      }
  }


  virtual Object * clone () const { return new Expr(*this);}
  
private:
  bool unary_;
  int op_;
  Object * left_;
  Object * right_;
  Object * atom_;
};

class ExprList : public Object {
public :
  ExprList (Object * expr) : expr_(expr),expr_list_(NULL){assert(expr_);}
  ExprList (Object * expr, Object * expr_list) : expr_(expr),expr_list_(expr_list) {assert(expr_ && expr_list_);}

  ExprList(const ExprList& exp){
    expr_ = exp.expr_->clone();
    expr_list_ = exp.expr_list_->clone();
  }

  virtual ~ExprList(){
    if (expr_) delete expr_;
    if (expr_list_) delete expr_list_;
  }

  void print (ostream& os) {
		os<<"Node name : ExprList";
		assert( expr_);
    expr_->print(os);
    if (expr_list_){
      expr_list_->print(os);
    }
  }
  void pcodegen(ostream& os) {
      assert(expr_);
      expr_->pcodegen(os);
      if (expr_list_) {
          expr_list_->pcodegen(os);
      }
  }
  virtual Object * clone () const { return new ExprList(*this);}
  
private:
  Object * expr_;
	Object * expr_list_;
};

class Dim : public Object {
public:
  Dim (Object * exp) : exp_(exp), dim_(NULL) {assert(exp_);}
  Dim (Object * exp, Object * dim) : exp_(exp),dim_(dim) {assert(exp_ && dim_);}

  Dim(const Dim& d){
    exp_ = d.exp_->clone();
    dim_ = d.dim_->clone();
  }

  virtual ~Dim(){
    if (exp_) delete exp_;
    if (dim_) delete dim_;
  }
  
  void print (ostream& os) {
    os<<"Node name : Dim"<<endl;
    assert(exp_);
    exp_->print(os);
    if (dim_){
      dim_->print(os);
    }
  }

  void GetVer(string& ver) {
    this->ver_ = &table->symT[ver];
    indexDim = 1;
  }

  void SetVer(Variable ver) {
    this->ver_ = &ver;
  }

  void pcodegen(ostream& os) {
     assert(exp_);
      os<<"ldc ";
      exp_->pcodegen(os);
      int d = ver_->GetD().size();
      for (int i = indexDim; i < d; i++){
        ixa *= (ver_->GetD())[i];
      }
      os<<"ixa "<< ixa << endl;
      indexDim++;
      if(indexDim == d)
        os<<"dec "<< ver_->GetSubpart()<<endl;
      else if(indexDim > d){
        string v_name;
        ((Type*)(ver_->GetType()))->GetVer(v_name);
        Variable v = table->symT[v_name];
        os<<"dec "<<v.GetSubpart()*v.GetSize()<<endl;
      }
      if (dim_) {
        ((Dim*)(dim_))->SetVer(temp);
        dim_->pcodegen(os);
      }
  }
  virtual Object * clone () const { return new Dim(*this);}

private:
//למה לתת לאחתל את וור במשתנה טמפ?
  Object * exp_;
  Object * dim_;
  Variable * ver_;
  int ixa = 1;
};

class Atom : public Object {
};

class IntConst : public Atom {
public:
  IntConst(const int i) : i_(i) {}
  IntConst(const IntConst& in) : i_(in.i_) {}
  bool codel(ostream& os, int op){
    if(op == 286){
      os<<"inc ";
      return true; 
    }
    if (op == 287) {
        os<<"dec ";
      return true; 
    }
    os<<"ldc ";
    return false; 
  }
  
  void print (ostream& os) {
    os<<"Node name : IntConst. Value is :"<<i_<<endl;
  }

  void pcodegen(ostream& os) {
    os<<i_<<endl;  
  }
  virtual Object * clone () const { return new IntConst(*this);}

private:
  const int i_;
};

class RealConst : public Atom {
public:
  RealConst(const double r) : r_(r) {}
  RealConst(const RealConst& in) : r_(in.r_) {} 
   bool codel(ostream& os, int op){
    if(op == 286){
      os<<"inc ";
      return true; 
    }
    if (op == 287) {
        os<<"dec ";
      return true; 
    }
    os<<"ldc ";
    return false; 
  } 

  void print (ostream& os) {
    os<<"Node name : RealConst. Value is :"<<r_<<endl;
  }
  void pcodegen(ostream& os) {
    os << fixed << std::setprecision(1) << r_ << endl;
  }
  virtual Object * clone () const { return new RealConst(*this);}

private:
  const double r_;
};

class True : public Atom {
public:
  void print (ostream& os) {
    os<<"Node name : trueConst. Value is true"<<endl;
  }
  void pcodegen(ostream& os) {
    os<<"ldc 1"<<endl;
  }
  virtual Object * clone () const { return new True();}

};

class False : public Atom {
public :
  void print (ostream& os) {
    os<<"Node name : trueConst. Value is false"<<endl;
  }
  void pcodegen(ostream& os) {
    os<<"ldc 0"<<endl;

  }
  virtual Object * clone () const { return new False();}
};

class Var : public Atom {
};

class ArrayRef : public Var {
public :
  ArrayRef (Object * var, Object * dim) : var_(var),dim_(dim) {assert(var_ && dim_);}
  ArrayRef(const ArrayRef& arr){
    var_ = arr.var_->clone();
    dim_ = arr.dim_->clone();
  }

  virtual ~ArrayRef(){
    if (var_) delete var_;
    if (dim_) delete dim_;
  }

  void print (ostream& os) {
    os<<"Node name : ArrayRef"<<endl;
    assert(var_ && dim_);
    var_->print(os);
    dim_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(var_ && dim_);
      var_->pcodegen(os);
      var_->GetVer(name);
      dim_->GetVer(name);
      dim_->pcodegen(os);
  }
  virtual Object * clone () const { return new ArrayRef(*this);}


//when we init name?
private:
  Object * var_;
  Object * dim_;
  string name;
};

class RecordRef : public Var {
public :
  RecordRef (Object * varExt, Object * varIn) : varExt_(varExt),varIn_(varIn) {assert(varExt_ && varIn_);}
  RecordRef(const RecordRef& rec){
    varExt_ = rec.varExt_->clone();
    varIn_ = rec.varIn_->clone();
  }
  
  virtual ~RecordRef(){
    if (varExt_) delete varExt_;
    if (varIn_) delete varIn_;
  }

  void print (ostream& os) {
    os<<"Node name : RecordRef"<<endl;
    assert(varExt_ && varIn_);
    varExt_->print(os);
    varIn_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(varExt_ && varIn_);
      varExt_->pcodegen(os);
      varIn_->pcodegen(os);
  }
  virtual Object * clone () const { return new RecordRef(*this);}

  int GetOverollSize() const{
    return varIn_->GetOverollSize();
  }


private:
  Object * varExt_;
  Object * varIn_;
};

class AddressRef : public Var {
public :
  AddressRef (Object * var) : var_(var) {assert(var_);}
  AddressRef(const AddressRef& addre){
    var_ = addre.var_->clone();
  }

  virtual ~AddressRef(){
    if (var_) delete var_;
  }

  void print (ostream& os) {
    os<<"Node name : AddressRef"<<endl;
    assert(var_);
    var_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(var_);
      var_->pcodegen(os);
  }
  virtual Object * clone () { return new AddressRef(*this);}

private:
  Object * var_;
};

class Statement : public Object {
};

class NewStatement : public Statement {
public :
  NewStatement (Object * var) : var_(var) {assert(var_);}
  NewStatement(const NewStatement& ns){
    var_ = ns.var_->clone();
  }

  virtual ~NewStatement(){
    if (var_) delete var_;
  }

  void print (ostream& os) {
		os<<"Node name : NewStatement";
		assert(var_);
    var_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(var_);
      var_->pcodegen(os);
      os<<"ldc "<< var_->GetOverollSize()<<endl;
      os << "new" <<endl;
  }
  virtual Object * clone () { return new NewStatement(*this);}
  
private:
  Object * var_;
};

class WriteStrStatement : public Statement {
public :
  WriteStrStatement (const char * str) { 
		str_ = new string(str); 
	}
  
  WriteStrStatement(const WriteStrStatement& ns){
    str_ = new string(*ns.str_); 
  }

  virtual ~WriteStrStatement () {
		if (str_) delete str_;
	}
  void print (ostream& os) {
		os<<"Node name : WriteStrStatement";
		assert(str_);
    os<<"Str statement is: "<<str_<<endl;
  }
  void pcodegen(ostream& os) {
      assert(str_);

  }
  virtual Object * clone () { return new WriteStrStatement(*this);}
  
private:
  string * str_;
};

class WriteVarStatement : public Statement {
public :
  WriteVarStatement (Object * exp) : exp_(exp) {assert(exp_);}

  WriteVarStatement(const WriteVarStatement& ex){
    exp_ = ex.clone();
  }

  virtual ~WriteVarStatement(){
    if (exp_) delete exp_;
  }

  void print (ostream& os) {
		os<<"Node name : WriteVarStatement";
		assert(exp_);
    exp_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(exp_);
      exp_->codel(os, 283);     
      exp_->pcodegen(os);
      exp_->coder(os);
      os << "print" << endl;
  }
  virtual Object * clone () const { return new WriteVarStatement(*this);}
  
private:
  Object * exp_;
};

class ProcedureStatement : public Statement {
public :
  ProcedureStatement (const char * str) { 
		str_ = new string(str); 
	}

  ProcedureStatement (Object * expr_list, const char * str) : expr_list_(expr_list) {
    assert(expr_list_);
    str_ = new string(str);
  }

  ProcedureStatement(const ProcedureStatement& ps){
    expr_list_ = ps.expr_list_->clone();
    str_ = new string(*ps.str_);
  }

   virtual ~ProcedureStatement () {
		if (str_) delete str_;
    if (expr_list_) delete expr_list_;
	} 

  void print (ostream& os) {
    os<<"Node name : ProcedureStatement. Proc name : "<<str_<<endl;
    if (expr_list_ ){
      expr_list_->print(os);
    }
  }
  void pcodegen(ostream& os) {
      if (expr_list_) {
          expr_list_->pcodegen(os);
      }
  }
  virtual Object * clone () const { return new ProcedureStatement(*this);}
  
private:
	Object * expr_list_;
  string * str_;
};

class Case : public Object {
public :
  Case (Object * stat_list, int val) : leafChild_ (NULL), stat_list_(stat_list) {
		// note the special treatment in miny.y for this case (makenode)
		leafChild_ = new IntConst(val);
		assert(stat_list_);
    this->val_=val;
	}

  Case(const Case& c){
    stat_list_ = c.stat_list_->clone();
    leafChild_ = c.leafChild_->clone();
  }

  void switch_index(int x){
    switchI = x;
  }
  int getVal(){
    return val_;
  }


  virtual ~Case () {
    if (stat_list_) delete stat_list_;
    if (leafChild_) delete leafChild_;
	} 
  
  void print (ostream& os) {
		os<<"Node name : Case";
		assert(stat_list_);
	  stat_list_->print(os);
  }
  void pcodegen(ostream& os) {
    os << "case_" << val_ << "_" << switchI << ":" << endl;
    assert(stat_list_);

    stat_list_->pcodegen(os);
    os << "ujp end_switch_" << switchI << endl;
  }
  virtual Object * clone () const { return new Case(*this);}

private:
  Object * stat_list_;
	Object * leafChild_;
  int val_;
  int switchI;
};


class CaseList : public Object {
public :
  CaseList (Object * ccase) : case_(ccase),case_list_(NULL) {assert(case_);}
  CaseList (Object * ccase, Object * case_list) : case_(ccase),case_list_(case_list) {assert(case_ && case_list_);}

  CaseList(const CaseList& cl){
    case_ = cl.case_->clone();
    case_list_ = cl.case_list_->clone();
  }

  void switch_index(int x){
    switchI=x;
  }

  virtual ~CaseList () {
    if (case_) delete case_;
    if (case_list_) delete case_list_;
	} 

  void print (ostream& os) {
		os<<"Node name : CaseList";
		assert( case_ );
    case_->print(os);
    if (case_list_){
      case_list_->print(os);
    }
  }
  void pcodegen(ostream& os) {
      assert(case_);
      case_->switch_index(switchI);
      case_->pcodegen(os);
      if (case_list_) {
          case_list_->switch_index(switchI);
          case_list_->pcodegen(os);
      }
      os << "ujp case_" << ((Case*)case_)->getVal()<<"_"<< switchI <<endl;

  }
  virtual Object * clone () const { return new CaseList(*this);}

private:
  Object * case_;
	Object * case_list_;
  int switchI;
};

class CaseStatement : public Statement {
public :
  CaseStatement (Object * exp, Object * case_list) : exp_(exp),case_list_(case_list) {assert(exp_ && case_list_);}

  CaseStatement(const CaseStatement& cs){
    exp_ = cs.exp_->clone();
    case_list_ = cs.case_list_->clone();
  }

  virtual ~CaseStatement () {
    if (exp_) delete exp_;
    if (case_list_) delete case_list_;
	} 

  void print (ostream& os) {
		os<<"Node name : CaseStatement";
		assert( exp_ && case_list_);
    exp_->print(os);
		case_list_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(exp_ && case_list_);
      exp_->codel(os, 0);
      exp_->pcodegen(os);
      exp_->coder(os);
      os << "neg" << endl;  
      int switchcouter = switchNum++;
      os << "ixj end_switch_" << switchcouter << endl;
      case_list_->switch_index(switchcouter);
      case_list_->pcodegen(os);
      os << "end_switch_" << switchcouter << ":" << endl;
      
  }
  virtual Object * clone () const { return new CaseStatement(*this);}
  
private:
  Object * exp_;
	Object * case_list_;
};

class LoopStatement : public Statement {
public :
  LoopStatement (Object * exp, Object * stat_list) : exp_(exp),stat_list_(stat_list) {assert(exp_ && stat_list_);}

  LoopStatement(const LoopStatement& ls){
    exp_ = ls.exp_->clone();
    stat_list_ = ls.stat_list_->clone();
  }

  virtual ~LoopStatement () {
    if (exp_) delete exp_;
    if (stat_list_) delete stat_list_;
	} 

  void print (ostream& os) {
		os<<"Node name : LoopStatement";
		assert( exp_ && stat_list_);
    exp_->print(os);
		stat_list_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(exp_ && stat_list_);
      os << "loop_" << loopCounter << ":" << endl;
      exp_->codel(os, 0);      
      exp_->pcodegen(os);
      exp_->coder(os);
      int cLoop = loopCounter++;
      os << "fjp end_loop_" << cLoop << endl;
      stat_list_->pcodegen(os);

      os << "ujp loop_" << cLoop << endl;
      os << "end_loop_" << cLoop << ":" << endl;
  }
  virtual Object * clone () const { return new LoopStatement(*this);}

private:
  Object * exp_;
	Object * stat_list_;
};


class ConditionalStatement : public Statement {
public :
  ConditionalStatement (Object * exp, Object * stat_list_if) : exp_(exp),stat_list_if_(stat_list_if), stat_list_else_(NULL) {assert(exp_ && stat_list_if_);}
  ConditionalStatement (Object * exp, Object * stat_list_if, Object * stat_list_else) : exp_(exp),stat_list_if_(stat_list_if), stat_list_else_(stat_list_else) {assert(exp_ && stat_list_if_ && stat_list_else_ );}

  ConditionalStatement(const ConditionalStatement& cs){
    exp_ = cs.exp_->clone();
    stat_list_if_ = cs.stat_list_if_->clone();
    stat_list_else_ = cs.stat_list_else_->clone();
  }

  virtual ~ConditionalStatement () {
    if (exp_) delete exp_;
    if (stat_list_if_) delete stat_list_if_;
    if (stat_list_else_) delete stat_list_else_;
	} 

  void print (ostream& os) {
		os<<"Node name : ConditionalStatement";
		assert( exp_ && stat_list_if_);
    exp_->print(os);
		stat_list_if_->print(os);
		if (stat_list_else_){
			stat_list_else_->print(os);
		}
  }
  void pcodegen(ostream& os) {
      assert(exp_ && stat_list_if_);
      exp_->codel(os, 0);   //274 next time ->if
      exp_->pcodegen(os);
      exp_->coder(os);
      int if_Cout = elseCounter++;
      if (stat_list_else_){
        os << "fjp else_if_" << if_Cout << endl;
        stat_list_if_->pcodegen(os);
        os << "ujp end_if_" << if_Cout << endl;
        os << "else_if_" << if_Cout<< ":" << endl;
        stat_list_else_->pcodegen(os);   
      } else{
          os << "fjp end_if_" << if_Cout << endl;
          stat_list_if_->pcodegen(os);
      }
      os << "end_if_" << if_Cout << ":" << endl;
  }
  virtual Object * clone () const { return new ConditionalStatement(*this);}
  
private:
  Object * exp_;
	Object * stat_list_if_;
	Object * stat_list_else_;
};


class Assign : public Statement {
public :
  Assign (Object * var, Object * exp) : var_(var), exp_(exp) {assert(var_ && exp_);}
  
  Assign(const Assign& as){
    var_ = as.var_->clone();
    exp_ = as.exp_->clone();
  }

  virtual ~Assign () {
    if (exp_) delete exp_;
    if (var_) delete var_;
	} 

  void print (ostream& os) {
		os<<"Node name : Assign";
		assert(var_ && exp_);
    exp_->print(os);
    var_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(var_ && exp_);
      exp_->pcodegen(os);
      var_->codel(os,0);
      var_->pcodegen(os);
      var_->coder(os);
      os << "sto"<< endl;
  }
  virtual Object * clone () const { return new Assign(*this);}

private:
  Object * var_;
  Object * exp_;
};

class StatementList : public Object {
public :
  StatementList (Object * stat) : stat_list_(NULL), stat_(stat)  { assert(stat_);}
  StatementList (Object * stat_list, Object * stat) : stat_list_(stat_list),stat_(stat)  {assert(stat_list_ && stat);}

  StatementList (const StatementList& sl){
    stat_ = sl.stat_->clone();
    stat_list_ = sl.stat_list_->clone();
  }

  virtual ~StatementList () {
    if (stat_) delete stat_;
    if (stat_list_) delete stat_list_;
	} 

  void print (ostream& os) {
	  os<<"Node name : StatementList"<<endl;
	  if (stat_list_){
		  stat_list_->print(os);
	  }
	  assert(stat_);
	  stat_->print(os);
  }
  void pcodegen(ostream& os) {
    //std::cout << "assigh check" << std::endl;
      if (stat_list_) {
          stat_list_->pcodegen(os);
      }
      assert(stat_);
     
      stat_->pcodegen(os);
  }
  virtual Object * clone () const { return new StatementList(*this);}
  
private:
  Object * stat_;
  Object * stat_list_;
};

class RecordList : public Object {
public :
  RecordList (Object * var_decl) : record_list_(NULL), var_decl_(var_decl)  { assert(var_decl_);}
  RecordList (Object * record_list, Object * var_decl) : record_list_(record_list),var_decl_(var_decl)  {assert(record_list_ && var_decl);}
  
  RecordList(const RecordList& li){
    var_decl_= li.var_decl_->clone();
    record_list_ = li.record_list_->clone();
  }

  virtual ~RecordList () {
    if (var_decl_) delete var_decl_;
    if (record_list_) delete record_list_;
	} 

  void print (ostream& os) {
	  os<<"Node name : RecordList"<<endl;
	  if (record_list_){
		  record_list_->print(os);
	  }
	  assert(var_decl_);
	  var_decl_->print(os);
  }
  void pcodegen(ostream& os) {
      if (record_list_) {
          record_list_->pcodegen(os);
      }
      assert(var_decl_);
      
      var_decl_->pcodegen(os);
  }
  virtual Object * clone () const { return new RecordList(*this);}

  //added
  int GetSize() const {
    if(!record_list_)
      return var_decl_->GetSize();
    return record_list_->GetSize() + var_decl_->GetSize();
  }

  void updateRAddress(int address){
    string name_;
    if (record_list_){
      record_list_->GetVer(name_);
      Variable& s = table->symT[name_];
      s.SetrAddress(address);
      ((RecordList*)var_decl_)->updateRAddress(address+s.GetSize());
    }else{
      var_decl_->GetVer(name_);
      Variable& s = table->symT[name_];
      s.SetrAddress(address);
    }
  }

 
private:
  Object * var_decl_;
  Object * record_list_;
};


class SimpleType : public Type {
public:
  SimpleType (const char * name) { 
		name_ = new string(name); 
    SetSize(1);
	}

  virtual ~SimpleType () {
		if (name_ )delete name_;
	}

  SimpleType(const SimpleType& s){
    name_ = new string(*s.name_);
    SetSize(1);
  }

  void print (ostream& os) {
		os<<"Node name : SimpleType"<<endl;;
		os<<"Type is : "<< (*name_) <<endl;
	}
  void pcodegen(ostream& os) {
  }
  virtual Object * clone () const { return new SimpleType(*this);}

private:
  string * name_;
};

class IdeType : public Type {
public:
  IdeType (const char * name) { 
    //have to have a default_obj?
    name_ = new string(name); 
    Variable v = table->symT[*name_];
    SetSize(v.GetSize());
  }

  virtual ~IdeType () {
    if (name_) delete name_;
  }

  IdeType(const IdeType& s){
    name_ = new string(*s.name_);
    Variable v = table->symT[*name_];
    SetSize(v.GetSize());
  }

  void coder(ostream& os){
    os<<"ind"<<endl;
  }

  void GetVer(string& temp){
    temp = *name_;
  }
  void Build_Dim_Vec(vector<int>& dims) {
    Variable v = table->symT[*name_];
    d.push_back(v.GetSize());
  }

  void print (ostream& os) {
    os<<"Node name : IdeType"<<endl;
  }
  void pcodegen(ostream& os) {
    Variable v = table->symT[*name_];
    int address = v.GetrAddress();
    if (address != -1){
        os << "inc ";
        os<<address<<endl;
    }
    else{
      os << "ldc ";
      os<<v.GetAddress()<<endl;
    }
  }

    int GetOverollSize() const {
    Variable v = table->symT[*name_];
    return v.GetType()->GetOverollSize();
  }


  virtual Object * clone () const { return new IdeType(*this);}

private:
  string * name_;
};

class ArrayType : public Type {
public :
  ArrayType (int l,int u, Object * type) : low_(l),up_(u),type_(type)  {
    assert(type_);
    SetSize((up_-low_+1)*type_->GetSize());
    this->SetSubpart(low_*type_->GetSize()+type_->GetSubpart());
    this->Build_Dim_Vec(d);
     }
  
  ArrayType(const ArrayType& a) : low_(a.low_), up_(a.up_){
    type_ = a.type_->clone();
    SetSize((up_-low_+1)*type_->GetSize());
    this->SetSubpart(low_*type_->GetSize()+type_->GetSubpart());
    this->Build_Dim_Vec(d);
  }

  void GetVer(string& temp){
    ((Type*)type_)->GetVer(temp);
  }

  virtual ~ArrayType () {
    if (type_) delete type_;
  }

  void Build_Dim_Vec(vector<int>& d) {
    d.push_back(up_-low_+1);
    if (type_)
      ((Type*)type_)->Build_Dim_Vec(d);
  }
  void print (ostream& os) {
		os<<"Node name : ArrayType: low bound is: "<<low_<<", up bound is: "<<up_<<endl;
	  assert(type_);
	  type_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(type_);
      type_->pcodegen(os);
  }
  virtual Object * clone () const { return new ArrayType(*this);}

private:
  Object * type_;
	int low_,up_;
};

class RecordType : public Type {
public :
  RecordType (Object * record_list) : record_list_(record_list)  { 
    assert(record_list_);
    SetSize(record_list_->GetSize());
    ((RecordList*)record_list_)->updateRAddress(0);
    }

  RecordType(const RecordType& y){
    record_list_ = y.record_list_->clone();
  }

  virtual ~RecordType () {
    if (record_list_) delete record_list_;
  }
  
  void print (ostream& os) {
		os<<"Node name : RecordType"<<endl;
	  assert(record_list_);
	  record_list_->print(os);
  }
  void pcodegen(ostream& os) {
    //std::cout << "check record type" << std::endl;
      assert(record_list_);
      record_list_->pcodegen(os);
  }
  virtual Object * clone () const { return new RecordType(*this);}
  virtual int GetAddress() {
    int a;
    if (table->symT.size() == 0)
        a = table->addressCounter;
    else{
        Variable v = table->symT[table->currentVarName];
        a = v.GetAddress() + v.GetSize();
    }
    return  a - record_list_->GetSize();
} 

private:
  Object * record_list_;
};


class AddressType : public Type {
public :
  AddressType (Object * type) : type_(type)  { 
    assert(type_);
    SetSize(1);
    }

  AddressType(const AddressType& t){
    type_ = t.type_->clone();
    SetSize(1);
  }

  virtual ~AddressType () {
    if (type_) delete type_;
  }
  
  void print (ostream& os) {
		os<<"Node name : AddressType"<<endl;
	  assert(type_);
	  type_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(type_);
      type_->pcodegen(os);
  }
  virtual Object * clone () const { return new AddressType(*this);}

private:
  Object * type_;
};


class Declaration : public Object {
};

class VariableDeclaration : public Declaration {
public:
  VariableDeclaration (Object * type, const char * str) : type_(type){
    assert(type_);
    name_ = new string(str);
    //added
    AddToTable(*name_, type_);
  }

  GetOverollAddress(){
   if (table->symT.empty())
      return table->addressCounter;
    else{
      Variable v = table->symT[table->currentVarName];
      return  v.GetAddress() + v.GetSize();
    }
}
  AddToTable(string& name_, Object * type_){
    SetSize(type_->GetSize()); 
    int address = GetOverollAddress();
    Variable* currentVar = new Variable(type_->GetSize(), address, type_,
     name_, type_->GetSubpart(),((Type*)type_)->d);
    table->symT.insert(make_pair(name_, *currentVar));
    table->currentVarName = name_;
  }

  void getVer(string& temp){
    temp = *name_;
  }

  VariableDeclaration(const VariableDeclaration& p){
    type_ = p.type_->clone();
    name_ = new string(*p.name_); 
    AddToTable(name_, type_);
  }

  virtual ~VariableDeclaration () {
    if (type_) delete type_;
		delete name_;
	}

  void print (ostream& os) {
    os<<"Node name : VariableDeclaration. Var name is: "<<*name_<<endl;
		assert(type_);
	  type_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(type_);
      // why remove line?
      // type_->pcodegen(os);
  }
  virtual Object * clone () const { return new VariableDeclaration(*this);}

private:
  Object * type_;
  string * name_;
};

class Parameter : public Object {
public :
  Parameter (Object * type, const char * name) : type_(type){
    assert(type_);
    name_ = new string(name);
  }
  
  Parameter(const Parameter& p){
    type_ = p.type_->clone();
    name_ = new string(*p.name_);
  }

  virtual ~Parameter () {
    if (type_) delete type_;
		delete name_;
	}
  
  void print (ostream& os) {
		printWayOfPassing(os);
    os<<"Parameter name :" <<name_<<endl;
		assert(type_);
	  type_->print(os);
  }
  void pcodegen(ostream& os) {
      //std::cout << "test" << std::endl;
      printWayOfPassing(os);
      assert(type_);
      type_->pcodegen(os);
  }
protected:
	virtual void printWayOfPassing (ostream& os) const = 0;
  
private:
  Object * type_;
  string * name_;
};

class ByReferenceParameter : public Parameter {
public :
	ByReferenceParameter (Object * type, const char * name) : Parameter (type,name) {}
  virtual Object * clone () const { return new ByReferenceParameter(*this);}
protected:
  void printWayOfPassing (ostream& os) const{
	  os<<"Node name : ByReferenceParameter ";
	}
};

class ByValueParameter : public Parameter {
public :
	ByValueParameter (Object * type, const char * name) : Parameter(type,name) {}
  virtual Object * clone () const { return new ByValueParameter(*this);}
protected:
  void printWayOfPassing (ostream& os) const{
	  os<<"Node name : ByValueParameter ";
	}
};

class ParameterList : public Object {
public :
  ParameterList (Object * formal) : formal_(formal),  formal_list_(NULL) { assert(formal_);}
  ParameterList (Object * formal, Object * formal_list) : formal_(formal), formal_list_(formal_list) {assert(formal_ && formal_list_);}
  
  ParameterList(const ParameterList& pl){
    formal_ = pl.formal_->clone();
    formal_list_ = pl.formal_list_->clone();
  }
  
  virtual ~ParameterList () {
    if (formal_) delete formal_;
    if (formal_list_) delete formal_list_;
	}

  void print (ostream& os) {
	  os<<"Node name : ParameterList"<<endl;
	  if (formal_list_){
		  formal_list_->print(os);
	  }
	  assert(formal_);
	  formal_->print(os);
  }
  void pcodegen(ostream& os) {
      if (formal_list_) {
          formal_list_->pcodegen(os);
      }
      assert(formal_);
      
      formal_->pcodegen(os);
  }
  virtual Object * clone () const { return new ParameterList(*this);}
  
private:
  Object * formal_;
  Object * formal_list_;
};

class FunctionDeclaration : public Declaration {
public :
  FunctionDeclaration (Object * type, Object * block, const char * name) : type_(type), block_(block), formal_list_(NULL) {
    assert(type_ && block_);
    name_ = new string(name);
  }

  FunctionDeclaration (Object * type, Object * formal_list, Object * block,  const char * name) : type_(type), formal_list_(formal_list),block_(block) {
    assert(type_ && formal_list_ && block_ );
    name_ = new string(name);
  }

  virtual ~FunctionDeclaration () {
    if (type_) delete type_;
    if (block_) delete block_;
    if (formal_list_) delete formal_list_;
    if (name_) delete name_;
  }

  FunctionDeclaration(const FunctionDeclaration& fd){
    type_ = fd.type_->clone();
    block_ = fd.block_->clone();
    formal_list_ = fd.formal_list_->clone();
    name_ = new string(*fd.name_);
  }

  void print (ostream& os) {
    os<<"Node name : FunctionDeclaration. Func name is: "<<*name_<<endl;
	  assert(type_ && block_);
	  type_->print(os);
	  if (formal_list_){
		  formal_list_->print(os);
	  }
	  block_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(type_ && block_);
      type_->pcodegen(os);
      if (formal_list_) {
          formal_list_->pcodegen(os);
      }
      block_->pcodegen(os);
  }
  virtual Object * clone () const { return new FunctionDeclaration(*this);}
  
private:
  Object * type_;
  Object * block_;
  Object * formal_list_;
  string * name_;
};

class ProcedureDeclaration : public Declaration {
public :
  ProcedureDeclaration (Object * block, const char * name) : formal_list_(NULL), block_(block) { 
    assert(block_);
    name_ = new string(name);
  }

  ProcedureDeclaration (Object * formal_list, Object * block, const char * name) : formal_list_(formal_list),block_(block)  {
    assert(formal_list_ && block_);
    name_ = new string(name);
  }

  virtual ~ProcedureDeclaration () {
    if (block_) delete block_;
    if (formal_list_) delete formal_list_;
    if (name_) delete name_;
  }

  ProcedureDeclaration(const ProcedureDeclaration& pd){
    block_ = pd.block_->clone();
    formal_list_ = pd.formal_list_->clone();
    name_ = new string(*pd.name_);
  }

  void print (ostream& os) {
	  os<<"Node name : ProcedureDeclaration. Proc name is: "<<*name_<<endl;
	  assert(block_);
	  if (formal_list_){
		  formal_list_->print(os);
	  }
	  block_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(block_);
      if (formal_list_) {
          formal_list_->pcodegen(os);
      }
      block_->pcodegen(os);
  }
  virtual Object * clone () const { return new ProcedureDeclaration(*this);}
  
private:
  Object * block_;
  Object * formal_list_;
  string * name_;
};

class DeclarationList : public Object {
public :
  DeclarationList (Object * decl) : decl_(decl), decl_list_(NULL) { assert(decl_);}
  DeclarationList (Object * decl_list, Object * decl) : decl_list_(decl_list),decl_(decl)  {assert(decl_list_ && decl);}

  DeclarationList(const DeclarationList& dl){
    decl_ = dl.decl_->clone();
    decl_list_ = dl.decl_list_->clone();
  }

  virtual ~DeclarationList () {
    if (decl_) delete decl_;
    if (decl_list_) delete decl_list_;
  }
  
  void print (ostream& os) {
	  os<<"Node name : DeclarationList"<<endl;
	  if (decl_list_){
		  decl_list_->print(os);
	  }
	  assert(decl_);
	  decl_->print(os);
  }
  void pcodegen(ostream& os) {
      if (decl_list_) {
          decl_list_->pcodegen(os);
      }
      assert(decl_);
      decl_->pcodegen(os);
  }
  virtual Object * clone () const { return new DeclarationList(*this);}

private:
  Object * decl_;
  Object * decl_list_;
};

class Block : public Object {
public :
  Block (Object * stat_seq) : stat_seq_(stat_seq), decl_list_(NULL)  { assert(stat_seq_);}
  Block (Object * decl_list, Object * stat_seq) : decl_list_(decl_list), stat_seq_(stat_seq)  {assert(decl_list_ && stat_seq_);}

  Block(const Block& b){
    decl_list_ = b.decl_list_->clone();
    stat_seq_ = b.stat_seq_->clone();
  }

  virtual ~Block () {
    if (stat_seq_) delete stat_seq_;
    if (decl_list_) delete decl_list_;
  }
  
  void print (ostream& os) {
	  os<<"Node name : Begin"<<endl;
	  if (decl_list_){
		  decl_list_->print(os);
	  }
	  assert(stat_seq_);
	  stat_seq_->print(os);
  }
  void pcodegen(ostream& os) {
      if (decl_list_) {
          decl_list_->pcodegen(os);
      }
      assert(stat_seq_);
      stat_seq_->pcodegen(os);
  }

  virtual Object * clone () const { return new Block(*this);}
  
private:
  Object * decl_list_;
  Object * stat_seq_;
};

class Program : public Object {
public :
  Program (Object * block, const char * str) : block_(NULL) {
    block_ = dynamic_cast<Block *>(block);
    assert(block_);
    name_ = new string(str);
  }

  Program(const Program& prog){
    block_ = dynamic_cast<Block *>(prog.block_->clone());
    assert(block_);
    name_ = new string(*prog.name_);
  }

  virtual ~Program () {
    if (block_) delete block_;
    delete name_;
  }
  
  void print (ostream& os) {
    os<<"Node name : Root/Program. Program name is: "<<*name_<<endl;
	  assert(block_);
    block_->print(os);
  }
  void pcodegen(ostream& os) {
    // os<<"check"<<endl;
      assert(block_);
      block_->pcodegen(os);
  }
  virtual Object * clone () const { return new Program(*this);}
  
private:
  Block * block_;
  string * name_;
};



#endif //AST_H