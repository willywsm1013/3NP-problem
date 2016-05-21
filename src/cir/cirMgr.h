#ifndef CIRMGR
#define CIRMGR

#include "circuit.h"
#include "cirKey.h"
class simKey;
class CirMgr{
public:
	CirMgr(){init();}
	~CirMgr(){clean();}
	
	//Reading Function
	bool readCircuit(string &,string &);
	bool readSingleCircuit(string &);
	//Printing Function
	void printSummary();
	void printNetlist();
	void printPIs();
	void printPOs();
	void printFECpairs(); 
	//Simulation Function
	void randomSim();
	void aigRandomSimCheck();
	void randomAigSim();
	void fileSim(ifstream & );
	void aigFileSimCheck(ifstream &);

	//Prove Function
	void satProve();
	void genProveModel(bool = false);

	//Check Function
	void CheckEquil()const;

	// convert to aig circuit
	void toAig();

	// Optimize 
	void optimization();

private:
	Circuit *one,*two;
	vector<vector<CirGate*> > _input;
	vector<vector<CirGate*> > _FECgroup;
	SatSolver *_solver;
	Var _true;
	Var _false;

	void clean();
	void init();

	//DFS function
	void DFS(vector<CirGate*> &);

	//simulation function
	void getWire(Circuit*,vector<CirGate*>&);
	void randomInput();
	bool fileInput(ifstream&);
	void simulate(int=0);
	void findFECgroup();
	void checkFEC(CirGate*,vector<vector<CirGate*> >&,HashMap<simKey,CirGate*>&);
	bool checkAig();
	//prove function
	void inputSetVar(SatSolver*);
	bool prove(CirGate*,CirGate*,SatSolver*&);
	void sat(Circuit*);
	void getInputPattern(SatSolver*&);
	

};



#endif
