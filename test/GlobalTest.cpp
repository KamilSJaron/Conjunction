// Global test

int testParameterLoading(){

	SettingHandler complete_setting("./test/data/complete_setting");
	if(complete_setting.checkParameters() != 0){
		cerr << " Rejecting correct setting: ./test/data/complete_setting \n";
		return 1;
	}
//	tests for correct reading

	return 0;
}
