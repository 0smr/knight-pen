## Knight-Pen Icon Font builder
---

Icon-font is used in the knight pen application.

### Requirements

- `python 3.7`
- `fontForge module` (*python3*) <small>sudo apt-get install </small>

### How to use it?
Place an icon with a square aspect ratio in the `./SVG` directory and then run `font-generator.py`.

The file name must be formatted as `unicode-glyph_name.svg` (for example, `e006-knight_pen.svg`).

*It should be noted that the script will <u>copy</u> the font file to application resources, <u>delete</u> it from the current directory, and <u>install</u> it on the system.*

### Glyph Viewer
Viewer.bat can be used to display glyph files as a table.

### Template
To make glyphs, you may also use the template below.

<center><img src="tmple.svg" width="40%"></center>