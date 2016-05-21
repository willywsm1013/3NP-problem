#include "cirGate.h"
#include "sat.h"
#include <cmath>
/***********************/
/*   Static Function   */
/***********************/
#include "circuit.h"
/***********************/
/*   Public Function   */
/***********************/
void Circuit::fraig(SatSolver* s,const Var &T,const Var &F){
	cout<<"Simulate first..."<<endl;
	size_t max_pattern=_input.size()>15?_input.size()*1000:pow(2,_input.size());
	size_t max_fail=_input.size()<100?_input.size():100;
	size_t count=0,fail_num=0,ori_size;
	_fecGroup.push_back(_dfsAig);
	cout<<"max_pattern:"<<max_pattern<<endl;
	cout<<"max_fail:"<<max_fail<<endl;
	while(count*32 < max_pattern && fail_num < max_fail){
		++count;
		randomInput();
		simulate(2);
		ori_size=_fecGroup.size();
		cout<<"count:"<<count<<endl;
		if(!findFEC()){
			cout<<"zero"<<endl;
			break;
		}
		if(ori_size==_fecGroup.size())
			++fail_num;
		else
			fail_num=0;
	}		
	cout<<"    total simulate pattern:"<<count*32<<endl;
	cout<<"    fecGroup size:"<<_fecGroup.size()<<endl;
	cout<<"    fail:"<<fail_num<<endl;  
	cout<<"start doing fraig..."<<endl;
	for(size_t i=0;i<_fecGroup.size();++i){
 		cout<<"["<<i+1<<"] "<<"("<<_fecGroup[i].size()<<") "<<_fecGroup[i][0]->_curSim<<endl;
		for(size_t j=0;j<_fecGroup[i].size();++j){
			int tmp=_fecGroup[i][0]->_curSim ^ _fecGroup[i][j]->_curSim;
			assert(tmp==0 || tmp==~0);
		}
	}
	_true->_var=T;
	_false->_var=F;
	genProveModel(s,_dfsAig);
	// incremental fraig
	for(size_t i=0,n=_dfsAig.size();i<n;++i){
		if(_dfsAig[i]!=0){
			


		}
	}

	_dfsAig.clear();
	vector<size_t> outputAig;
	for(size_t i=0;i<_output.size();++i)
		outputAig.push_back(((POGate*)_output[i])->_aig);
	AigDFSearch(outputAig,_dfsAig);
}

/************************/
/*   Private Function   */
/************************/
void Circuit::genProveModel(SatSolver* s,const AigList& aigList){	
	for(size_t i=0,n=_input.size();i<n;++i){
		Var v=s->newVar();
		_input[i]->_var=v;
	}
	for(size_t i=0,n=aigList.size();i<n;++i){
		AigGate *gate=aigList[i];
		Var v=s->newVar();
		gate->_var=v;
		Var v0=((AigGate*)(gate->_in0 & ~MASK_INVERT & ~MASK_AIG))->_var;
		Var v1=((AigGate*)(gate->_in1 & ~MASK_INVERT & ~MASK_AIG))->_var;
		bool i0=gate->_in0 & MASK_INVERT;
		bool i1=gate->_in1 & MASK_INVERT;
		s->addAigCNF(v,v0,i0,v1,i1);	
	}
}
bool Circuit::satProve(SatSolver* solver,const AigGate* a,const AigGate* b){
	if(solver==0){
		assert(0);
	}
	Var nvar=solver->newVar();
	unsigned int tmp=(a->_curSim ^ b->_curSim);
	if(tmp==0)
		solver->addXorCNF(nvar,a->_var,false,b->_var,false);
	else if(tmp==(unsigned int)~0)
		solver->addXorCNF(nvar,a->_var,false,b->_var,true);
	else{
		cout<<"two gate are not in one FEC group...no need to prove"<<endl;
		return false;
	}
	solver->assumeRelease();
	solver->assumeProperty(nvar,true);
	solver->assumeProperty(_true->_var,true);
	solver->assumeProperty(_false->_var,false);
	
	bool result=solver->assumpSolve();
	
	return !result; 
}

#include "cirMgr.h"
/***********************/
/*   Public Function   */
/***********************/
void CirMgr::Fraig(){
	if(_solver!=0)
		delete _solver;
	_solver=new SatSolver();
	_solver->initialize();
	_true=_solver->newVar();
	_false=_solver->newVar();
	if(one){
		cout<<"do fraig for circuit one"<<endl;
		one->fraig(_solver,_true,_false);
	}
	if(two){
		cout<<"do fraig for circuit two"<<endl;
		two->fraig(_solver,_true,_false);
	}
}
