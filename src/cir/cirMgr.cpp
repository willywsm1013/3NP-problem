#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;
enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
bool CirMgr::readCircuit(string &fileOne,string &fileTwo){
	if(one || two)
		clean();
	one = new Circuit;
	two = new Circuit;

	if(!one->read(fileOne,1) || !two->read(fileTwo,2))
		return false;

	size_t n=one->_input.size();
	_input.resize(n);
	for(size_t i=0;i<n;++i){
		_input[i].push_back(one->_input[i]);
		_input[i].push_back(two->_input[i]);
	}
	
	return true;

}
bool CirMgr::readSingleCircuit(string &fileOne){
	if(one)
		clean();
	one = new Circuit;

	if(!one->read(fileOne,1))
		return false;

	size_t n=one->_input.size();
	_input.resize(n);
	for(size_t i=0;i<n;++i){
		_input[i].push_back(one->_input[i]);
	}
	
	return true;

}
/************************/
/*  printing functions  */
/************************/
void CirMgr::printSummary(){
	if(one){
		cout<<"Circuit ONE"<<endl;
		one->printSummary();
		cout<<"\n===========\n"<<endl;
	}
	if(two){
		cout<<"Circuit TWO"<<endl;
		two->printSummary();
		cout<<"\n===========\n"<<endl;
	}
}
void CirMgr::printNetlist(){
	cout<<"CirCuit ONE"<<endl;
	one->printNetlist();
	cout<<endl;

	cout<<"CirCuit TWO"<<endl;
	two->printNetlist();
	cout<<endl;
	return;
}
void CirMgr::printPIs(){
	cout<<"PI"<<endl;
	return;
}
void CirMgr::printPOs(){
	cout<<"PO"<<endl;
	return;
}
void CirMgr::printFECpairs(){
	if(_FECgroup.size()==0){
		cout<<"no FEC pair."<<endl;
		return;
	}
	for(size_t i=0;i<_FECgroup.size();++i){
		cout<<"["<<i<<"] ";
		for(size_t j=0;j<_FECgroup[i].size();++j)
		cout<<" "<<_FECgroup[i][j]->_name<<"("<<((WireGate*)_FECgroup[i][j])->_cirNumber<<")";
		cout<<endl;
	}
}
/********************/
/*  Check Function  */
/********************/
void CirMgr::CheckEquil()const{
	assert(one->_output.size()==two->_output.size());
	bool equi=true;
	for(size_t i=0,n=one->_output.size();i<n;++i){
		if(	one->_output[i]->_curSim != two->_output[i]->_curSim &&
			one->_output[i]->_curSim != ~(two->_output[i]->_curSim)   
		){
		cout<<"output:"<<i<<" is not equil."<<endl;
		equi=false;
		//return;
		}
	}
	if(equi)
		cout<<"Crcuit equivalent."<<endl;
	else
		cout<<"Circuit not equivalent."<<endl;




}

/**************/
/*    toAig   */
/**************/
void CirMgr::toAig(){
	if(one){
		cout<<"Converting circuit One...\n";
		one->convert2Aig();
		cout<<"    aig gate size:"<<one->_dfsAig.size()<<endl;
	}
	if(two){
		cout<<"Converting circuit Two...\n";
		two->convert2Aig();
		cout<<"    aig gate size:"<<two->_dfsAig.size()<<endl;
	}
}

/*********************/
/*  Helper Function  */
/*********************/
/*****************************/
/*  clean and init function  */ 
/*****************************/
void CirMgr::init(){
	one=0;
	two=0;
	_solver=0;
	//srand(0);
	srand(time(0));
}

void CirMgr::clean(){
	delete one;
	delete two;
	init();
}
