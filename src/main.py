import os

def execute():
	cppFileNames = ["christofides.cpp","tsp_dp.cpp"]
	inputFileNames = ["tc_2.txt","tc_3.txt"]
	for cppFile in cppFileNames:
		for inputFile in inputFileNames:
			run_command = "g++ "+cppFile
			if(os.system(run_command) == 0):
				for inputFile in inputFileNames:
					exec_command = "./a.out " + inputFile
					os.system(exec_command)
execute()