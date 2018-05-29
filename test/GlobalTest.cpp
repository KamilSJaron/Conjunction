// Global test

int testParameterLoading(){

	SettingHandler complete_setting("./test/data/complete_setting");
	try {
		complete_setting.checkParameters();
	} catch(const std::runtime_error& exeption) {
		std::cerr << " Rejecting correct setting: ./test/data/complete_setting.\n";
		std::cerr << " The problem is : " << exeption.what() << "\n";
		return 1;
	}
//	tests for correct reading

	SimulationSetting one_sim_setting;

	Simulation *sim;

	if(complete_setting.getNumberOfSimulations() != 12){
		std::cerr << " Setting ./test/data/complete_setting was misinterpreted, suggested "
		<< complete_setting.getNumberOfSimulations() << " instead of 12 sims;" << std::endl;
		return 1;
	}

	one_sim_setting = complete_setting.getSimualtionSetting(0);
	sim = new Simulation(one_sim_setting);
	if(sim->simulate() != 0){
		std::cerr << " A problem during simulation has occurred \n";
		return 1;
	}
	delete sim;

	return 0;
}
