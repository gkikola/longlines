longlines
=========

A simple utility for finding lines in a text file that exceed a
certain width. The default acceptable line width is 80, which allows
lines that fit within the typical 80-character-wide terminal.


Copyright
---------

Copyright &copy; 2017 Gregory Kikola. License GPLv3+: [GNU GPL version 3
or later](http://www.gnu.org/licenses/gpl.html).

longlines is free software: you are free to change and redistribute
it. There is NO WARRANTY, to the extent permitted by law. See the file
[COPYING](COPYING) for more details.


Usage
-----

    longlines [OPTION...] FILES
    
Read each text file in FILES and display any lines that exceed the
length specified by the --length option (the default length is 80
characters).

| Option | Long name   | Description                                         |
|--------|-------------|-----------------------------------------------------|
| -?     | --help      | Display detailed usage information and then exit    |
| -a     | --all       | Show whole lines instead of just beginning of each  |
| -f     | --filenames | Suppress filenames                                  |
| -l     | --length=_MAXLENGTH_ | Set maximum line length (default 80)       |
| -L | --output-length=_MAXLENGTH_ | Set maximum output length (default 80)  |
| -n     | --line-numbers | Suppress line numbers                            |
| -u     | --usage     | Display a short usage message and then exit         |
|        | --version   | Display program version and exit                    |
