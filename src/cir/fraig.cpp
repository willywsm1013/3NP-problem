#include "cirGate.h"
#include "circuit.h"
#include "cirMgr.h"


/***********************/
/*   Static Function   */
/***********************/
static void satProve(SatSolver* solver){


}


/***************/
/*   Circuit   */
/***************/
void Circuit::genProofModel(SatSolver* s,const GateList& aigList){
	if(s!=0)
		delete s;
	s=new SatSolver();
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
		s.addAigCNF(v,v0,i0,v1,i1);	
	}
}

void Circuit::fraig(SatSolver* s,GateList & fecgroup){
	for(size_t i=0,n=fecgroup.size();i<n;++i){
		


	}

}
/**************/
/*   CirMgr   */
/**************/
