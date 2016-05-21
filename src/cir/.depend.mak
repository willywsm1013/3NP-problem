cirCmd.o: cirCmd.cpp cirMgr.h circuit.h ../../include/util.h \
 ../../include/rnGen.h ../../include/myUsage.h ../../include/myHashMap.h \
 cirGate.h ../../include/sat.h ../../include/Solver.h \
 ../../include/SolverTypes.h ../../include/Global.h \
 ../../include/VarOrder.h ../../include/Heap.h ../../include/Proof.h \
 ../../include/File.h Key.h cirCmd.h ../../include/cmdParser.h \
 ../../include/cmdCharDef.h
circuit.o: circuit.cpp circuit.h ../../include/util.h \
 ../../include/rnGen.h ../../include/myUsage.h ../../include/myHashMap.h \
 cirGate.h ../../include/sat.h ../../include/Solver.h \
 ../../include/SolverTypes.h ../../include/Global.h \
 ../../include/VarOrder.h ../../include/Heap.h ../../include/Proof.h \
 ../../include/File.h Key.h
cirGate.o: cirGate.cpp cirGate.h ../../include/sat.h \
 ../../include/Solver.h ../../include/SolverTypes.h \
 ../../include/Global.h ../../include/VarOrder.h ../../include/Heap.h \
 ../../include/Proof.h ../../include/File.h ../../include/util.h \
 ../../include/rnGen.h ../../include/myUsage.h
cirMgr.o: cirMgr.cpp cirMgr.h circuit.h ../../include/util.h \
 ../../include/rnGen.h ../../include/myUsage.h ../../include/myHashMap.h \
 cirGate.h ../../include/sat.h ../../include/Solver.h \
 ../../include/SolverTypes.h ../../include/Global.h \
 ../../include/VarOrder.h ../../include/Heap.h ../../include/Proof.h \
 ../../include/File.h Key.h
cirSim.o: cirSim.cpp cirMgr.h circuit.h ../../include/util.h \
 ../../include/rnGen.h ../../include/myUsage.h ../../include/myHashMap.h \
 cirGate.h ../../include/sat.h ../../include/Solver.h \
 ../../include/SolverTypes.h ../../include/Global.h \
 ../../include/VarOrder.h ../../include/Heap.h ../../include/Proof.h \
 ../../include/File.h Key.h
fraig.o: fraig.cpp cirGate.h ../../include/sat.h ../../include/Solver.h \
 ../../include/SolverTypes.h ../../include/Global.h \
 ../../include/VarOrder.h ../../include/Heap.h ../../include/Proof.h \
 ../../include/File.h ../../include/util.h ../../include/rnGen.h \
 ../../include/myUsage.h circuit.h ../../include/myHashMap.h Key.h \
 cirMgr.h
