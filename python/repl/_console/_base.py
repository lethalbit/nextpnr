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

from abc import ABCMeta, abstractmethod

__all__ = (
	'REPLConsoleBase',
)

class REPLConsoleBase(metaclass = ABCMeta):
	def __init__(self) -> None:
		pass

	def print_banner(self, version: str, arch: str) -> None:
		self.print(f'YosysHQ nextpnr Python REPL ({version})')
		self.print(f'Current nextpnr architecture: {arch}')
		self.print('\nGlobal nextpnr context is present as \'ctx\'')
		self.print('\nPress ^D (Ctrl-D) to exit this REPL session')

	@abstractmethod
	def prompt(self, prompt: str) -> str:
		raise NotImplementedError

	@abstractmethod
	def print(self, message: str, end: str = '\n') -> None:
		raise NotImplementedError

	@abstractmethod
	def warn(self, message: str) -> None:
		raise NotImplementedError

	@abstractmethod
	def error(self, message: str) -> None:
		raise NotImplementedError
