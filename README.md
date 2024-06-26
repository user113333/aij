# typewriter/text editor: Aij (WIP)

It uses a [Gap buffer](https://en.wikipedia.org/wiki/Gap_buffer). It can save a utf-8 file. Created using [Raylib](https://github.com/raysan5/raylib).

## Building
Make file written for Windows (`rm` tool is from [CoreUtils for Windows](https://gnuwin32.sourceforge.net/packages/coreutils.htm)) but the code has to work on Linux.
```
make build
```

## Usage
core:
- <kbd>Enter</kbd> for new line
- <kbd>Tab</kbd> for `\t`
- <kbd>Backspace</kbd> for deleting one character to left
- <kbd>Shift</kbd> + <kbd>Space</kbd> to insert two spaces
- <kbd>Control</kbd> + <kbd>l</kbd> to move one character to right
- <kbd>Control</kbd> + <kbd>h</kbd> to move one character to left
- <kbd>Alt</kbd> + <kbd>l</kbd> to move to the next start of a word
- <kbd>Alt</kbd> + <kbd>h</kbd> to move to the previous start of a word
- <kbd>Alt</kbd> + <kbd>e</kbd> to move to the next end of a word
- <kbd>Control</kbd> + <kbd>e</kbd> to move to the end of line
- <kbd>Control</kbd> + <kbd>a</kbd> to move to the start of text (start of line + after indentation, spaces etc.)
- <kbd>Control</kbd> + <kbd>=</kbd> to increase font size
- <kbd>Control</kbd> + <kbd>-</kbd> to decrease font size
- <kbd>Control</kbd> + <kbd>Shift</kbd> + <kbd>=</kbd> (<kbd>Control</kbd>+<kbd>+</kbd>) to reset font size to default value
- <kbd>Control</kbd> + <kbd>s</kbd> to save into a file: "file.txt"

secondary:
- <kbd>Menu</kbd> + <kbd>k</kbd> to insert Left-Pointing Single Angle Quotation Mark
- <kbd>Menu</kbd> + <kbd>l</kbd> to insert Right-Pointing Single Angle Quotation Mark
- <kbd>Menu</kbd> + <kbd>Comma</kbd> to insert Right-Pointing Double Angle Quotation Mark
- <kbd>Menu</kbd> + <kbd>Period</kbd> to insert Left-Pointing Double Angle Quotation Mark
