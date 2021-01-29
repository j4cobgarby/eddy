![](demo.gif)

## Description

Eddytion is a simple, fast, clean text editor based on the source code for eddy.

## Getting Started (installing the program)

#### First install the dependencies with:
```bash
sudo apt install libncurses-dev g++ make 
```

#### Then clone the repository with
```bash
git clone https://github.com/honestly-nuts/eddytion.git
```

#### Then cd into the repository and build the app with:
```bash
cd eddytion
sudo make install
```

#### Now you are good to go.

### Windows

If running eddytion on windows, it'd be a good idea to _not use cmd_, since some bits (such as dialog boxes) don't work particularly well with it. Use a third party terminal emulator - I'd suggest [Hyper.js](https://hyper.is) since it looks pretty, but an emulator with more features is [Cmder](http://cmder.net).

The best way to get started on Windows is by installing the Windows subsystem for Linux. Basically this is
a way of opening up an ubuntu shell in Windows! You can find out more about that
[here](https://msdn.microsoft.com/en-gb/commandline/wsl/install_guide), but

> Your PC must be running a 64-bit version of Windows 10 Anniversary Update or later (build 1607+).

Once you've got the bash shell up and running, you can follow the steps for debian

## Built With

* g++ - The GNU c++ compiler
* [ncurses](http://invisible-island.net/ncurses/man/ncurses.3x.html) - To do the interactive stuff in a terminal

## Contributing

 1. Fork it
 2. Create a new branch for your new feature
 3. Develop this branch - comment (briefly or in detail) how it all works
 4. Make sure you've changed all the necessary bits and pieces to reflect the new version. These are
   - In `main.cpp`: the title bar's text, and the title of the splash dialog.
   - Made relevant changed to `README.md` (commands, etc..)
 5. Push your fork

## How to use

Similar to vim, eddytion has different modes for editing files. eddy has two modes:

 - `NORMAL`
 - `INSERT`
 - `VISUAL`

### NORMAL mode

When you open up eddytion, you'll be in `NORMAL`. This is the mode in which you can make use of eddy's
different commands. To switch from `NORMAL` to `INSERT`, simply press `i`.

### INSERT mode

Once you're in `INSERT`, you can start writing text in the file. You may enter text just as in any
other editor. When you want to go back into `NORMAL`, press `escape`.

### NORMAL mode commands

*note: there are a lot of keybindings missing in this table mostly because I am not finished with adding all the vim keybindings*

|Key|Action|Notes|
|---|---|---|
|`x or q`|Quits eddytion|
|`s`|Saves the current file|If you're not editing a file and instead writing a new file from scratch, eddytion will open a dialog box asking for you to name your new file.|
|`i`|Enters insert mode.||
|`[`|Scroll up by one character.|You can of course scroll normally by getting near to the bottom or top of the viewport using the arrow keys.|
|`]`|Scroll down by one character.|See above.|
|`{`|Scroll up by 10 characters.||
|`}`|Scroll down by 10 characters.||
|`f`|Find and replace|The find field takes a regex, but you can find a simple word too. Also, in the replace field, you can include things such as $1 or $2 to get capture groups from the regex.|
|`o`|appends a new line below|
|`O`|appends a new line above|

### Opening files

```
eddytion path/to/file
```

### opening files within eddytion
the code does exist but i haven't decided on an appropriate keybinding for that so.............

### Creating new files

You can create a new file the same way you'd open one which already exists. Suppose you want to create a file called `new_file.txt`, you could do this

```
eddytion new_file.txt
```

And then, once it's loaded, press `s` to create it.

Alternatively you could rely on your operating system's commands

```
touch new_file.txt
eddytion new_file.txt
```

or something like that. If that doesn't work, you could always do

```
echo> new_file.txt
eddytion new_file.txt
```

## Authors

* **Honestly Nuts** - *Eddytion developer* - [Honestly Nuts](https://github.com/honestly-nuts)
* **Jacob Garby** - *Eddy developer* - [j4cobgarby](https://github.com/j4cobgarby)

See also the list of [contributors](https://github.com/honestly-nuts/eddytion/contributors) who participated in this project.
