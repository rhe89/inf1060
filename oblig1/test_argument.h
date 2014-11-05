FILE* test_argument(char *name, int checkState) {
	if (!name || *name == '-') {
		return NULL;
	}
	FILE* file;
	/*Klargjor filen for lesing og skriving
	Returnerer NULL om filen ikke finnes, og gjør dermed tastaturet/terminalen
	klar for input/output
	*/
	if (checkState == 0) {
		file = fopen(name, "r");
	} else if (checkState == 1) {
		file = fopen(name, "w");
	}
	
	if (!file) {
		return NULL;
	}
	return file;
}