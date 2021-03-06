// These functions mainly deal with combining different files.

#include "turtle_functions.h"

// This is a wrapper for all of the functions necessary for combining a WhiteSpace and a regular file.

void combine_ws_reg_files(program *prog, whitespace_info *whitespace_prog, char *file_one, char *file_two)
{
	initialise_whitespace_prog(whitespace_prog);

	open_program_file(file_one, prog);

	open_whitespace_program_file_verbatim(file_two, whitespace_prog);

	write_combination_to_file(prog, whitespace_prog);
}

// This is different to the other opening WhiteSpace function, that function is used to decode what is meant
// from the WhiteSpace, whereas this is used to open the file and directly scan in the contents as is. It opens
// the file and then while it is not at the end scans in the components.

void open_whitespace_program_file_verbatim(char *filename, whitespace_info *whitespace_prog)
{
	FILE *prog_file;
	char ch;

	if( (prog_file = fopen(filename, "r")) == NULL){
		clear_screen();
		fprintf(stderr, "\nThere was an error reading in the file.\n\n");
		exit(EXIT_CODE);
	}
	else{
		while( ( ch = fgetc(prog_file) ) != EOF ){
			scan_in_whitespace_components_verbatim(whitespace_prog, ch);
		}
	}

	fclose(prog_file);

}

// Here it scans in all of the WhiteSpace from the file into a verbatim array.
// If it senses that it has scanned a new line then it scans the next set of WhiteSpace
// into the next element of the array.

// As it goes through it increments the number of lines it has detected to be in
// the WhiteSpace file.

void scan_in_whitespace_components_verbatim(whitespace_info *whitespace_prog, char ch)
{

	static int i = 0;

	whitespace_prog -> ws_verbatim_file[ whitespace_prog -> ws_num_lines ][i] = ch;
	++i;

	if(i > MAX_SIZE_OF_LINE){
		clear_screen();
		fprintf(stderr, "\nWhiteSpace scanned in is too large.\n\n");
		exit(EXIT_CODE);
	}

	if(ch == '\n'){
		whitespace_prog -> num_chars_per_line[ whitespace_prog -> ws_num_lines ] = i;
		++whitespace_prog -> ws_num_lines;
		i = 0;
	}

}

// Now we write the combined code to file. If we can't open the necessary file we print an error.
// Whilst we are on a line that is below the number of lines in both files we are trying to combine
// then we print both to the file. Then if there are more lines in the regular file we print only
// the regular file and the same for the WhiteSpace file.

void write_combination_to_file(program *prog, whitespace_info *whitespace_prog)
{
	FILE *combined_file = NULL;
	int current_line = 0;

	if( (combined_file = fopen("Combined_File.txt", "w")) == NULL){
		clear_screen();
		fprintf(stderr, "\nThere was an error reading in the file.\n\n");
		exit(EXIT_CODE);
	}

	while(current_line < whitespace_prog -> ws_num_lines && current_line < prog -> num_lines_in_file){
		print_combination_to_file(prog, whitespace_prog, &combined_file, current_line);
		++current_line;
	}

	if(whitespace_prog -> ws_num_lines > current_line){

		while(current_line <= whitespace_prog -> ws_num_lines){
			print_only_ws_to_file(whitespace_prog, &combined_file, current_line);
			++current_line;
		}
	}
	else if(prog -> num_lines_in_file > current_line){

		//This is printed so that we don't end up with two commands together with no space.
		fprintf(combined_file, "\n");
		while(current_line <= prog -> num_lines_in_file){
			print_only_non_ws_to_file(prog, &combined_file, current_line);
			++current_line;
		}
	}

	fclose(combined_file);
}

// This prints the regular file and then the WhiteSpace file to Combined_File.txt It prints the WhiteSpace file
// on a character by character basis as that is how we scanned it in, and you can't print WhiteSpace strings.

void print_combination_to_file(program *prog, whitespace_info *whitespace_prog, FILE **combined_file, int current_line)
{
	int i;

	fprintf(*combined_file, "%s", prog -> prog_line[current_line]);

	for(i = 0; i < whitespace_prog -> num_chars_per_line[current_line]; ++i){
		fprintf(*combined_file, "%c", whitespace_prog -> ws_verbatim_file[ current_line ][i]);
	}

}

// This is used if there is more WhiteSpace than regular file and prints only the WhiteSpace component to a file.

void print_only_ws_to_file(whitespace_info *whitespace_prog, FILE **combined_file, int current_line)
{
	int i;

	for(i = 0; i < whitespace_prog -> num_chars_per_line[current_line]; ++i){
		fprintf(*combined_file, "%c", whitespace_prog -> ws_verbatim_file[ current_line ][i]);
	}

}

// Same as above. If we have run out of WhiteSpace to print in a file then we only print the regular text.

void print_only_non_ws_to_file(program *prog, FILE **combined_file, int current_line)
{
	fprintf(*combined_file, "%s\n", prog -> prog_line[current_line]);

}
