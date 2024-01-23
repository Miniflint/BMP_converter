#include "../inc/main.h"
#include "../inc/bmp.h"

// calculate aspect ratio:
// racine carrée

int handle_error(char *str, int error_code)
{
    fprintf(stderr, "%s\n", str);
    return (error_code);
}

void print_help(char *filename)
{
    printf("Usage: %s [OPTION]... [INFILE]... [OUTFILE]\n", filename);
    printf("Encode any ACII file to a bitmap or decode a bitmap file to a textfile\n\n");
    printf("   -e, --encode\t\t Encode the file\n");
    printf("   -d, --decode\t\t Decode the file\n");
    printf("   -h, --help\t\t Display the help ande exit\n");
    printf("\n");
}

int usage(char *prog_name)
{
    fprintf(stderr, "Error:\n\t");
    fprintf(stderr, "Usage: %s -h, --help\n", prog_name);
    return (1);
}

int check_argument(char *argv)
{
    int found;

    found = 0;
    if (argv[0] == '-')
    {
        if (my_strcmp(argv, "-h") == 0 || my_strcmp(argv, "--help") == 0)
            found = 1;
        else if (my_strcmp(argv, "-e") == 0 || my_strcmp(argv, "--encode") == 0)
            found = 2;
        else if (my_strcmp(argv, "-d") == 0 || my_strcmp(argv, "--decode") == 0)
            found = 3;
    }
    return (found);
}

int main(int argc, char **argv)
{
    char *program_name;
    char *filename;
    char *outfile;
    int out;

    program_name = *(argv)++;
    if (argc < 2)
        return (usage(program_name));
    out = check_argument(*(argv)++);
    if (!out)
        return (usage(program_name));
    else if (out == 1)
    {
        print_help(program_name);
        return (1);
    }
    filename = *(argv++);
    if (!filename)
        return (usage(program_name));
    outfile = *(argv++);
    if (!outfile)
        return (usage(program_name));
    else if (out == 2)
    {
        if (encode(filename, outfile))
            return (handle_error("An error as occured", 1));
    }
    else if (out == 3)
    {
        if (decode(filename, outfile))
            return (handle_error("An error as occured", 1));
    }
    return (0);
}
