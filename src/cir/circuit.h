#ifndef CIRCUIT
#define CIRCUIT

#include <vector>
#include <fstream>
#include <string>
#include "util.h"
#include "myHashMap.h"
#include "cirGate.h"
#include "sat.h"
#include "Key.h"
using namespace std;
class Circuit{
friend class CirMgr;
public:
	typedef vector<CirGate*> GateList; 
	typedef vector<AigGate*> AigList; 
	friend AigGate ;
	Circuit(){init();}
	~Circuit(){clear();}
	//printing function
	void printSummary();
	void printNetlist();	
	//reading function
	bool read(string &,const int);//read netlist from file

	//Access Function

	//Sweep Function
	void sweep(){return;}

	//simple optimization
	void simpleOptimize(){return;}
	//DFS Function
	void DFSearch(const vector<CirGate*> &,vector<CirGate*> &);
	void AigDFSearch(const vector<size_t> &,vector<AigGate*> &);
	//Simulation Function
	void simulate(int);
	bool checkAig();
	//fraig Function
	void fraig(SatSolver *,const Var&,const Var&);
	// convert oto Aig
	void convert2Aig();
private:
	int _cirNumber;
	HashMap<nameKey,CirGate*> *_hash;
	vector<CirGate*> _input;
	vector<CirGate*> _output;
	vector<CirGate*> _wire;
	vector<CirGate*> _gate;
	vector<CirGate*> _dfsList;
	AigList _dfsAig;
	vector<AigList > _fecGroup;
	
	ConstGate *_true;
	ConstGate *_false;

	void clear(){delete _hash;_hash=0;}
	void init();
	//read function
	bool readTop(const vector<string> &);
	bool readInput(const vector<string> &);
	bool readOutput(const vector<string> &);
	bool readWire(const vector<string> &);
	bool readGate(const vector<string> &);

	//DFS function
	void DFS(CirGate* const,vector<CirGate*> &);
	void AigDFS(const size_t &,vector<AigGate*> &list);	
	//simulation fection
	bool findFEC();
	void grouping(vector<AigList>&,const AigList &);
	void randomInput();
	//SAT function
	void genProveModel(SatSolver *,const AigList&); 	
	bool satProve(SatSolver* ,const AigGate*,const AigGate*);
};
#endif
