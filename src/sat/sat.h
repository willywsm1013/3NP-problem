/****************************************************************************
  FileName     [ sat.h ]
  PackageName  [ sat ]
  Synopsis     [ Define miniSat solver interface functions ]
  Author       [ Chung-Yang (Ric) Huang, Cheng-Yin Wu ]
  Copyright    [ Copyleft(c) 2010-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef SAT_H
#define SAT_H

#include <cassert>
#include <iostream>
#include "Solver.h"

using namespace std;

/********** MiniSAT_Solver **********/
class SatSolver
{
   public : 
      SatSolver():_solver(0) { }
      ~SatSolver() { }

      // Solver initialization and reset
      void initialize() {
         reset();
         if (_curVar == 0) { _solver->newVar(); ++_curVar; }
      }
      void reset() {
         if (_solver) delete _solver;
         _solver = new Solver();
         _assump.clear(); _curVar = 0;
      }

      // Constructing proof model
      // Return the Var ID of the new Var
      inline Var newVar() { _solver->newVar(); return _curVar++; }
	 //////  Buf Gate  //////
	 void addEquilCNF(Var vf,Var va,bool fa){
		 vec<Lit> lits;
         
		 Lit lf = Lit(vf);
         
		 Lit la = fa? ~Lit(va): Lit(va);
         
		 lits.push(~la); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
         
		 lits.push(la); lits.push(lf);
         _solver->addClause(lits); lits.clear();
	 }
	 //////  OR Gate  //////
	  // fa/fb = true if it is inverted
	  void addOrCNF(Var vf, Var va, bool fa, Var vb, bool fb) {
         vec<Lit> lits;
         
		 Lit lf = Lit(vf);
         
		 Lit la = fa? ~Lit(va): Lit(va);
         Lit lb = fb? ~Lit(vb): Lit(vb);
         
		 lits.push(~la); lits.push(lf);
         _solver->addClause(lits); lits.clear();
         
		 lits.push(~lb); lits.push(lf);
         _solver->addClause(lits); lits.clear();
         
		 lits.push(la); lits.push(lb); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
      }
	  void addOrCNFS(Var vf, const vec<Var> &v, const vec<bool> &f) {
		 vec<Lit> lits;
         vec<Lit> l;
		 l.growTo(v.size());
		 
		 Lit lf = Lit(vf);
         for(int i=0;i<v.size();++i){
		 	l[i] = f[i]? ~Lit(v[i]): Lit(v[i]);
         	lits.push(~l[i]); lits.push(lf);
         	_solver->addClause(lits); lits.clear();
         }
         for(int i=0;i<l.size();++i) 
		 	lits.push(l[i]);
		 lits.push(~lf);
         _solver->addClause(lits); lits.clear();
      }
      //////  AND GATE  //////
      // fa/fb = true if it is inverted
      void addAndCNF(Var vf, Var va, bool fa, Var vb, bool fb) {
         vec<Lit> lits;
         
		 Lit lf = Lit(vf);
         
		 Lit la = fa? ~Lit(va): Lit(va);
         Lit lb = fb? ~Lit(vb): Lit(vb);
         
		 lits.push(la); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
         
		 lits.push(lb); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
         
		 lits.push(~la); lits.push(~lb); lits.push(lf);
         _solver->addClause(lits); lits.clear();
      }
	void addAndCNFS(Var vf, const vec<Var> &v, const vec<bool> &f) {
         vec<Lit> lits;
         vec<Lit> l;
		 l.growTo(v.size()); 
		
		 Lit lf = Lit(vf);
		 for(int i=0;i<v.size();++i){
		 	l[i] = f[i]? ~Lit(v[i]): Lit(v[i]);
			lits.push(l[i]); lits.push(~lf);
        	 _solver->addClause(lits); lits.clear();
		 }
   	 	 for(int i=0;i<l.size();++i)      
		 	lits.push(~l[i]);
		 lits.push(lf);_solver->addClause(lits); lits.clear();
      }

      // fa/fb = true if it is inverted
      void addXorCNF(Var vf, Var va, bool fa, Var vb, bool fb) {
         vec<Lit> lits;
         Lit lf = Lit(vf);
         
		 Lit la = fa? ~Lit(va): Lit(va);
         Lit lb = fb? ~Lit(vb): Lit(vb);
         
		 lits.push(~la); lits.push( lb); lits.push( lf);
         _solver->addClause(lits); lits.clear();
         
		 lits.push( la); lits.push(~lb); lits.push( lf);
         _solver->addClause(lits); lits.clear();
         
		 lits.push( la); lits.push( lb); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
         
		 lits.push(~la); lits.push(~lb); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
      }
	  void addXor(vec<Lit> &l,vec<bool> &b,int pos,int n){
		if(n>l.size()-pos)
		 	return;
		else if(l.size()==pos){
			vec<Lit> lits;
			for(int i=0;i<l.size();++i){
				if(!b[i])lits.push(~l[i]);
				else lits.push(l[i]);
			}
			return;
		}

		b[pos]=false;
		addXor(l,b,pos+1,n-1);
		b[pos]=true;
		addXor(l,b,pos+1,n);
	  }
	  void addXorCNFS(Var vf, vec<Var> v, vec<bool> f) {
         vec<Lit> lits;
         vec<Lit> l;
		 vec<bool> b;
		 b.growTo(v.size()+1);
		 l.push(Lit(vf));
		 for(int i=0;i<v.size();++i)
		 	l.push((f[i]? ~Lit(v[i]): Lit(v[i])));
		 for(int i=1;i<=l.size();i+=2)
		 	addXor(l,b,0,i);	 
      }
      // For incremental proof, use "assumeSolve()"
      void assumeRelease() { _assump.clear(); }
      void assumeProperty(Var prop, bool val) {
         _assump.push(val? Lit(prop): ~Lit(prop));
      }
      bool assumpSolve() { return _solver->solve(_assump); }

      // For one time proof, use "solve"
      void assertProperty(Var prop, bool val) {
         _solver->addUnit(val? Lit(prop): ~Lit(prop));
      }
      bool solve() { _solver->solve(); return _solver->okay(); }

      // Functions about Reporting
      // Return 1/0/-1; -1 means unknown value
      int getValue(Var v) const {
         return (_solver->modelValue(v)==l_True?1:
                (_solver->modelValue(v)==l_False?0:-1)); }
      void printStats() const { const_cast<Solver*>(_solver)->printStats(); }

   private : 
      Solver           *_solver;    // Pointer to a Minisat solver
      Var               _curVar;    // Variable currently
      vec<Lit>          _assump;    // Assumption List for assumption solve
};

#endif  // SAT_H

