# nextpnr Interactive REPL

This module provides a more complete/advanced interactive REPL environment than the old [`interactive.py`] script
and also targets the Next-generation Python API.

It can be used without any dependencies other than core Python, however, for a better experience, the following
additional Python packages are recommended:

* [Rich] - Allows for colorful output and formatting
* [plataformdirs] - Ensures REPL config/cache/state files are in the plat

## Usage

[`interactive.py`]: ../examples/interactive.py
[Rich]: https://github.com/Textualize/rich
[plataformdirs]: https://platformdirs.readthedocs.io/en/latest/
[readline]: https://docs.python.org/3/library/readline.html
