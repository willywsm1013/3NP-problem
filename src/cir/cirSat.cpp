#include"circuit.h"
/********************/
/*  Prove Function  */
/********************/
void Circuit::genProveModel(SatSolver *&s,Var True,Var False){
	_true->setVar(True);
	_false->setVar(False);
	for(size_t i=0,n=_dfsList.size();i<n;++i){
		GateType type=_dfsList[i]->_type;
		//cout<<_dfsList[i]->getTypeString()<<"  "<<_dfsList[i]->getName()<<endl;
		if(type!=Input && type!=Const && !_dfsList[i]->buildSAT(s)){
			//cout<<_dfsList[i]->getName()<<endl;
			assert(0);
		}
		//cout<<endl;
	}
	cout<<"ok!"<<endl;
}
