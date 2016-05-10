#include "circuit.h"
void Circuit::init(){
	_true=new ConstGate(0,"1'b1",true,_cirNumber);
	_false=new ConstGate(1,"1'b0",false,_cirNumber);
	_hash=0;
}
/********************/
/*  read operation  */
/********************/
bool Circuit::read(string &path,const int number ){
	cout<<"Reading circuit from \""<<path<<"\"..."<<endl;
	cout<<"--------------------"<<endl;
	_cirNumber = number;
	fstream file;
	file.open(path.c_str(),ios::in);
	if(!file){
		cout<<"input file not exist..."<<endl;
		return false;
	}

	string tmp;
	while(readFromFile(file,tmp)){
		vector<string> options;
		if(!takeOptions(tmp,options)){
			cout<<"parsing error"<<endl;
			return false;
		}

		if(options[0]=="module"){
			if(!readTop(options)){
				file.close();
				return false;
			}
		}
		else if(options[0]=="input"){
			if(!readInput(options)){
				file.close();
				return false;
			}
		}
		else if(options[0]=="output"){
			if(!readOutput(options)){
				file.close();
				return false;
			}
		}
		else if(options[0]=="wire"){
			if(!readWire(options)){
				file.close();
				return false;
			}
		}
		else{
			if(!readGate(options)){
				file.close();
				return false;
			}
		}
	}
	cout<<endl;
	DFSearch(_output,_dfsList);
	return true;
}

//private function
bool Circuit::readTop(const vector<string> &options){
	cout<<"reading Top..."<<endl;
	if(!_hash){
		_hash=new HashMap<nameKey,CirGate*>(getHashSize(options.size()*2));
		_hash->insert(nameKey(_true->_name),_true);
		_hash->insert(nameKey(_false->_name),_false);
	}
	else{	
		return false;
		assert(0);
	}
	return true;
}

bool Circuit::readInput(const vector<string> &options){
	cout<<"reading Input..."<<endl;

	for(size_t i=1;i<options.size();++i){
		CirGate *tmp=new PIGate(_input.size(),options[i],_cirNumber);
		nameKey k(options[i]);
		if(!_hash->insert(k,tmp)){
			cout<<"gate "<<options[i]<<" are insert twice. insert error"<<endl;
			return false;
			assert(0);
		}
		_input.push_back(tmp);
	}
	cout<<"    input size: "<<_input.size()<<endl;
	return true;

}

bool Circuit::readOutput(const vector<string> &options){
	cout<<"reading Output..."<<endl;
	
	for(size_t i=1;i<options.size();++i){
		CirGate *tmp=new POGate(_output.size(),options[i],_cirNumber);
		nameKey k(options[i]);
		if(!_hash->insert(k,tmp)){
			cout<<"gate "<<options[i]<<" are insert twice. insert error"<<endl;
			assert(0);
			return false;
		}
		_output.push_back(tmp);
	}
	cout<<"    output size: "<<_output.size()<<endl;
	return true;
}

bool Circuit::readWire(const vector<string> &options){
	cout<<"reading Wire..."<<endl;
		
	for(size_t i=1;i<options.size();++i){
		CirGate *tmp=new WireGate(_wire.size(),options[i],_cirNumber);
		nameKey k(options[i]);
		if(!_hash->insert(k,tmp)){
			cout<<"gate "<<options[i]<<" are insert twice. insert error"<<endl;
			assert(0);
			return false;
		}
		_wire.push_back(tmp);
	}
	cout<<"    wire size: "<<_wire.size()<<endl;
	return true;
}
bool Circuit::readGate(const vector<string>&options){
	//cout<<"reading Gate "<<options[0]<<"..."<<endl;
	
	CirGate *tmp;
	size_t id=_gate.size();
	if(options[0]=="buf")
		tmp=new BufGate(id,false,_cirNumber);
	else if(options[0]=="not")
		tmp=new BufGate(id,true,_cirNumber);
	else if(options[0]=="and")
		tmp=new AndGate(id,false,_cirNumber);
	else if(options[0]=="nand")
		tmp=new AndGate(id,true,_cirNumber);
	else if(options[0]=="or")
		tmp=new OrGate(id,false,_cirNumber);
	else if(options[0]=="nor")
		tmp=new OrGate(id,true,_cirNumber);
	else if(options[0]=="xor")
		tmp=new XorGate(id,false,_cirNumber);
	else if(options[0]=="xnor")
		tmp=new XorGate(id,true,_cirNumber);
	else{
		cout<<"unknow gate type:"<<options[0]<<endl;
		assert(0);
	}
	CirGate *gate;
	//find fanout
	nameKey k(options[1]);//fanout's name
	if(!_hash->check(k,gate)){
		cout<<"gate \""<<options[1]<<"\" not found(find fanout). "<<endl;
		return false;
	}
	tmp->_fanout.push_back(gate);
	gate->_fanin.push_back(tmp);
	//find fanin
	for(size_t i=2;i<options.size();++i){
		nameKey y(options[i]);
		if(!_hash->check(y,gate)){
			cout<<"gate \""<<options[i]<<"\" not found(find fanin). "<<endl;
			cout<<"output:"<<options[1]<<endl;
			return false;
		}
		tmp->_fanin.push_back(gate);
		gate->_fanout.push_back(tmp);
	}
	_gate.push_back(tmp);

	return true;

}

