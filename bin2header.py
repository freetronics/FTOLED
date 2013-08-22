#!/usr/bin/env python3
"""
Python program to take any file and output a C header file (suitable
for Arduino use) containing the same binary data that was in the file.

This is particularly useful for embedding .BMP images in sketches for
use with FTOLED.

Formats the binary data nicely so you can read back values by hand if
you ever need to.

** Requirements **
Python 3. You can download it from http://python.org

** Usage **
bin2header.py <input file> [<output file>]

Output file is optional and defaults to the name of the input file,
with the extension changed to .h.

bin2header.py was written by Angus Gratton.
Copyright 2013 Freetronics Pty Ltd.

This program is free software: you can redistribute it and/or modify it under the terms
of the version 3 GNU General Public License as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.

"""
import argparse, sys, re, os.path

TEMPLATE = """
// Generated with bin2header.py by Freetronics
// Source file name: %SOURCE%

#include <stdint.h>

#ifdef __AVR__
#define _PGM_ PROGMEM
#else
#define _PGM_
#endif

static const uint8_t %ITEMNAME%[] _PGM_ = {
%CONTENT%
};
"""

parser = argparse.ArgumentParser(description="Process a binary file")
parser.add_argument('input', type=argparse.FileType('rb'), help="Input binary file")
parser.add_argument('output', type=argparse.FileType('wb'), help="Output binary file", nargs="?")

def bin2header(source, destination):
    content = source.read()
    as_hex = [ ("0x%02x" % b) for b in content ]
    content = ""
    offset = 0
    for chunk in chunk_by(as_hex, 16):
        content += "    /* 0x%04x: */ " % offset
        content += ", ".join(chunk)
        content += ",\n"
        offset += 16
    output = TEMPLATE
    output = output.replace("%CONTENT%", content)
    output = output.replace("%SOURCE%", sys.argv[1])
    itemname = re.sub(r'[^a-zA-Z0-9]', '_',  os.path.splitext(os.path.basename(destination.name))[0])
    output = output.replace("%ITEMNAME%", itemname)
    destination.write(bytes(output, 'UTF-8'))

def chunk_by(seq, chunk_size):
    """ Yield successive chunks of length chunk_size from seq. """
    for i in range(0, len(seq), chunk_size):
        yield seq[i:i+chunk_size]


if __name__ == "__main__":
    args = parser.parse_args()
    output = args.output
    if output is None:
        path = os.path.splitext(args.input.name)[0] + ".h"
        output = open(path, "wb")
    bin2header(args.input, output)

