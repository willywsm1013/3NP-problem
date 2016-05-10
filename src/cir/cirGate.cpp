#include "cirGate.h"

/*******************************/
/*  global or static variable  */
/*******************************/
size_t CirGate::_gloRef=1;
size_t AigGate::_gloref=1;
/*********************/
/*  helper function  */
/*********************/
void CirGate::init(){
	_localRef=0;
}

size_t CirGate::p2s(CirGate *p, bool invert){
	if(invert)return (size_t)p | MASK_INVERT;
	else return (size_t)p;
}

CirGate* CirGate::s2p(size_t s){
	return (CirGate*)(s & ~MASK_INVERT);
}

bool CirGate::getInvert(size_t s){
	if ((s & MASK_INVERT)==1)return true;
	else return false;
}

void CirGate::DFSfanin(int curLevel){
	if(_type!=Wir){
		for(int i=0;i<curLevel;++i)
			cout<<"    ";
		
		cout<<getTypeString()<<":"<<_curSim;
		
		if(_type!=Input){
			unsigned int tmp;
			if((_aig & MASK_AIG)==0)
				tmp=((CirGate*)(_aig & ~MASK_INVERT & ~MASK_AIG))->_curSim;
			else
				tmp=((AigGate*)(_aig & ~MASK_INVERT & ~MASK_AIG))->_curSim;
			cout<<":"<<tmp;
		
		}
		cout<<endl;
		curLevel++;
	}
	for(size_t i=0;i<_fanin.size();++i){
		_fanin[i]->DFSfanin(curLevel);
	}
}
/*********************/
/*  Access Function  */
/*********************/
string CirGate::getTypeString()const{
	switch (_type){
		case Input:
			return "Input";
		case Output:
			return "Output";
		case And:{
			if(_invert)return "Nand";
			else return "And";
		}
		case Or:{
			if(_invert)return "Nor";
			else return "Or";
		}
		case Xor:{
			if(_invert)return "Xnor";
			else return "Xor";
		}
		case Buf:{
			if(_invert)return "Not";
			else return "Buf";
		}
		case Wir:
			return "Wire";
		case Const:{
			if(_invert) return "True";
			else return "False";
		}
		default:{
			cout<<"Gatetype not define. Error!"<<endl;
			cout<<"Gate name:"<<_name<<endl;
			assert(0);
		}

	}
}

void CirGate::reportGate()const{
	
	cout<<	"========================================\n"<<
			"GateName"<<setw(42)<<_name<<endl<<
			"GateType"<<setw(42)<<getTypeString()<<endl<<
			"========================================\n";

}
void CirGate::reportFanin(){
	DFSfanin(0);
}
void CirGate::reportFanout(){
	return;
}
void CirGate::printGate()const{
	cout<<"["<<_ID<<"]"<<setw(6)<<left<<getTypeString();
	if(_name.size()==0){
		cout<<" fanin:";
		for(size_t i=0;i<_fanin.size();++i)
			cout<<" "<<_fanin[i]->_name;
	}
	else
		cout<<" name:"<<_name;

	cout<<endl;
}	
/*************************/
/*  simulation function  */
/*************************/

void POGate::operate(){
	if(_fanin.size()==0){
		return;
	}
	else if(_fanin.size()==1){
		if(_fanin[0]->_invert)
			_curSim=~(_fanin[0]->getCurSim());
		else
			_curSim=_fanin[0]->getCurSim();
	}
	else{
		cout<<"too many fanin for output "<<_name<<endl;
		assert(0);
	}
}

void WireGate::operate(){
	if(_fanin.size()==0){
		cout<<"no fanin wire "<<endl;
		assert(0);
	}
	else if(_fanin.size()==1){
		if(_fanin[0]->_invert)
			_curSim=~(_fanin[0]->getCurSim());
		else
			_curSim=_fanin[0]->getCurSim();
	}
	else{
		cout<<"too many fanin for wire"<<endl;
		assert(0);
	}
}

void AndGate::operate(){
	if(_fanin.size()<=1){
		cout<<"only "<<_fanin.size()<<" fanin for AndGate..."<<endl;
		assert(0);
	}
	
	_curSim=_fanin[0]->getCurSim();
	for(size_t i=1;i<_fanin.size();++i){
		unsigned int value= _fanin[i]->getCurSim();
		_curSim=(_curSim & value);
	}
}

void OrGate::operate(){
	if(_fanin.size()<=1){
		cout<<"only "<<_fanin.size()<<" fanin for OrGate..."<<endl;
		assert(0);
	}
	
	_curSim=_fanin[0]->getCurSim();
	for(size_t i=1;i<_fanin.size();++i){
		unsigned int value= _fanin[i]->getCurSim();
		_curSim=(_curSim | value);
	}
}

void XorGate::operate(){
	if(_fanin.size()<=1){
		cout<<"only "<<_fanin.size()<<" fanin for XorGate..."<<endl;
		assert(0);
	}
	
	_curSim=_fanin[0]->getCurSim();
	for(size_t i=1;i<_fanin.size();++i){
		unsigned int value= _fanin[i]->getCurSim();
		_curSim=(_curSim & ~value) | (~_curSim & value);
	}
}

void BufGate::operate(){
	if(_fanin.size()==0){
		cout<<"no fanin Buf "<<endl;
		assert(0);
	}
	else if(_fanin.size()==1){
		_curSim=_fanin[0]->getCurSim();
	}
	else{
		if(_invert)
			cout<<"too many fanin for Not"<<endl;
		else
			cout<<"too many fanin for Buf"<<endl;
		assert(0);
	}
}


