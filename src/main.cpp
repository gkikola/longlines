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

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include "option.hpp"
#include "version.h"

constexpr unsigned int default_length = 80u;

struct Settings {
  unsigned int length = default_length;
};

bool process_options(const OptionParser& op, Settings& settings);
void process_file(std::ostream& out, std::istream& in,
                  const Settings& settings);

int main(int argc, char* argv[])
{
  try {
    OptionParser op = {
      {'l', "length", "MAXLENGTH", "display lines with length exceeding"
       "MAXLENGTH characters"},
      {'?', "help", "", "display detailed usage information and then exit"},
      {'u', "usage", "", "display a short usage message and then exit"},
      {0, "version", "", "display program version and exit"}
    };
  
    op.parse(argc, argv);

    Settings settings;
    if (process_options(op, settings))
      return 0; //exit on --version or --help

    for (const auto& s : op.program_args()) {
      if (s == "-")
        process_file(std::cout, std::cin, settings);
      else {
        std::ifstream ifs(s);
        if (ifs.is_open())
          process_file(std::cout, ifs, settings);
        else
          throw std::runtime_error("could not open file \"" + s + "\"");
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;

    return 1;
  }

  return 0;
}

bool process_options(const OptionParser& op, Settings& settings)
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
  
  for (const auto& o : op) {
    if (o.long_name == "version") {
      std::cout << version;
      return true;
    }
    else {
      int arg;
      switch (o.short_name) {
      case 'l':
        arg = std::stoi(o.argument);

        if (arg < 0)
          throw std::runtime_error("length cannot be negative");
        else
          settings.length = static_cast<unsigned int>(arg);
        break;
      case 'u':
        std::cout << usage;
        return true;
      case '?':
        std::cout << usage << std::endl;
        op.print_usage(std::cout);
        return true;
      }
    }
  }

  return false;
}

void process_file(std::ostream& out, std::istream& in,
                  const Settings& settings)
{
  std::string line;
  while (getline(in, line)) {
    if (line.length() > settings.length) {
      out << line << "\n";
    }
  }

  out << std::flush;
}
