physiology: TmsiAmplifier.cpp nexus/tmsi.c AmplifierDriver.cpp physiology.cpp
	g++ -g -lbluetooth $^ -fpermissive -o $@

