#include "includes/minihell.h"

void init_shell(void)
{
    // Load history from file if it exists
    char *home = getenv("HOME");
    if (home)
    {
        char *histpath = ft_strjoin(home, "/" HISTFILE);
        if (histpath)
        {
            read_history(histpath);
            free(histpath);
        }
    }
}

void cleanup_shell(void)
{
    // Save history to file
    char *home = getenv("HOME");
    if (home)
    {
        char *histpath = ft_strjoin(home, "/" HISTFILE);
        if (histpath)
        {
            write_history(histpath);
            free(histpath);
        }
    }
    clear_history();
}

int main(int argc, char **argv, char **envp)
{
    char *input;
    
    (void)argc;
    (void)argv;

    init_shell();
    using_history();

    while (1)
    {
        input = readline("minihell> ");
        if (!input) // Handle EOF (Ctrl+D)
        {
            printf("\n");
            break;
        }
        if (*input) // If input is not empty, process it
        {
            add_history(input);
            exec(input, envp);
        }
        free(input);
    }

    cleanup_shell();
    return 0;
}