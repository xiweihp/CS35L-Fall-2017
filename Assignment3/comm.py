#!/usr/bin/python

"""
Xiwei Ma 
lab3
"""

import sys, string, locale
from optparse import OptionParser
#use only the locale and string modules and                                        
#the modules that randline.py already uses 

tab = "\t"
EMPTY = ""

class commlines:
    def __init__(self, f1, f2, supress1,supress2, supressboth, unsorted):
        if f1 == "-" and f2 == "-":
            parser.error("only one file can be read from stdin")
        if f1 == "-":
            self.lines1 = sys.stdin.readlines()
        else:
            file1 = open(f1, 'r')
            self.lines1 = file1.readlines()
            file1.close()

        if f2 == "-":
            self.lines2 = sys.stdin.readlines()
        else:
            file2 = open(f2, 'r')
            self.lines2 = file2.readlines()
            file2.close()

        self.supress1 = supress1
        self.only_file_2 =supress2
        self.supressboth = supressboth
        self.unsorted = unsorted
        self.lines1 = self.modify(self.lines1)
        self.lines2 = self.modify(self.lines2)
        #read the files and split by line

    def modify(self, file):
        if not len(file):
            file.append("\n")
        else:
            temp = file[-1]
            NEW_LINE = "\n"
            if NEW_LINE not in temp:
                temp += NEW_LINE
            file[-1] = temp
        return file



    def issorted(self):
        sorted_line1= sorted(self.lines1)
        sorted_line2 = sorted(self.lines2)
        files_sorted = True

        if sorted_line1 != self.lines1:
            files_sorted = False
            sys.stderr.write("FILE1 is  not sorted\n")

        if sorted_line2 != self.lines2:
            files_sorted = False
            sys.stderr.write("FILE2 is not sorted\n")
        return files_sorted

    def compare_files(self):
        if self.unsorted:
            diff_line2 = self.lines2
            for line1 in self.lines1:
                if line1 not in self.lines2:
                    self.output_print(line1, int(1))
                else:
                    self.output_print(line1, int(3))
                    diff_line2.remove(line1)

            for line2 in diff_line2:
                self.output_print(line2, int(2))
            return

        if not self.issorted():
            return

        i = 0
        j = 0
        len_line1 = len(self.lines1)
        len_line2 = len(self.lines2)

        while i in range(len_line1) or j in range(len_line2):
            line1 = EMPTY
            line2 = EMPTY

            if i < len_line1:
                line1 = self.lines1[i]
            if j < len_line2:
                line2 = self.lines2[j]

            if line1 == EMPTY or (line2 != EMPTY and line1 > line2):
                self.output_print(line2, 2)
                j += 1
            elif line2 == EMPTY or line1 < line2:
                self.output_print(line1, 1)
                i += 1
            else:
                self.output_print(line2, 3)
                i += 1
                j += 1
        return

    def output_print(self, line, option):
        #output depends on options
        stream = ""
        if option == 1:
            if not self.supress1:
                return
        elif option == 2:
            if not self.only_file_2:
                return
            if self.supress1:
                stream += tab
        elif option == 3:
            if not self.supressboth:
                return
            if self.supress1:
                stream += tab
            if self.only_file_2:
                stream += tab
        else:
            sys.stderr.write("incorrect supression option\n")
        sys.stdout.write(stream + line)


def main():
    locale.setlocale(locale.LC_ALL, 'C')
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE1 FILE2
The comm utility shall read file1 and file2, and produce three text columns as output: 
lines only in file1, lines only in file2, and lines in both files..
"""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-1", "--nofile1",
                      action="store_false", dest="supress1", default=True,
                      help="suppress column 1 (lines unique to FILE1)")
    parser.add_option("-2", "--nofile2",
                      action="store_false", dest="supress2", default=True,
                      help="suppress column 2 (lines unique to FILE2)")
    parser.add_option("-3", "--nocommon",
                      action="store_false", dest="supressboth", default=True, help=
                      "suppress column 3 (lines that appear in both files)")
    parser.add_option("-u", "--unsorted",
                      action="store_true", dest = "unsorted",
                      default=False, help="work with unsorted files")
    options, args = parser.parse_args(sys.argv[1:])
    #Add options parsers

    try:
        supress1 = bool(options.supress1)
        supress2 = bool(options.supress2)
        supressboth = bool(options.supressboth)
        unsorted = bool(options.unsorted)
    except:
        parser.error("invalid argument")


    if len(args) != 2:
        parser.error("wrong number of operands")

    try:
        file_1 = args[0]
        file_2 = args[1]
        generator = commlines(file_1, file_2, supress1, supress2, supressboth, unsorted)
    except OSError as err:
        parser.error("OS Error: {0}".format(err))
    try:
        generator.compare_files()
    except:
        parser.error("error")

if __name__ == "__main__":
    main()
