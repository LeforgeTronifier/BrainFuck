#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define code(_code) fprintf(outfile, (_code))

FILE	* outfile,
	* infile;

void help(char **);
void convert();

int main(int argc, char ** argv){
	if(argc < 2){
		help(argv);
		return 0;
	}
	
	char	opt;
	int	verbose = 0,
		rm_code = 1;
	char	* infile_name,
		* outfile_name = "tbf.c",
		* object_code_name = NULL;
	char	compile_cmd[50] = "gcc ";

	while((opt = getopt(argc, argv, ":i:vrho:")) > -1){
		switch(opt){
			case 'i':
				infile_name = optarg;
				break;
				
			case 'v':
				verbose = 1;
				break;
				
			case 'o':
				object_code_name = optarg;
				break;
				
			case 'r':
				rm_code = 0;
				break;
				
			case 'h':
				help();
				return 0;
				
			case ':':
				printf("Error %s needs an argument\n", optopt);
				return 0;
				
			case '?':
				help(argv);
				return 0;
		}
	}
	
	if(verbose)
		printf("[+] Opening input file... ");
		
	if(!(infile = fopen(infile_name, "r"))){
		printf("%sbf: \033[0;31merror:\033[0m %s: No such file or directory\n", (verbose)?"\n":"", infile_name);
		printf("bf: \033[0;31mfatal error:\033[0m no input files\n");
		printf("Interpretation terminated\n");
		return 0;
	}
	
	printf("OK\n");
	
	if(verbose)
		printf("[+] Creating a new C file... ");

        if(!(outfile = fopen(outfile_name, "w"))){
                printf("%sbf: \033[0;31merror:\033[0m %s: Can't create file, access denied\n", (verbose)?"\n":"", infile_name);
                printf("bf: \033[0;31mfatal error:\033[0m check permission\n");
                printf("Interpretation terminated\n");
                return 0;
        }
	
	printf("OK\n");

	convert();
	
	fclose(infile);
	fclose(outfile);
	
	strcat(compile_cmd, outfile_name);
	strcat(compile_cmd, " -o ");
	if(object_code_name == NULL)
		object_code_name = "output";
	strcat(compile_cmd, object_code_name);

	if(verbose)
		printf("[+] Compiling using command... %s\n", compile_cmd);

	if(verbose)
		printf("[+] Compiling %s file... ", outfile_name);

	system(compile_cmd);
	
	if(verbose)
		printf("OK\n");
	if(verbose)
		printf("[+] Output file generate... %s\n", object_code_name);
	
	if(rm_code){
		if(verbose)
			printf("[+] Removing the intermediate C file... ");
		system("rm tbf.c");
		printf("OK\n");
	}
	return 0;
}

void convert(){
	fprintf(outfile, "/**\n * BrainFuck to C Interpretor\n * Author: Tronifier\n * Autogenerated C file\n */");
	fprintf(outfile, "\n\n#include<stdio.h>\nint main(){\n\tchar a[30000];\n\tchar *ptr = a;\n\n");
	char input;
	int tab_count = 1;
	while((input = fgetc(infile))!=EOF){
		int i = tab_count;
		while(i--)
			fprintf(outfile, "\t");
		if(input == '>'){
			code("ptr++;\n");
		}
		else if(input == '<'){
			code("ptr--;\n");
		}
		else if(input == '+'){
			code("++*ptr;\n");
		}
		else if(input == '-'){
			code("--*ptr;\n");
		}
		else if(input == '['){
			code("while(*ptr){\n");
			tab_count++;
		}
		else if(input == ']'){
			code("}\n");
			tab_count--;
		}
		else if(input == ','){
			code("*ptr = getchar();\n");
		}
		else if(input == '.'){
			code("putchar(*ptr);\n");
		}
	}
	fprintf(outfile, "return 0;\n}");

}

void help(char **argv){
	printf("BrainFuck to C Interpretor\nAuthor: Tronifier\n\nUsage: %s [options] file...\nOptions:\n\t-v\tVerbose On(Default off)\n\t-i\tInput file(Main BF code)\n\t-o\tOuput C File Name\n\t-r\tDo not remove the intermediate C file\n", argv[0]);
}
