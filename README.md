# Latin2 typewriter: Aij (WIP)

It uses a [Gap buffer](https://en.wikipedia.org/wiki/Gap_buffer). It can save latin2 text and it can wrap it when the screen becomes too small. Built using [Raylib](https://github.com/raysan5/raylib).

## Building
Make file written for Windows (`rm` tool is from [CoreUtils for Windows](https://gnuwin32.sourceforge.net/packages/coreutils.htm)) but the code has to work on Linux.
```
make build
```

## Usage
- <kbd>Enter</kbd> for new line
- <kbd>Tab</kbd> for `\t`
- <kbd>Backspace</kbd> for deleting one character to left
- <kbd>Shift</kbd> + <kbd>Space</kbd> to insert two spaces
- <kbd>Control</kbd> + <kbd>f</kbd> to move one character to right
- <kbd>Control</kbd> + <kbd>b</kbd> to move one character to left
- <kbd>Control</kbd> + <kbd>=</kbd> to increase font size
- <kbd>Control</kbd> + <kbd>-</kbd> to decrease font size
- <kbd>Control</kbd> + <kbd>Shift</kbd> + <kbd>=</kbd> (<kbd>Control</kbd>+<kbd>+</kbd>) to reset font size to default value
- <kbd>Control</kbd> + <kbd>s</kbd> to save into a file: "file.txt"