/***********************/
/*  Printing Function  */
/***********************/

void Circuit::printSummary(){
	cout<<	"Circuit Statistics\n"<<
			"==================\n"<<
			"  PI "<<setw(12)<<right<<_input.size()<<endl<<
			"  PO "<<setw(12)<<_output.size()<<endl<<
			"  Wire "<<setw(10)<<_wire.size()<<endl<<
			"  Gate "<<setw(10)<<_gate.size()<<endl<<
			"  Aig  "<<setw(10)<<_dfsAig.size()<<endl<<
			"==================\n"<<
			"  Total "<<setw(9)<<_input.size()+_output.size()+_wire.size()+_gate.size()<<endl;
	cout<<left;
}

void Circuit::printNetlist(){
	for(size_t i=0;i<_dfsList.size();++i)
		_dfsList[i]->printGate();
}


/***********************/
/*  DFSearch Function  */
/***********************/
void Circuit::DFSearch(const vector<CirGate* > &list, vector<CirGate*> &dfsList){
	for(size_t i=0, n=list.size();i<n;++i)
		DFS(list[i],dfsList);
	++CirGate::_gloRef;
}

void Circuit::DFS(CirGate* const source ,vector<CirGate*> &dfsList){	
	if(source->_type == Const){
		cout<<"const:"<<source->_curSim<<endl;
	}
	source->setLocalRef();
	for(size_t i=0,n=source->_fanin.size();i<n;++i){
		CirGate* fanin=source->_fanin[i];
		if(!(fanin->refEqual()))
			DFS(fanin,dfsList);
		
	}
/*
	GateType type=source->getType();
	if(type==Input || type==Output || type==Const || (type==Wir && ((WireGate*)source)->isCut())){
		cout<<"gate \""<<source->getName()<<"\" type "<<source->getTypeString()<<" can't be put in dfsList, return..."<<endl;
		return;
	}
*/
	dfsList.push_back(source);
}

void Circuit::AigDFSearch(const vector<size_t> & source,vector<AigGate*> &list){
	for(size_t i=0;i<source.size();++i){
		AigDFS(source[i],list);
	}
	++AigGate::_gloref;
}

void Circuit::AigDFS(const size_t &source,vector<AigGate*> &list){	
	if((source & MASK_AIG) == 0){
		//CirGate* tmp=(CirGate*)(source & ~MASK_INVERT);
		return ;
	}
	AigGate* gate=(AigGate*)(source & ~MASK_AIG & ~MASK_INVERT);	
	
	if(gate->checkRef())
		return ;
	gate->setRef();
	
	AigDFS(gate->_in0,list);
	AigDFS(gate->_in1,list);
		
	
	list.push_back(gate);
	return ;
}
/**************/
/*   To Aig   */
/**************/
void Circuit::convert2Aig(){

	for(size_t i=0;i<_output.size();++i){
		_output[i]->toAig();
	}
	vector<size_t> outputAig;
	for(size_t i=0;i<_output.size();++i){
		outputAig.push_back(((POGate*)_output[i])->_aig);
	}

	AigDFSearch(outputAig,_dfsAig);
}
