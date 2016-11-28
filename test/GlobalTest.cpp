// Global test

int testParameterLoading(){

	SettingHandler complete_setting("./test/data/complete_setting");
	if(complete_setting.checkParameters() != 0){
		cerr << " Rejecting correct setting: ./test/data/complete_setting \n";
		return 1;
	}
//	tests for correct reading

	SimulationSetting one_sim_setting;

	Simulation *sim;

	if(complete_setting.getNumberOfSimulations() != 48){
		cerr << " Setting ./test/data/complete_setting was misinterpreted, suggested "
		<< complete_setting.getNumberOfSimulations() << " instead of 12 sims;" << endl;
		return 1;
	}

	one_sim_setting = complete_setting.getSimualtionSetting(32);
	sim = new Simulation(one_sim_setting);
	if(sim->simulate() != 0){
		cerr << " A problem during simulation has occurred \n";
		return 1;
	}
	delete sim;

	return 0;
}