void AigGate::operate(){
	unsigned int fanin0,fanin1;
	bool in0=(_in0 & MASK_INVERT),in1=(_in1 & MASK_INVERT);
	//get cursim of in0
	if((_in0 & MASK_AIG) == 0){
		CirGate* tmp=(CirGate*)(_in0 & ~MASK_INVERT);	
		if(tmp->_type == Const){
			cout<<"Aig operate const"<<endl;
			//assert(s == 0 || s ==(unsigned int)~0);
		}
		fanin0 = tmp->_curSim;
		assert(fanin0 != 0 );
	}
	else
		fanin0 = ((AigGate*)(_in0 & ~MASK_INVERT & ~MASK_AIG))->_curSim;
	if(in0)
		fanin0=~fanin0;
	
	//get cursim of in1
	if((_in1 & MASK_AIG) == 0){
		CirGate* tmp=(CirGate*)(_in1 & ~MASK_INVERT & ~MASK_AIG);
		if(tmp->_type == Const){
			cout<<"Aig operate const"<<endl;
		}
		fanin1 = tmp->_curSim;
		assert(fanin1!=0);
	}
	else
		fanin1 = ((AigGate*)(_in1 & ~MASK_INVERT & ~MASK_AIG))->_curSim;
	if(in1)
		fanin1=~fanin1;
	_curSim=fanin0 & fanin1;
	
}
/******************/
/*  SAT Function  */
/******************/
bool POGate::buildSAT(SatSolver *&s){
	_var=s->newVar();
	s->addEquilCNF(_var,_fanin[0]->getVar(),_fanin[0]->_invert);
	//cout<<getName()<<"  "<<_var;
	return true;
}
bool WireGate::buildSAT(SatSolver *&s){
	assert(_fanin.size()==1);
	_var=_fanin[0]->_var;
	return true;
}
bool AndGate::buildSAT(SatSolver *&s){
	_var=s->newVar();
	vec<Var> v;
	vec<bool> f;
	for(size_t i=0;i<_fanin.size();++i){
		//assert(_fanin[i]->getType()==Wir);
		v.push(_fanin[i]->_var);
		f.push(_fanin[i]->_invert);
	}
	s->addAndCNFS(_var,v,f);
	//cout<<_var;
	return true;
}
bool OrGate::buildSAT(SatSolver *&s){
	_var=s->newVar();
	vec<Var> v;
	vec<bool> f;
	for(size_t i=0;i<_fanin.size();++i){
		//assert(_fanin[i]->getType()==Wir);
		v.push(_fanin[i]->_var);
		f.push(_fanin[i]->_invert);
	}
	s->addOrCNFS(_var,v,f);
	//cout<<_var;
	return true;
}
bool XorGate::buildSAT(SatSolver *&s){
	//cout<<_var;
	_var=s->newVar();
	vec<Var> v;
	vec<bool> f;
	for(size_t i=0;i<_fanin.size();++i){
		//assert(_fanin[i]->getType()==Wir);
		v.push(_fanin[i]->_var);
		f.push(_fanin[i]->_invert);
	}
	s->addXorCNFS(_var,v,f);
	return true;
}

bool BufGate::buildSAT(SatSolver *&s){
	assert(_fanin.size()==1);
	_var=_fanin[0]->_var;
	return true;
}
/**********************/
/*   convert to Aig   */
/**********************/
size_t AndGate::toAig(){
	if (_aig != 0 )return _aig;
	size_t fanin1=_fanin[0]->toAig(),fanin2;
	for(size_t i=1;i<_fanin.size();++i){
		fanin2=_fanin[i]->toAig();
		AigGate* gate=new AigGate(fanin1,fanin2);
		if(i==_fanin.size()-1){
			gate->setlead();
			gate->_rep=this;
			
		}
		fanin1=(size_t)gate | MASK_AIG;
	}
	_aig = _invert?(fanin1 ^ MASK_INVERT):fanin1; 
	return _aig;
}

size_t OrGate::toAig(){
	if (_aig!=0)return _aig;
	size_t fanin1=(_fanin[0]->toAig()) ^ MASK_INVERT,fanin2;
	for(size_t i=1;i<_fanin.size();++i){
		fanin2=_fanin[i]->toAig() ^ MASK_INVERT;
		AigGate* gate=new AigGate(fanin1,fanin2);
		if(i==_fanin.size()-1){
			gate->setlead();
			gate->_rep=this;
		}
		fanin1=(size_t)gate | MASK_AIG;
	}
	_aig = _invert?fanin1:(fanin1 ^ MASK_INVERT);
	return _aig;
}

size_t XorGate::toAig(){
	if (_aig!=0) return _aig;

	size_t fanin1=_fanin[0]->toAig(),fanin2;
	for(size_t i=1;i<_fanin.size();++i){
		fanin2=_fanin[i]->toAig();
		AigGate* and1=new AigGate(fanin1,fanin2 ^ MASK_INVERT);
		AigGate* and2=new AigGate(fanin1 ^ MASK_INVERT,fanin2);
		AigGate* and3=new AigGate((size_t)and1|(MASK_INVERT+MASK_AIG),(size_t)and2|(MASK_INVERT+MASK_AIG));
		if(i==_fanin.size()){	
			and3->setlead();
			and3->_rep=this;
		}
		fanin1=(size_t)and3 | (MASK_AIG+MASK_INVERT);
	}
	_aig=_invert?fanin1 ^ MASK_INVERT:fanin1;
	return _aig;

}

size_t BufGate::toAig(){
	if(_aig != 0 ) return _aig;
	
	_aig=_fanin[0]->toAig();
	if (_invert){
		_aig= (_aig ^ MASK_INVERT);
	}
	return _aig;

}
