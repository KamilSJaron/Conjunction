// Global test

int testParameterLoading(){

	SettingHandler all_setting("./test/data/parameters_simple");
	SimulationSetting one_sim_setting;
	one_sim_setting = all_setting.getSimualtionSetting(0);

	if(one_sim_setting.loci != 2 or one_sim_setting.chromosomes != 1 or one_sim_setting.selection != 0.42 or one_sim_setting.generations != 50){
		cerr << " Problem in loading numerical parameters.\n";
		return 1;
	}

	cerr << "Testing invalid setting:" << "./test/data/parameters_simple" << endl;
	if(all_setting.checkParameters() == 0){
		cerr << " Accepting incorrect setting. Program should report problem of non-defined world\n";
		return 1;
	}

	cerr << "Testing valid setting with missing values:" << "./test/data/missing_parameters" << endl;
	SettingHandler sparse_setting("./test/data/missing_parameters");
	if(sparse_setting.checkParameters() == 0){
		cerr << " Accepting setting with missing values. Program should report problem of non-defined parameters\n";
		return 1;
	}

	cerr << "Testing valid complete setting with all possible definitions of values:" << "./test/data/complete_setting" << endl;
	SettingHandler complete_setting("./test/data/complete_setting");
	if(complete_setting.checkParameters() != 0){
		cerr << " Rejecting correct setting.\n";
		return 1;
	}
//	tests for correct reading

	return 0;
}
