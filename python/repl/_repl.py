# nextpnr -- Next Generation Place and Route
#
# Copyright (C) 2025 Aki Van Ness <aki@lethalbit.net>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

import readline
import sys
from copy   import copy
from code   import InteractiveConsole
from types  import CodeType
from typing import Any

import nextpnr

from ._console import REPLConsoleBase, get_console
from ._paths   import NEXTPNR_REPL_CACHE, NEXTPNR_REPL_CONFIG, NEXTPNR_REPL_DATA, NEXTPNR_REPL_STATE

__all__ = (
	'InteractiveREPL',
)


NEXTPNR_REPL_HISTORY_FILE = NEXTPNR_REPL_STATE / 'history'

class REPLCompleter:
	env: dict[str, object]
	matches: list[str]

	def __init__(self, env: dict[str, object]) -> None:
		self.env = copy(env)
		self.matches = list[str]()

	def complete(self, text: str, state: int) -> str | None:
		if state == 0:
			opts = (key for key in self.env.keys() if key.startswith(text))
			self.matches = sorted(opts)

		try:
			return self.matches[state]
		except IndexError:
			pass

class InteractiveREPL(InteractiveConsole):

	def _initialize_readline(self, locals: dict[str, Any] | None) -> None:
		env = dict[str, object]()
		env.update(__builtins__)
		if (locals := locals):
			env.update(locals)

		if NEXTPNR_REPL_HISTORY_FILE.exists():
			readline.read_history_file(NEXTPNR_REPL_HISTORY_FILE)

		self._completer = REPLCompleter(env)

		readline.set_history_length(4096) # XXX(aki): Kinda arbitrary
		readline.set_completer(self._completer.complete)
		readline.parse_and_bind('tab: complete')

	def __init__(
		self, locals: dict[str, Any] | None = None, cons: REPLConsoleBase | None = None, *, local_exit: bool = False
	) -> None:
		if (cons := cons):
			self._console = cons
		else:
			self._console = get_console()

		if locals is None:
			self._locals = dict[str, Any]()
		else:
			self._locals = locals

		self._initialize_readline(self._locals)

		super().__init__(self._locals, 'nextpnr', local_exit = local_exit)

		self.runsource('import nextpnr')
		self.runsource('from nextpnr.types import *')

	def raw_input(self, prompt: str = '') -> str:
		return self._console.prompt(prompt)

	def write(self, data: str) -> None:
		self._console.print(data)

	def runcode(self, code: CodeType) -> None:
		super().runcode(code)
		self._completer.env.update(self._locals)

	def exec(self) -> None:
		more = False
		try:
			self._console.print_banner(nextpnr.__version__, nextpnr.ARCH)
			while True:
				try:
					try:
						line = self._console.prompt(sys.ps2 if more else sys.ps1)
					except EOFError:
						self.write('\n')
						break
					else:
						more = self.push(line)
				except KeyboardInterrupt:
					self.write('\nKeyboardInterrupt\n')
					self.resetbuffer()
					more = False
		finally:
			self._console.print('Exiting REPL, goodbye!')
			readline.write_history_file(NEXTPNR_REPL_HISTORY_FILE)
