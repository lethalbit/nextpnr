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

import sys

from ._base import REPLConsoleBase

__all__ = (
	'PlainConsole',
)

class PlainConsole(REPLConsoleBase):

	def __init__(self) -> None:
		super().__init__()

	def prompt(self, prompt: str) -> str:
		return input(prompt)

	def print(self, message: str, end: str = '\n') -> None:
		print(message, end, file = sys.stdout)

	def warn(self, message: str) -> None:
		print(message, file = sys.stdout)

	def error(self, message: str) -> None:
		print(message, file = sys.stderr)
