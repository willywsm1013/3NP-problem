#include "cirMgr.h"
#include <cmath>
/***********************/
/*   public function   */
/***********************/
void CirMgr::randomSim(){
	cout<<"start random simulation..."<<endl;
	size_t count=0;
	size_t max_pattern=_input.size()>30?_input.size()*100:pow(2,_input.size());
	while(count*32 < max_pattern){
		++count;
		randomInput();
		simulate(2);
	}
	cout<<"total pattern : "<<count*32<<endl;
	cout<<"random sim over"<<endl;
}

void CirMgr::fileSim(ifstream & file){
	int count=0;
	while(fileInput(file)){
		++count;
		simulate(2);
		cout<<"pattern : "<<count*32<<char(13);
	}
	cout<<"total pattern < "<<count*32<<endl;
	cout<<"fileSim over!"<<endl;
}
void CirMgr::aigRandomSimCheck(){
	cout<<"start random simulation check..."<<endl;
	size_t count=0;
	size_t max_pattern=_input.size()>30?_input.size()*100:pow(2,_input.size());
	cout<<"randomSim check:";
	while(count*32 < max_pattern){
		++count;
		randomInput();
		simulate();
		if(!checkAig()){
			cout<<"nonequivalence\nerror happended"<<endl;
			assert(0);	
		}
		//cout<<"pattern : "<<count*32<<char(13);
	}
	cout<<"equivalence"<<endl;
	cout<<"total pattern : "<<count*32<<endl;
}
void CirMgr::aigFileSimCheck(ifstream & file){
	int count=0;
	cout<<"fileSim check:";
	while(fileInput(file)){
		++count;
		simulate();
		if(!checkAig()){
			cout<<"nonequivalence\nerror happended"<<endl;
			assert(0);	
		}
		//cout<<"pattern : "<<count*32<<char(13);
	}
	cout<<"equivalence"<<endl;
	cout<<"total pattern < "<<count*32<<endl;
}
/************************/
/*   private function   */
/************************/
// three mode: 
//     0:simulate both netlist ane aig list
//     1:simulate netlist only
//     2:simulate aig list only
void CirMgr::simulate(int mode){
	if(one){
		one->simulate(mode);
	}
	if(two)
		two->simulate(mode);
}

void CirMgr::randomInput(){
	for(size_t i=0,n=_input.size();i<n;++i){
		unsigned int value=rand();
		assert(_input[i].size() > 0);
		for(size_t j=0;j<_input[i].size();++j)
			if(!_input[i][j]->setCurSim(value))
				assert(0);
	}
}

bool CirMgr::fileInput(ifstream & file){
	string line;
	int count=0;
	while(count!=32){
		if(getline(file,line)){
			if(line.size() != _input.size()){
				cout<<"input size of file is "<<line.size()<<", but circuit input size is "<<_input.size()<<endl;
				return false;
			}
	
			for(size_t i=0,n=_input.size();i<n;++i){
				bool value;
				if(line[i]=='1')
					value=true;
				else if(line[i]=='0')
					value=false;
				else{
					cout<<line[i]<<" is not 0 or 1"<<endl;
					return false;
				}
				for(size_t j=0;j<_input[i].size();++j)
					if(!_input[i][j]->setCurSimBit(value))
						assert(0);
			}
			++count;
		}
		else
			break;
	}
	if (count==0)
		return false;
	else 
		return true;
}
bool CirMgr::checkAig(){
	if((one!=0) && !(one->checkAig()))
		return false;
	if((two!=0) && !(two->checkAig()))
		return false;
	
	return true;
	
}
void CirMgr::findFECgroup(){
	return;
}

void CirMgr::checkFEC(CirGate* gate,vector<vector<CirGate*> > &FEC,HashMap<simKey,CirGate*> &hash){
	simKey k(gate->_curSim);
	CirGate* refGate;
	if(hash.check(k,refGate)){
		assert(FEC.size()>((WireGate*)refGate)->FECNum());
		FEC[((WireGate*)refGate)->FECNum()].push_back(gate);
	}
	else{
		hash.insert(k,gate);
		((WireGate*)gate)->FECNum(FEC.size());

		vector<CirGate*> tmp;
		tmp.push_back(gate);
		FEC.push_back(tmp);
	}
}

#include "circuit.h"

/***********************/
/*   public function   */
/***********************/
void Circuit::simulate(int mode){
	if(mode==0 || mode == 1)
		for(size_t i=0,n=_dfsList.size();i<n;++i)
			_dfsList[i]->operate();

	if(mode ==0 || mode == 2)
		for(size_t i=0,n=_dfsAig.size();i<n;++i)
			_dfsAig[i]->operate();
}
bool Circuit::checkAig(){
	for(size_t i=0;i<_output.size();++i){
		unsigned int sim1=_output[i]->_curSim,sim2=((POGate*)_output[i])->_aig;
		bool invert=((sim2 & MASK_INVERT) == 1);
		if((sim2 & MASK_AIG)==0){
			sim2=((CirGate*)(sim2 & ~MASK_AIG & ~MASK_INVERT))->_curSim;
		}
		else{
			sim2=((AigGate*)(sim2 & ~MASK_AIG & ~MASK_INVERT))->_curSim;
		}
		//cout<<"invert:"<<invert<<";"<<sim2<<":"<<~sim2<<endl;
		if(invert)
			sim2=~sim2;
		
		if(sim1!=sim2){
			cout<<"error output name:"<<_output[i]->_name<<endl;
			cout<<sim1<<":"<<sim2<<endl;
			_output[i]->reportFanin();
			return false;
		}
		
	}

	return true;

}

/************************/
/*   private function   */
/************************/
void Circuit::findFEC(){




}


