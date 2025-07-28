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

from typing    import Any

from ._console import get_console
from ._repl    import InteractiveREPL

REPL_CONSOLE = get_console()

try:
	import nextpnr # noqa: F401
except ImportError:
	REPL_CONSOLE.error('Unable to import `nextpnr` API, this module may only be used inside nextpnr')
	raise SystemExit(1)

def main(ctx) -> None:
	repl_locals = dict[str, Any]()
	repl_locals.update(locals())
	repl_locals['ctx'] = ctx

	InteractiveREPL(locals = repl_locals, cons = REPL_CONSOLE).exec()
