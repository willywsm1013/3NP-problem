
#ifndef CIRCUIT
#define CIRCUIT

#include <vector>
#include <fstream>
#include <string>
#include "util.h"
#include "myHashMap.h"
#include "cirGate.h"
#include "sat.h"
using namespace std;
class Circuit{
friend class CirMgr;
public:
	friend AigGate ;
	typedef GateList vector<CirGate*>
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
	void AigDFSearch(const vector<size_t> &,vector<BaseGate*> &);
	//Simulation Function
	void simulate(int);
	bool checkAig();
	//SAT Function
	void satProve(SatSolver *);
	void genProveModel(SatSolver *&,Var,Var);

	// convert oto Aig
	void convert2Aig();
private:
	int _cirNumber;
	HashMap<nameKey,CirGate*> *_hash;
	GateList _input;
	GateList _output;
	GateList _wire;
	GateList _gate;
	GateList _dfsList;
	vector<BaseGate*> _dfsAig;
	vector<GateList> _fecGroup;
	
	ConstGate *_true;
	ConstGate *_false;
	constGate *_aigConst;

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
	void AigDFS(const size_t &,vector<BaseGate*> &list);	
	//simulation fection
	void findFEC();
	//SAT function
};
#endif
