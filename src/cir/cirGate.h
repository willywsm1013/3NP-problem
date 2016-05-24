#ifndef CIRGATE
#define CIRGATE

#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>
#include "sat.h"
#include "util.h"
#include "cirKey.h"
class CirMgr;
class AigGate;
using namespace std;
#define MASK_INVERT (size_t)0x1
#define MASK_AIG (size_t)0x2
/************************************/
/*  Global functions and variables  */
/************************************/

enum GateType{
  Input = 0,
  Output = 1,
  And = 2,
  Or = 3,
  Xor = 4,
  Buf = 5,
  Wir = 6,
  Const = 7,
  Aig = 8,
  Dummy=9
};

/**********************/
/*  Class definition  */
/**********************/

//base class
class CirGate{
	public:
		// constructor for gates have name
		// such as Input Output Wire Const
		CirGate(size_t id,string name,GateType type,bool invert,int cirNum):_ID(id),_name(name),_type(type),_invert(invert),_cirNumber(cirNum){init();}
		// constructor for hates have no name
		// such as And Or Xor Buf
		CirGate(size_t id,GateType type,bool invert,int cirNum):_ID(id),_type(type),_invert(invert),_cirNumber(cirNum){init();}
		~CirGate(){}
		//static variable	
		static size_t _gloRef;
		
		//public variable
		

		//access function
		string getTypeString()const;
		void printGate()const;
		void reportGate()const;
		void reportFanin();
		void reportFanout();
		
		//DFS function
		size_t getLocalRef()const{return _localRef;}
		void setLocalRef(){_localRef=_gloRef;}
		bool refEqual()const{return _localRef==_gloRef;}

		//simulation function
		virtual bool setCurSim(unsigned int ){return false;}
		virtual bool setCurSimBit(bool ){return false;}
		virtual unsigned int getCurSim()const{return _curSim;}
		virtual void operate(){return;}

		//sat function
		void setVar(Var v){_var=v;}
		Var getVar()const{return _var;}
		virtual bool buildSAT(SatSolver *&){return false;}

		//convert to aig
		virtual size_t toAig()=0;

	//protected:
		size_t _ID; // gate 
		string _name;// gate name
		GateType _type;// gate type(and,or,not...)
		bool _invert;// output invert or not
		int _cirNumber; // this gate is in which circuit
		
		unsigned int _curSim; // record crrent simulation value
		size_t _localRef;
		Var _var;
		size_t _aig;
			
		vector<CirGate*> _fanin;
		vector<CirGate*> _fanout;
	private:
		//helper function
		void init();
		static size_t p2s(CirGate* p,bool invert);
		static CirGate* s2p(size_t s);
		static bool getInvert(size_t s);

		void DFSfanin(int);

};

/*********************/
/*   inherit class   */
/*********************/
class PIGate:public CirGate{
public:
	PIGate(size_t id,string name,int cirNum):CirGate(id,name,Input,false,cirNum){}
	bool setCurSim(unsigned int c){_curSim=c;return true;}
	bool setCurSimBit(bool v){_curSim=(_curSim<<1)+(v?1:0);return true;}

	size_t toAig(){return (size_t)this;}

};

class POGate:public CirGate{
public:
	POGate(size_t id,string name,int cirNum):CirGate(id,name,Output,false,cirNum){_aig=0;}
	void operate();
	bool buildSAT(SatSolver *&);
	
	size_t toAig(){if(_aig!=0)return _aig;_aig=_fanin[0]->toAig();return _aig;}
	
	//size_t _aig;


private:
};

class ConstGate:public CirGate{
public:
	ConstGate(size_t id,string name,bool phase,int cirNum,size_t aig):CirGate(id,name,Const,phase,cirNum){
		_aig=aig;
		if(phase){_curSim=~0;_aig=(aig | MASK_INVERT);}
		else {_curSim=0;_aig=aig;}
	}
	size_t toAig(){assert(_aig!=0);return _aig;}
};

class WireGate:public CirGate{
public:
	WireGate(size_t id,string name,int cirNum):CirGate(id,name,Wir,false,cirNum){_in=false;}

	void operate();
	
	size_t toAig(){return _fanin[0]->toAig();}

	void FECNum(const size_t fecNum){_fecNum=fecNum;}
	size_t FECNum()const {return _fecNum;}
	void inFEC(){_in=true;}
	void outFEC(){_in=false;}

	bool buildSAT(SatSolver *&);
private:
	size_t _fecNum;
	bool _in;
};

class AndGate:public CirGate{
public:
	AndGate(size_t id,bool invert,int cirNum):CirGate(id,And,invert,cirNum){_aig=0;}
	size_t toAig();
	void operate();
	bool buildSAT(SatSolver *&);
private:
	//size_t _aig;
};

class OrGate:public CirGate{
public:
	OrGate(size_t id,bool invert,int cirNum):CirGate(id,Or,invert,cirNum){_aig=0;}
	size_t toAig();
	void operate();
	bool buildSAT(SatSolver *&);
private:
	//size_t _aig;
};

class XorGate:public CirGate{
public:
	XorGate(size_t id,bool invert,int cirNum):CirGate(id,Xor,invert,cirNum){_aig=0;}
	size_t toAig();
	void operate();
	bool buildSAT(SatSolver *&);
private:
	//size_t _aig;
};

class BufGate:public CirGate{
public:
	BufGate(size_t id,bool invert,int cirNum):CirGate(id,Buf,invert,cirNum){_aig=0;}
	size_t toAig();
	void operate();
	bool buildSAT(SatSolver *&);

	//size_t _aig;
};

/****************/
/*   Aig Gate   */
/****************/
class BaseGate{
	public:	
		BaseGate();
		Var _var;
		static size_t _gloref;

		void setRef(){_ref=_gloref;}
		bool checkRef(){return _ref==_gloref;}
		virtual void operate()=0;

	protected:
		size_t _ref;
};

class AigGate: public BaseGate{
	//friend class Circuit;	
	public:
		AigGate(size_t one,size_t two):BaseGate(),_in0(one),_in1(two){_ref=0;_lead=false;}
		size_t _in0,_in1;
		int _curSim;
		CirGate* _rep;

		void setlead(){_lead=true;}
		void operate();
	private:
		bool _lead;
	
};

class constGate:public BaseGate{
	public:
		constGate(bool invert):BaseGate(),_curSim(0){}
		const int _curSim;
		
		void operate(){return;}
	private:
};
#endif
