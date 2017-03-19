/* longlines -- find long lines in text files
   Copyright (C) 2017 Gregory Kikola.

   This file is part of longlines.

   longlines is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   longlines is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with longlines.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Written by Gregory Kikola <gkikola@gmail.com>. */

#include <iostream>
#include <stdexcept>
#include <string>

#include "option.hpp"
#include "version.h"

constexpr unsigned int default_length = 80u;

int main(int argc, char* argv[])
{
  std::string usage = "\
Usage: longlines [OPTION...] FILES\n\
Read each text file in FILES and display any lines that exceed the\n\
length specified by the --length option (the default length is 80\n\
characters).\n\
";

  std::string version = "\
longlines " LONGLINES_VERSION "\n\
Copyright (C) 2017 Gregory Kikola\n\
License GPLv3+: GNU GPL version 3 or later\n\
<http://www.gnu.org/licenses/gpl.html>.\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n\
\n\
Written by Gregory Kikola <gkikola@gmail.com>.\n\
";

  OptionParser op = {
    {'l', "length", "MAXLENGTH", "display lines with length exceeding"
    "MAXLENGTH characters"},
    {'?', "help", "", "display detailed usage information and then exit"},
    {'u', "usage", "", "display a short usage message and then exit"},
    {0, "version", "", "display program version and exit"}
  };
  
  op.parse(argc, argv);

  unsigned int length = default_length;
  for (const auto& o : op) {
    if (o.long_name == "version") {
      std::cout << version;
      return 0;
    }
    else {
      int arg;
      switch (o.short_name) {
      case 'l':
        arg = std::stoi(o.argument);

        if (arg < 0)
          throw std::runtime_error("length cannot be negative");
        else
          length = static_cast<unsigned int>(arg);
        break;
      case 'u':
        std::cout << usage;
        return 0;
      case '?':
        std::cout << usage << std::endl;
        op.print_usage(std::cout);
        return 0;
      }
    }
  }
  return 0;
}
